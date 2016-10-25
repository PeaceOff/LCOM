#ifndef _SPRITE_H
#define _SPRITE_H

#include "vector.h"
#include "rectangle.h"

/** @defgroup Sprite Sprite module
 * @{
 *
 * Module responsible for Sprite
 *
 */

 /** @name Sprite structure */
/** @{
 *
 * Sprite structure
 */
typedef struct tmpSprite{
	Vector2d pos;
	Vector2d velo;
	char destroy;
	Vector2 dimensions;
	char *map;
}Sprite;
/** @} end of Sprite */

/** @name SubSprite structure */
/** @{
 *
 * SubSprite structure
 */
typedef struct{
	Sprite* s;
	Rectangle rect;

}SubSprite;
/** @} end of SubSprite */


void Sprite_Move( Sprite *s);

/**
 * @brief Function to Create a new Sprite
 *
 *This function is used to alocated memory and create a new instance of Sprite
 *
 * @param x initial Abcissa of the Sprite
 * @param y initial Coordenate of the Sprite
 * @param filename Path to the sprite.bmp
 *
 * @return Returns a pointer to the newly created instance
 */
Sprite *Sprite_Create(double x, double y, char *filename);

/**
 * @brief Function to Set the Sprite position
 *
 * @param s Sprite to change position
 * @param x Abcissa of the Sprite position
 * @param y Coordenate of the Sprite position
 *
 * @return Returns a pointer to the newly created instance
 */
void Sprite_SetPosition(Sprite *s, double x, double y);
/**
 * @brief Function to Set the Sprite velocity
 *
 * @param s Sprite to set velocity
 * @param x Abcissa of the Sprite velocity
 * @param y Coordenate of the Sprite velocity
 *
 * @return Returns a pointer to the newly created instance
 */
void Sprite_SetVelocity(Sprite *s, double x, double y);
/**
 * @brief Function to delete an instance of Sprite
 *
 *The function deletes a Sprite and frees it's memory
 *
 * @param s Sprite to be deleted
 *
 * @return void
 */
void Sprite_Delete( Sprite *s);
/**
 * @brief Function to draw a subpart of a Sprite
 *
 * @param s Sprite to be drawn
 * @param x the x coord of the subsprite
 * @param y the y coord of the subsprite
 * @param w the width of the subsprite
 * @param h the height of the subsprite
 * @param reverse if the sprite to draw is reversed
 *
 * @return void
 */
void Sprite_SubDraw(Sprite *s, double x, double y, double w, double h,char reverse);

/**
 * @brief Function to draw a Sprite
 *
 *The function draws a Sprite on the screen
 *
 * @param s Sprite to be drawn
 *
 * @return void
 */
void Sprite_Draw( Sprite *s);

/**
 * @brief Function to delete an instance of SubSprite
 *
 *The function deletes a SubSprite and frees it's memory
 *
 * @param s SubSprite to be deleted
 *
 * @return void
 */
void SubSprite_Delete(SubSprite* s);


/**
 * @brief Function to draw a SubSprite
 *
 *The function draws a SubSprite on the screen
 *
 * @param s SubSprite to be drawn
 *
 * @return void
 */
void SubSprite_Draw( SubSprite *s);

/**
 * @brief Function to Create a new SubSprite
 *
 *This function is used to alocated memory and create a new instance of SubSprite
 *
 * @param x initial Abcissa of the SubSprite
 * @param y initial Coordenate of the SubSprite
 * @param filename Path to the SubSprite.bmp
 *
 * @return Returns a pointer to the newly created instance
 */

SubSprite* SubSprite_Create(double x, double y, char* filename);



#endif
