#ifndef _MOUSE_H
#define _MOUSE_H

#include "vector.h"
/** @defgroup Mouse Mouse module
 * @{
 *
 * Module responsible for Mouse
 * 
 */

/**
 * @brief Function to Get the State of a Mouse Button
 *
 * @param btn the value of the button pressed ranges from 0 to 2
 *
 * @return Returns 1 if the button is pressed or 0 if not
 */
char mouse_getButton(char btn);

/**
 * @brief Function to Get the Global Mouse Position
 *	
 * @return Returns the global position of the mouse in form of Vector2d
 */
Vector2d mouse_getPos();
/**
 * @brief Function to Get the Mouse Position relative to the screen
 *	
 * @return Returns the mouse position relative to the screen in form of Vector2d
 */
Vector2d mouse_getPosScreen();

/**
 * @brief Function to Initialize all the Mouse Data Related
 *	
 * @return void
 */
void mouse_init();

/**
 * @brief Function to Update all the Mouse Data Related
 *	
 * @return void
 */
void mouse_update();

/**
 * @brief Function to Clear Mouse Packets
 *	
 * @return void
 */
void clearPacket();

/**
 * @brief Function to Handle the Mouse
 *	
 * @return Returns 1 if all the Mouse Packets are in correct order and ready to read , 0 otherwise
 */
int mouse_handler();

/**
 * @brief Function to Enable the Mouse to stream mode
 *	
 * @return Returns 1 if succeeded , 0 otherwise
 */
int mouse_set_stream();
/**
 * @brief Function to Disable the Mouse to stream mode
 *	
 * @return Returns 1 if succeeded , 0 otherwise
 */
int mouse_disable_stream_mode();

/**
 * @brief Function to Read the Status
 *	
 * @param command the command to receive the status
 * @return Returns 1 if succeeded , 0 otherwise
 */
int mouse_readStatus(unsigned long *command);
/**
 * @brief Function to Read the Argument
 *	
 * @param argument the argument to receive  
 * 
 * @return Returns 1 if succeeded , 0 otherwise
 */
int mouse_get_argument(unsigned long *argument);
/**
 * @brief Function to Send the Argument
 *	
 * @param argument the argument to send
 *    
 * @return Returns 1 if succeeded , 0 otherwise
 */
int mouse_sendArgument(unsigned long argument);


#endif
