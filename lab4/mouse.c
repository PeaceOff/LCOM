#include "keyboard.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"


int mouse_handler() {
	unsigned long status;
	unsigned long mouseByte = 0;

	if (keyboard_readStatus(&status) != OK)
		return 1;

	if (status & KEYBOARD_STATUS_MOUSE == 0)
		return 1;

	if (sys_inb(KBC_BUFF_ADRESS, &mouseByte) != OK)
		return 1;

	if (mouse_counter == 0 && ((mouseByte & MOUSE_VER) == 0)) {
		//printf("Counter 0: %d/n",mouseByte);
		clearPacket();
		return 1;
	}
	//printf("%d <MOUSE COUNTER Byte > %d\n", mouse_counter,mouseByte);
	packet[mouse_counter] = mouseByte;
	mouse_counter += 1;
	if (mouse_counter > 2)
		mouse_counter = 0;

	if (mouse_counter == 0)
		return 2;

	return 0;
}

int mouse_set_stream_mode() {
	/*
	 if(keyboard_sendCommand(MOUSE_COMMAND)){
	 return 1;
	 }
	 if(keyboard_sendArgument(MOUSE_COMMAND_SET_STREAM))
	 return 1;
	 */
	if (keyboard_sendCommand(MOUSE_COMMAND)) {
		return 1;
	}
	if (keyboard_sendArgument(MOUSE_COMMAND_ENABLE_STREAM)) {
		return 1;
	}
}

int mouse_disable_stream_mode() {
	if (keyboard_sendCommand(MOUSE_COMMAND)) {
		return 1;
	}
	if (keyboard_sendArgument(MOUSE_COMMAND_DISABLE_STREAM)) {
		return 1;
	}
}

int mouse_readStatus(unsigned long *command) {
	return (sys_inb(KBC_BUFF_ADRESS, command) != OK);
}

int mouse_get_argument(unsigned long *argument) {
	if (sys_inb(KBC_BUFF_ADRESS, argument) != OK)
		return 1;

	return 0;
}

int mouse_sendArgument(unsigned long argument) {
	unsigned long response = 0;

	do {
		if (sys_outb(KBC_BUFF_ADRESS, argument) != OK)
			return 1;

		if (sys_inb(KBC_BUFF_ADRESS, &response) != OK)
			return 1;

		if (response == KEYBOARD_BUFF_ACK)
			break;

		if (response == KEYBOARD_BUFF_RESEND)
			return 1;

		if (response == KEYBOARD_BUFF_ERROR)
			return 1;

	} while (1);

	return 0;
}
