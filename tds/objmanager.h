#ifndef _OBJMANAGER_H
#define _OBJMANAGER_H

enum priority{
	FIRST_PLANE,
	SECOND_PLANE,
	THIRD_PLANE,
	USER_INTERFACE
};

enum object_type{
	MAP,
	PLAYER,
	OBJ_BALL,
	FONT,
	SPRITE,
	SUB_SPRITE,
	BULLET,
	ENEMYMANAGER,
	OBJ_PARTICLE,
	MENU
};

typedef struct{
	enum object_type type;

	void* object;

} Object ;

void OM_Init();

void OM_Exit();

void OM_Update();

void OM_AddObject(void* obj,enum object_type tipo , enum priority p);


#endif
