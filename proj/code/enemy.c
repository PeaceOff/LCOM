#include "enemy.h"
#include "texturemanager.h"
#include "sprite.h"
#include "stdio.h"
#include "map.h"
#include "particle.h"
#include "objmanager.h"

Enemy* Enemy_Create(int x, int y, unsigned int type){
	Enemy* e = (Enemy*)malloc(sizeof(Enemy));
	e->type = type;
	e->pos.x=x;
	e->pos.y=y;
	e->positions=NULL;

	switch(type){
	case E_WORM:
		e->maxHealth = 5;
		e->velocity=1.7;
		break;
	case E_ROBOT:
		e->maxHealth = 2;
		e->velocity = 2;
		break;
	}

	e->rect.w=50;
	e->rect.h=50;
	return e;
}

void Enemy_Delete(Enemy* e){
	Enemy_MoveTo(e,NULL);
	free(e);
	e=NULL;
}

char* Enemy_NetPos(Enemy* e , char id){
	char* res = (char*)malloc(sizeof(char)*3 + sizeof(int)*2);

	int posX = (int)e->pos.x;
	int posY = (int)e->pos.y;


	res[0] = 'M';
	int i = sizeof(char);

	res[i] = id;
	i += sizeof(char);

	memcpy(res+i,&posX,sizeof(int));
	i += sizeof(int);

	memcpy(res+i,&posY, sizeof(int));
	i += sizeof(int);

	res[i] = '!';

	return res;
}

char Enemy_Update(Enemy* e){
	Debug_LOG("Enemy_Update", "STARTED -------------");
	e->time ++;
	if(e->health == 0)
		return 0;
	else
		e->draw=1;


	e->state= E_IDLE;
	if(e->positions !=NULL){

		ListNode* node = e->positions->node->nextNode;

		/*//DebugPathfinding
		ListNode*temp = node;

		while(temp!=NULL){
			Vector2 v = *(Vector2*)temp->conteudo;
			vg_drawSquare(v.x*50+25-10, v.y*50+25-10, 20, 0xFF00);
			temp=temp->nextNode;
		}/**/

		if(node!=NULL){

			if(node->conteudo !=NULL){

				Vector2* pos = node->conteudo;
				if(	(int)e->pos.x/50 == pos->x &&
						(int)e->pos.y/50 == pos->y){

					List_Remove(e->positions, pos);
					free(pos);
				}else{
					Debug_LOG("Enemy_Update", "Moving");

					e->state = E_WALKING;
					Vector2d p1;
					p1.x=(double)pos->x*50+25;
					p1.y=(double)pos->y*50+25;
					double magn = Vector2_Magnitude(Vector2_Sub(p1,e->pos));
					e->pos = Vector2_Lerp(e->pos,p1, (double)e->velocity / magn);

					p1 = Vector2_Sub(p1,e->pos);
					e->dir = p1.x > 0;
				}
			}
		}
	}

	if(e->time%6==0){
		e->time = 0;
		switch(e->type){
		case E_WORM:
			e->animationFrame = (++e->animationFrame > 3)? 0 : e->animationFrame;
			break;
		case E_ROBOT:
			e->animationFrame = (++e->animationFrame > 6)? 0 : e->animationFrame;
			break;
		}

		e->dir=0;
	}

}

void Enemy_MoveTo(Enemy* e, List* pos){

	if(e->positions !=NULL){
		ListNode* node = e->positions->node->nextNode;

		while(node != NULL){
			free(node->conteudo);
			node = node->nextNode;
		}
		List_Destroy(e->positions);
	}
	e->positions = pos;
}

void Enemy_Draw(Enemy* e){

	if(!e->draw) return;

	Sprite* s = TM_GetTexture(SI_ENEMY+e->type);

	if(!e->health){
		Enemy_MoveTo(e,NULL);
		switch(e->type){
		case E_WORM:
			Map_ApplyDecal((int)e->rect.x,(int)e->rect.y,(int)4*50,(int)0,50,50,s);

			break;
		case E_ROBOT:
			Map_ApplyDecal((int)e->rect.x,(int)e->rect.y,(int)7*50,(int)0,50,50,s);

			Particle* part = Particle_Create((int)e->pos.x, (int)e->pos.y,1,SI_EXPLOSION);
			OM_AddObject(part , OBJ_PARTICLE , FIRST_PLANE);

			Map_Brush((int)e->pos.x, (int)e->pos.y, 3 , 7);
			EM_DamagePlayerInRadius(e->pos, 1.5*50.0, 1);

			break;
		}
		e->draw=0;
		return;
	}

	if(e==NULL){
		Debug_LOG("Enemy_Draw","Inimigo Nulo!");
		return;
	}

	Rectangle spritePos;
	spritePos.w=50;
	spritePos.h=50;

	switch(e->state){
	case E_DEAD:
		return;
		break;

	case E_IDLE:
		spritePos.x=0;
		spritePos.y=0;
		break;

	case E_WALKING:

		spritePos.y =0;
		spritePos.x = e->animationFrame*50;


		break;
	}
	s->pos.x=e->pos.x-50/2;
	s->pos.y=e->pos.y-50;

	e->rect.x = e->pos.x - e->rect.w/2;
	e->rect.y = e->pos.y - e->rect.h;

	Sprite_SubDraw(s,spritePos.x,spritePos.y,spritePos.w,spritePos.h,e->dir);
	//Rectangle_Debug(e->rect, 0x00FF);

	Rectangle temp;
	temp.x = e->pos.x-20*e->health/10.0/2;
	temp.y = e->pos.y-2;
	temp.w = 20*e->health/10.0;
	temp.h = 4;
	Rectangle_Debug(temp, 0xF100);

}
