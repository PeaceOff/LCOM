#ifndef _MAP_H
#define _MAP_H

#include "list.h"
#include "sprite.h"
/** @defgroup Map Map module
 * @{
 *
 * Module responsible for the map
 *
 */

#define DAMAGE_STATES 2

#define MAP_STATE_1 1
#define MAP_STATE_2 2
#define MAP_STATE_3 3

/** @name Node structure */
/** @{
 *
 * Map Node structure
 */
typedef struct Node{
	int x;
	int y;
	int iCost; //Distancia ate Node
	int fCost; //Distancia ate Fim
	int tCost; //Total;
	int solid; //solido ou nao
	struct Node* parent;
}Node;
/** @} end of Node */

/**
 * @brief Function get for the map dimensions
 *
 * @return Returns a Vector2 with the maps dimensions
 */
Vector2 Map_getDimensions();

/**
 * @brief Function to init the map
 *
 *This function load the tileset of the map image and generates the map
 *
 *@param x X dimension of the map
 *@param y Y dimension of the map
 *@param filename Path to the image of the map
 *
 * @return Returns an int indicating if it succided or not
 */
int Map_Init(char * fileName, int x, int y);

/**
 * @brief Function to fill the buffer
 *
 *This function fills the buffer with the map setting the right textures
 *
 * @return void
 */
void Map_FillBuffer();

/**
 * @brief Function to fill the buffer
 *
 *This function fills the buffer with the map setting the right textures
 *
 * @return void
 */
void Map_Draw();

/**
 * @brief Function to deal with information of explosion from the UART
 *
 *Reads the coded information about an explosion on the map from the serial port and handles it
 *
 * @return void
 */
void Map_ReadExplosion(char* msg);

/**
 * @brief Function to code the information about map damage with explosions
 *
 *Codes the information in a string to be sent via the serial port
 *
 *@param x Abcissa of the map
 *@param y Coordenate of the map
 *@param dmg damage dealt
 *
 * @return A string with the information codified
 */
char* Map_SendExplosion(int x, int y, char dmg);

/**
 * @brief Function to get the lowest cost node
 *
 *This function is used in the path finding to determine the shortest way
 *
 *@param l1 list of nodes to be checked
 *
 * @return Returns a pointer to the result of the shortest way
 */
inline Node* GetLowCost(List* l1);

/**
 * @brief Function apply the damage to the map
 *
 *@param x Abcissa of the map
 *@param y Coordenate of the map
 *@param dmg damage dealt
 *
 * @return A char indicating if the damage was applied or not
 */
char Map_Damage(int x, int y, char dmg);

/**
 * @brief Function to the read the damage dealt
 *
 * This function receives a coded string with information about map damage
 * via the serial port and applies that information (map damage)
 *
 *@param msg coded message with the information
 *
 * @return void
 */
void Map_NetReadDamage(char * msg);

/**
 * @brief Function to get a specific node of the map
 *
 *@param x Abcissa of the node we are looking for
 *@param y Coordenate of the node we are looking for
 *
 * @return Returns a pointer to the node or NULL if not found
 */
Node* Map_GetNode(int x, int y);

/**
 * @brief Function to get the shortest path to the player
 *
 *@param xi Abcissa of the enemy
 *@param yi Coordenate of the enemy
 *@param xf Abcissa of the player
 *@param yf Coordenate of the enemy
 *
 * @return Returns a pointer to the list of nodes of the path
 */
List* Map_GetPath(int xi , int yi, int xf, int yf);

/**
 * @brief Function to apply a decal to the map
 *
 *@param x Abcissa to apply the decal
 *@param y Coordenate to apply the decal
 *@param iX Initial x of the decal
 *@param iY Initial y of the decal
 *@param iW width of the decal
 *@param iH height of the decal
 *@param s Sprite of the decal to be drawn
 *
 * @return void
 */
void Map_ApplyDecal(int x,int y, int iX, int iY, int iW, int iH,Sprite* s);

/**
 * @brief Function to check collision with the map
 *
 *@param x Abcissa to check collision
 *@param y Coordenate to check collision
 *
 * @return An int indicating whether or not there is collision
 */
int Map_CheckCollision(int x, int y);

/**
 * @brief Function to get a list of available spawn points for the enemies
 *
 * @return A list containing the available spawns
 */
List* Map_GetSpawnPoints();

/**
 * @brief Function to Texturize an specific point
 * This Function texturize a point in relation of the neighbours
 * @param x the abcissa of the position to texturize
 * @param y the coordenate of the position to texturize
 * @return void
 */
inline void Map_AutoTexture(int x, int y);

/**
 * @brief Function to Get the neighbours in a specific point
 * This Function create a bitmask that as information if the neighbours are solid(1) or not (0)
 * @param x the abcissa of the position
 * @param y the coordenate of the position
 * @return Returns char bitmask that has information about it's neighbours
 */
inline unsigned char Map_GetNeighbours(int x, int y);
/**
 * @brief Function to Texturize the map
 *
 * @param x1 the abcissa of the position to texturize
 * @param y1 the coordenate of the position to texturize
 * @param x1 the abcissa of the image to use as texture
 * @param y1 the coordenate of the image to use as texture
 *
 * @return void
 */
inline void Map_Texturize(int x1, int y1 , int imgX, int imgY);
/**
 * @brief Function to Get the Data in a specific point
 * This Function create a bitmask that as information if the neighbours are solid(1) or not (0)
 * @param x the abcissa of the position
 * @param y the coordenate of the position
 * @return Returns the data of the specific point
 */
inline unsigned char Map_GetData(int x, int y);

/**
 * @brief Function to exit the map
 *
 * The Functions clears the memory occupied by the map and deletes the map
 *
 * @return void
 */
void Map_Exit();

#endif
