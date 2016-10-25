#ifndef _BULLET_H
#define _BULLET_H

#include "sprite.h"
#include "vector.h"
#include "sprite.h"
#include "video_gr.h"
#include <minix/syslib.h>
#include "bmp.h"

#define BULLET_SPEED  4

typedef struct {
	Sprite* bullet_Sprite;//Bullet sprite (TBD)
	Vector2d vel; //Speed
	Vector2d pos; //Pos
	char destroy;

} Bullet;

Bullet *Bullet_Create(double x, double y, char *filename);

void Bullet_SetPosition(Bullet* b, double x, double y);

void Bullet_SetVelocity(Bullet* b, double x, double y);

char Bullet_Update(Bullet *b);

void Bullet_Delete(Bullet *b);

void Bullet_Draw(Bullet *b);

#endif
