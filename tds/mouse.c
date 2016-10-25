#include "mouse.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8042.h"
#include "video_gr.h"

struct mouseData {
	int x;
	int y;

	int deltax;
	int deltay;

	int m1;
	int m2;
	int m3;

};

static struct mouseData m_data;

static unsigned short packet[] = { 0, 0, 0 };
static int mouse_counter = 0;



void clearPacket() {
	packet[0] = 0;
	packet[1] = 0;
	packet[2] = 0;
}

char mouse_getButton(char btn){
	switch (btn) {
	case 0:
		return m_data.m1;
		break;
	case 1:
		return m_data.m2;
		break;
	case 2:
		return m_data.m3;
		break;
	default:
		return 0;
		break;
	}
}

Vector2d mouse_getPos(){
	Vector2d temp;
	temp.x = m_data.x;
	temp.y = -m_data.y;

	return temp;
}

Vector2d mouse_getPosScreen(){
	Vector2d temp;
	temp.x = m_data.x;
	temp.y = -m_data.y;

	Vector2d res = vg_getScreenRes();

	if(temp.x<0)
		temp.x=0;
	if(temp.x >= res.x)
		temp.x=res.x-1;
	if(temp.y<0)
		temp.y=0;
	if(temp.y >= res.y)
		temp.y=res.y-1;

	m_data.x=temp.x;
	m_data.y=-temp.y;

	temp = Vector2_Add(temp, vg_getCameraPos());

	return temp;
}

void mouse_init(){
	m_data.x=0;
	m_data.y=0;
	m_data.m1=0;
	m_data.m2=0;
	m_data.m3=0;

}

void mouse_update(){

	unsigned short fByte;
	fByte = packet[0];
	unsigned int i = packet[1];

	if ((fByte & BIT(1)) != 0)
		m_data.m2=1;
	else
		m_data.m2=0;
	if ((fByte & BIT(2)) != 0)
		m_data.m3=1;
	else
		m_data.m3=0;

	if ((fByte & BIT(0)) != 0)
		m_data.m1 = 1;
	else
		m_data.m1 = 0;

	m_data.x +=(char) packet[1];
	m_data.y +=(char) packet[2];


}

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
