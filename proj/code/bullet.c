#include "bullet.h"
#include "texturemanager.h"
#include "enemymanager.h"
#include "enemy.h"

Bullet *Bullet_Create(double x, double y, char *filename){
	Bullet * b = (Bullet*) malloc(sizeof(Bullet));

	b->bullet_Sprite = TM_GetTexture(SI_BULLET);
	b->vel.x = 0;
	b->vel.y = 0;

	b->pos.x = x;
	b->pos.y = y;
	b->destroy=0;
	return b;
}

void Bullet_SetPosition(Bullet* b, double x, double y){
	if(b == NULL)
		return;
	b->bullet_Sprite->pos.x = x;
	b->bullet_Sprite->pos.y = y;
}

void Bullet_SetVelocity(Bullet* b,double x, double y){
	if(b == NULL)
			return;
	b->vel.x = x;
	b->vel.y = y;
	b-> vel = Vector2_Normalize(b->vel);
}

char Bullet_Update(Bullet* b){
	if(b == NULL) return;
	if(b->destroy == 1)
		return 0;

	Enemy** en = EM_GetEnemies();

	int i;
	if(en!=NULL){
		for(i = 0; i < EM_GetEnemyCount(); i ++){
			if(en[i]->draw==1)
				if(Rectangle_CheckPointCollision(en[i]->rect,b->pos)){
					en[i]->health = (--en[i]->health < 0)? 0: en[i]->health;
					b->destroy =1;
					break;
				}
		}
	}

	b->pos.x += b->vel.x*5;
	b->pos.y += b->vel.y*5;

	if(Map_CheckCollision((int)b->pos.x,(int)b->pos.y))
	{

		Map_Damage((int)b->pos.x, (int)b->pos.y, 1);
		return 0;
	}
	return 1;
}


void Bullet_Delete(Bullet * b){
	if (b == NULL) return;
	free(b);
	b = NULL;
}

void Bullet_Draw(Bullet* b){
	if(b == NULL)
			return;
	b->bullet_Sprite->pos.x = b->pos.x - b->bullet_Sprite->dimensions.x/2;
	b->bullet_Sprite->pos.y = b->pos.y - b->bullet_Sprite->dimensions.y/2;

	Sprite_Draw(b->bullet_Sprite);
}
