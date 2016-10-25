#ifndef _NETWORK_MANAGER_H
#define _NETWORK_MANAGER_H

#include "ball.h"
#include "player.h"

void NM_Init();

void NM_Exit();

char NM_IsServer();

void NM_SendBallPos();

char NM_PlayerConnected();

char NM_WaitForServer();

void NM_CheckBallColl();

void NM_SetServer(char r);

void NM_SendScore();

void NM_SetBall(Ball* b);

void NM_ReadScore(char* msg);

void NM_SendMessage(char* message);

void NM_SetSinglePlayer(char state);

char NM_IsSinglePlayer();

void NM_SetPlayer(char id , Player* p);

#endif
