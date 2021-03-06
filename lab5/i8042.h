#ifndef _i8042_H
#define _i8042_H

#define BIT(n) (0x01<<(n))

#define DELAY_US    20000

#define KBC_ADRESS 0x64
#define KBC_BUFF_ADRESS 0x60

#define BREAK_CODE 0x80
#define ESC_BREAK_CODE 0x81
#define SCAN_TWO_BYTES 0xE0

#define	KEYBOARD_LEDS_CMD 0xED

#define IKEYBOARD_IRQ 0x01

#define KEYBOARD_BUFF_ACK 0xFA
#define KEYBOARD_BUFF_RESEND 0xFE
#define KEYBOARD_BUFF_ERROR 0xFC

#define KEYBOARD_STATUS_PAR 0x80
#define KEYBOARD_STATUS_TIMEOUT 0x40
#define KEYBOARD_STATUS_INBUFF_FULL 0x02
#define KEYBOARD_STATUS_MOUSE 0x10

#define MOUSE_COMMAND 0xD4
#define MOUSE_COMMAND_SET_STREAM 0xEA
#define MOUSE_COMMAND_ENABLE_STREAM 0xF4
#define MOUSE_COMMAND_STATUS_REQUEST 0xE9
#define MOUSE_COMMAND_DISABLE_STREAM 0xF5

#define MOUSE_LEFT_MOUSE BIT(0)
#define MOUSE_RIGHT_MOUSE BIT(1)
#define MOUSE_MIDDLE_MOUSE BIT(2)
#define MOUSE_VER BIT(3)
#define MOUSE_X_SIGN BIT(4)
#define MOUSE_Y_SIGN BIT(5)
#define MOUSE_X_OVF BIT(6)
#define MOUSE_Y_OVF BIT(7)


#define MOUSE_BYTE1
#define MOUSE_BYTE2
#define MOUSE_BYTE3




#define MOUSE_IRQ 12


#endif
