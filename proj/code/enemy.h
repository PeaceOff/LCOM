#ifndef _ENEMY_H
#define _ENEMY_H

#include "vector.h"
#include "rectangle.h"
#include "list.h"
/** @defgroup Enemy Enemy module
 * @{
 *
 * Module responsible for enemies
 *
 */

enum E_AnimationState{
	E_DEAD,
	E_IDLE,
	E_WALKING
};

enum E_EnemyType{
	E_WORM,
	E_ROBOT
};

/** @name Enemy structure */
/** @{
 *
 * Enemy structure
 */
typedef struct{
	unsigned int time;
	unsigned char animationFrame;
	unsigned char dir;
	unsigned char maxHealth;
	char draw;
	enum E_AnimationState state;
	enum E_EnemyType type;
	unsigned int velocity;
	List* positions;

	Rectangle rect;
	Vector2d pos;
	short health;

} Enemy;
/** @} end of Enemy */

/**
 * @brief Function to Create a new Enemy
 *
 *This function is used to allocate memory and create a new instance of enemy
 *
 * @param x initial Abcissa of the enemy
 * @param y initial Coordenate of the enemy
 * @param type Indicates the type of enemy
 *
 * @return Returns a pointer to the newly created instance
 */
Enemy* Enemy_Create(int x, int y, unsigned int type);

/**
 * @brief Function to update an enemy
 *
 *This function updates an enemies and checks if it is still alive after the update
 *also deals with the animation frame.
 *
 * @param e Enemy to be updated
 *
 * @return Returns and indicator if the enemy has been killed or not
 */
char Enemy_Update(Enemy* e);

/**
 * @brief Function to draw an enemy
 *
 * It draws the enemy, if the enemy died the function will apply the decal (dead body) to the map
 *
 * @param e Enemy to be drawn
 *
 * @return void
 */
void Enemy_Draw(Enemy* e);

/**
 * @brief Function to move an enemy within the enemy list
 *
 * @param e Enemy to be moved
 * @param pos Pointer to the list in which the enemy will be moved to
 *
 * @return void
 */
void Enemy_MoveTo(Enemy* e, List* pos);

/**
 * @brief Function to delete an Enemy
 *
 * The function deletes the instance and frees it's memory
 *
 * @param e Enemy to be deleted
 *
 * @return void
 */
void Enemy_Delete(Enemy* e);

/**
 * @brief Function to return the position of the enemy
 *
 * The enemies position is codified to a string to be sent by serial port
 *
 * @param e Enemy to be sent info
 * @param id Identifier of the enemy
 *
 * @return A string with the coded information
 */
char* Enemy_NetPos(Enemy* e, char id);

#endif
