#ifndef _SPRITE_H
#define _SPRITE_H

#include "vector.h"
#include "rectangle.h"

typedef struct tmpSprite{
	Vector2d pos;
	Vector2d velo;
	char destroy;
	Vector2 dimensions;
	char *map;
}Sprite;

typedef struct{
	Sprite* s;
	Rectangle rect;

}SubSprite;

void Sprite_Move( Sprite *s);

Sprite *Sprite_Create(double x, double y, char *filename);

void Sprite_SetPosition(Sprite *s, double x, double y);

void Sprite_SetVelocity(Sprite *s, double x, double y);

void Sprite_Delete( Sprite *s);

void Sprite_SubDraw(Sprite *s, double x, double y, double w, double h,char reverse);

void Sprite_Draw( Sprite *s);

void SubSprite_Delete(SubSprite* s);

void SubSprite_Draw( SubSprite *s);

SubSprite* SubSprite_Create(double x, double y, char* filename);



#endif
