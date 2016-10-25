#include "player.h"
#include "keyboard.h"
#include "bullet.h"
#include "mouse.h"
#include "objmanager.h"
#include "enemymanager.h"
#include "enemy.h"
#include "texturemanager.h"
#include "map.h"
#include "rectangle.h"

Player* Player_Create(double x, double y, double width, double height,char textureIndex, char usable){
	Player*p = (Player*) malloc(sizeof(Player));
	p->usable = usable;
	p->timeNextHit=0;
	p->shoot = 1;

	p->player_Sprite = TM_GetTexture(textureIndex);

	if(p->usable){
		p->player_Health = SubSprite_Create(0,0,"home/lcom/lcom1516-t6g05/tds/img/healthbar.bmp");
		OM_AddObject(p->player_Health,SUB_SPRITE,USER_INTERFACE);
	}

	p->player_Sprite->pos.x -= p->player_Sprite->dimensions.x/2;
	p->player_Sprite->pos.y -= p->player_Sprite->dimensions.y;
	p->health = PLAYER_HEALTH;
	p->pos.x = x;
	p->pos.y = y;

	p->newPos = p->pos;

	p->rect.x = x-width/2;
	p->rect.y = y-width/2;
	p->rect.w = width;
	p->rect.h = height;

	p->animationFrame=0;
	p->time=0;
	p->state=P_IDLE;
	return p;
}

Vector2 Player_Input(Player* p){
	Vector2 nPos;
	nPos.x=0;
	nPos.y=0;

	if(!p->usable) return nPos;
	//CheckPosition

	if(inputGetKey(KEY_A))
		nPos.x=-3;
	if(inputGetKey(KEY_D))
		nPos.x=+3;
	if(inputGetKey(KEY_W))
		nPos.y=-3;
	if(inputGetKey(KEY_S))
		nPos.y=+3;

	Rectangle r = p->rect;
	r.x+=nPos.x;
	if(Rectangle_CheckTerrainCollision(r))
		nPos.x=0;

	r.x = p->rect.x;
	r.y+=nPos.y;
	if(Rectangle_CheckTerrainCollision(r))
		nPos.y=0;

	p->pos.x += nPos.x;
	p->pos.y += nPos.y;
	p->rect.x = p->pos.x-p->rect.w/2;
	p->rect.y = p->pos.y - p->rect.h;

	vg_setCameraPos(p->pos.x - 400 + 50,p->pos.y - 300 + 50);

	//Shoot
	if(mouse_getButton(0) && p->shoot && p->time%6==0){
		if(p->disp){
			//	p->disp=0;
			Bullet *b = Bullet_Create( (int)p->pos.x , (int)p->pos.y-25, "home/lcom/lcom1516-t6g05/tds/img/david.bmp");
			Vector2d dir = p->pos;
			dir.y-=25;
			dir = Vector2_Sub(mouse_getPosScreen(),dir);
			Bullet_SetVelocity(b,dir.x,dir.y);
			OM_AddObject(b,BULLET,FIRST_PLANE);
		}
	}else{
		p->disp=1;
	}

	return nPos;
}

Vector2 Player_SyncPos(Player* p){
	Debug_LOG("Player_SyncPos","Inicializado");

	Vector2 mapDim;
	mapDim = Map_getDimensions();

	if((int)p->newPos.x < 0 || (int) p->newPos.x >= mapDim .x ||(int)p->newPos.y < 0 || (int) p->newPos.y >= mapDim .y )
		return;

	Vector2d temp = Vector2_Lerp(p->pos, p->newPos, 0.1);
	p->pos = temp;
	Debug_LOGInt("Player_SyncPos: temp X", (int)p->newPos.x);
	Debug_LOGInt("Player_SyncPos: temp Y", (int)p->newPos.y);

	Vector2 vec;
	Vector2d sub;
	sub = Vector2_Sub(p->newPos,p->pos);

	vec.x = (int)(sub.x/10.0);
	vec.y = (int)(sub.y/10.0);

	p->rect.x = p->newPos.x-p->rect.w/2;
	p->rect.y = p->newPos.y - p->rect.h;

	Debug_LOGInt("Player_SyncPos: X", (int)p->newPos.x);
	Debug_LOGInt("Player_SyncPos: Y", (int)p->newPos.y);

	return vec;
}

char Player_Update(Player* p){

	Enemy** en =(Enemy**) EM_GetEnemies();
	int i;
	if(p->timeNextHit > 10){
		p->timeNextHit=0;
		for(i = 0; i < EM_GetEnemyCount(); i++){
			Rectangle hitbox;
			hitbox.w = 50;
			hitbox.h = 50;
			hitbox.x = p->pos.x - hitbox.w/2;
			hitbox.y = p->pos.y - hitbox.h;
			if(Rectangle_CheckCollision(en[i]->rect,hitbox) && en[i]->draw){
				Player_Damage(p,1);
				break;
			}
		}
	}else{
		p->timeNextHit++;
	}

	Vector2 nPos = 	Player_Input(p);
	if(!p->usable)
		nPos = Player_SyncPos(p);


	//Update Animation
	p->time++;


	if(p->usable)
		if(!NM_IsSinglePlayer()){
			if(p->time % 30 == 0){
				if(p->health < PLAYER_HEALTH)
					p->health++;
			}

			if(inputGetKey(KEY_SPACE) && p->health >= PLAYER_HEALTH){
				 p->health=0;
				if(NM_IsServer()){
					Map_Brush((int)p->pos.x,(int)p->pos.y,5,7);
				}else{
					char* msg = Map_SendExplosion((int)p->pos.x,(int)p->pos.y,7);
					NM_SendMessage(msg);
					free(msg);
				}
			}
		}

	if(nPos.x!=0 || nPos.y!=0){
		p->state = P_WALKING;

		if(p->time % 3 == 0){
			p->animationFrame = (++p->animationFrame > 5)? 0 : p->animationFrame;
			if(nPos.x > 0){
				p->dir = P_RIGHT;
			}else if(nPos.x < 0){
				p->dir = P_LEFT;
			}
		}
	}else{
		p->animationFrame = 0;
		p->state = P_IDLE;
	}

	return 1;
}

void Player_Delete(Player* player) {
	if (player == NULL) return;
	player->player_Sprite=NULL;
	player->player_Health=NULL;
	free(player);
	player = NULL;
}

void Player_Damage(Player* player, char damage){
	player->health-=damage;
	if(player->health < 0)
		player->health=0;
}

void Player_NetReadPos(Player* p, char* msg){
	Debug_LOG("Player_NetReadPos", "Inicializado");

	if(p == NULL) return ;
	Debug_LOG("Player_NetReadPos", "Existe");
	int i =0;
	if(msg[i]!= 'P'){
		Debug_LOG("Player_NetReadPos","Tag Errada!");
		return;
	}
	i+=sizeof(char);
	Vector2 pos;

	memcpy(&pos.x , msg + i , sizeof(int));
	i += sizeof(int);

	memcpy(&pos.y , msg + i , sizeof(int));

	p->newPos.x = (double)pos.x;
	p->newPos.y = (double)pos.y;
}

char* Player_NetSendPos(Player* p){
	char * msg = (char*)malloc(sizeof(int)*2 + sizeof(char)*2);
	Vector2 pos;
	pos.x = (int)p->pos.x;
	pos.y = (int)p->pos.y;

	int i =0;

	msg[i] = 'P';
	i += sizeof(char);

	memcpy(msg + i , &pos.x, sizeof(int));
	i += sizeof(int);

	memcpy(msg + i , &pos.y, sizeof(int));
	i += sizeof(int);

	msg[i]= '!';

	return msg;

}

void Player_Draw(Player* p){
	if(p==NULL){
		Debug_LOG("Player_Draw","Player Nulo!");
		return;
	}

	if(p->usable){
		Vector2d pos = vg_getScreenRes();
		pos.x = pos.x/2 - p->player_Health->s->dimensions.x*((double)p->health/(double)PLAYER_HEALTH)/2;
		pos.y -=p->player_Health->s->dimensions.y;
		pos = Vector2_Add(pos,vg_getCameraPos());


		p->player_Health->s->pos = pos;
		p->player_Health->rect.w = p->player_Health->s->dimensions.x*((double)p->health/(double)PLAYER_HEALTH);
	}
	Rectangle spritePos;
	spritePos.w=50;
	spritePos.h=50;

	switch(p->state){
	case P_IDLE:
		spritePos.x=0;
		spritePos.y=0;
		break;

	case P_WALKING:
		spritePos.y =0;
		spritePos.x = (p->animationFrame + 1)*50;

		break;
	}

	p->player_Sprite->pos.x=p->pos.x-50/2;
	p->player_Sprite->pos.y=p->pos.y-50;

	Rectangle hitbox;
	hitbox.w = 50;
	hitbox.h = 50;
	hitbox.x = p->pos.x - hitbox.w/2;
	hitbox.y = p->pos.y - hitbox.h;

	//Sprite_SubDraw(p->player_Sprite,0,0,50,50);
	Sprite_SubDraw(p->player_Sprite,spritePos.x,spritePos.y,spritePos.w,spritePos.h,p->dir);
	//Rectangle_Debug(hitbox, 0xFF00);
}
