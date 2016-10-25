#ifndef _ENEMY_H
#define _ENEMY_H

#include "vector.h"
#include "rectangle.h"
#include "list.h"

enum E_AnimationState{
	E_DEAD,
	E_IDLE,
	E_WALKING
};

enum E_EnemyType{
	E_WORM,
	E_ROBOT
};

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

Enemy* Enemy_Create(int x, int y, unsigned int type);

char Enemy_Update(Enemy* e);

void Enemy_Draw(Enemy* e);

void Enemy_MoveTo(Enemy* e, List* pos);

void Enemy_Delete(Enemy* e);

char* Enemy_NetPos(Enemy* e, char id);

#endif
