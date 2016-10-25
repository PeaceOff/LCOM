#ifndef _PLAYER_H
#define _PLAYER_H

#include "sprite.h"
#include "vector.h"
#include "sprite.h"
#include "video_gr.h"
#include <minix/syslib.h>
#include "bmp.h"
#include "rectangle.h"

/** @defgroup Player Player module
 * @{
 *
 * Module responsible for Players
 *
 */

#define PLAYER_HEALTH 5

enum Player_States{
	P_IDLE,
	P_WALKING
};
#define P_LEFT 1
#define P_RIGHT 0

/** @name Player structure */
/** @{
 *
 * Player structure
 */
typedef struct {

	Sprite* player_Sprite;
	Vector2d pos;
	Vector2d newPos;
	SubSprite* player_Health;
	char disp;
	char health;

	char usable;
	char shoot;
	unsigned int timeNextHit;
	unsigned int time;
	unsigned char animationFrame;
	enum Player_States state;
	char dir;
	Rectangle rect;
} Player;
/** @} end of Player */

/**
 * @brief Function to Create a new Player
 *
 *This function is used to alocated memory and create a new instance of Player
 *
 * @param x initial Abcissa of the Player
 * @param y initial Coordenate of the Player
 * @param width Width of the player
 * @param height Height of the player
 * @param textureIndex Index of the texture
 * @param usable Player bool of usage
 *
 * @return Returns a pointer to the newly created instance
 */
Player* Player_Create(double x, double y, double width, double height,char textureIndex, char usable);

/**
 * @brief Function to update the instance of the ball
 *
 *The function moves the player and checks collision with the map
 *
 * @param p Player to be updated
 *
 * @return Returns a char indicating if the player is still alive
 */
char Player_Update(Player* p);

/**
 * @brief Function to code the information of the player to be sent via serial port
 *
 * @param p Player we want to send the information of
 *
 * @return Returns a string with the information coded
 */
char* Player_NetSendPos(Player* p);

/**
 * @brief Function read a string of information that was sent via serial player regarding the player
 *
 * @param p Player relative to the information
 * @param msg the message received via uart
 *
 * @return void
 */
void Player_NetReadPos(Player* p, char* msg);

/**
 * @brief Function to delete an instance of Player
 *
 *It clears the memory occupied by the instance and deletes it
 *
 * @param p Player to be deleted
 *
 * @return void
 */
void Player_Delete(Player* player);

/**
 * @brief Function to aplly damage to a player
 *
 * @param p Player to be damaged
 * @param damage Damage to deal the player
 *
 * @return void
 */
void Player_Damage(Player* player, char damage);

/**
 * @brief Function to draw a player
 *
 * @param p Player to be drawn
 *
 * @return void
 */
void Player_Draw(Player* p);

#endif
