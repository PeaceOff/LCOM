#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "texturemanager.h"
#include "vector.h"
#include "rectangle.h"
#include "sprite.h"

/** @defgroup Particle Particle module
 * @{
 *
 * Module responsible for particles
 *
 */

/** @name Particle structure */
/** @{
 *
 * Particle structure
 */
typedef struct{
	Vector2d pos;
	Rectangle subDiv;
	SubSprite sub;
	unsigned int imgPos;
	char destroyOnEnd;
	unsigned int time;
	unsigned int frames;
}Particle;
/** @} end of Particle */

/**
 * @brief Function to Create a new particle
 *
 *This function is used to alocated memory and create a new instance of particle
 *
 * @param x initial Abcissa of the particle
 * @param y initial Coordenate of the particle
 * @param destroy Bool indicating the destruction of the particle
 * @param index Index of the instance for the object manager
 *
 * @return Returns a pointer to the newly created instance
 */
Particle* Particle_Create(int x, int y,char destroy, unsigned int index);

/**
 * @brief Function to update the instance of the particle
 *
 * @param p Particle to be updated
 *
 * @return Returns an int indicating if the particle effect has ended
 */
int Particle_Update(Particle* p);

/**
 * @brief Function to draw a particle
 *
 * @param p Particle to be drawn
 *
 * @return void
 */
void Particle_Draw(Particle* p);

/**
 * @brief Function to delete a particle
 *
 *The function deletes the instance and frees it's memory
 *
 * @param p Particle to be deleted
 *
 * @return void
 */
void Particle_Delete(Particle* p);

#endif
