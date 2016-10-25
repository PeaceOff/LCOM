#include "font.h"
#include "bmp.h"
#include "vector.h"
#include "stdio.h"
#include "debug.h"


static FontInfo* fonts;

Font* Font_Create(int x, int y, char* text, unsigned short color, enum font_type font){
	Font* f = (Font*)malloc(sizeof(Font));
	f->pos.x = x;
	f->pos.y = x;
	f->text = text;
	f->color = color;
	f->font = font;
}

char Font_Load(){
	if(fonts != NULL) return 1;
	fonts = (FontInfo*)malloc(sizeof(FontInfo)*FONT_TOTAL);

	if(fonts==NULL)
		return 0;

	BitmapInfoHeader info;
	fonts[FONT_BIG].fontImg = LoadBitmapFile("home/lcom/lcom1516-t6g05/tds/img/texto.bmp", &info);
	fonts[FONT_BIG].dimImg.x=info.width;
	fonts[FONT_BIG].dimImg.y=info.height;

	fonts[FONT_BIG].dimCh.x=fonts[FONT_BIG].dimImg.x/94;
	fonts[FONT_BIG].dimCh.y=info.height;

	fonts[FONT_NORMAL].fontImg = LoadBitmapFile("home/lcom/lcom1516-t6g05/tds/img/fontnormal.bmp", &info);
	fonts[FONT_NORMAL].dimImg.x=info.width;
	fonts[FONT_NORMAL].dimImg.y=info.height;

	fonts[FONT_NORMAL].dimCh.x=fonts[FONT_NORMAL].dimImg.x/94;
	fonts[FONT_NORMAL].dimCh.y=info.height;




	return 1;
}

void Font_Delete(Font* f){
	if(f == NULL) return;
	//if(f->text != NULL)
	//	free(f->text);
	free(f);
	f=NULL;
}

void Font_WriteFont(Font* f){
	Font_Write(f->pos.x,f->pos.y,f->text,f->color,f->font);
}

void Font_Write(int x, int y, char* text,unsigned short color, enum font_type font){
	if(Font_Load()){
		Vector2 dimImg = fonts[font].dimImg;
		Vector2 dimCh = fonts[font].dimCh;
		char* fontImg = fonts[font].fontImg;
		//vg_writeFont(x,y,fonts[font].dimImg.x,fonts[font].dimCh.x,fonts[font].dimCh.y,fonts[font].fontImg,text,color);
		vg_writeFont(x,y,dimImg.x,dimCh.x,dimCh.y,fontImg,text,color);
	}
}

