#ifndef _VECTOR_H_
#define _VECTOR_H_

typedef struct{
	unsigned char r,g,b;
}Color;

typedef struct{
	int x,y;
}Vector2;

typedef struct{
	double x;
	double y;
} Vector2d;

inline Vector2d Vector2_Lerp(Vector2d v1, Vector2d v2, double d);

inline Vector2d Vector2_Sub(Vector2d v1, Vector2d v2);

inline Vector2d Vector2_Add(Vector2d v1, Vector2d v2);

inline Vector2d Vector2_Normalize(Vector2d v);

inline double Vector2_Magnitude(Vector2d v);

inline int rgb(Color c);
inline int rgb1(unsigned char r, unsigned char g, unsigned char b);

#endif
