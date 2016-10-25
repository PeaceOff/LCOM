#ifndef __KEYBOARD_H
#define __KEYBOARD_H


int geral_subscribe_int(int *hookID, unsigned int end);

int geral_unsubscribe_int(int *hookID);

int keyboard_sendCommand(unsigned long command);

int keyboard_sendArgument(unsigned long argument);

int keyboard_readStatus(unsigned long *command);

int kbd_scan_handler();

int break_code_test(unsigned long scancode);

#endif
