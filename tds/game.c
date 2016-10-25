//#include <minix/drivers.h>
//#include "testgame.h"
//#include "pixmap.h"
#include "menu.h"



int main() {

	sef_startup();
	sys_enable_iop(SELF);

	printf("Top Down Shooter: Welcome!\n");

	if(game()){
		printf("TDS: An error occurred and the program stopped!\n");
		return 1;
	}

	return 0;
}
