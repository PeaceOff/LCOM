#include "ball.h"
#include "networkmanager.h"
#include "texturemanager.h"
#include <minix/syslib.h>
#include "map.h"
#include "sprite.h"


Ball* Ball_Create(int x, int y){
	Ball * b = (Ball*)malloc(sizeof(Ball));
	b->pos.x = x;
	b->pos.y = y;
	Sprite* s = TM_GetTexture(SI_BALL);

	b->rect.w = s->dimensions.x;
	b->rect.h = s->dimensions.y;

	b->rect.x = b->pos.x-b->rect.w/2;
	b->rect.y = b->pos.y-b->rect.h/2;

	b->newPos = b->pos;

	b->direction.x = 0;
	b->direction.y = 1;
	b->velocity = 3;

	return b;
}

void Ball_SyncPos(Ball* b){
	Debug_LOG("Ball_SyncPos","Inicializado");

	Vector2 mapDim;
	mapDim = Map_getDimensions();

	if((int)b->newPos.x < 0 || (int) b->newPos.x >= mapDim .x ||(int)b->newPos.y < 0 || (int) b->newPos.y >= mapDim .y ){
		NM_SendMessage("R!");
		return;
	}
	Vector2d temp = Vector2_Lerp(b->pos, b->newPos, 0.5);
	b->pos = temp;
	Debug_LOGInt("Ball_SyncPos: temp X", (int)b->newPos.x);
	Debug_LOGInt("Ball_SyncPos: temp Y", (int)b->newPos.y);

}

void Ball_NetReadPos(Ball* b, char * msg){
	Debug_LOG("Ball_NetReadPos", "Inicializado");
	if(b == NULL) return;

	Debug_LOG("Ball_NetReadPos", "Existe");
	int i =0;
	if(msg[i]!= 'B'){
		Debug_LOG("Ball_NetReadPos","Tag Errada!");
		return;
	}
	i+=sizeof(char);
	Vector2 pos;

	memcpy(&pos.x , msg + i , sizeof(int));
	i += sizeof(int);

	memcpy(&pos.y , msg + i , sizeof(int));
	i += sizeof(int);
	Vector2 dim = Map_getDimensions();
	if(pos.x < 0 || pos.x >= dim.x || pos.y < 0 || pos.y >= dim.x){
		NM_SendMessage("R!");
		return;
	}

	if(msg[i] != 'F'){
		NM_SendMessage("R!");
		return;
	}
	b->newPos.x = (double)pos.x;
	b->newPos.y = (double)pos.y;
}

char* Ball_NetSendPos(Ball* b){
	char * msg = (char*)malloc(sizeof(int)*2 + sizeof(char)*3 + sizeof(double)*2);
	Vector2 pos;
	pos.x = (int)b->pos.x;
	pos.y = (int)b->pos.y;

	int i =0;

	msg[i] = 'B';
	i += sizeof(char);

	memcpy(msg + i , &pos.x, sizeof(int));
	i += sizeof(int);

	memcpy(msg + i , &pos.y, sizeof(int));
	i += sizeof(int);

	Vector2d dir = b->direction;

	msg[i]='F';
	i += sizeof(char);

	msg[i]= '!';
	return msg;
}

void Ball_Update(Ball* b){
	Debug_LOG("Ball_Update","Inicializado");
	Vector2d nPos = b->pos;
	Vector2d dir = b->direction;
	dir = Vector2_Normalize(dir);

	Vector2 col;

	if(NM_IsServer()){

		col.x = (int)(nPos.x + dir.x * b->velocity);
		col.y = (int)nPos.y;

		if(Map_CheckCollision(col.x, col.y)){
			dir.x = -dir.x;
			if(NM_IsServer())
				Map_Damage(col.x,col.y,3);
		}

		col.x = (int)nPos.x;
		col.y = (int)(nPos.y + dir.y * b->velocity);

		if(Map_CheckCollision(col.x ,col.y )){
			dir.y = -dir.y;
			if(NM_IsServer())
				Map_Damage(col.x,col.y,3);
		}

		dir = Vector2_Normalize(dir);
		b->direction = dir;
		b->pos.x += dir.x * b->velocity;
		b->pos.y += dir.y * b->velocity;

	}else{
		Ball_SyncPos(b);
	}

	b->rect.x = b->pos.x-b->rect.w/2;
	b->rect.y = b->pos.y-b->rect.h/2;
}

void Ball_Draw(Ball* b){
	Debug_LOG("Ball_DRAW","Inicializado");
	Sprite* s = TM_GetTexture(SI_BALL);

	Debug_LOGInt("Ball_Draw: PosX",b->rect.x );
	Debug_LOGInt("Ball_Draw: PosX",b->rect.y );



	s->pos.x = b->rect.x;
	s->pos.y = b->rect.y;
	Sprite_Draw(s);
	Rectangle_Debug(b->rect, 0x00FF);
}

void Ball_Delete(Ball* b){
	if( b == NULL ) return;
	free(b);
	b = NULL;
}
