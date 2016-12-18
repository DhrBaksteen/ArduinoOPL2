/**
 * This is an example sketch from the OPL2 library for Arduino. It plays a DRO audio file from SD card using the YM3812
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
 * By default this example will look for the phemopop.dro file in the root of the SD card. This file is found among the
 * files for this example. For more information about the DRO file format please visit
 * http://www.shikadi.net/moddingwiki/DRO_Format
 *
 * Code by Maarten Janssen (maarten@cheerful.nl) 2016-12-17
 * Song Phemo-pop! by Olli Niemitalo/Yehar 1996
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */


#include <SPI.h>
#include <SdFat.h>
#include <OPL2.h>

OPL2 opl2;
SdFat SD;
SdFile file;

long offset            = 0;
long songLength        = 0;
long songDuration      = 0;
byte codeShortDelay    = 0;
byte codeLongDelay     = 0;
byte registerMapLength = 0;
byte registerMap[256];


void setup() {
  opl2.init();
  SD.begin(7);

  loadDroSong("phemopop.dro");
}


void loop() {
  while (songLength > 0) {
    int wait = playDroSong();
    if (wait > 0) {
      delay(wait);
    }
    songLength --;
  }
}


void loadDroSong(char* fileName) {
  file.open(fileName);
  file.seekSet(12);

  songLength  = file.read();
  songLength += file.read() << 8;
  songLength += file.read() << 16;
  songLength += file.read() << 24;

  songDuration  = file.read();
  songDuration += file.read() << 8;
  songDuration += file.read() << 16;
  songDuration += file.read() << 24;

  file.seekSet(23);
  codeShortDelay = file.read();
  codeLongDelay  = file.read();
  registerMapLength = file.read();

  for (byte i = 0; i < registerMapLength; i ++) {
    registerMap[i] = file.read();
  }
}


int playDroSong() {
  byte code = file.read();
  byte data = file.read();

  if (code == codeShortDelay) {
    return data + 1;
  } else if (code == codeLongDelay) {
    return (data + 1) << 8;
  } else if (code < 128) {
    opl2.write(registerMap[code], data);
  }

  return 0;
}
