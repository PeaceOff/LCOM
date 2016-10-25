#include "bmp.h"
#include "stdio.h"

unsigned char *LoadBitmapFile(char *filename, BitmapInfoHeader *bitmapInfoHeader)
{

	FILE* fch;

	fch = fopen(filename , "rb");

	BitmapFileHeader header;

	fread(&header, sizeof(BitmapFileHeader), 1, fch);

	if(header.type == 'MB'){
		printf("Tipo Certo\n");
		//return NULL;
	}

	printf("\nReservado: %d , %d",header.reserved, header.reserved2);

	BitmapInfoHeader infoHeader;
	fread(&infoHeader , sizeof(BitmapInfoHeader), 1 , fch);

	char* map = (char*)malloc(infoHeader.width * infoHeader.height * 2);
	*bitmapInfoHeader = infoHeader;

	printf("\nTamanho: %d x %d \n BPP: %d\n", infoHeader.width, infoHeader.height,infoHeader.bits);
	int i;
	unsigned int size =infoHeader.width* infoHeader.height * 2;
	char* temp = (char*)malloc(size);

	fseek(fch,header.offset, SEEK_SET);

	fread(temp, 1, size,fch);

	int i2 =0;
	int x1;
	int y1;
	//Inverter ordem;
	for( x1 = 0 ; x1 < infoHeader.width ; x1++){
		int y2 = 0;
		for( y1 = infoHeader.height-1 ; y1 >= 0; y1--){
			map[(x1 + y2*infoHeader.width)*2 +1] = temp[(x1 + y1*infoHeader.width)*2 +1];
			map[(x1 + y2*infoHeader.width)*2] = temp[(x1 + y1*infoHeader.width)*2];
			y2++;
		}
	}

	if(temp!=NULL);
		free(temp);
	temp=NULL;

	fclose(fch);

	return map;
}
