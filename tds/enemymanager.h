#ifndef _ENEMYMANAGER_H
#define _ENEMYMANAGER_H

#include "player.h"
#include "enemy.h"

void EM_Init(unsigned int enemyNumber);

void EM_Update();

void EM_AddPlayer(Player* p);

void EM_Draw();

Enemy** EM_GetEnemies();

void EM_ReadMsg(char* msg);

unsigned int EM_GetEnemyCount();

void EM_DamagePlayerInRadius(Vector2d pos, double raio, char dmg);

void EM_Exit();

#endif
