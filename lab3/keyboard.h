#ifndef __KEYBOARD_H
#define __KEYBOARD_H


int geral_subscribe_int(int *hookID);

int geral_unsubscribe_int(int *hookID);

int keyboard_sendCommand(unsigned long command);

int keyboard_sendArgument(unsigned long argument);

int keyboard_readStatus(unsigned long *command);

#endif
