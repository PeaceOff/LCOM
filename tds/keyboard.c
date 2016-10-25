#include "keyboard.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "timer.h"
#include "i8042.h"

static char* input;


void inputInit(){

	input = (char *)malloc(KEY_TOTAL);
	int i=0;
	for(i = 0; i<KEY_TOTAL ; i++){
		input[i]=0;
	}

}

char inputGetKey(unsigned int key){
	if(key > KEY_F12){
		switch (key) {
			case 0xE01C:
				/*KEY_NUM_ENTER*/
				return (char)*(input + (KEY_TOTAL - 1));
				break;
			case 0xE01D:
				return (char)*(input + (KEY_TOTAL - 2));
				/*KEY_R_CTRL*/
				break;
			case 0xE035:
				return (char)*(input + (KEY_TOTAL - 3));
				/*KEY_NUM_SLASH*/
				break;
			case 0xE038:
				return (char)*(input + (KEY_TOTAL - 4));
				/*KEY_ALT_GR*/
				break;
			case 0xE047:
				return (char)*(input + (KEY_TOTAL - 5));
				/*KEY_HOME*/
				break;
			case 0xE048:
				return (char)*(input + (KEY_TOTAL - 6));
				/*KEY_ARR_UP*/
				break;
			case 0xE049:
				return (char)*(input + (KEY_TOTAL - 7));
				/*KEY_PGUP*/
				break;
			case 0xE04B:
				return (char)*(input + (KEY_TOTAL - 8));
				/*KEY_ARR_LEFT*/
				break;
			case 0xE04D:
				return (char)*(input + (KEY_TOTAL - 9));
				/*KEY_ARR_RIGHT*/
				break;
			case 0xE050:
				return (char)*(input + (KEY_TOTAL - 10));
				/*KEY_ARR_DOWN*/
				break;
			case 0xE051:
				return (char)*(input + (KEY_TOTAL - 11));
				/*KEY_PGDN*/
				break;
			case 0xE052:
				return (char)*(input + (KEY_TOTAL - 12));
				/*KEY_INS*/
				break;
			case 0xE053:
				return (char)*(input + (KEY_TOTAL - 13));
				/*KEY_DEL*/
				break;
			case 0xE05B:
				return (char)*(input + (KEY_TOTAL - 14));
				/*KEY_WIN*/
				break;
			case 0xE05D:
				return (char)*(input + (KEY_TOTAL - 15));
				/*KEY_CNTX*/
				break;
			case 0xE04F:
				return (char)*(input + (KEY_TOTAL - 16));
				/*KEY_END*/
				break;
			default:
				break;
			}
	}

	return (char)*(input + key);
}

static int is_two_byte_scan=0;

int break_code_test(unsigned long scancode) {
	if ((scancode & BREAK_CODE) == BREAK_CODE)
		return 1;
	return 0;
}

int kbd_two_byte_handler(unsigned long scancode){
	unsigned long newKey = (scancode & 0x007F);
	unsigned char isMake = (scancode & 0x0080)? 0: 1;
	switch (newKey) {
	case 0x1C:
		/*KEY_NUM_ENTER*/
		input[KEY_TOTAL - 1]=(char)isMake;
		break;
	case 0x1D:
		input[KEY_TOTAL - 2]=(char)isMake;
		/*KEY_R_CTRL*/
		break;
	case 0x35:
		input[KEY_TOTAL - 3]=(char)isMake;
		/*KEY_NUM_SLASH*/
		break;
	case 0x38:
		input[KEY_TOTAL - 4]=(char)isMake;
		/*KEY_ALT_GR*/
		break;
	case 0x47:
		input[KEY_TOTAL - 5]=(char)isMake;
		/*KEY_HOME*/
		break;
	case 0x48:
		input[KEY_TOTAL - 6]=(char)isMake;
		/*KEY_ARR_UP*/
		break;
	case 0x49:
		input[KEY_TOTAL - 7]=(char)isMake;
		/*KEY_PGUP*/
		break;
	case 0x4B:
		input[KEY_TOTAL - 8]=(char)isMake;
		/*KEY_ARR_LEFT*/
		break;
	case 0x4D:
		input[KEY_TOTAL - 9]=(char)isMake;
		/*KEY_ARR_RIGHT*/
		break;
	case 0x50:
		input[KEY_TOTAL - 10]=(char)isMake;
		/*KEY_ARR_DOWN*/
		break;
	case 0x51:
		input[KEY_TOTAL - 11]=(char)isMake;
		/*KEY_PGDN*/
		break;
	case 0x52:
		input[KEY_TOTAL - 12]=(char)isMake;
		/*KEY_INS*/
		break;
	case 0x53:
		input[KEY_TOTAL - 13]=(char)isMake;
		/*KEY_DEL*/
		break;
	case 0x5B:
		input[KEY_TOTAL - 14]=(char)isMake;
		/*KEY_WIN*/
		break;
	case 0x5D:
		input[KEY_TOTAL - 15]=(char)isMake;
		/*KEY_CNTX*/
		break;
	case 0x4F:
		input[KEY_TOTAL - 16]=(char)isMake;
		/*KEY_END*/
		break;
	default:
		break;
	}

	return 0;
}

int kbd_scan_handler() {

	unsigned long scanCode;

	if (sys_inb(KBC_BUFF_ADRESS, &scanCode) != OK) {
		return 1;
	}
	if (scanCode == SCAN_TWO_BYTES) {
		is_two_byte_scan = 1;
		return 0;
	} else if (is_two_byte_scan) {
		scanCode |= (SCAN_TWO_BYTES << 8);
		is_two_byte_scan = 0;
	}
	if (break_code_test(scanCode)) {
		//printf("BreakCode: 0x%X\n", scanCode);

		unsigned long key = scanCode & 0x7F;
		if(key < KEY_TOTAL && !is_two_byte_scan){

			//printf("Alterar Posicao de memoria: 0x%X\n", key);
			input[key]=(char)0;
		}


		/*******************ALTERAR AQUI*************************************************/
		if(scanCode & (SCAN_TWO_BYTES << 8)) kbd_two_byte_handler(scanCode);
		/*******************ALTERAR AQUI*************************************************/


		if (scanCode == ESC_BREAK_CODE)
			return 81;

	} else {
		unsigned long key = scanCode & 0x7F;
		if(scanCode < KEY_TOTAL && !is_two_byte_scan){

			//printf("Alterar Posicao de memoria: 0x%X\n", key);
			input[key]=(char)1;
		}

		if(scanCode & (SCAN_TWO_BYTES << 8)) kbd_two_byte_handler(scanCode);

		//printf("MakeCode: 0x%X\n", scanCode);
	}
	return 0;
}

int geral_subscribe_int(unsigned int *hookID,unsigned int end) {

	if (sys_irqsetpolicy(end, IRQ_REENABLE | IRQ_EXCLUSIVE, hookID)
			== OK) {
		if (sys_irqenable(hookID) == OK)
			return 0;
	}
	return 1;
}

int geral_unsubscribe_int(unsigned int *hookID) {
	if(sys_irqdisable(hookID) == OK)
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

