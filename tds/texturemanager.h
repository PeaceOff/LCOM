#ifndef _TEXTUREMANAGER_H
#define _TEXTUREMANAGER_H

#include "sprite.h"

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

Sprite* TM_GetTexture(unsigned int index);
void TM_AddTexture(unsigned int index, char * fileDir);
void TM_Init();
void TM_Exit();
#endif
