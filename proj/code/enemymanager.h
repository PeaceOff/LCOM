#ifndef _ENEMYMANAGER_H
#define _ENEMYMANAGER_H

#include "player.h"
#include "enemy.h"

/** @defgroup EnemyManager EnemyManager module
 * @{
 *
 * Module responsible for enemy management
 *
 */

 /**
 * @brief Function to Initialize EnemyManager
 *
 *This function is used to alocated memory and create new instances of enemies
 *
 * @param enemyNumber Number of enemies to spawn
 *
 * @return void
 */
void EM_Init(unsigned int enemyNumber);

 /**
 * @brief Function to Update EnemyManager
 *
 *This function is used to controll the behaviour of the enemies
 *
 * @return void
 */

void EM_Update();

 /**
 * @brief Function to add a player to the EnemyManager
 *
 * @param p the address of the player to send
 *
 * @return void
 */
void EM_AddPlayer(Player* p);


 /**
 * @brief Function to draw the enemies
 *
 *
 * @return void
 */
void EM_Draw();

 /**
 * @brief Function to get the enemies array
 *
 * @return Returns an Array of Enemy*
 */
Enemy** EM_GetEnemies();


/**
 * @brief Function to read a message
 *
 *This function is used to read a mesage in from of char*
 *
 * @return void
 */
void EM_ReadMsg(char* msg);

/**
 * @brief Function to return the enemy count
 *
 * @return Returns number of enemies
 */
unsigned int EM_GetEnemyCount();

/**
 * @brief Function to damage the player in a given radius
 *
 * @param pos the address position of the player
 * @param raio the radius of the damage
 * @param dmg the damage value
 *
 * @return void
 */
void EM_DamagePlayerInRadius(Vector2d pos, double raio, char dmg);

/** 
 * @brief Function to Terminate EnemyManager
 *
 *This deletes every enemie in the enemie array and frees it's memory
 *
 * @return void
 */

void EM_Exit();

#endif
