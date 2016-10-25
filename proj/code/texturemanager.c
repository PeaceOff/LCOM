#include "texturemanager.h"
#include "stdio.h"

static Sprite** sprites;

void TM_Init(){
	sprites = (Sprite**)malloc(sizeof(Sprite*)*SI_TOTAL);
	memset(sprites,NULL,SI_TOTAL);
}

void TM_AddTexture(unsigned int index, char * fileDir){
	if(index< SI_TOTAL)
		sprites[index] =Sprite_Create(0,0,fileDir);
}

Sprite* TM_GetTexture(unsigned int index){
	if(index >= SI_TOTAL) return NULL;
	return sprites[index];
}

void TM_Exit(){
	int i;
	for(i=0 ; i < SI_TOTAL ; i++){
		if(sprites[i]!=NULL)
			Sprite_Delete(sprites[i]);
		sprites[i]=NULL;
	}
	free(sprites);
	sprites=NULL;
}
