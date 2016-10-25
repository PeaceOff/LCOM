#ifndef _PLAYER_H
#define _PLAYER_H

#include "sprite.h"
#include "vector.h"
#include "sprite.h"
#include "video_gr.h"
#include <minix/syslib.h>
#include "bmp.h"
#include "rectangle.h"


#define PLAYER_HEALTH 5

enum Player_States{
	P_IDLE,
	P_WALKING
};
#define P_LEFT 1
#define P_RIGHT 0
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


/*
 * Creates a new instance of Player
 */
Player* Player_Create(double x, double y, double width, double height,char textureIndex, char usable);
/*
 *Updates the player position
 */
char Player_Update(Player* p);

char* Player_NetSendPos(Player* p);

void Player_NetReadPos(Player* p, char* msg);

/*
 *Frees the memory occupied by the player
 *Used right before terminating the program
 */
void Player_Delete(Player* player);

void Player_Damage(Player* player, char damage);


/*
 *Draws the player (it's sprite) in it's current position
 */
void Player_Draw(Player* p);

#endif
