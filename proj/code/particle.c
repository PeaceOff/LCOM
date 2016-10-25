#include "particle.h"
#include <minix/syslib.h>


Particle* Particle_Create(int x, int y,char destroy, unsigned int index){
	Particle* res = (Particle*)malloc(sizeof(Particle));

	res->sub.s = TM_GetTexture(index);
	res->pos.x = (double)x;
	res->pos.y = (double)y;

	res->subDiv.h = res->sub.s->dimensions.y;
	res->subDiv.w = res->subDiv.h;
	res->imgPos = 0;
	res->destroyOnEnd = destroy;
	res->time = 0;
	res->frames = (int)(res->sub.s->dimensions.x/res->subDiv.w);

	return res;
}

int Particle_Update(Particle* p){
	if(p == NULL) return 0;

	p->time ++;

	if(p->time % 4 == 0){
		p->time = 0;
		if( (p->imgPos == (p->frames -1)) && p->destroyOnEnd){
			Debug_LOG("Particle_Update","Destruir");
			return 0;
		}
		p->imgPos = (++p->imgPos >= p->frames)? 0 : p->imgPos;
	}
	return 1;
}

void Particle_Draw(Particle* p){
	if(p == NULL) return;
	if(p->sub.s == NULL) return;
	Rectangle draw;
	draw = p->subDiv;
	draw.y = 0;
	draw.x = p->imgPos*draw.w;

	if(draw.x+draw.w > p->sub.s->dimensions.x)
		return;

	p->sub.rect = draw;

	Vector2d pos;
	pos.x = p->pos.x - draw.w/2;
	pos.y = p->pos.y - draw.h/2;

	p->sub.s->pos = pos;

	SubSprite_Draw(&p->sub);
}

void Particle_Delete(Particle* p){
	if(p == NULL) return;
	p->sub.s = NULL;

	free(p);
	p=NULL;
}
