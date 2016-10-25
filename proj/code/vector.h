#ifndef _VECTOR_H_
#define _VECTOR_H_
/** @defgroup Vector Vector module
 * @{
 *
 * Module responsible for vectors
 *
 */

/** @name Color structure */
/** @{
 *
 * Color structure
 */
typedef struct{
	unsigned char r,g,b;
}Color;
/** @} end of Color */

/** @name Vector2 structure */
/** @{
 *
 * Vector2 structure
 */
typedef struct{
	int x,y;
}Vector2;
/** @} end of Vector2 */

/** @name Vector2d structure */
/** @{
 *
 * Vector2d structure
 */
typedef struct{
	double x;
	double y;
} Vector2d;
/** @} end of Vector2d */

/**
 * @brief Function to lerp two vectors
 *
 *This function is used to lerp (linear interpolation) 2 vectors
 *
 * @param v1 Vector to be lerped with v2
 * @param v2 Vector to be lerped with v1
 * @param d Multiplying factor of the lerp
 *
 * @return Returns the new vector (position)
 */
inline Vector2d Vector2_Lerp(Vector2d v1, Vector2d v2, double d);

/**
 * @brief Function to subtract two vectors
 *
 * @param v1 Vector to be subtracted with v2
 * @param v2 Vector to be subtracted with v1
 *
 * @return Returns the new vector (position)
 */
inline Vector2d Vector2_Sub(Vector2d v1, Vector2d v2);

/**
 * @brief Function to add two vectors
 *
 * @param v1 Vector to be added with v2
 * @param v2 Vector to be added with v1
 *
 * @return Returns the new vector (position)
 */
inline Vector2d Vector2_Add(Vector2d v1, Vector2d v2);

/**
 * @brief Function to normalize a vector
 *
 * This function takes in a vector and normalizes it,this means it maintains the same direction but the magnitude of the vector will be one
 * It is used when a player shots a bullet to determine the direction of the bullet;
 *
 * @param v Vector to be normalized
 *
 * @return Returns the new normalized vector
 */
inline Vector2d Vector2_Normalize(Vector2d v);

/**
 * @brief Function to determine the magnitude of a vector
 *
 * @param v
 *
 * @return Returns the magnitude of Vector v
 */
inline double Vector2_Magnitude(Vector2d v);

/**
 * @brief Function to convert a color to R5 G6 B5 mode
 *
 * @param c Color to be converted
 *
 * @return Returns the value of the color in the R5 G6 B5 mode
 */
inline int rgb(Color c);

/**
 * @brief Function to convert a color to R5 G6 B5 mode
 *
 * @param r Red grade of the color
 * @param g Green grade of the color
 * @param b Blue grade of the color
 *
 * @return Returns the value of the color in the R5 G6 B5 mode
 */
inline int rgb1(unsigned char r, unsigned char g, unsigned char b);

#endif
