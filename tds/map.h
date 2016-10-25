#ifndef _MAP_H
#define _MAP_H

#include "list.h"
#include "sprite.h"

#define DAMAGE_STATES 2

#define MAP_STATE_1 1
#define MAP_STATE_2 2
#define MAP_STATE_3 3


typedef struct Node{
	int x;
	int y;
	int iCost; //Distancia ate Node
	int fCost; //Distancia ate Fim
	int tCost; //Total;
	int solid; //solido ou nao
	struct Node* parent;
}Node;

Vector2 Map_getDimensions();

int Map_Init(char * fileName, int x, int y);

void Map_FillBuffer();

void Map_Draw();

void Map_ReadExplosion(char* msg);

char* Map_SendExplosion(int x, int y, char dmg);

inline Node* GetLowCost(List* l1);

char Map_Damage(int x, int y, char dmg);

void Map_NetReadDamage(char * msg);

Node* Map_GetNode(int x, int y);

List* Map_GetPath(int xi , int yi, int xf, int yf);

void Map_ApplyDecal(int x,int y, int iX, int iY, int iW, int iH,Sprite* s);

int Map_CheckCollision(int x, int y);

List* Map_GetSpawnPoints();

inline void Map_AutoTexture(int x, int y);

inline unsigned char Map_GetNeighbours(int x, int y);

inline void Map_Texturize(int x1, int y1 , int imgX, int imgY);

inline unsigned char Map_GetData(int x, int y);

void Map_Exit();

#endif
