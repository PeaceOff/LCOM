#include "sprite.h"
#include "video_gr.h"
#include <minix/syslib.h>

struct Sprite *Sprite_Create(double x, double y, double vX, double vY, char *xpm[]){
	struct Sprite *s = (struct Sprite *) malloc ( sizeof(struct Sprite));

	if(s==NULL)
		return NULL;

	s->pos.x=x;
	s->pos.y=y;
	s->velo.x=vX;
	s->velo.y=vY;

	s->map = vg_xpmToPixmap(xpm,&s->dimensions.x,&s->dimensions.y);
	return s;
}

void Sprite_Delete(struct Sprite *s){
	if(s==NULL)
		return;

	if(s->map !=NULL)
		free(s->map);

	free(s);
	s=NULL;
}

void Sprite_Draw(struct Sprite *s){
	vg_drawSprite(s);
}

void Sprite_Move(struct Sprite *s){
	s->pos.x +=s->velo.x;
	s->pos.y +=s->velo.y;
}
