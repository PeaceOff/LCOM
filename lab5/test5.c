#include "test5.h"
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


void *test_init(unsigned short mode, unsigned short delay) {
	void *adress =vg_init(mode);
	timer_waitForSeconds((long)delay);
	vg_exit();
	vg_info();
	return adress;
}



int test_square(unsigned short x, unsigned short y, unsigned short size, unsigned long color) {

	int hookID = KEYBOARD_IRQ;
	unsigned short mode = RES_1024x768;
	char *adress = vg_init(mode );
	
	if(adress == NULL)
		return 1;

	if (geral_subscribe_int(&hookID, KEYBOARD_IRQ) == 1)
		return 1;

	int ipc_status;
	message msg;
	int counterInt = 0;
	int KEYBOARD_INT = BIT(KEYBOARD_IRQ);
	int r;
	int fim = 1;

	//Draw Square
	int y1 =y;
	int x1=x;
	for (x1; x1< x+size; ++x1) {
		y1 = y;
		for (y1; y1 < y+size; ++y1) {
			if(vg_setPixel(x1,y1,color)){
				geral_unsubscribe_int(&hookID);
				vg_exit();
				return 1;
			}
		}
	}

	x1=0;
	y1=0;
	while (fim) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if(msg.NOTIFY_ARG & KEYBOARD_INT){//Keyboard interrupt
					if(kbd_scan_handler()==81)
						fim=0;
				}
				break;
			default:
				break;
			}
		} else {

		}
	}

	geral_unsubscribe_int(&hookID);
	vg_exit();
	//printf("Bla");
	return 0;
}

int test_line(unsigned short xi, unsigned short yi, unsigned short xf, unsigned short yf, unsigned long color) {
	float deltaX = xf-xi;
	float deltaY = yf-yi;
	float size = sqrt(deltaX*deltaX + deltaY*deltaY);

	int hookID = KEYBOARD_IRQ;
	unsigned short mode = RES_1024x768;
	char *adress = vg_init(mode);

	if(adress == NULL)
		return 1;

	if (geral_subscribe_int(&hookID, KEYBOARD_IRQ) == 1)
		return 1;

	int ipc_status;
	message msg;
	int counterInt = 0;
	int KEYBOARD_INT = BIT(KEYBOARD_IRQ);
	int r;
	int fim = 1;
	int d=0;

	//Draw Line
	for(d = 0; d < size; ++d){
		unsigned short  y = yi + deltaY/size*d;
		unsigned short  x = xi + deltaX/size*d;

		if(vg_setPixel(x,y,color)){
			geral_unsubscribe_int(&hookID);
			vg_exit();
			return 1;
		}
	}

	while (fim) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if(msg.NOTIFY_ARG & KEYBOARD_INT){//Keyboard interrupt
					if(kbd_scan_handler()==81)
						fim=0;
				}
				break;
			default:
				break;
			}
		} else {

		}
	}

	geral_unsubscribe_int(&hookID);
	vg_exit();
	return 0;
}

int test_xpm(unsigned short xi, unsigned short yi, char *xpm[]) {

	unsigned short mode = RES_1024x768;
	char *adress = vg_init(mode);
	if(adress==NULL)
		return 1;

	int hookID = KEYBOARD_IRQ;
	if (geral_subscribe_int(&hookID, KEYBOARD_IRQ) == 1)
		return 1;

	int ipc_status;
	message msg;
	int counterInt = 0;
	int KEYBOARD_INT = BIT(KEYBOARD_IRQ);
	int r;
	int fim = 1;


	//Draw XPM
	if(vg_drawXPM(xi,yi,xpm)){
		geral_unsubscribe_int(&hookID);
		vg_exit();
		return 1;
	}
	//End Drawing XPM

	while (fim) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if(msg.NOTIFY_ARG & KEYBOARD_INT){//Keyboard interrupt

					if(kbd_scan_handler()==81)
						fim=0;
				}
				break;
			default:
				break;
			}
		} else {

		}
	}


	geral_unsubscribe_int(&hookID);
	vg_exit();
	return 0;
}	

int test_move(unsigned short xi, unsigned short yi, char *xpm[], 
		unsigned short hor, short delta, unsigned short time) {




	char *adress = vg_init(RES_1024x768);
	if(adress==NULL)
		return 1;

	if (timer_subscribe_int() != OK)
		return 1;

	int hookID = KEYBOARD_IRQ;
	if (geral_subscribe_int(&hookID , KEYBOARD_IRQ) == 1) {
		return 1;
	}

	//Inicializacao Sprite

	double vx=0;
	double vy=0;

	if(hor)
		vx=(double)delta/(double)time *(double)(1.0/TIMER_FREQ_DEFAULT);
	else
		vy=(double)delta/(double)time *(double)(1.0/TIMER_FREQ_DEFAULT);

	struct Sprite *spriteObj;
	spriteObj = Sprite_Create(xi,yi,vx,vy,xpm);
	if(spriteObj==NULL){
		timer_unsubscribe_int();
		geral_unsubscribe_int(&hookID);

		vg_exit();
		return 1;

	}


	//Fim Inicializacao Sprite

	int ipc_status;
	message msg;
	int counterInt = 0;
	int TIMER_INT = BIT(TIMER0_IRQ);
	int KEYBOARD_INT = BIT(KEYBOARD_IRQ);
	int r;
	int fim = 1;

	int x=0;
	int y=0;

	while (fim) {

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("driver_receive failed with: %d", r);
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & TIMER_INT) { //Timer interrupt
					if(counterInt <= time * TIMER_FREQ_DEFAULT){
						counterInt++;

						vg_clearSprite(spriteObj);
						Sprite_Move(spriteObj);

						Sprite_Draw(spriteObj);
					}
				}
				if(msg.NOTIFY_ARG & KEYBOARD_INT){//Keyboard interrupt

					if(kbd_scan_handler()==81)
						fim=0;


				}
				break;
			default:
				break;
			}
		} else {

		}
	}

	Sprite_Delete(spriteObj);

	timer_unsubscribe_int();
	geral_unsubscribe_int(&hookID);

	vg_exit();

	return 0;


}					

int test_controller() {
	
	vbe_info_block vbe_info_struct;//Struct para a leitura de informacao
	uint16_t *v_modes;
	unsigned int n_v_modes;
	if (vbe_return_info(&vbe_info_struct,&v_modes,&n_v_modes) == 0) return 1;
	if(print_vbe_info(&vbe_info_struct,&v_modes,&n_v_modes)) return 1;//Funcao de print para as informacoes
	return 0;
	
}					
	
