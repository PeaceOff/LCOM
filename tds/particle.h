#ifndef _PARTICLE_H
#define _PARTICLE_H

#include "texturemanager.h"
#include "vector.h"
#include "rectangle.h"
#include "sprite.h"

typedef struct{
	Vector2d pos;
	Rectangle subDiv;
	SubSprite sub;
	unsigned int imgPos;
	char destroyOnEnd;
	unsigned int time;
	unsigned int frames;
}Particle;


Particle* Particle_Create(int x, int y,char destroy, unsigned int index);

int Particle_Update(Particle* p);

void Particle_Draw(Particle* p);

void Particle_Delete(Particle* p);
#endif
