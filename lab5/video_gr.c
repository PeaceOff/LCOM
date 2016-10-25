#include <minix/syslib.h>
#include <minix/drivers.h>
#include <machine/int86.h>
#include <sys/mman.h>
#include <sys/types.h>
#include "lmlib.h"
#include "pixmap.h"
#include "vbe.h"
#include "sprite.h"

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

static unsigned h_res;		/* Horizontal screen resolution in pixels */
static unsigned v_res;		/* Vertical screen resolution in pixels */
static unsigned bits_per_pixel; /* Number of VRAM bits per pixel */

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

	*(video_mem + (x + y * h_res)) = (char)color;

	return 0;
}

int vg_drawSprite(struct Sprite *s){
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
			char color = (* (map+x+y*width));

			if(vg_setPixel(x+xi,y+yi,color)){
				return 1;
			}
		}
	}

}

int vg_clearSprite(struct Sprite *s){
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

int vg_clearScreen(unsigned short color){
	int x=0;
	int y=0;

	for (x; x < h_res; ++x) {
			y=0;
			for(y; y<v_res; ++y){

				if(vg_setPixel(x,y,(char)color)){
					return 1;
				}
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
	video_mem = vm_map_phys(SELF, (void *)mr.mr_base, h_res*v_res*bits_per_pixel/8);

	if(video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");

	/* retornar a posicao de memoria */
	return video_mem;
}

void vg_info(){
	printf("\nPhysical Adress %x \n", phys_mem);
}
