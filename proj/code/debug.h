#ifndef _DEBUG_H
#define _DEBUG_H
/** @defgroup Debug Debug module
 * @{
 *
 * Module responsible for Debug
 *
 */

/**
 * @brief Function to write a string in the debug file
 *
 * @param tag The tag of the message
 * @param value the string to be printed to the file
 *
 * @return void
 */
void Debug_LOG(char* tag, char* value);

/**
 * @brief Function to write a hexadecimal value to the debug file
 *
 * @param tag The tag of the message
 * @param value the hexadecimal value to be printed to the file
 *
 * @return void
 */
void Debug_LOGHex(char* tag, long value);

/**
 * @brief Function to write a decimal value to the debug file
 *
 * @param tag The tag of the message
 * @param value the decimal value to be printed to the file
 *
 * @return void
 */
void Debug_LOGInt(char* tag, long value);


#endif
