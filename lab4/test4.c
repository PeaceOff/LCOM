#include "test4.h"
#include "keyboard.h"
#include "i8042.h"
#include "i8254.h"
#include "mouse.h"
#include <minix/drivers.h>
#include <minix/com.h>

typedef enum {
	M_RDOWN, M_RUP
} mouse_state_t;

struct mouseData {
	int x;
	int y;
};

static mouse_state_t m_state = M_RDOWN;
static struct mouseData m_data;

static unsigned short packet[] = { 0, 0, 0 };
static int mouse_counter = 0;

void clearPacket() {
	packet[0] = 0;
	packet[1] = 0;
	packet[2] = 0;
}

int drawConf() {
	unsigned short fByte;
	fByte = packet[0];
	unsigned int i = packet[1];
	if ((fByte & BIT(1)) != 0) {
		printf("Right-Buton: Pressed\n");
	} else {
		printf("Right-Buton: Released\n");
	}
	if ((fByte & BIT(2)) != 0) {
		printf("Middle-Buton: Pressed\n");
	} else {
		printf("Middle-Buton: Released\n");
	}
	if ((fByte & BIT(0)) != 0) {
		printf("Left-Buton: Pressed\n");
	} else {
		printf("Left-Buton: Released\n");
	}
	if ((fByte & BIT(4)) != 0) {
		printf("Scaling: Set to 2:1\n");
	} else {
		printf("Scaling: Set to 1:1\n");
	}
	if ((fByte & BIT(5)) != 0) {
		printf("Data Reporting: Enable\n");
	} else {
		printf("Data Reporting: Disable \n");
	}
	if ((fByte & BIT(6)) != 0) {
		printf("Mode: Remote\n");
	} else {
		printf("Mode: Stream\n");
	}
	switch (i)
 	{
 	case 0:
 		printf("Resolution: 1 unit per mm\n");
 		break;
 	case 1:
 		printf("Resolution: 2 units per mm\n");
 		break;
 	case 2:
 		printf("Resolution: 4 units per mm\n");
 		break;
 	case 3:
 		printf("Resolution: 8 units per mm\n");
 		break;
 	default:
 		break;
 	}
	printf("Sample Rate: %d Hz\n",(unsigned int)packet[2]);
}

int drawInfo() {
	unsigned short firstByte;
	firstByte = packet[0];
	printf("B1=0x%X ", packet[0]);
	printf("B2=0x%X ", packet[1]);
	printf("B3=0x%X ", packet[2]);
	printf("LB=%d ", ((firstByte & MOUSE_LEFT_MOUSE) ? 1 : 0));
	printf("MB=%d ", ((firstByte & MOUSE_MIDDLE_MOUSE) ? 1 : 0));
	printf("RB=%d ", ((firstByte & MOUSE_RIGHT_MOUSE) ? 1 : 0));
	printf("XOV=%d ", ((firstByte & MOUSE_X_OVF) ? 1 : 0));
	printf("YOV=%d ", ((firstByte & MOUSE_Y_OVF) ? 1 : 0));
	printf("X=%d ", (char) packet[1]);
	printf("Y=%d ", (char) packet[2]);

	m_data.x += ((char) packet[1]);
	m_data.y += ((char) packet[2]);

	if (firstByte & MOUSE_RIGHT_MOUSE)
		m_state = M_RDOWN;
	else
		m_state = M_RUP;
	printf("\n");
}

int mouse_get_config() {
	while (1) {
		//printf("0\n");
		if (keyboard_sendCommand(MOUSE_COMMAND))
			continue;
		//printf("01\n");

		if (mouse_sendArgument(MOUSE_COMMAND_STATUS_REQUEST))
			continue;
		unsigned long arg;
		//printf("1\n");
		//tickdelay(micros_to_ticks(DELAY_US));
		if (mouse_get_argument(&arg))
			continue;
		packet[0] = arg;
		//printf("2\n");
		//tickdelay(micros_to_ticks(DELAY_US));
		if (mouse_get_argument(&arg))
			continue;
		packet[1] = arg;
		//printf("3\n");
		//tickdelay(micros_to_ticks(DELAY_US));
		if (mouse_get_argument(&arg))
			continue;
		packet[2] = arg;
		break;
	}
	return 0;
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

int test_packet(unsigned short cnt) {
	unsigned int mouseHookId = MOUSE_IRQ;
	if (geral_subscribe_int(&mouseHookId, MOUSE_IRQ)) {
		return 1;
	}

	mouse_set_stream_mode();
	clearPacket();
	mouse_counter = 0;

	int irq_setMouse = BIT(MOUSE_IRQ);

	int ipc_status;
	message msg;
	int r;
	int count = 0;
	while (count < cnt) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_setMouse) {

					if (mouse_handler() == 2) {
						count++;
						drawInfo();
					}
				}
				break;
			default:
				break;
			}
		} else {
		}
	}
	printf("Function terminated.\n");
	geral_unsubscribe_int(&mouseHookId);
	//unsigned long temp;
	//sys_inb(KBC_ADRESS, &temp);
	return 0;

}

/*Interrupt handler le os bytes do out_buf
 * ler apensa 1 por interrupcao
 * usar packet[] porque sao 3 bytes
 * counter para saber o numero do byte
 *
 * Para Input/Output requere ler o status register
 * verificar a paridade timeout etc,
 * Bit5 mouse data! importante
 * nao escrever no In-Buf(0x60) ou CTRL_Reg(0x64) se IBF =1
 *
 * Mouse Controller retorna para  aporta 0x60 por cada argumento 0xD4 lido
 * 0xFA - tudo ok
 * 0xFE byte invalido - Voltar a enviar a sequencia(se for argumento)
 * 0xFC segundo byte consecutivo invalido
 */
int test_async(unsigned short idle_time) {
	unsigned int mouseHookId = MOUSE_IRQ;
	timer_subscribe_int();
	if (geral_subscribe_int(&mouseHookId, MOUSE_IRQ)) {
		return 1;
	}

	mouse_set_stream_mode();
	clearPacket();
	mouse_counter = 0;

	int irq_setMouse = BIT(MOUSE_IRQ);
	int irq_setTimer = BIT(TIMER0_IRQ);
	int ipc_status;
	message msg;
	int r;
	int count = 0;
	while (count <= idle_time * TIMER_FREQ_DEFAULT) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_setMouse) {

					if (mouse_handler() == 2) {
						count = 0;
						count++;
						drawInfo();
					}
				}
				if (msg.NOTIFY_ARG & irq_setTimer) {
					count++;

				}
				break;
			default:
				break;
			}
		} else {
		}
	}

	printf("Function terminated.\n");
	timer_unsubscribe_int();
	geral_unsubscribe_int(&mouseHookId);
	//unsigned long temp;
	//sys_inb(KBC_ADRESS, &temp);
	return 0;
}

int test_config(void) {
	unsigned int mouseHookId = MOUSE_IRQ;
	clearPacket();
	if (geral_subscribe_int(&mouseHookId, MOUSE_IRQ)) {
		return 1;
	}
	//printf("a\n");
	mouse_disable_stream_mode();
	//printf("b\n");
	mouse_get_config();
	//printf("c\n");
	drawConf();
	//printf("d\n");
	geral_unsubscribe_int(&mouseHookId);
	return 0;

	/* To be completed ...
	 * Ler e mostrar a configuracao do rato
	 * usar 0xE9 (status request)
	 * ler 1 byte do kbc nao ha necessidade de verificar stat reg
	 * 3 bytes pertencem ao mesmo packet
	 * se estiver em stream mode e for ativado com 0xF4 o host deve desativar 0xF5 antes de enviar qualquer outro comando
	 * se um byte estiver no out buff kbc nao vai gerar mais interrupcoes*/
}

int checkState(short length, unsigned short tolerance) {
	int delta_y = 0;
	int delta_x = 0;
	switch (m_state) {
	case M_RUP:
		m_data.x = 0;
		m_data.y = 0;
		break;
	case M_RDOWN:

		delta_x = (m_data.x > 0) ? m_data.x : -m_data.x;

		if(length > 0){
			if (m_data.y >= length && tolerance > delta_x)
				return 1;
			else if( m_data.y < 0 ){
				m_data.y = 0;
				m_data.x = 0;
			}
		}else{
			if (m_data.y <= length && tolerance > delta_x)
				return 1;
			else if( m_data.y > 0 ){
				m_data.y = 0;
				m_data.x = 0;
			}
		}
		if (tolerance < delta_x) {
			m_data.x = 0;
			m_data.y = 0;
		}
		break;
	}
	return 0;
}

int test_gesture(short length, unsigned short tolerance) {

	unsigned int mouseHookId = MOUSE_IRQ;
	if (geral_subscribe_int(&mouseHookId, MOUSE_IRQ)) {
		return 1;
	}

	mouse_set_stream_mode();
	clearPacket();
	mouse_counter = 0;
	m_data.x = 0;
	m_data.y = 0;
	int irq_setMouse = BIT(MOUSE_IRQ);

	int ipc_status;
	message msg;
	int r;
	int count = 0;
	while (1) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}

		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:

				if (msg.NOTIFY_ARG & irq_setMouse) {

					if (mouse_handler() == 2) {
						drawInfo();

					}
				}
				break;
			default:
				break;
			}
		} else {
		}

		if (checkState(length, tolerance))
			break;
	}
	printf("Function terminated.\n");
	geral_unsubscribe_int(&mouseHookId);
	//unsigned long temp;
	//sys_inb(KBC_ADRESS, &temp);
	return 0;

}
