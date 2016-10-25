#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "lmlib.h"
#include "pixmap.h"
#include "vbe.h"
#include "sprite.h"
#include "vector.h"

static Vector2 cameraPos;

static mmap_t graphic_info;
/* Constants for VBE 0x105 mode */

/* The physical address may vary from VM to VM.
 * At one time it was 0xD0000000
 *  #define VRAM_PHYS_ADDR    0xD0000000 
 * Currently on lab B107 is 0xF0000000
 * Better run my version of lab5 as follows:
 *     service run `pwd`/lab5 -args "mode 0x105"
 */
#define VRAM_PHYS_ADDR	0xE0000000
#define H_RES             1024
#define V_RES		  768
#define BITS_PER_PIXEL	  8

/* Private global variables */



static phys_bytes phys_mem;
static char *video_mem;		/* Process address to which VRAM is mapped */
static char *video_mem1;		/* Process address to which VRAM is mapped */
static char *video_mem2;		/* Process address to which VRAM is mapped */


static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */
static char videoMode;

int vg_exit() {
  struct reg86u reg86;

  reg86.u.b.intno = 0x10; /* BIOS video services */

  reg86.u.b.ah = 0x00;    /* Set Video Mode function */
  reg86.u.b.al = 0x03;    /* 80x25 text mode*/

  if( sys_int86(&reg86) != OK ) {
      printf("\tvg_exit(): sys_int86() failed \n");
      return 1;
  } else
      return 0;
}

int vg_setPixel(unsigned short x, unsigned short y, unsigned long color){
	if(x>=h_res || y>=v_res)
		return 1;

	int id = (x + y * h_res) * bits_per_pixel/8;
	video_mem2[id]= (char)(color & 0xFF);
	video_mem2[id +1] = (char)((color >> 8) & 0xFF);

	return 0;
}

void vg_drawCrosshair(int x1, int x2, int w, unsigned long color){
	int x=0;
	for (x; x < w; ++x) {
		int y=0;
		for(y; y<w; ++y){
			if((y < 4 && x <=7 && x >= 5) || (y > 8 && x <=7 && x >= 5) || (x < 4 && y <=7 && y >= 5) || (x > 8 && y <=7 && y >= 5))
				vg_setPixel(x+(x1-cameraPos.x),y+(x2-cameraPos.y),color);
		}
	}
}

void vg_swapBuff(){
	memcpy(video_mem, video_mem2, h_res * v_res *bits_per_pixel/8);
}

void vg_setCameraPos(int x, int y){
	cameraPos.x=x;
	cameraPos.y=y;
}

Vector2d vg_getScreenRes(){
	Vector2d temp;
	temp.x = h_res;
	temp.y = v_res;

	return temp;
}

Vector2d vg_getCameraPos(){
	Vector2d temp;
	temp.x = cameraPos.x;
	temp.y = cameraPos.y;
	return temp;
}

void vg_writeFont(int x, int y,int wt, int w, int h,char* fontImg, char*text ,unsigned short color){
	Debug_LOG("vg_writeFont","Inicializado");
	int nX, nY;
	int i=0;


	while(text[i]!='\0'){
		Debug_LOG("vg_writeFont","Caracter Encontrado!");
		Debug_LOGInt("vg_writeFont: Caracter",text[i]);

		unsigned char letter = (unsigned char)text[i] -33;
		Debug_LOGInt("vg_writeFont: Letra",letter);

		if(letter < 0 || letter> 126){
			i++;
			continue;
		}

		for(nX=0; nX <w ; nX++ ){
			for(nY=0; nY<h ; nY++){

				unsigned int imgPos = (letter*w+nX)*2 + nY*wt*2;

				if((unsigned char)fontImg[imgPos]== 0xFF && (unsigned char)fontImg[imgPos+1] == 0xFF) continue;

				if(nX+x+(i*w)<0 || nY+y<0 || nX+x+(i*w)>=h_res || nY+y>=v_res) continue;

				unsigned int buffPos = (x+(w*i) + nX)*2 + (y+nY)*h_res*2;
				video_mem2[buffPos] = (unsigned char)(color);
				video_mem2[buffPos+1] = (unsigned char)(color>>8);
			}
		}
		i++;
	}

}

void vg_drawMap(char* imgBuff, int w, int h, int c){

	Vector2 posC;
	posC.x = -cameraPos.x;
	posC.y = -cameraPos.y;
	h_res=800;
	v_res=600;
	Vector2 rI;
	Vector2 rC;
	if(posC.x >= (signed)h_res || posC.y >= (signed)v_res ||  w < cameraPos.x || h < cameraPos.y){
		return;
	}
	rI.x=0;
	rI.y=0;
	rC.x=posC.x;
	rC.y=posC.y;

	if(posC.x < 0){
		rI.x = -posC.x;
		rC.x=0;
	}

	if(posC.y < 0){
		rI.y = -posC.y;
		rC.y=0;
	}

	Vector2 dimN;

	if(w >= cameraPos.x + (signed)h_res){

		dimN.x =(cameraPos.x+ (signed)h_res) - rI.x;
	}else{
		dimN.x = w - rI.x;
	}
	if(h >= cameraPos.y + (signed)v_res)
		dimN.y = (cameraPos.y + (signed)v_res) - rI.y;
	else
		dimN.y = h - rI.y;

	int y;
	for(y=0; y<dimN.y; y++){
		unsigned int buffPos= rI.x * 2 + (rI.y+y) * w *2;
		unsigned int memePos= rC.x * 2 + (rC.y+y) * h_res * 2;
		memcpy(video_mem2 + memePos, imgBuff + buffPos , dimN.x*2);
	}
}

int vg_drawSquare(int x1, int x2, int w, unsigned int color){

	int x=0;
	for (x; x < w; ++x) {
		int y=0;
		for(y; y<w; ++y){
			vg_setPixel(x+(x1-cameraPos.x),y+(x2-cameraPos.y),color);
		}
	}
	return 0;
}

int vg_drawRectangle(int x, int y, int w, int h,  unsigned short color){
	int x1=x;
	int x2=y;
	for (x = 0; x < w; ++x) {
		for(y = 0; y<h; ++y){
			vg_setPixel(x+(x1-cameraPos.x),y+(x2-cameraPos.y),color);
		}
	}
	return 0;
}

int vg_drawRectangleOutline(int x, int y, int w, int h,  unsigned short color){
	int x1=x;
	int x2=y;
	for (x = 0; x < w; ++x) {
		for(y = 0; y<h; ++y){
			if(x == 0){
				vg_setPixel(x+(x1-cameraPos.x),y+(x2-cameraPos.y),color);
				continue;
			}
			if(x == (w - 1)){
				vg_setPixel(x+(x1-cameraPos.x),y+(x2-cameraPos.y),color);
				continue;
			}
			if(y == 0 || y == (h-1))
				vg_setPixel(x+(x1-cameraPos.x),y+(x2-cameraPos.y),color);
		}
	}
	return 0;
}

void vg_drawSubSprite(Sprite *s, double x1, double y1, double w, double h, char reverse){
	int imgWidth = s->dimensions.x;

	int width = (int)w;
	if((int)(w+x1) > s->dimensions.x)
		width = s->dimensions.x - x1;
	int height = (int)h;
	if((int)(h+y1) > s->dimensions.y)
		height = s->dimensions.y - y1;

	char* map = s->map;
	if(map == NULL)
		return ;

	int xi = s->pos.x;
	int yi = s->pos.y;

	int x0=(int)x1;
	int y0=(int)y1;
	int x,y;
	for (x = 0; x < width; ++x) {
		for(y = 0; y< height; ++y){
			int xT=x;
			if(reverse)
				x= width-x-1;

			unsigned char l1 = map[(x+x0+(y+y0)*imgWidth)*2 +1];
			unsigned char l2 = map[(x+x0+(y+y0)*imgWidth)*2];

			if((l1 & 0xff) == 0xff && (l2 & 0xff) == 0xff){
				x=xT;
				continue;
			}

			unsigned short color = (l1 << 8 ) | l2;
			vg_setPixel(xT+(xi-cameraPos.x),y+(yi-cameraPos.y),color);
			x=xT;
		}
	}
}

int vg_drawSprite(Sprite *s){
	int width = s->dimensions.x;
	int height = s->dimensions.y;
	char* map = s->map;

	if(map == NULL)
		return 1;

	int xi = s->pos.x;
	int yi = s->pos.y;

	int x,y;

	for (x; x < width; ++x) {
		y=0;
		for(y; y<height; ++y){
			unsigned char l1 = map[(x+y*width)*2 +1];
			unsigned char l2 = map[(x+y*width)*2];
			if((l1 & 0xff) == 0xff && (l2 & 0xff) == 0xff)
				continue;
			unsigned short color = (l1 << 8 ) | l2;
			vg_setPixel(x+(xi-cameraPos.x),y+(yi-cameraPos.y),color);
		}
	}

}

int vg_clearSprite(Sprite *s){
	int width = s->dimensions.x;
	int height = s->dimensions.y;
	char* map = s->map;

	int xi = s->pos.x;
	int yi = s->pos.y;

	int x=0;
	int y=0;

	for (x; x < width; ++x) {
		y=0;
		for(y; y<height; ++y){
			char color = 0;
			if(vg_setPixel(x+xi,y+yi,color)){
				return 1;
			}
		}
	}

}

int vg_clearScreen(unsigned int color){
	int x=0;
	int y=0;

	for (x; x < h_res; ++x) {
		y=0;
		for(y; y<v_res; ++y){

			vg_setPixel(x,y,color);
		}
	}
}

char* vg_xpmToPixmap(char *xpm[], int *wd, int *ht){

	return (char *)read_xpm(xpm, wd, ht,h_res,v_res);

}

int vg_drawXPM(unsigned short xi, unsigned short yi, char* xpm[]){
	int x = 0;
	int y = 0;

	int width;
	int height;
	char * map;
	map = (char *)read_xpm(xpm, &width, &height,h_res,v_res);


	for (x; x < width; ++x) {
		y=0;
		for(y; y<height; ++y){
			char color = (* (map+x+y*width));

			if(vg_setPixel(x+xi,y+yi,color)){
				free(map);
				return 1;
			}
		}
	}
	free(map);
	return 0;
}


void *vg_init(unsigned short mode){
	void* add;

	struct reg86u reg;
	reg.u.w.ax = 0x4F02; // Set VBE MODE
	reg.u.w.bx = 1<<14 | mode; // set bit 14: linear framebuffer
	reg.u.b.intno = 0x10;
	if( sys_int86(&reg) != OK ) {
		printf("set_vbe_mode: sys_int86() failed \n");
		return NULL;
	}

	if(reg.u.b.ah != 0){
		printf("set_vbe_mode failed");
		return NULL;
	}
	vbe_mode_info_t info;
	vbe_get_mode_info(mode,&info);


	h_res = info.XResolution;
	v_res = info.YResolution;

	bits_per_pixel=info.BitsPerPixel;


	int r;
	struct mem_range mr;
	//LAB1
	/* Habilitar mapeamento de memoria*/
	phys_mem=(phys_bytes)info.PhysBasePtr;
	mr.mr_base = (phys_bytes)info.PhysBasePtr;
	mr.mr_limit = mr.mr_base + h_res * v_res *bits_per_pixel/8;

	if( OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Mapeamento de memoria */
	//video_mem = vm_map_phys(SELF, (void *)mr.mr_base, h_res*v_res*bits_per_pixel/8);

	//video_mem2= vm_map_phys(SELF, (void *)mr.mr_base, h_res*v_res*bits_per_pixel/8);
	video_mem2= (char*)malloc(h_res*v_res*bits_per_pixel/8);
	video_mem1= vm_map_phys(SELF, (void *)mr.mr_base, h_res*v_res*bits_per_pixel/8);

	/* retornar a posicao de memoria */
	video_mem=video_mem1;
	cameraPos.x=0;
	cameraPos.y=0;
	return video_mem;
}

void vg_info(){
	printf("\nPhysical Adress %x \n", phys_mem);
}
