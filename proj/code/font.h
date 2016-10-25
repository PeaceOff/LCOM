#ifndef _FONT_H
#define _FONT_H

#include "vector.h"
/** @defgroup Font Font module
 * @{
 *
 * Module responsible for fonts
 *
 */


enum font_type{
	FONT_BIG,
	FONT_NORMAL,
	FONT_TOTAL
};

/** @name Font structure */
/** @{
 *
 * Font structure
 */
typedef struct{
	Vector2 pos;
	char* text;
	unsigned short color;
	enum font_type font;
} Font;
/** @} end of Rectangle */

/** @name FontInfo structure */
/** @{
 *
 * FontInfo structure
 */
typedef struct{
	char* fontImg;
	Vector2 dimImg;
	Vector2 dimCh;
}FontInfo;
/** @} end of FontInfo */

/**
 * @brief Function to create a Font
 *
 *This function is used to create a new Font it allocs memory for the new instance
 *
 * @param x Abcissa of the top left corner
 * @param y Coordenate of the top left corner
 * @param text text of the font
 * @param color color of the font
 * @param font type of font
 *
 * @return Returns a pointer to the new font created
 */
Font* Font_Create(int x, int y, char* text, unsigned short color, enum font_type font);

/**
 * @brief Function to delete a Font
 *
 *This function is used to create a new Font it frees the memory of the instance
 *
 * @param f font to be deleted
 *
 * @return void
 */
void Font_Delete(Font* f);

/**
 * @brief Function to write a Font
 *
 *This function is used to write a font on the screen
 *
 * @param f font to be written
 *
 * @return void
 */
void Font_WriteFont(Font* f);

/**
 * @brief Function to write a Font
 *
 *This function loads the fonts sprite sheet and checks it before writing the font to the screen
 *
 * @param x Abcissa of the top left corner
 * @param y Coordenate of the top left corner
 * @param text Text of the font to be written
 * @param color Color of the font to be written
 * @param font Type of the font to be written
 *
 * @return void
 */
void Font_Write(int x, int y, char* text,unsigned short color, enum font_type font);

#endif
