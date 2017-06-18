/**
 * This is a simple demo sketch for the OPL2 library. It plays a drum loop using instrument settings form the Adlib
 * standard instrument library. Note that channel 6 is used by the base drum, 7 is used by snare and hi-hat and 8 is
 * used by tom tom and cymbal.
 *
 * OPL2 board is connected as follows:
 * Pin  8 - Reset
 * Pin  9 - A0
 * Pin 10 - Latch
 * Pin 11 - Data     (Use pin 51 for Arduino Mega)
 * Pin 13 - Shift    (Use pin 52 for Arduino Mega)
 *
 * Code by Maarten Janssen (maarten@cheerful.nl) 2017-04-13
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */


#include <SPI.h>
#include <OPL2.h>
#include <instruments.h>


OPL2 opl2;
int i = 0;


void setup() {
  opl2.init();

  opl2.setPercussion(true);
  opl2.setInstrument(0, BDRUM1);
  opl2.setInstrument(0, RKSNARE1);
  opl2.setInstrument(0, TOM2);
  opl2.setInstrument(0, CYMBAL1);
  opl2.setInstrument(0, HIHAT2);
  opl2.setBlock     (6, 4);
  opl2.setFrequency (6, opl2.getNoteFrequency(6, 4, NOTE_C));
  opl2.setBlock     (7, 4);
  opl2.setFrequency (7, opl2.getNoteFrequency(7, 3, NOTE_C));
  opl2.setBlock     (8, 4);
  opl2.setFrequency (8, opl2.getNoteFrequency(8, 3, NOTE_A));
}


void loop() {
  opl2.setDrums(i % 4 == 0, (i + 2) % 4 == 0, 0, i % 32 == 0 , i % 2);
  i ++;
  delay(250);
}

