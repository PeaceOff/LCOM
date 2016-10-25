#ifndef _FONT_H
#define _FONT_H

#include "vector.h"

enum font_type{
	FONT_BIG,
	FONT_NORMAL,
	FONT_TOTAL
};

typedef struct{
	Vector2 pos;
	char* text;
	unsigned short color;
	enum font_type font;
} Font;

typedef struct{

	char* fontImg;
	Vector2 dimImg;
	Vector2 dimCh;

} FontInfo;

void Font_Delete(Font* f);

void Font_WriteFont(Font* f);

void Font_Write(int x, int y, char* text,unsigned short color, enum font_type font);

#endif
