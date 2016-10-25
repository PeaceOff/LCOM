#ifndef _BALL_H
#define _BALL_H

#include "vector.h"
#include "rectangle.h"

/** @defgroup Ball Ball module
 * @{
 *
 * Module responsible for balls
 *
 */

/** @name Ball structure */
/** @{
 *
 * Ball structure
 */  
typedef struct{

	Rectangle rect;
	Vector2d pos;
	Vector2d newPos;
	Vector2d direction;
	int velocity;

} Ball;
/** @} end of Ball */

/**
 * @brief Function to Create a new ball
 *
 *This function is used to alocated memory and create a new instance of ball
 *
 * @param x initial Abcissa of the Ball
 * @param y initial Coordenate of the Ball
 *
 * @return Returns a pointer to the newly created instance
 */
Ball* Ball_Create(int x, int y);

/**
 * @brief Function to update the instance of the ball
 *
 *The function moves the ball and checks if it hits the map
 *
 * @param b Ball to be updated
 *
 * @return void
 */
void Ball_Update(Ball* b);

/**
 * @brief Function to synchronize the instance of the ball
 *
 *The function moves the ball to last position received
 *
 * @param b Ball to be synchronized
 *
 * @return void
 */
void Ball_SyncPos(Ball* b);

/**
 * @brief Function to synchronize the instance of the ball
 *
 *The function transform the position of the ball into a string 
 *
 * @param b Ball to be synchronized
 *
 * @return Returns the position of the ball coded into a string
 */

char* Ball_NetSendPos(Ball* b);
/**
 * @brief Function to delete an instance of ball
 *
 *The function deletes a ball and frees it's memory
 *
 * @param b ball to be deleted
 *
 * @return void
 */
void Ball_Delete(Ball* b);

/**
 * @brief Function to draw a ball
 *
 *The function draws a ball on the screen
 *
 * @param b ball to be drawn
 *
 * @return void 
 */
void Ball_Draw(Ball* b);

#endif
