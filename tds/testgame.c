#include "testgame.h"
#include "timer.h"
//#include "vbe.h"
#include "video_gr.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"
#include <math.h>
#include "pixmap.h"
#include "graphic_info.h"
#include "lmlib.h"
#include "sprite.h"
#include "vector.h"
#include "mouse.h"
#include "bmp.h"
#include "map.h"
#include "player.h"
#include "list.h"
#include "bullet.h"
#include "font.h"
#include "objmanager.h"
#include "texturemanager.h"
#include "enemymanager.h"
#include <string.h>
#include "uart.h"

int test_uart(int addr, int value, char obj){
	unsigned int addr_base = COM2_BASE;
	unsigned int IRQ = COM2_IRQ;

	if(addr==1){
		addr_base = COM1_BASE;
		IRQ = COM1_IRQ;
	}
	UART_SetConfiguration(addr_base, 3, 2, 7, 64000);
	UART_ClearFIFO(addr_base);
	UART_EnableFIFO(addr_base);


	unsigned int keyboardhookID = KEYBOARD_IRQ;
	if (geral_subscribe_int(&keyboardhookID , KEYBOARD_IRQ) == 1) {
		return 1;
	}

	if(UART_SubscribeInt(addr_base,IRQ))
			return 1;


	inputInit();

	int ipc_status;
	message msg;
	int counterInt = 0;
	int UART_INT = BIT(IRQ);
	int KEYBOARD_INT = BIT(KEYBOARD_IRQ);
	int r;
	int fim = 1;
	while (fim) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:


				if(msg.NOTIFY_ARG & UART_INT ||UART_NeedToSend() ){//Keyboard interrupt
					printf("!!!Uart Interrupt!!!\n");
					UART_IntHandler(addr_base);
				}


				if(msg.NOTIFY_ARG & KEYBOARD_INT){//Keyboard interrupt
					if(kbd_scan_handler()==81)
						fim=0;

					if(inputGetKey(KEY_A) || inputGetKey(KEY_D))
						UART_GetConfig(addr_base);

					if(inputGetKey(KEY_D)){
						char* ch = (char*)malloc(sizeof(ch)*17);
						UART_PrintError(addr_base);
					}
					if(inputGetKey(KEY_SPACE)){
						UART_PrintBuffer();
					}
					if(inputGetKey(KEY_W)){
						char* ch = (char*)malloc(sizeof(ch)*2);
						ch = "a!";
						UART_SendMsg(ch);
					}
				}

				break;
			default:
				break;
			}
		} else {
		}
	}
	UART_DisableFIFO(addr_base);
	printf("A Sair..\n");
	geral_unsubscribe_int(&keyboardhookID);
	if(UART_UnSubscribeInt())
		return 1;
	return 0;
}
