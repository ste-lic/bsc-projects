#include "ch.h"
#include "hal.h"
#include "ssd1306.h"
#include "morse.h"
#include "shell.h"
#include "chprintf.h"

#define SHELL_WA_SIZE   THD_WORKING_AREA_SIZE(2048)
#define MAXDIM 100

#define BUFF_SIZE   20
char buff[BUFF_SIZE];

static const I2CConfig i2ccfg = {
  OPMODE_I2C,
  400000,
  FAST_DUTY_CYCLE_2,
};

static const SSD1306Config ssd1306cfg = {
  &I2CD1,
  &i2ccfg,
  SSD1306_SAD_0X78,
};

static SSD1306Driver SSD1306D1;

/*Quando è digitato il comando "receive", il dispositivo va in modalità ricezione ed attende un messaggio tramite il ricevitore ad Infrarossi.
 * La decodifica avviene nel seguente modo:
 * 1)ricezione continua per meno di 600 ms --> .
 * 2)ricezione continua per piu di 600 ms e meno di 1500 ms --> -
 * 3)ricezione continua per piu di 1500 ms e meno di 3000 ms --> spazio
 * 4)ricezione continua per piu di 3000 ms --> messaggio terminato
 *
 * Durante la ricezione l'utente riceve anche un feedback visivo tramite l'utilizzo di un led RGB. Per le varie fasi i colori sono rispettivamente:
 * 1)blu
 * 2)rosso
 * 3)verde
 * 4)bianco*/
static void cmd_receive(BaseSequentialStream *chp, int argc, char *argv[]) {
  char input_decode[100] = "";
  char output_decode[100] = "";
  int done = 0;
  int counter = 0;
  while(!done){

    if (palReadLine(LINE_RX_LED) == PAL_HIGH) {

            while (palReadLine(LINE_RX_LED) != PAL_LOW) {

              palSetLine(LINE_BUZZER);//attivo il buzzer durante la ricezione
              chThdSleepMilliseconds(30);
              counter += 30;

              if(counter > 3000) {
                palSetLine(LINE_RGB_RED); //accendi led rosso
                palSetLine(LINE_RGB_GREEN); //accendi led verde
                palSetLine(LINE_RGB_BLU); //accendi led blu
              }
              else if(counter > 1500) {

                palClearLine(LINE_RGB_RED); //spegni led rosso
                palClearLine(LINE_RGB_BLU); //spegni led blu
                palSetLine(LINE_RGB_GREEN); //accendi led verde
              }
              else if(counter > 600){
                palClearLine(LINE_RGB_GREEN); //spegni led verde
                palClearLine(LINE_RGB_BLU); //spegni led blu
                palSetLine(LINE_RGB_RED); //accendi led rosso
              }
              else{
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
            else if(counter > 1500) {//se premo piu di 1.5 secondi è uno spazio
              strcat(input_decode," ");
              chprintf((BaseSequentialStream *) &SD2, "%s", " ");
            }
            else if(counter > 600){//se premo piu di 2 secondi è una linea
              strcat(input_decode,"-");
              chprintf((BaseSequentialStream *) &SD2, "%s", "-");
            }
            else{//altrimenti è un punto
              strcat(input_decode,".");
              chprintf((BaseSequentialStream *) &SD2, "%s", ".");
            }

            counter = 0;
          }

  }

  morseToText(input_decode, output_decode);
  chprintf((BaseSequentialStream *) &SD2, "\n\r%s\n\r", output_decode);

  ssd1306FillScreen(&SSD1306D1, SSD1306_COLOR_BLACK);
  ssd1306GotoXy(&SSD1306D1, 35, 20);
  chsnprintf(buff, BUFF_SIZE, output_decode);
  ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
  ssd1306UpdateScreen(&SSD1306D1);

  //Se il messaggio ricevuto è un messaggio di SOS è acceso il led rosso ed è emesso un segnale acustico per 10 secondi.
  if(strcmp(output_decode,"sos")== 0){
    palSetLine(LINE_RGB_RED);
    palSetLine(LINE_BUZZER);
    chThdSleepMilliseconds(10000);
    palClearLine(LINE_RGB_RED);
    palClearLine(LINE_BUZZER);
  }

}

static void cmd_transmit(BaseSequentialStream *chp, int argc, char *argv[]) {

  char input[100] = "";
  char output[100] = "";
  getUserInput(input);
  morseToText(input, output);

  ssd1306FillScreen(&SSD1306D1, SSD1306_COLOR_BLACK);
  ssd1306GotoXy(&SSD1306D1, 35, 20);
  chsnprintf(buff, BUFF_SIZE, output);
  ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
  ssd1306UpdateScreen(&SSD1306D1);

  for (int i = 0; input[i]; i++){

    switch (input[i]) {
      case '.':
        palSetLine(LINE_TX_LED);
        chThdSleepMilliseconds(300);
        palClearLine(LINE_TX_LED);
        chThdSleepMilliseconds(100);
        break;

      case '-':
        palSetLine(LINE_TX_LED);
        chThdSleepMilliseconds(800);
        palClearLine(LINE_TX_LED);
        chThdSleepMilliseconds(100);
        break;

      case ' ':
        palSetLine(LINE_TX_LED);
        chThdSleepMilliseconds(1700);
        palClearLine(LINE_TX_LED);
        chThdSleepMilliseconds(100);
        break;

      default:
        palClearLine(LINE_TX_LED);
    }

  }

  //Invio il segnale di terminazione
  palSetLine(LINE_TX_LED);
  chThdSleepMilliseconds(3200);
  palClearLine(LINE_TX_LED);

  chprintf((BaseSequentialStream *) &SD2,"\n\r");
}

static void cmd_encode(BaseSequentialStream *chp, int argc, char *argv[]) {
  static uint8_t buf[] = "ciao\r\n";

  if (argc == 0) {
    chnWrite(&SD2, buf, sizeof buf - 1);
    chprintf(chp, "\r\n\nstopped\r\n");
  }
  else if(argc == 1){

    char input[100] = "";
    char output[100] = "";
    strcpy(input,(char*)argv[0]);
    textToMorse(input,output);
    chprintf(chp, "%s\r\n", output);

    ssd1306FillScreen(&SSD1306D1, SSD1306_COLOR_BLACK);
    ssd1306GotoXy(&SSD1306D1, 0, 1);
    chsnprintf(buff, BUFF_SIZE, output);
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_7x10, SSD1306_COLOR_WHITE);
    ssd1306UpdateScreen(&SSD1306D1);

  }
  else{
    chprintf(chp, "Usage: encode [|SOMETHING]\r\n");
    return;
  }
}

static void cmd_decode(BaseSequentialStream *chp, int argc, char *argv[]) {
  char input_decode[100] = "";
  char output_decode[100] = "";

  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: decode\r\n");
  }
  else {

    getUserInput(input_decode);

    morseToText(input_decode, output_decode);
    chprintf((BaseSequentialStream *) &SD2, "\n\r%s\n\r", output_decode);

    ssd1306FillScreen(&SSD1306D1, SSD1306_COLOR_BLACK);
    ssd1306GotoXy(&SSD1306D1, 35, 20);
    chsnprintf(buff, BUFF_SIZE, output_decode);
    ssd1306Puts(&SSD1306D1, buff, &ssd1306_font_11x18, SSD1306_COLOR_WHITE);
    ssd1306UpdateScreen(&SSD1306D1);
  }
}



static const ShellCommand commands[] = {
                                        {"encode", cmd_encode},
                                        {"decode", cmd_decode},
                                        {"receive", cmd_receive},
                                        {"transmit", cmd_transmit},
                                        {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
                                       (BaseSequentialStream *)&SD2,
                                       commands
};

int main(void) {

  halInit();
  chSysInit();

  //Configurazione Pin I2C
  palSetLineMode(LINE_ARD_D15, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                 PAL_STM32_PUPDR_PULLUP);
  palSetLineMode(LINE_ARD_D14, PAL_MODE_ALTERNATE(4) |
                 PAL_STM32_OTYPE_OPENDRAIN | PAL_STM32_OSPEED_HIGHEST |
                 PAL_STM32_PUPDR_PULLUP);

  //Configurazione schermo
  ssd1306ObjectInit(&SSD1306D1);
  ssd1306Start(&SSD1306D1, &ssd1306cfg);
  ssd1306FillScreen(&SSD1306D1, SSD1306_COLOR_BLACK);
  ssd1306UpdateScreen(&SSD1306D1);

  palSetLineMode(LINE_RX_LED,PAL_MODE_INPUT_PULLDOWN);//linea collegata al ricevitore IR
  palSetLineMode(LINE_TX_LED,PAL_MODE_OUTPUT_PUSHPULL);//linea collegata al trasmettitore IR


  palSetLineMode(LINE_EXT_BUTTON, PAL_MODE_INPUT);//linea bottone esterno
  palSetLineMode(LINE_BUZZER, PAL_MODE_OUTPUT_PUSHPULL); //linea alimentazione buzzer
  palSetLineMode(LINE_RGB_GREEN, PAL_MODE_OUTPUT_PUSHPULL); //linea alimentazione led verde
  palSetLineMode(LINE_RGB_RED, PAL_MODE_OUTPUT_PUSHPULL); //linea alimentazione led rosso
  palSetLineMode(LINE_RGB_BLU, PAL_MODE_OUTPUT_PUSHPULL); //linea alimentazione led blu

  palClearLine(LINE_BUZZER);
  palClearLine(LINE_TX_LED);

  //Inizializzazione shell
  shellInit();
  sdStart(&SD2, NULL);

  while (true) {
    thread_t *shelltp = chThdCreateFromHeap(NULL, SHELL_WA_SIZE, "shell", NORMALPRIO + 1,
                                            shellThread, (void *)&shell_cfg1);

    chThdWait(shelltp);               /* Waiting termination.             */
  }
}
