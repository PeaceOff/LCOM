#include "keyboard.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "timer.h"
#include "i8042.h"

int geral_subscribe_int(int *hookID,unsigned int end) {

	if (sys_irqsetpolicy(end, IRQ_REENABLE | IRQ_EXCLUSIVE, hookID)
			== OK) {
		if (sys_irqenable(hookID) == OK)
			return 0;
	}
	return 1;
}

int geral_unsubscribe_int(int *hookID) {

	if (sys_irqrmpolicy(hookID) == OK)
		return 0;

	return 1;
}

int keyboard_sendCommand(unsigned long command) {
	unsigned long response = 0;

	if(keyboard_readStatus(&response) != OK)return 1;
	if(response & KEYBOARD_STATUS_INBUFF_FULL != 0) return 1;
	if (sys_outb(KBC_ADRESS, command) == OK)
		return 0;
	return -1;
}

int keyboard_readStatus(unsigned long *command){
	return sys_inb(KBC_ADRESS,command);
}

int keyboard_sendArgument(unsigned long argument) {
	unsigned long response = 0;

	if (keyboard_readStatus(&response) != OK) return 1;

	if (response & KEYBOARD_STATUS_INBUFF_FULL	|| response & KEYBOARD_STATUS_PAR || response & KEYBOARD_STATUS_TIMEOUT)
		return 1;
	do {
		if (sys_outb(KBC_BUFF_ADRESS, argument) != OK) return 1;
		while(1){
			if (keyboard_readStatus(&response) != OK) return 1;
			if(!(response & KEYBOARD_STATUS_TIMEOUT)){

				break;
			}
			tickdelay(micros_to_ticks(DELAY_US));
		}
		if (sys_inb(KBC_BUFF_ADRESS, &response) != OK) return 1;
		if(response == KEYBOARD_BUFF_ACK ) break;

	} while (1);

	return 0;
}

