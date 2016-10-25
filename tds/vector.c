#include "vector.h"
#include <math.h>

inline int rgb(Color c) {
	if (c.r < 0 || 255 < c.r || c.g < 0 || 255 < c.g || c.b < 0 || c.b > 255)
		return -1;

	int red, green, blue;
	red = c.r * 31 / 255;
	green = c.g * 63 / 255;
	blue = c.b * 31 / 255;

	return (red << 11) | (green << 5) | blue;
}

inline double Vector2_Magnitude(Vector2d v){
	return sqrt(v.x*v.x + v.y*v.y);
}

inline int rgb1(unsigned char r, unsigned char g, unsigned char b){
	if (r < 0 || 255 < r || g < 0 || 255 < g || b < 0 || b > 255)
			return -1;

		int red, green, blue;
		red = r * 31 / 255;
		green = g * 63 / 255;
		blue = b * 31 / 255;

		return (red << 11) | (green << 5) | blue;
}

inline Vector2d Vector2_Lerp(Vector2d v1, Vector2d v2, double d){
	Vector2d vetor = Vector2_Sub(v2,v1);
	vetor.x *= d;
	vetor.y *= d;

	vetor = Vector2_Add(v1, vetor);

	return vetor;

}

inline Vector2d Vector2_Sub(Vector2d v1, Vector2d v2){
	v1.x = v1.x - v2.x;
	v1.y = v1.y - v2.y;
	return v1;
}

inline Vector2d Vector2_Add(Vector2d v1, Vector2d v2){
	v1.x = v1.x + v2.x;
	v1.y = v1.y + v2.y;
	return v1;
}

inline Vector2d Vector2_Normalize(Vector2d v){
	Vector2d res;
	double sqr = sqrt(v.x*v.x + v.y*v.y);
	res.x = v.x/sqr;
	res.y = v.y/sqr;
	return res;
}
