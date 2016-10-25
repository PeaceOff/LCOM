#ifndef _BULLET_H
#define _BULLET_H

#include "sprite.h"
#include "vector.h"
#include "sprite.h"
#include "video_gr.h"
#include <minix/syslib.h>
#include "bmp.h"

#define BULLET_SPEED  4
/** @defgroup Bullet Bullet module
 * @{
 *
 * Module responsible for Bullets
 *
 */

/** @name Bullet structure */
/** @{
 *
 * Bullet structure
 */
typedef struct {
	Sprite* bullet_Sprite;//Bullet sprite (TBD)
	Vector2d vel; //Speed
	Vector2d pos; //Pos
	char destroy;
} Bullet;
/** @} end of Bullet */

/**
 * @brief Function to Create a new bullet
 *
 *This function is used to alocated memory and create a new instance of bullet
 *
 * @param x initial Abcissa of the bullet
 * @param y initial Coordenate of the bullet
 * @param filename Path to the bullet.bmp
 *
 * @return Returns a pointer to the newly created instance
 */
Bullet *Bullet_Create(double x, double y, char *filename);

/**
 * @brief Function to set the position of a given bullet
 *
 *
 * @param b Bullet which the position we will alter
 * @param x the new abcissa for the bullet
 * @param y the new coordenate for the bullet
 *
 * @return void
 */
void Bullet_SetPosition(Bullet* b, double x, double y);

/**
 * @brief Function to set the velocity of a given bullet
 *
 *
 * @param b Bullet which the position we will alter
 * @param x the new abcissa for the velocity vector of the bullet
 * @param y the new coordenate for the velocity vector of the bullet
 *
 * @return void
 */
void Bullet_SetVelocity(Bullet* b, double x, double y);

/**
 * @brief Function to update the instance of the bullet
 *
 *The function moves the bullet and checks if it hits either the map or an enemy
 *
 * @param b Bullet to be updated
 *
 * @return char 0 if the bullet hit something or 1 if not
 */
char Bullet_Update(Bullet *b);

/**
 * @brief Function to delete an instance of bullet
 *
 *The function deletes a bullet and frees it's memory
 *
 * @param b Bullet to be deleted
 *
 * @return void
 */
void Bullet_Delete(Bullet *b);

/**
 * @brief Function to draw a bullet
 *
 *The function draws a bullet on the screen
 *
 * @param b Bullet to be drawn
 *
 * @return void
 */
void Bullet_Draw(Bullet *b);

#endif
