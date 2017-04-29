/**
 * This is a simple demo sketch for the OPL2 library. It plays a musical scale on channels 0, 1 and 2 of the YM3812
 * using a bell sound.
 *
 * OPL2 board is connedted as follows:
 * Pin  8 - Reset
 * Pin  9 - A0
 * Pin 10 - Latch
 * Pin 11 - Data
 * Pin 13 - Shift
 *
 * Code by Maarten Janssen (maarten@cheerful.nl) 2016-12-18
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */


#include <SPI.h>
#include <OPL2.h>


OPL2 opl2;


void setup() {
  opl2.init();

  // Setup channels 0, 1 and 2 to produce a bell sound.
  for (byte i = 0; i < 3; i ++) {
    opl2.setTremolo   (i, CARRIER, true);
    opl2.setVibrato   (i, CARRIER, true);
    opl2.setMultiplier(i, CARRIER, 0x04);
    opl2.setAttack    (i, CARRIER, 0x0A);
    opl2.setDecay     (i, CARRIER, 0x04);
    opl2.setSustain   (i, CARRIER, 0x0F);
    opl2.setRelease   (i, CARRIER, 0x0F);
    opl2.setBlock     (i, 0x05);
  }

  // Play notes on alternating channels.
  for (byte i = 0; i < 13; i ++) {
    short freq = opl2.getNoteFrequency(i % 3, 4, i);
    opl2.setFrequency(i % 3, freq);
    opl2.setKeyOn    (i % 3, false);
    opl2.setKeyOn    (i % 3, true);
    delay(500);
  }
}


void loop() {
}
