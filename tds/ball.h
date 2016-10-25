#ifndef _BALL_H
#define _BALL_H

#include "vector.h"
#include "rectangle.h"

typedef struct{

	Rectangle rect;
	Vector2d pos;
	Vector2d newPos;
	Vector2d direction;
	int velocity;

} Ball;

Ball* Ball_Create(int x, int y);

void Ball_Update(Ball* b);

void Ball_SyncPos(Ball* b);

char* Ball_NetSendPos(Ball* b);

void Ball_Delete(Ball* b);

void Ball_Draw(Ball* b);

#endif
