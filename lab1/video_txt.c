#include <minix/drivers.h>
#include <sys/video.h>
#include <sys/mman.h>

#include <assert.h>

#include "vt_info.h"

#include "video_txt.h"

/* Private global variables */

static char *video_mem; /* Address to which VRAM is mapped */

static unsigned scr_width; /* Width of screen in columns */
static unsigned scr_lines; /* Height of screen in lines */

void vt_fill(char ch, char attr) {

	char* mem = video_mem;
	int i;
	for (i = 0; i < scr_width * scr_lines; ++i) {
		mem[2 * i] = ch;
		mem[2 * i + 1] = attr;
	}
}

void vt_blank() {

	char* mem = video_mem;
	int i;
	for (i = 0; i < scr_width * scr_lines; i++) {
		mem[2 * i] = 0x00;
		mem[2 * i + 1] = 0x00;
	}

}

int vt_print_char(char ch, char attr, int r, int c) {
	if(!( (c + (scr_width * r)) <= scr_width * scr_lines && c>=0 && r>=0) )
		return 1;

	char* mem = video_mem;
	mem[2 * (c + (scr_width * r))] = ch;
	mem[2 * (c + (scr_width * r)) + 1] = attr;
	return 0;

}

int vt_print_string(char *str, char attr, int r, int c) {

	if(!( (c + (scr_width * r)) <= scr_width * scr_lines && c>=0 && r>=0) )
		return 1;

	char* mem = video_mem;

	mem += 2 * (c + scr_width * (r));

	while (*str != 0) {

		mem[0] = (*str);
		mem++;
		mem[0] = attr;
		mem++;
		str++;
	}

	return 0;
}

int vt_print_int(int num, char attr, int r, int c) {

	if(!( (c + (scr_width * r)) <= scr_width * scr_lines && c>=0 && r>=0) )
		return 1;

	char str[25];
	sprintf(str, "%d", num);
	vt_print_string(str,attr,r,c);
	return 0;

}

int vt_draw_frame(int width, int height, char attr, int r, int c) {

	if(width + c > scr_width || height + r > scr_lines || r<0 || c<0)
		return 1;

	int i;
	char* mem = video_mem;
	mem += 2 * (c + r * scr_width);
	for (i = 0; i < width; ++i) {

		mem[0] = 0xCD;
		if (i == 0)
			mem[0] = 0xC9;
		else if (i == width - 1)
			mem[0] = 0xBB;

		mem++;
		mem[0] = attr;
		mem++;
	}

	for (i = 1; i < height - 1; ++i) {
		mem = video_mem;
		mem += 2 * (c + (r + i) * scr_width);
		mem[0] = 0xBA;
		mem++;
		mem[0] = attr;

		mem += (width - 1) * 2 - 1;
		mem[0] = 0xBA;
		mem++;
		mem[0] = attr;

	}
	mem = video_mem;
	mem += (c + (r + height - 1) * scr_width) * 2;
	for (i = 0; i < width; ++i) {

		mem[0] = 0xCD;
		if (i == 0)
			mem[0] = 0xC8;
		else if (i == width - 1)
			mem[0] = 0xBC;

		mem++;
		mem[0] = attr;
		mem++;
	}

	return 0;

}

/*
 * THIS FUNCTION IS FINALIZED, do NOT touch it
 */

char *vt_init(vt_info_t *vi_p) {

	int r;
	struct mem_range mr;

	/* Allow memory mapping */

	mr.mr_base = (phys_bytes)(vi_p->vram_base);
	mr.mr_limit = mr.mr_base + vi_p->vram_size;

	if (OK != (r = sys_privctl(SELF, SYS_PRIV_ADD_MEM, &mr)))
		panic("video_txt: sys_privctl (ADD_MEM) failed: %d\n", r);

	/* Map memory */

	video_mem = vm_map_phys(SELF, (void *) mr.mr_base, vi_p->vram_size);

	if (video_mem == MAP_FAILED)
		panic("video_txt couldn't map video memory");

	/* Save text mode resolution */

	scr_lines = vi_p->scr_lines;
	scr_width = vi_p->scr_width;

	return video_mem;
}
