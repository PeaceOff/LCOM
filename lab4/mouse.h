#ifndef _MOUSE_H
#define _MOUSE_H


int mouse_set_stream();

int mouse_disable_stream_mode();

int mouse_readStatus(unsigned long *command);

int mouse_get_argument(unsigned long *argument);

int mouse_sendArgument(unsigned long argument);


#endif
