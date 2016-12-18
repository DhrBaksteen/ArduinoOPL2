/**
 * This is an example sketch from the OPL2 library for Arduino. It plays an IMF audio file from SD card using the YM3812
 * audio chip.
 *
 * OPL2 board is connedted as follows:
 * Pin  8 - Reset
 * Pin  9 - A0
 * Pin 10 - Latch
 * Pin 11 - Data
 * Pin 13 - Shift
 *
 * Connect the SD card with Arduino SPI pins as usual and use pin 7 as CS.
 *
 * By default this example will look for the k5t06.imf file in the root of the SD card. This file is found among the
 * files for this example. For more information about the IMF file format please visit
 * http://www.shikadi.net/moddingwiki/IMF_Format
 *
 * Code by Maarten Janssen (maarten@cheerful.nl) 2016-12-17
 * Song from the game Keen5 by Bobby Prince 1991
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */


#include <SPI.h>
#include <SdFat.h>
#include <OPL2.h>

OPL2 opl2;
SdFat SD;
SdFile file;

long songLength = 0;
float imfSpeed  = 560.0;

void setup() {
  opl2.init();
  SD.begin(7);

  loadImfSong("k5t06.imf");
}

void loop() {
  while (songLength > 0) {
    int wait = playImfSong();
    if (wait > 0) {
      delay(wait);
    }
    songLength --;
  }
}


void loadImfSong(char* fileName) {
  file.open(fileName);
  file.seekSet(0);

  songLength  = file.read();
  songLength += file.read() << 8;
  if (songLength == 0) {
    songLength = 65535;
    file.seekSet(4);
  }
}


int playImfSong() {
  byte reg  = file.read();
  byte data = file.read();
  float  wait = file.read();
  wait += file.read() << 8;

  if (reg != 0x00) {
    opl2.write(reg, data);
  }
  songLength -= 3;

  return round(wait * (1000 / imfSpeed));
}
