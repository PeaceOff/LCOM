#ifndef _NETWORK_MANAGER_H
#define _NETWORK_MANAGER_H

#include "ball.h"
#include "player.h"

/** @defgroup NetworkManager NetworkManager module
 * @{
 *
 * Module responsible for Network management 
 *
 */

 /**
 * @brief Function to Initialize the NetworkManager
 *
 * @return void
 */
void NM_Init();
 /**
 * @brief Function to Free the memory of the NetworkManager
 *
 * @return void
 */
void NM_Exit();
 /**
 * @brief Function to Check if the NetworkManager is Server
 *
 * @return Returns 1 if the NetworkManager is the server, 0 otherwise
 */
char NM_IsServer();

 /**
 * @brief Function to Send the Position of the ball
 *
 * @return void
 */
void NM_SendBallPos();
 /**
 * @brief Function to Check if a player is connected
 *
 * @return Returns 1 if a player is connected, 0 otherwise
 */
char NM_PlayerConnected();
 /**
 * @brief Function to Check if the Client is Waiting for the Server
 *
 * @return Returns 1 if is waiting, 0 otherwise
 */
char NM_WaitForServer();

 /**
 * @brief Function to Check the ball collision
 *
 * Check The ball collision between the players	
 *
 * @return void
 */
void NM_CheckBallColl();
 /**
 * @brief Function to Set NetworkManager Server Status
 *
 * @return void
 */
void NM_SetServer(char r);
 /**
 * @brief Function to Send Score
 *
 * @return void
 */
void NM_SendScore();
 /**
 * @brief Function to Set the ball of the NetworManager Score
 *
 * @return void
 */
void NM_SetBall(Ball* b);
 /**
 * @brief Function to Read the Score
 *
 * @return void
 */
void NM_ReadScore(char* msg);
 /**
 * @brief Function to Read a Message Using the UART
 *
 * @return void
 */
void NM_SendMessage(char* message);
 /**
 * @brief Function to Set the SinglePlayer Mode
 *
 * @return void
 */
void NM_SetSinglePlayer(char state);
 /**
 * @brief Function to Check if the NetworManager is in SinglePlayer Mode
 *
 * @return Returns 1 if it is in singleplayer, 0 otherwise
 */
char NM_IsSinglePlayer();
 /**
 * @brief Function to Set a Player in the NetworManager
 *
 * @param id the id of the player, ranges from 0 to 1
 * @param p the adress of the player
 *
 * @return void
 */ 
void NM_SetPlayer(char id , Player* p);

#endif
