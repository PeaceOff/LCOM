#ifndef _OBJMANAGER_H
#define _OBJMANAGER_H
/** @defgroup ObjManager Object Manager module
 * @{
 *
 * Module responsible for managing objects
 *
 */

enum priority{
	FIRST_PLANE,
	SECOND_PLANE,
	THIRD_PLANE,
	USER_INTERFACE
};

enum object_type{
	MAP,
	PLAYER,
	OBJ_BALL,
	FONT,
	SPRITE,
	SUB_SPRITE,
	BULLET,
	ENEMYMANAGER,
	OBJ_PARTICLE,
	MENU
};

/** @name Object structure */
/** @{
 *
 * Object structure
 */
typedef struct{
	enum object_type type;

	void* object;

} Object ;
/** @} end of Object */

/**
 * @brief Function to initiate the object manager
 *
 *This function allocates memory and initiates the object manager
 *
 * @return void
 */
void OM_Init();

/**
 * @brief Function to exit the object manager
 *
 *This function frees the memory occupied by the object manager
 *
 * @return void
 */
void OM_Exit();

/**
 * @brief Function to update the objects
 *
 *This function updates all the objects it contains, call the correct function for each type of object
 *
 * @return void
 */
void OM_Update();

/**
 * @brief Function to add a new object
 *
 *This function adds a new object to the list of object of the manager
 *
 *@param obj A pointer to the object
 *@param tipo Type of object
 *@param p Priority of the object
 *
 * @return void
 */
void OM_AddObject(void* obj,enum object_type tipo , enum priority p);


#endif
