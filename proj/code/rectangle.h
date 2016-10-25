#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "vector.h"

/** @defgroup Rectangle Rectangle module
 * @{
 *
 * Module responsible for rectangles, map collision, collisions in general
 *
 */

/** @name Rectangle structure */
/** @{
 *
 * Rectangle structure
 */
typedef struct{
	double x,y,w,h;
}Rectangle;
/** @} end of Rectangle */

/**
 * @brief Function to create a Rectangle
 *
 *This function is used to create a new Rectangle it allocs memory for the new instance
 *
 * @param x Abcissa of the top left corner
 * @param y Coordenate of the top left corner
 * @param w Weidth of the rectangle
 * @param h Height of the rectangle
 *
 * @return Returns the new Rectangle created
 */
Rectangle Rectangle_Create(double x, double y, double w, double h);

/**
 * @brief Function to check the terrain Collision
 *
 *This function checks if a given rectangle is colliding with the map
 *
 * @param r Rectangle to be checked
 *
 * @return 1 if positive or 0 if false
 */
inline char Rectangle_CheckTerrainCollision(Rectangle r);

/**
 * @brief Function to check the collision between two given rectangles
 *
 *
 * @param r1 Rectangle to be checked with r2
 * @param r2 Rectangle to be checked with r1
 *
 * @return 1 if positive or 0 if false
 */
inline char Rectangle_CheckCollision(Rectangle r1, Rectangle r2);

/**
 * @brief Function to check the collision between a rectangle a vector (point)
 *
 *This function is used for example with the position of the mouse and the boxes on the menu
 *
 * @param r Rectangle to be checked
 * @param point Vector to check the collision
 *
 * @return 1 if positive or 0 if false
 */
inline char Rectangle_CheckPointCollision(Rectangle r, Vector2d point);

/**
 * @brief Function to draw a Rectangle
 *
 *This function draw a rectangle on the screen it is used for degub porpuses
 *
 * @param r Rectangle to be drawn
 * @param color color with which the rectangle will be drawn
 *
 * @return void
 */
inline void Rectangle_Debug(Rectangle r,  unsigned short color);


#endif
