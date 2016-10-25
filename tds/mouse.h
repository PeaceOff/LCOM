#ifndef _MOUSE_H
#define _MOUSE_H

#include "vector.h"

char mouse_getButton(char btn);

Vector2d mouse_getPos();

Vector2d mouse_getPosScreen();

void mouse_init();

void mouse_update();

void clearPacket();

int mouse_handler();

int mouse_set_stream();

int mouse_disable_stream_mode();

int mouse_readStatus(unsigned long *command);

int mouse_get_argument(unsigned long *argument);

int mouse_sendArgument(unsigned long argument);


#endif
