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

/**
 * @brief Function to get the camera position
 *
 * @return the position of the camera in form of Vector2d
 */
Vector2d vg_getCameraPos();
/**
 * @brief Function to get the screen resolution
 *
 * @return the resolution of the screen in form of Vector2d
 */
Vector2d vg_getScreenRes();

/**
 * @brief Function to Clear the screen
 * @param color the color to clear the screen
 * @return Returns 1 on success, 0 otherwise
 */
int vg_clearScreen(unsigned int color);
/**
 * @brief Function to Set the color of a pixel
 * @param x the x coord of the pixel
 * @param y the y coord of the pixel
 * @param color the color of the pixel
 * @return Returns 1 on success, 0 otherwise
 */
int vg_setPixel(unsigned short x, unsigned short y, unsigned int color);
/**
 * @brief Function to Draw the crosshair
 * 
 * @param x1 the x coord of the crosshair
 * @param y1 the y coord of the crosshair
 * @param w the width of the crosshair
 * @param color the color of the pixel
 * @return void
 */
void vg_drawCrosshair(int x1, int x2, int w, unsigned long color);
/**
 * @brief Function to Draw XMP formats
 * 
 * @param xi the x coord to draw
 * @param yi the y coord to draw
 * @param xmp the xmp to draw
 * @return Returns 1 on success, 0 otherwise
 */
int vg_drawXPM(unsigned short xi, unsigned short yi, char* xpm[]);
/**
 * @brief Function to SwapBuffers
 * Function to Swap the Draw buffer to the Screen Buffer
 * @return void
 */
void vg_swapBuff();
/**
 * @brief Function to Print the Physical Adress
 * @return void
 */
void vg_info();

/**
 * @brief Function to Write a font to the screen
 * @param x the x coord
 * @param y the y coord
 * @param wt the width of the bmp
 * @param w the width of a character
 * @param h the height of the character
 * @param fontImg the image data
 * @param text the string to write
 * @param color the color of the font
 * @return void
 */
void vg_writeFont(int x, int y,int wt, int w, int h,char* fontImg, char*text ,unsigned short color);

/**
 * @brief Function to Draw the map
 * @param imgBuff the buffer of the map
 * @param w the with of the map
 * @param h the height of the map
 * @param c the width in pixels of the map
 * @return void
 */
void vg_drawMap(char* imgBuff, int w, int h, int c);
/**
 * @brief Function to Draw a square
 * @param x1 the x coord
 * @param x2 the y coord
 * @param w the width of the square
 * @param color the color of the square
 * @return Returns 0 if success, 0 otherwise
 */
int vg_drawSquare(int x1, int x2, int w, unsigned int color);
/**
 * @brief Function to Draw a rectangle
 * @param x the x coord
 * @param y the y coord
 * @param w the width of the square
 * @param h the width of the square
 * @param color the color of the square
 * @return Returns 0 if success, 0 otherwise
 */
int vg_drawRectangle(int x, int y, int w, int h,  unsigned short color);
/**
 * @brief Function to Draw Rectangle Outline
 * @param x the x coord
 * @param y the y coord
 * @param w the width of the square
 * @param h the width of the square
 * @param color the color of the square
 * @return Returns 0 if success, 0 otherwise
 */
int vg_drawRectangleOutline(int x, int y, int w, int h,  unsigned short color);
/**
 * @brief Function to Set the Camera Position
 * @param x the x coord
 * @param y the y coord
 * @return void
 */
void vg_setCameraPos(int x, int y);
/**
 * @brief Function to clear a sprite of the screen
 * @param s sprite to clear
 * @return Returns 0 if success, 0 otherwise
 */
int vg_clearSprite(Sprite *s);
/**
 * @brief Function to Draw a Sprite
 * @param s sprite to draw
 * @return Returns 0 if success, 0 otherwise
 */
int vg_drawSprite(Sprite *s);
/**
 * @brief Function to Draw a SubSprite
 * @param s sprite to draw the SubSprite
 * @param x1 the x coord
 * @param y1 the y coord
 * @param w the width
 * @param h the height
 * @param reverse if sprite is draw reversed or not
 * @return void
 */
void vg_drawSubSprite(Sprite *s, double x1, double y1, double w, double h, char reverse);

/**
 * @brief Function to Convert an XMP to a PixMap
 * @param xpm the xpm to convert
 * @param wd the address to store the width of the xmp
 * @param ht the address to store the height of the xmp  

 * @return void  
 */
char* vg_xpmToPixmap(char *xpm[], int *wd, int *ht);

 /** 
 * @brief Returns to default Minix 3 text mode (0x03: 25 x 80, 16 colors)
 * 
 * @return 0 upon success, non-zero upon failure
 */
int vg_exit(void);

 /** @} end of video_gr */
 
#endif /* __VIDEO_GR_H */
