#include "rtc.h"
#include <minix/syslib.h>
#include <minix/drivers.h>
#include <minix/com.h>

#include "keyboard.h"


static unsigned long rtc_info[10];
static unsigned long rtc_hookID;

void RTC_UpdateTime(){
	unsigned long i=0x0000;
	do{
		sys_outb(RTC_ADDR_REG,RTC_REG_A);
		sys_inb(RTC_DATA_REG,&i);
	}while(i & 0x40);

	for(i = 0; i < 10 ; i ++){
		sys_outb(RTC_ADDR_REG,i);
		sys_inb(RTC_DATA_REG,&rtc_info[i]);
	}
}

int RTC_getTime(){
	int h;
	int m;
	int s;
	RTC_UpdateTime();
	h = ((rtc_info[4] >> 4) * 10) + (rtc_info[4] & 0x0F);
	m = ((rtc_info[2] >> 4) * 10) + (rtc_info[2] & 0x0F);
	s = ((rtc_info[0] >> 4) * 10) + (rtc_info[0] & 0x0F);
	int res = s + (60 * m) + (60*60*h);
	return res;
}

void RTC_secondsToString(int seconds,char* res){

	int h,m,s;
	h = seconds / 3600;
	seconds %= 3600;
	m  = seconds / 60;
	seconds %= 60;
	s = seconds;
	Debug_LOGInt("HOURS = ", h);
	Debug_LOGInt("Minutes = ", m);
	Debug_LOGInt("seconds = ", s);
	sprintf(res,"%dH:%dM:%dS",h,m,s);

}

int RTC_DisplayTime(){


	RTC_UpdateTime();

	char* weekDay;
	switch(rtc_info[6]){
	case 0:
		weekDay="Sunday";
		break;
	case 1:
		weekDay="Monday";
		break;
	case 2:
		weekDay="Tuesday";
		break;
	case 3:
		weekDay="Wednesday";
		break;
	case 4:
		weekDay="Thursday";
		break;
	case 5:
		weekDay="Friday";
		break;
	case 6:
		weekDay="Saturday";
		break;

	}
	printf("%s %x/%x/%x %x h %x m %x s\n",weekDay,rtc_info[7],rtc_info[8],rtc_info[9]+0x2000,rtc_info[4], rtc_info[2],rtc_info[0]);
/*
	unsigned int hookID = RTC_IRQ;
	if(geral_subscribe_int(&hookID, RTC_IRQ))
		printf("Cenas\n");

	sys_outb(RTC_ADDR_REG,RTC_REG_B);
	unsigned long temp = RTC_PF;
	sys_inb(RTC_DATA_REG, &temp);
	temp |= RTC_PF;
	sys_outb(RTC_ADDR_REG,RTC_REG_B);
	sys_outb(RTC_DATA_REG, &temp);


	int RTC_INT = 1<<RTC_IRQ;
	int r;
	int fim = 1;
	int ipc_status;
	message msg;
	int time = 20;
	while (time>0) {

		--time;
		printf("Loop! %d\n",time);

		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			printf("Test4!\n");
			continue;
		}
		printf("Test2!\n");
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if (msg.NOTIFY_ARG & RTC_INT) { //RTC interrupt

					int cause;
					unsigned long regA;

					printf("Interrupt!\n");

					sys_outb(RTC_ADDR_REG,RTC_REG_C);
					cause = sys_inb(RTC_DATA_REG, &regA);
					if( cause & RTC_UF )
						printf("Update int\n");
					if( cause & RTC_AF )
						printf("Alarm int\n");
					if( cause & RTC_PF )
						printf("Periodic int\n");

				}
				break;
			default:
				break;
			}
		} else {
		}
		printf("End!\n");
	}
	printf("Unsubscribing..");
	geral_unsubscribe_int(&hookID);
*/

	return 0;
}
