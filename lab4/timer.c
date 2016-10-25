#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>
#include "i8254.h"
#include "timer.h"
static unsigned long counterInt;
static int hookID;

int timer_set_square(unsigned long timer, unsigned long freq) {
	if (timer > 2 || freq == 0 || freq > TIMER_FREQ)
		return 1; //verifica se o timer esta entre 0 e 2;

	char conf; //configuracao atual do timer;
	timer_get_conf(timer, &conf);
	unsigned int Fi = TIMER_FREQ;
	unsigned int n = (Fi / freq); //numero que vamos mandar para o controlador

	if (n >> 16 > 0)
		return 1;
	unsigned char lsb = (char) n;
	unsigned char msb = (char) (n >> 8);

	unsigned char porta = TIMER_0;
	unsigned char comando = TIMER_SEL0;
// selecionar o timer e o comando condicionado pelo timer pretendido;
	if (timer == 1) {
		comando = TIMER_SEL1;
		porta = TIMER_1;
	} else if (timer == 2) {
		comando = TIMER_SEL2;
		porta = TIMER_2;
	}

	comando |= TIMER_LSB_MSB; //indicamos que vamos enviar a informacao de modo (lsb seguido de msb);
	comando |= TIMER_SQR_WAVE | TIMER_BIN; //indicamos o modo (sqr_wave) e que a informacao se encontra em binario;

	if (sys_outb(TIMER_CTRL, comando) == 0) { //enviamos o comando com as informacoes para o controlador;
		if (sys_outb(porta, lsb) == 0) //mandamos o lsb;
			if (sys_outb(porta, msb) == 0) //seguido do msb;
				return 0;
	}

	return 1;
}

int timer_subscribe_int(void) {
	hookID = TIMER0_IRQ;
	if (sys_irqsetpolicy(TIMER0_IRQ, IRQ_REENABLE, &hookID) == OK) {
		if (sys_irqenable(&hookID) == OK)
			return 0;
	}
	return 1;
}

int timer_unsubscribe_int() {

	if (sys_irqrmpolicy(&hookID) == OK)
		return 0;
	return 1;
}

void timer_int_handler() {
	counterInt++;
	return;
}

int timer_waitForSeconds(unsigned long time) {
	if (timer_subscribe_int() != OK)
		return 1;

	int ipc_status;
	message msg;
	counterInt = 0;

	int irq_set = BIT(TIMER0_IRQ);
	int r;
	while (counterInt<= time * TIMER_FREQ_DEFAULT) {
		/* You may want to use a different condition */
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* TIMER0 interrompe */
				if (msg.NOTIFY_ARG & irq_set) { /* Está subscrito*/
					timer_int_handler();
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (timer_unsubscribe_int() != OK)
		return 1;
	return 0;

}

int timer_get_conf(unsigned long timer, unsigned char *st) {

	//int sys_inb(port_t port, unsigned long *byte);
	//int sys_outb(port_t port, unsigned long byte);

	if (timer > 2)
		return 1;
	unsigned long comando = TIMER_RB_CMD | TIMER_RB_COUNT_ | TIMER_RB_SEL(timer); //Definimos o comando como ReadBack, no modo de contagem e selecionamos o timer;

	unsigned char porta; //timer em questao;
	porta = TIMER_0;
	if (timer == 1)
		porta = TIMER_1;
	else if (timer == 2)
		porta = TIMER_2;
	long unsigned conf; //variavel onde guardamos a configuracao;
	if (sys_outb(TIMER_CTRL, comando) == 0) { //envia o comando para o controlador;
		if (sys_inb(porta, &conf) == 0) { //vai buscar a configuracao do timer;
			*st = (unsigned char) conf; //guarda a configuracao numa string;
			return 0;
		}
	}
	return 1;
}

void byteToBinary(unsigned char byte) {
	int i = 0;
	for (i = 7; i >= 0; --i) {
		if ((byte >> i) & 1 == 1)
			printf("1");
		else
			printf("0");
	}
}

int timer_display_conf(unsigned char conf) {
	printf("Timer Config: Value[");
	byteToBinary(conf);
	printf("] \n");
	if ((conf & BIT(7)) != 0)
		printf("Output = 1\n");
	else
		printf("Output = 0\n");

	if ((conf & BIT(6)) != 0)
		printf("Null Count= 1\n");
	else
		printf("Null Count = 0\n");

	printf("Type of Access: ");

	switch ((conf >> 4) & 3) {
	case 1:
		printf("LSB \n");
		break;
	case 2:
		printf("MSB \n");
		break;
	case 3:
		printf("LSB followed by MSB \n");
		break;
	default:
		printf("Counter Latch Command\n");
		break;
	}
	printf("Programmed Mode:");
	switch ((conf >> 1) & 7) {
	case 0:
		printf(" 0 \n");
		break;
	case 1:
		printf("1 \n");
		break;
	case 2:
		printf("2\n");
		break;
	case 6:
		printf("2 \n");
		break;
	case 3:
		printf("3 \n");
		break;
	case 7:
		printf("3 \n");
		break;
	case 4:
		printf("4 \n");
		break;
	case 5:
		printf("5 \n");
		break;
	default:
		return 1;
		break;
	}

	printf("Counting mode: %s\n", ((conf & 1) == 1) ? "BCD" : "Binary");

	return 0;
}

int timer_test_square(unsigned long freq) {

	return timer_set_square(0, freq);
}

int timer_test_int(unsigned long time) {

	if (timer_subscribe_int() != OK)
		return 1;

	int ipc_status;
	message msg;
	counterInt = 0;

	int irq_set = BIT(TIMER0_IRQ);
	int r;
	while (counterInt <= time * TIMER_FREQ_DEFAULT) {
		/* You may want to use a different condition */
		/* Get a request message. */
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) { /* received notification */
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE: /* TIMER0 interrompe */
				if (msg.NOTIFY_ARG & irq_set) { /* Está subscrito*/
					timer_int_handler();
					//printf("CounterValue %d\n", counterInt);
					if (counterInt % TIMER_FREQ_DEFAULT == 0)
						printf("Seconds passed: %d\n",
								counterInt / TIMER_FREQ_DEFAULT);
				}
				break;
			default:
				break; /* no other notifications expected: do nothing */
			}
		} else { /* received a standard message, not a notification */
			/* no standard messages expected: do nothing */
		}
	}

	if (timer_unsubscribe_int() != OK)
		return 1;
	printf("Interrupt Finalized!\n");
	return 0;
}

int timer_test_config(unsigned long timer) {
	unsigned char info;
	if (timer > 2 || timer < 0)
		return 1;

	if (timer_get_conf(timer, &info) == 0)
		if (timer_display_conf(info) == 0)
			return 0;

	return 1;
}

