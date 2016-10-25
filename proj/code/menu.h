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

/** @defgroup Menu Menu module
 * @{
 *
 * Module responsible for the menu and game management
 *
 */

/** @name MainMenu structure */
/** @{
 *
 * MainMenu structure
 */
typedef struct{
	Sprite* menu_background;
	char mouseOnPlay;
	char mouseOnLeave;
	char mouseOnCreate;
	char mouseOnJoin;
	int exit;
} MainMenu;
/** @} end of MainMenu */

/**
 * @brief Function to Create a new Menu
 *
 * @param filename the absolute path to the main menu picture
 *
 * @return Returns a pointer to the newly created instance
 */
MainMenu* MainMenu_Create(char *filename);

/**
 * @brief Function to Delete Menu
 *
 * The function deletes a menu and frees it's memory
 *
 * @param m the instance of the menu
 *
 * @return void
 */
void MainMenu_Delete(MainMenu* m);

/**
 * @brief Function to Update the Menu
 *
 * @param m the instance of the menu
 *
 * @return void
 */
void MainMenu_Update(MainMenu* m);

/**
 * @brief Function that handles the game loop
 * 
 * @return Returns 1 if't theres an error or not
 */
int game();
/**
 * @brief Function that handles the game logic
 * 
 * @return void
 */
void GameLogic();

#endif
