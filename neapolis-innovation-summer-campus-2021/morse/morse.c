
#include "ch.h"
#include "hal.h"
#include "morse.h"
static const Morse morse[] =
{
 {" ", "/"},
 {"a", ".-"},
 {"b", "-..."},
 {"c", "-.-."},
 {"d", "-.."},
 {"e", "."},
 {"f", "..-."},
 {"g", "--."},
 {"h", "...."},
 {"i", ".."},
 {"j", ".---"},
 {"k", "-.-"},
 {"l", ".-.."},
 {"m", "--"},
 {"n", "-."},
 {"o", "---"},
 {"p", ".--."},
 {"q", "--.-"},
 {"r", ".-."},
 {"s", "..."},
 {"t", "-"},
 {"u", "..-"},
 {"v", "...-"},
 {"w", ".--"},
 {"x", "-..-"},
 {"y", "-.--"},
 {"z", "--.."},
 {"1", ".----"},
 {"2", "..---"},
 {"3", "...--"},
 {"4", "....-"},
 {"5", "....."},
 {"6", "-...."},
 {"7", "--..."},
 {"8", "---.."},
 {"9", "----."},
 {"0", "-----"}

};




char* morseDecode(char* x){

  int found = 0;
  int i = 0;
  while(found != 1){
    if(strcmp(x,morse[i].morse) == 0){
      found = 1;
    }
    else{
      i++;
    }
  }

  return morse[i].letter;

}

char* morseEncode(char x)
{

  // refer to the Morse table
  // image attached in the article
  switch (x) {
  case 'a':
    return ".-";
  case 'b':
    return "-...";
  case 'c':
    return "-.-.";
  case 'd':
    return "-..";
  case 'e':
    return ".";
  case 'f':
    return "..-.";
  case 'g':
    return "--.";
  case 'h':
    return "....";
  case 'i':
    return "..";
  case 'j':
    return ".---";
  case 'k':
    return "-.-";
  case 'l':
    return ".-..";
  case 'm':
    return "--";
  case 'n':
    return "-.";
  case 'o':
    return "---";
  case 'p':
    return ".--.";
  case 'q':
    return "--.-";
  case 'r':
    return ".-.";
  case 's':
    return "...";
  case 't':
    return "-";
  case 'u':
    return "..-";
  case 'v':
    return "...-";
  case 'w':
    return ".--";
  case 'x':
    return "-..-";
  case 'y':
    return "-.--";
  case 'z':
    return "--..";
  case '1':
    return ".----";
  case '2':
    return "..---";
  case '3':
    return "...--";
  case '4':
    return "....-";
  case '5':
    return ".....";
  case '6':
    return "-....";
  case '7':
    return "--...";
  case '8':
    return "---..";
  case '9':
    return "----.";
  case '0':
    return "-----";
  case ' ':
    return "/";
  default:
    //TODO: carattere non valido.
    return "\n";
  }
}



void morseToText(char* input,char* output){
  //char str[] = "strtok needs to be called several times to split a string";
  //int init_size = strlen(str);
  char temp[100] = "";
  strcpy(temp,input);
  char delim[] = " ";

  char *ptr = strtok(temp, delim);

  while(ptr != NULL)
  {
    strcat(output,morseDecode(ptr));
    ptr = strtok(NULL, delim);
  }

}

void textToMorse(char* input,char* output)
{

  for (int i = 0; input[i]; i++){
    strcat(output,morseEncode(input[i]));
    strcat(output," ");
  }

}

void getUserInput(char* input_decode){
  int done = 0;
  int counter = 0;
  while (!done) {

        if (palReadLine(LINE_EXT_BUTTON) == PAL_LOW) {

          while (palReadLine(LINE_EXT_BUTTON) != PAL_HIGH) {

            palSetLine(LINE_BUZZER);
            chThdSleepMilliseconds(30);
            counter += 30;

            if(counter > 3000) { //se premo piu di 3 secondi è finita la parola.
              palSetLine(LINE_RGB_RED); //accendi led rosso
              palSetLine(LINE_RGB_GREEN); //accendi led verde
              palSetLine(LINE_RGB_BLU); //accendi led blu
            }
            else if(counter > 1500) {//se premo piu di 2 secondi è uno spazio

              palClearLine(LINE_RGB_RED); //spegni led rosso
              palClearLine(LINE_RGB_BLU); //spegni led blu
              palSetLine(LINE_RGB_GREEN); //accendi led verde
            }
            else if(counter > 600){//se premo piu di 1 secondo è una linea
              palClearLine(LINE_RGB_GREEN); //spegni led verde
              palClearLine(LINE_RGB_BLU); //spegni led blu
              palSetLine(LINE_RGB_RED); //accendi led rosso
            }
            else{//altrimenti è un punto
              palClearLine(LINE_RGB_GREEN); //spegni led verde
              palClearLine(LINE_RGB_RED); //spegni led rosso
              palSetLine(LINE_RGB_BLU); //accendi led blu
            }

          }//


          palClearLine(LINE_RGB_GREEN); //spegni led verde
          palClearLine(LINE_RGB_RED); //spegni led rosso
          palClearLine(LINE_RGB_BLU); //spegni led blu
          palClearLine(LINE_BUZZER); //spegni buzzer

          if(counter > 3000) { //se premo piu di 3 secondi è finita la parola.
            done = 1;
          }
          else if(counter > 1500) {//se premo piu di 2 secondi è uno spazio
             //This is short press.
            strcat(input_decode," ");
            chprintf((BaseSequentialStream *) &SD2, "%s", " ");
          }
          else if(counter > 600){//se premo piu di 1 secondo è una linea
            strcat(input_decode,"-");
            chprintf((BaseSequentialStream *) &SD2, "%s", "-");
          }
          else{//altrimenti è un punto
            strcat(input_decode,".");
            chprintf((BaseSequentialStream *) &SD2, "%s", ".");
          }
           //Resetting the counter.
          counter = 0;
        }

        chThdSleepMilliseconds(10);
      }
}

