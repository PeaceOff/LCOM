#ifndef _MENU_H
#define _MENU_H

#include "testgame.h"
#include "timer.h"
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

typedef struct{
	Sprite* menu_background;
	char mouseOnPlay;
	char mouseOnLeave;
	char mouseOnCreate;
	char mouseOnJoin;
	int exit;
} MainMenu;

MainMenu* MainMenu_Create(char *filename);

void MainMenu_Delete(MainMenu* m);

void MainMenu_Update(MainMenu* m);

int game();

void GameLogic();

#endif
