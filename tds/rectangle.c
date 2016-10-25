#include "rectangle.h"
#include "map.h"

Rectangle Rectangle_Create(double x, double y, double w, double h){
	Rectangle r;
	r.x=x;
	r.y=y;
	r.w=w;
	r.h=h;
	return r;
}

inline void Rectangle_Debug(Rectangle r, unsigned short color){
	vg_drawRectangle((int)r.x,(int)r.y,(int)r.w,(int)r.h , color);
}

inline char Rectangle_CheckTerrainCollision(Rectangle r){

	if(Map_CheckCollision((int)r.x,(int)r.y))
		return 1;
	if(Map_CheckCollision((int)(r.x + r.w),(int)r.y))
		return 1;
	if(Map_CheckCollision((int)r.x,(int)(r.y + r.h)))
		return 1;
	if(Map_CheckCollision((int)(r.x + r.w),(int)(r.y + r.h)))
		return 1;

	return 0;

}

inline char Rectangle_CheckPointCollision(Rectangle r, Vector2d point){
	if(point.x < r.x || point.x > r.x+r.w || point.y < r.y || point.y > r.y+r.h)
		return 0;
	return 1;

}


inline char Rectangle_CheckCollision(Rectangle r1, Rectangle r2){
	if(r1.x > r2.x + r2.w || r1.y > r2.y + r2.h || r1.x+r1.w < r2.x || r1.y+r1.h < r2.y)
		return 0;
	return 1;
}
