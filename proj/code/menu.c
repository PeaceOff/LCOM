#include "menu.h"
#include "networkmanager.h"
#include "uart.h"
#include "rectangle.h"
#include "ball.h"


static MainMenu * menu;
static void (*functionPtr)();
static int endGame = 0;
static int timeStart = 0;
static int timeEnd = 0;

void MP_GameLogic();
void GameLogic();

MainMenu* MainMenu_Create(char *filename){
	MainMenu* m = (MainMenu*) malloc(sizeof(MainMenu));
	m->menu_background = Sprite_Create(0,0,filename);
	m->mouseOnPlay = 0;
	m->mouseOnLeave = 0;
	m->exit = 0;
	return m;
}

void MainMenu_Delete(MainMenu* m){
	if(m == NULL) return;
	Sprite_Delete(m->menu_background);
	free(m);
	m = NULL;
}


void MainMenu_Update(MainMenu* m){
	if(m == NULL) return;
	Vector2d mouse = mouse_getPosScreen();
	Sprite_Draw(m->menu_background);
	vg_drawSquare( (unsigned int)mouse.x-5 , (unsigned int)mouse.y-5 , 10 , 0xFFFF);

	Rectangle recPlay = Rectangle_Create(20,160,200,50);
	Rectangle recLeave = Rectangle_Create(20,460,200,50);
	Rectangle recCreate = Rectangle_Create(20,260,300,50);
	Rectangle recJoin = Rectangle_Create(20,360,350,50);

	Font_Write(0,0,"TOP DOWN SHOOTER",0x0000,FONT_BIG);

	if(Rectangle_CheckPointCollision(recPlay,mouse)){//Se esta na box do Play
		m->mouseOnPlay = 1;
	} else {
		m->mouseOnPlay = 0;
	}
	if(Rectangle_CheckPointCollision(recLeave,mouse)){//Se esta na box do leave
		m->mouseOnLeave = 1;
	} else {
		m->mouseOnLeave = 0;
	}
	if(Rectangle_CheckPointCollision(recCreate,mouse)){//Se esta na box do create
		m->mouseOnCreate = 1;
	} else {
		m->mouseOnCreate = 0;
	}
	if(Rectangle_CheckPointCollision(recJoin,mouse)){//Se esta na box do join
		m->mouseOnJoin = 1;
	} else {
		m->mouseOnJoin = 0;
	}

	Font_Write(0,0,"TOP DOWN SHOOTER",0x0000,FONT_BIG);


	int color = rgb1(255,65,0);
	if (m->mouseOnPlay){
		color = rgb1(255,255,255);
	}

	Font_Write(20,140,"Play",(short)color, FONT_BIG);

	color = rgb1(255,138,0);
	if(m->mouseOnLeave){
		color = rgb1(255,255,255);
	}

	Font_Write(20,440,"Exit",(short)color, FONT_BIG);

	color = rgb1(255,96,0);
	if(m->mouseOnCreate){
		color = rgb1(255,255,255);
	}

	Font_Write(20,240,"Create",(short)color, FONT_BIG);

	color = rgb1(255,120,0);
	if(m->mouseOnJoin){
		color = rgb1(255,255,255);
	}

	Font_Write(20,340,"Connect",(short)color, FONT_BIG);

}


void MenuTest(){
	vg_setCameraPos(0,0);
	vg_clearScreen(0);
	MainMenu_Update(menu);
	vg_swapBuff();

	if(mouse_getButton(0) && menu->mouseOnPlay){
		functionPtr = &GameLogic;

	}else if(mouse_getButton(0) && menu->mouseOnLeave){
		menu->exit = 1;

	}else if(mouse_getButton(0) && menu->mouseOnCreate){
		NM_SetServer(1);
		functionPtr = &MP_GameLogic;

	}else if(mouse_getButton(0) && menu->mouseOnJoin){
		NM_SetServer(0);
		functionPtr = &MP_GameLogic;
	}
}

// Por enquanto ficam aqui

int initSystems(unsigned int* mouseHook, unsigned int* keyboardHook){
	mouse_init();
	*mouseHook = MOUSE_IRQ;
	if (geral_subscribe_int(mouseHook, MOUSE_IRQ)) {
		return 1;
	}
	mouse_set_stream_mode();
	clearPacket();

	if (timer_subscribe_int() != OK)
		return 1;

	*keyboardHook = KEYBOARD_IRQ;
	if (geral_subscribe_int(keyboardHook , KEYBOARD_IRQ) == 1) {
		return 1;
	}
	inputInit();
	return 0;
}

void unsubscribeSystems(unsigned int* mouseHook, unsigned int* keyboardHook){


	printf("UnSubscribing Keyboard\n");
	geral_unsubscribe_int(keyboardHook);
	printf("\nUnsubscribing Timer\n");
	timer_unsubscribe_int();
	printf("UnSubscribing Mouse\n");
	geral_unsubscribe_int(mouseHook);
	printf("Program finalized without errors!\n");
}

void TDS_Init(){
	NM_SetSinglePlayer(1);
	TM_Init();

	TM_AddTexture(SI_PLAYER,"home/lcom/lcom1516-t6g05/proj/img/bluebirdSheet.bmp");
	TM_AddTexture(SI_PLAYER2,"home/lcom/lcom1516-t6g05/proj/img/redbirdsheet.bmp");
	TM_AddTexture(SI_BULLET,"home/lcom/lcom1516-t6g05/proj/img/bullet.bmp");
	TM_AddTexture(SI_ENEMY,"home/lcom/lcom1516-t6g05/proj/img/wormSheet.bmp");
	TM_AddTexture(SI_ENEMY2,"home/lcom/lcom1516-t6g05/proj/img/robot.bmp");

	TM_AddTexture(SI_EXPLOSION,"home/lcom/lcom1516-t6g05/proj/img/explosion.bmp");

	TM_AddTexture(SI_MAP,"home/lcom/lcom1516-t6g05/proj/img/tileset.bmp");
	Font_Load();
	menu = MainMenu_Create("home/lcom/lcom1516-t6g05/proj/img/mainMenuTest.bmp");
	functionPtr = &MenuTest;

}

void TDS_Exit(unsigned int* m_HookId, unsigned int* k_HookId) {
	TM_Exit();
	MainMenu_Delete(menu);
	vg_exit();
	unsubscribeSystems(m_HookId, k_HookId);
	while(kbd_scan_handler());
}



/*GAME LOGIC*/

Player * GameLogicInit(){
	NM_SetSinglePlayer(1);
	EM_Init(10);
	OM_Init();
	unsigned int mapWitdh = 20;
	unsigned int mapHeight = 20;
	Map_Init("home/lcom/lcom1516-t6g05/proj/img/tileset.bmp", mapWitdh,mapHeight);
	Map_FillBuffer();

	int spawnX = mapWitdh/2* 50;
	int spawnY = mapHeight/2 * 50;

	Player *p = Player_Create(spawnX,spawnY,25,25,SI_PLAYER2,1);
	EM_AddPlayer(p);

	OM_AddObject(NULL,MAP,THIRD_PLANE);
	OM_AddObject(NULL,ENEMYMANAGER,	SECOND_PLANE);
	OM_AddObject(p,PLAYER,SECOND_PLANE);

	//Font* f = (Font*)Font_Create(0,0,"Ola!",0x07DF);

	//OM_AddObject(f,FONT,USER_INTERFACE);
	timeStart = RTC_getTime();
	return p;
}

void GameLogic(){
	static char firstRun = 1;
	static Player* p = NULL;
	static int time =0;
	static int endMsg =0;
	static char res[20];
	Vector2d mouse = mouse_getPosScreen();

	if(firstRun){ //Se Executar a funcao pela primeira vez
		endMsg=1;
		firstRun=0;
		p = GameLogicInit();
	}
	vg_clearScreen(0);
	if(!endGame)
		OM_Update();

	vg_drawCrosshair((unsigned int)mouse.x-5 , (unsigned int)mouse.y-5 , 13 , 0xFFFF);

	if(p != NULL){
		if(p->health == 0){
			endGame = 1;
		}
	}
	if(inputGetKey(KEY_ESC)){//Sair
		endGame = 1;
	}

	if(endGame){
		if(endMsg){
			endMsg=0;
			time = 1;
			/************RTC**************/
			timeEnd = RTC_getTime();
			int timeLasted = timeEnd - timeStart;
			Debug_LOGInt("RTC TIME = ", timeLasted);
			RTC_secondsToString(timeLasted,res);
			Debug_LOG("ENDGAME : ",res);
			/************RTC**************/
		}else{
			time++;
		}
		Font_Write(170,125,"Game OVER",0xF800, FONT_BIG);
		Font_Write(125,225, "You lasted:", 0xF800, FONT_BIG);
		Font_Write(170,325,res,0xF800, FONT_BIG);
		if(time % 300 == 0){
			OM_Exit();
			functionPtr = &MenuTest;
			firstRun=1;
			endGame = 0;
		}
	}
	vg_swapBuff();
}

/**/
/*#GAME LOGIC*/

void MP_GameLogicInit(){
	UART_ClearFIFO((int)COM1_BASE);
	NM_SetSinglePlayer(0);
	OM_Init();
	NM_Exit();

	unsigned int mapWidth = 20;
	unsigned int mapHeight = 20;
	Map_Init("home/lcom/lcom1516-t6g05/proj/img/tileset.bmp", mapWidth,mapHeight);
	Map_FillBuffer();

	Vector2 spawn1;
	spawn1.x= (mapWidth -2) *50;
	spawn1.y = mapHeight/2 *50;
	Vector2 spawn2;
	spawn2.x= 1 * 50;
	spawn2.y = mapHeight/2 *50;
	int id = SI_PLAYER2;
	int id2 = SI_PLAYER;
	if(NM_IsServer()){
		id = SI_PLAYER;
		id2 = SI_PLAYER2;
		Vector2 temp = spawn2;
		spawn2 = spawn1;
		spawn1 = temp;
	}

	Player *p1 = Player_Create(spawn1.x,spawn1.y,25,25,id,1);
	Player *p2 = Player_Create(spawn2.x,spawn2.y,25,25,id2,0);
	p1->shoot=0;
	p1->disp=0;


	NM_SetPlayer(1,p1);
	NM_SetPlayer(2,p2);

	OM_AddObject(NULL,MAP,THIRD_PLANE);
	OM_AddObject(p1,PLAYER,SECOND_PLANE);
	OM_AddObject(p2,PLAYER,SECOND_PLANE);

	if(!NM_IsServer())
		NM_SendMessage("C!");
}
void MP_GameLogic(){
	static char firstRun = 1;
	static char	waitForInfo = 1;
	static int time =0;
	if(firstRun){
		firstRun=0;
		waitForInfo=1;
		MP_GameLogicInit();
		time = 0;
	}
	vg_clearScreen(0);

	if(waitForInfo){
		if( NM_PlayerConnected()){
			waitForInfo=0;
			if(NM_IsServer())
				NM_SendMessage("C!");
		}else{
			if(NM_IsServer())
				Font_Write(0,0,"Waiting Player",0xD181, FONT_BIG);
			else
				Font_Write(0,0,"Waiting Server",0xD181, FONT_BIG);
		}
	}else{
		time ++;

		if(time % 6 == 0){
			NM_SendPlayerInfo();
			time = 0;
		}
		OM_Update();

	}

	Debug_LOGInt("WaitForInfo", waitForInfo);
	Debug_LOGInt("NM_PLAYER CONNECTED STATUS", NM_PlayerConnected());

	if(inputGetKey(KEY_ESC) || ( !waitForInfo && !NM_PlayerConnected() )){//Sair

		endGame = 1;
	}

	if(endGame){
		endGame = 0;
		firstRun = 1;
		if(NM_PlayerConnected()){
				Debug_LOG("MP_GameLogic","NM_SendMessage");
				NM_Exit();
				NM_SendMessage("D!");
		}else{
			NM_Exit();
		}
		NM_SetSinglePlayer(0);
		NM_SetSinglePlayer(1);
		OM_Exit();
		functionPtr = &MenuTest;
	}

	vg_swapBuff();
}

int game(){

	TDS_Init();

	unsigned int mouseHookId = MOUSE_IRQ ;
	unsigned int keyboardHookId = IKEYBOARD_IRQ;

	unsigned int addr = COM1_BASE;
	unsigned int IRQ = COM1_IRQ;

	UART_SetConfiguration(addr, 3, 1, 7, (unsigned short)1200);
	UART_EnableFIFO(addr);

	if(initSystems(&mouseHookId,&keyboardHookId))
		return 1;

	if(UART_SubscribeInt(addr,COM1_IRQ))
		return 1;

	NM_Init();
	Debug_LOG("TESTE","Teste1");
	int ipc_status;
	message msg;
	int UART_INT = BIT(COM1_IRQ);
	int TIMER_INT = BIT(TIMER0_IRQ);
	int KEYBOARD_INT = BIT(KEYBOARD_IRQ);
	int MOUSE_INT = BIT(MOUSE_IRQ);
	int r;
	int fim = 1;
	char *adress = vg_init(0x114);
	while (fim) {
		if ((r = driver_receive(ANY, &msg, &ipc_status)) != 0) {
			continue;
		}
		if (is_ipc_notify(ipc_status)) {
			switch (_ENDPOINT_P(msg.m_source)) {
			case HARDWARE:
				if(msg.NOTIFY_ARG & UART_INT || UART_NeedToSend()){//UART interrupt
					UART_IntHandler(addr);
					Debug_LOG("game", "UART INTERRUPT!");
				}
				NM_Update();

				if (msg.NOTIFY_ARG & TIMER_INT) { //Timer interrupt

					functionPtr(); // Apontador para a função atual do programa (Game ou Menu)
					if(NM_IsServer())
						vg_drawSquare(0,0,200,600);

					if (menu->exit) fim = 0;

				}
				if (msg.NOTIFY_ARG & MOUSE_INT) {//Mouse Interrupt

					if(mouse_handler()==2){
						mouse_update();
					}
				}

				if(msg.NOTIFY_ARG & KEYBOARD_INT){//Keyboard interrupt
					kbd_scan_handler();
					if(inputGetKey(KEY_B) || inputGetKey(KEY_D))

					if(inputGetKey(KEY_B))
						NM_SetServer(1);
					if(inputGetKey(KEY_N))
						NM_SetServer(0);

					if(inputGetKey(KEY_H) || inputGetKey(KEY_J))
						printf("Enviado Input\n");

					if(inputGetKey(KEY_H)){
						char* ch = (char*)malloc(sizeof(ch)*17);
						ch = "0000000011111111!";
						UART_SendMsg(ch);
					}
					if(inputGetKey(KEY_J)){
						char* ch = (char*)malloc(sizeof(ch)*4);
						ch = "TSD!";
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
	NM_Exit();
	UART_UnSubscribeInt();
	TDS_Exit(&mouseHookId, &keyboardHookId);

	return 0;
}




