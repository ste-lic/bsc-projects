
#ifndef __MORSE_H__
#define __MORSE_H__

#include "string.h"

#define LINE_RGB_GREEN PAL_LINE(GPIOA, 8U)
#define LINE_RGB_RED PAL_LINE(GPIOA, 9U)
#define LINE_RGB_BLU PAL_LINE(GPIOA, 10U)
#define LINE_EXT_BUTTON PAL_LINE(GPIOC, 7U)
#define LINE_BUZZER PAL_LINE(GPIOA, 5U)
#define LINE_RX_LED PAL_LINE(GPIOB,5U)
#define LINE_TX_LED PAL_LINE(GPIOB,4U)

typedef struct Morse
{
  char letter[2];
  char morse[7];
} Morse;



char* morseDecode(char* x);
char* morseEncode(char x);
void morseToText(char* input,char* output);
void textToMorse(char* input,char* output);
void getUserInput(char* input_decode);

#endif  //__MORSE_H__

