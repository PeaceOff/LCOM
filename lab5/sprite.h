#ifndef _SPRITE_H
#define _SPRITE_H

struct Vector2{
	int x,y;
};

struct Vector2d{
	double x,y;
};

struct Sprite{
	struct Vector2d pos;
	struct Vector2d velo;

	struct Vector2 dimensions;
	char *map;
};

void Sprite_Move(struct Sprite *s);

struct Sprite *Sprite_Create(double x, double y, double vX, double vY, char *xpm[]);

void Sprite_Delete(struct Sprite *s);

void Sprite_Draw(struct Sprite *s);

#endif
