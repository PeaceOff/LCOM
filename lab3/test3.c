#include "test3.h"
#include <minix/drivers.h>
#include <minix/com.h>
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"

static int is_two_byte_scan = 0;

int break_code_test(unsigned long scancode) {
	if ((scancode & BREAK_CODE) == BREAK_CODE)
		return 1;
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
		printf("BreakCode: 0x%X\n", scanCode);
		if (scanCode == ESC_BREAK_CODE)
			return 81;
	} else {
		printf("MakeCode: 0x%X\n", scanCode);
	}
	return 0;
}

int kbd_test_scan(unsigned short ass) {
	if (ass > 1)
		return 1;
	int hookID = KEYBOARD_IRQ;

	if (geral_subscribe_int(&hookID) == 1) {
		return 1;
	}

	int irq_set = BIT(KEYBOARD_IRQ);

	int ipc_status;
	message msg;
	int fim = 1;
	int r;

	while (fim) { /* You may want to use a different condition */
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* hardware interrupt notification */
				if (msg.NOTIFY_ARG & irq_set) { /* subscribed interrupt */
					if (ass) {
						if (kbd_test_scan_asm() == 81)
							fim = 0;
					} else {
						if (kbd_scan_handler() == 81)
							fim = 0;
					}
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	return geral_unsubscribe_int(&hookID);

}

int kbd_leds(unsigned short* leds, unsigned short valor) {
	while (keyboard_sendArgument(KEYBOARD_LEDS_CMD))
		;
	unsigned long response;

	while (1) {
		if (keyboard_readStatus(&response) != OK)
			return 1;
		if (!(response & KEYBOARD_STATUS_TIMEOUT)) {

			break;
		}
		tickdelay(micros_to_ticks(DELAY_US));
	}

	leds[valor] = (leds[valor]) ? 0 : 1;
	unsigned long configLeds = 0;
	configLeds |= (leds[0] == 1) ? BIT(0) : 0;
	configLeds |= (leds[1] == 1) ? BIT(1) : 0;
	configLeds |= (leds[2] == 1) ? BIT(2) : 0;
	//printf("Valor, %d, %d, %d\n",counter%3,valor[counter], configLeds);
	while (keyboard_sendArgument(configLeds));

}

int kbd_test_leds(unsigned short n, unsigned short *leds) {

	int contador;
	unsigned short led[] = { 0, 0, 0 };
	while (contador < n) {

		if (timer_waitForSeconds(1))
			return 1;
		kbd_leds(led, leds[contador]);
		contador++;
	}
	return 0;
}

int kbd_test_timed_scan(unsigned short n) {
	if (timer_subscribe_int() != OK)
		return 1;

	int hookID = KEYBOARD_IRQ;

	if (geral_subscribe_int(&hookID) == 1) {
		return 1;
	}

	int ipc_status;
	message msg;
	int counterInt = 0;
	int TIMER_INT = BIT(TIMER0_IRQ);
	int KEYBOARD_INT = BIT(KEYBOARD_IRQ);
	int r;
	int fim = 1;
	while (counterInt <= n * TIMER_FREQ_DEFAULT && fim) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & TIMER_INT) { //Timer interrupt
					counterInt++;
				}
				if(msg.NOTIFY_ARG & KEYBOARD_INT){//Keyboard interrupt
					if(kbd_scan_handler()==81)
						fim=0;
					counterInt = 0;
				}
				break;
			default:
				break;
			}
		} else {

		}
	}

	if (timer_unsubscribe_int() != OK)
		return 1;

	return geral_unsubscribe_int(&hookID);

}
