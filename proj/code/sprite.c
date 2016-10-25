#include "sprite.h"
#include "video_gr.h"
#include <minix/syslib.h>
#include "bmp.h"

Sprite *Sprite_Create(double x, double y, char * filename){
	Sprite *s = (Sprite *) malloc ( sizeof(Sprite));

	if(s==NULL)
		return NULL;
	s->destroy=0;
	s->pos.x=x;
	s->pos.y=y;
	s->velo.x=0;
	s->velo.y=0;
	BitmapInfoHeader info;

	s->map = LoadBitmapFile(filename,&info);

	s->dimensions.x=info.width;
	s->dimensions.y=info.height;

	printf("\nSprite created!: %d x %d", s->dimensions.x , s->dimensions.y);
	int i;
	for(i = 0 ; i < s->dimensions.x* s->dimensions.y ; i++){
		unsigned short color = (s->map[i*2] << 4) | s->map[i*2+1];
		//printf("0x%x-",color);
	}

	return s;
}

SubSprite* SubSprite_Create(double x, double y, char* filename){
	SubSprite* s = (SubSprite*) malloc(sizeof(SubSprite));
	s->s = Sprite_Create(x,y,filename);
	s->rect.x = 0;
	s->rect.y = 0;
	s->rect.w = s->s->dimensions.x;
	s->rect.h = s->s->dimensions.y;

	return s;
}


void SubSprite_Delete(SubSprite* s){
	if(s==NULL) return;
	if(s->s != NULL){
		Sprite_Delete((Sprite*)s->s);
	}
	free(s);
	s=NULL;
}

void Sprite_Delete(Sprite *s){
	if(s==NULL)
		return;
	free(s->map);

	free(s);
	s=NULL;
}

void SubSprite_Draw( SubSprite *s){
	Sprite_SubDraw(s->s,(int)s->rect.x,(int)s->rect.y,(int)s->rect.w,(int)s->rect.h,0);
}

void Sprite_SetPosition(Sprite *s, double x, double y){
	s->pos.x = x;
	s->pos.y = y;
}

void Sprite_SetVelocity(Sprite *s, double x, double y){
	s->velo.x = x;
	s->velo.y = y;
}

void Sprite_Draw(Sprite *s){
	vg_drawSprite(s);
}

void Sprite_SubDraw(Sprite *s, double x, double y, double w, double h, char reverse){
	vg_drawSubSprite(s,x,y,w,h, reverse);
}

void Sprite_Move(Sprite *s){
	s->pos.x +=s->velo.x;
	s->pos.y +=s->velo.y;
}
