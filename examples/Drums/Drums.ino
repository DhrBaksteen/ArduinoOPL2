/**
 * This is a simple demo sketch for the OPL2 library. It plays a drum loop using instrument settings form the Adlib
 * standard instrument library. Note that channel 6 is used by the base drum, 7 is used by snare and hi-hat and 8 is
 * used by tom tom and cymbal.
 *
 * OPL2 board is connected as follows:
 *   Pin  8 - Reset
 *   Pin  9 - A0
 *   Pin 10 - Latch
 *   Pin 11 - Data     (Use pin 51 for Arduino Mega)
 *   Pin 13 - Shift    (Use pin 52 for Arduino Mega)
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

  // Set percussion mode and load instruments.
  opl2.setPercussion(true);
  opl2.setInstrument(0, INSTRUMENT_BDRUM1);
  opl2.setInstrument(0, INSTRUMENT_RKSNARE1);
  opl2.setInstrument(0, INSTRUMENT_TOM2);
  opl2.setInstrument(0, INSTRUMENT_CYMBAL1);
  opl2.setInstrument(0, INSTRUMENT_HIHAT2);

  // Set octave and frequency for bass drum.
  opl2.setBlock(6, 4);
  opl2.setFNumber(6, opl2.getNoteFNumber(NOTE_C));

  // Set octave and frequency for snare drum and hi-hat.
  opl2.setBlock(7, 3);
  opl2.setFNumber(7, opl2.getNoteFNumber(NOTE_C));
  // Set low volume on hi-hat
  opl2.setVolume(7, OPERATOR1, 16);

  // Set octave and frequency for tom tom and cymbal.
  opl2.setBlock(8, 3);
  opl2.setFNumber(8, opl2.getNoteFNumber(NOTE_A));
}


void loop() {
  bool bass   = i % 4 == 0;           // Bass drum every 1st tick
  bool snare  = (i + 2) % 4 == 0;     // Snare drum every 3rd tick
  bool tom    = false;                // No tom tom
  bool cymbal = i % 32 == 0;          // Cymbal every 32nd tick
  bool hiHat  = true;                 // Hi-hat every tick

  opl2.setDrums(bass, snare, tom, cymbal, hiHat);

  i ++;
  delay(200);
}
