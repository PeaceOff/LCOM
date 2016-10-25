#ifndef __VIDEO_GR_H
#define __VIDEO_GR_H

#include "sprite.h"
/** @defgroup video_gr video_gr
 * @{
 *
 * Functions for outputing data to screen in graphics mode
 */

/**
 * @brief Initializes the video module in graphics mode
 * 
 * Uses the VBE INT 0x10 interface to set the desired
 *  graphics mode, maps VRAM to the process' address space and
 *  initializes static global variables with the resolution of the screen, 
 *  and the number of colors
 * 
 * @param mode 16-bit VBE mode to set
 * @return Virtual address VRAM was mapped to. NULL, upon failure.
 */
void *vg_init(unsigned short mode);

Vector2d vg_getCameraPos();

Vector2d vg_getScreenRes();

int vg_clearScreen(unsigned int color);

int vg_setPixel(unsigned short x, unsigned short y, unsigned int color);

void vg_drawCrosshair(int x1, int x2, int w, unsigned long color);

int vg_drawXPM(unsigned short xi, unsigned short yi, char* xpm[]);

void vg_swapBuff();

void vg_info();
void vg_free();

void vg_writeFont(int x, int y,int wt, int w, int h,char* fontImg, char*text ,unsigned short color);

void vg_drawMap(char* imgBuff, int w, int h, int c);

int vg_drawSquare(int x1, int x2, int w, unsigned int color);

int vg_drawRectangle(int x, int y, int w, int h,  unsigned short color);

int vg_drawRectangleOutline(int x, int y, int w, int h,  unsigned short color);

void vg_setCameraPos(int x, int y);

int vg_clearSprite(Sprite *s);

int vg_drawSprite(Sprite *s);

void vg_drawSubSprite(Sprite *s, double x1, double y1, double w, double h, char reverse);


char* vg_xpmToPixmap(char *xpm[], int *wd, int *ht);

 /**
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
