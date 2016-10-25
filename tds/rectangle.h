#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "vector.h"

typedef struct{
	double x,y,w,h;
}Rectangle;

Rectangle Rectangle_Create(double x, double y, double w, double h);

inline char Rectangle_CheckTerrainCollision(Rectangle r);

inline char Rectangle_CheckCollision(Rectangle r1, Rectangle r2);

inline char Rectangle_CheckPointCollision(Rectangle r, Vector2d point);

inline void Rectangle_Debug(Rectangle r,  unsigned short color);


#endif
