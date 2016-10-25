#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include "sprite.h"

/** @defgroup TextureManager TextureManager module
 * @{
 *
 * Module responsible for TextureManager
 *
 */

enum SpriteIndex{
	SI_PLAYER,
	SI_PLAYER2,
	SI_BALL,
	SI_BULLET,
	SI_ENEMY,
	SI_ENEMY2,
	SI_EXPLOSION,
	SI_MAP,
	SI_TOTAL
};

/**
 * @brief Function to Get a Sprite from an Index
 *
 * @param index the index of the sprite

 * @return Returns a pointer to the sprite, NULL in case of failure
 */
Sprite* TM_GetTexture(unsigned int index);
/**
 * @brief Function to load an Texture to the TextureManager
 *
 * @param index the index of the sprite
 * @param fileDir the path of the bmp
 * @return void
 */
void TM_AddTexture(unsigned int index, char * fileDir);
/**
 * @brief Function to Initialize the TextureManager
 *
 * @return void
 */
void TM_Init();
/**
 * @brief Function to Clear the TextureManager
 *
 * This function clear all the Sprites loaded and frees it's memory
 *
 * @return void
 */
void TM_Exit();
#endif
