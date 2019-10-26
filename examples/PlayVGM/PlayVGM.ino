/**
 * This is an example sketch from the OPL2 library for Arduino. It plays a YM3812-based VGM audio file from SD card using the YM3812
 * audio chip.
 *
 * OPL2 board is connected as follows:
 *   Pin  8 - Reset  (OPL2 reset)
 *   Pin  9 - A0     (OPL2 register/value selector)
 *   Pin 10 - Latch  (OPL2 latch SPI byte)
 *   Pin 11 - Data   (SPI MOSI)
 *   Pin 13 - Shift  (SPI clock)
 *
 * Connect the SD card with Arduino SPI pins as usual and use pin 7 as CS.
 *   Pin  7 - CS     (SD select)
 *   Pin 11 - MOSI   (SPI MOSI)
 *   Pin 12 - MISO   (SPI MISO)
 *   Pin 13 - SCK    (SPI clock)
 *
 * Refer to the wiki at https://github.com/DhrBaksteen/ArduinoOPL2/wiki/Connecting to learn how to connect your platform
 * of choice!
 *
 * By default this example will look for the stunts01.vgm file in the root of the SD card. This file is found among the
 * files for this example.
 * stunts01.vgm: https://vgmrips.net/packs/pack/stunts-pc              , 01. Title Screen
 * wacky02.vgm:  https://vgmrips.net/packs/pack/wacky-wheels-ibm-pc-at , 02. Dream
 * 
 * This program will also print song info to serial output, 9600 baud.
 * 
 * VGM is a 44100Hz sample-accurate logging format, able of logging YM3812 data (register/data pairs) and delays.
 * 
 * Compatible VGM rips is available at https://vgmrips.net/packs/chip/ym3812
 * These files must be uncompressed to *.vgm.
 * Download gzip, copy gzip.exe and decompressVgz.bat to folder with vgz-files, and run the script.
 * Note that file names should not extend 8 characters.
 * 
 * For more information about the VGM file format please visit
 * http://www.smspower.org/Music/VGMFileFormat
 *
 * Code by Eirik Stople (eirik@pcfood.net) 24-10-19
 * Song Title Screen (Stunts) by Michael J. Sokyrka, Kris Hatlelid, Brian Plank, 1990
 * Song Dream (Wacky Wheels) by Mark Klem, 1994
 * Most recent version of the library can be found at GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */

#include <SPI.h>
#include <SD.h>
#include <OPL2.h>

OPL2 opl2;
File vgmFile;

enum playbackStatus{
  PLAYBACK_PLAYING = 1,
  PLAYBACK_COMPLETE = 2,
  PLAYBACK_ERROR_UNSUPPORTED_CODE = 3,
};

const int OFFSET_GD3 = 0x14;
const int OFFSET_SAMPLE_COUNT = 0x18;
const int OFFSET_LOOP_OFFSET = 0x1C;
const int OFFSET_VGM_DATA_OFFSET = 0x34;

uint32_t relativeLoopOffset = 0;
uint16_t waitSamples = 0;
enum playbackStatus PlaybackStatus = PLAYBACK_PLAYING;

const byte filename[] = "stunts01.vgm";
//const byte filename[] = "wacky02.vgm";

void setup() {
  opl2.init();
  SD.begin(7);
  Serial.begin(9600);
  loadVgmSong(filename);
}

void loop() {
  //Play until finished/error
  uint32_t time = millis();
  while (PlaybackStatus == PLAYBACK_PLAYING)
  {
    playVgmSongYM3812(); //Fetch and execute one command
    
    if (waitSamples) //Delay command found, delay n samples
    {
      const uint16_t samplesPrSecond = 44100;
      const double samplesPrMs = (double)samplesPrSecond / 1000; //44.1
      uint16_t waitMs = (uint16_t)((double)waitSamples / samplesPrMs);

      //Reduce delay with time spent on IO
      uint32_t ioTime = millis() - time;
      if (ioTime < waitMs) {
        delay(waitMs - ioTime);
      }
      time = millis();
    }
    waitSamples = 0;
  }
}

void readGd3Field(byte *p, int maxsize)
{
  //Gd3-fields use 16bit characters, zero-terminated
  uint16_t c;
  int pos = 0;
  do {
    c  = (uint16_t)vgmFile.read();
    c += (uint16_t)vgmFile.read() << 8;
    p[pos] = (byte)c; //Ignore high byte
    if (pos < maxsize - 1) pos++;
  } while (c != 0);
  p[pos] = 0;
}

void printStringln(byte *p)
{
  while (*p) Serial.print((char)*p++);
  Serial.println("");
}

void dumpVgmMetadata() {
  //Output similar to VGMPlay
  Serial.println(F("VGM player (YM3812)"));
  Serial.println("");
  Serial.print(F("File name:      "));
  printStringln(filename);
  Serial.println("");

  vgmFile.seek(OFFSET_SAMPLE_COUNT); //Sample count: Number of samples in file, 44100 pr second
  uint32_t sampleCount  = (uint32_t)vgmFile.read();
  sampleCount          += (uint32_t)vgmFile.read() << 8;
  sampleCount          += (uint32_t)vgmFile.read() << 16;
  sampleCount          += (uint32_t)vgmFile.read() << 24;

  Serial.print(F("Duration:       "));
  uint32_t tmp = sampleCount / (44100 * 60); //Number of minutes
  Serial.print(tmp);
  Serial.print(":");
  uint32_t rem = sampleCount - tmp * (44100 * 60);
  tmp = rem / 44100; //Number of seconds
  Serial.print(tmp);
  Serial.print(".");
  rem = rem - tmp * 44100;
  tmp = rem / 441; //Number of centiseconds
  Serial.print(tmp);
  Serial.print(" (");
  Serial.print(sampleCount);
  Serial.println(" samples)");

  vgmFile.seek(OFFSET_GD3); //GD3 data offset
  uint32_t relativeGd3Offset  = (uint32_t)vgmFile.read();
  relativeGd3Offset          += (uint32_t)vgmFile.read() << 8;
  relativeGd3Offset          += (uint32_t)vgmFile.read() << 16;
  relativeGd3Offset          += (uint32_t)vgmFile.read() << 24;

  if (relativeGd3Offset == 0)
  {
    Serial.println(F("No GD3 tag in file"));
  }
  else
  {
    //Expecting compability with GD3 1.00
    vgmFile.seek(OFFSET_GD3 + relativeGd3Offset + 12);

    const int BUF_SIZE = 256;
    byte buf[BUF_SIZE];

    //1: Track title
    readGd3Field(buf, BUF_SIZE);
    Serial.print(F("Track title:    "));
    printStringln(buf);

    //2: Track title (Japanese), ignore
    readGd3Field(buf, BUF_SIZE);

    //3: Game name
    Serial.print(F("Game Name:      "));
    readGd3Field(buf, BUF_SIZE);
    printStringln(buf);

    //4: Game name (Japanese), ignore
    readGd3Field(buf, BUF_SIZE);

    //5: System name
    Serial.print(F("System:         "));
    readGd3Field(buf, BUF_SIZE);
    printStringln(buf);

    //6: System name (Japanese), ignore
    readGd3Field(buf, BUF_SIZE);

    //7: Name of Original Track Author
    Serial.print(F("Composer:       "));
    readGd3Field(buf, BUF_SIZE);
    printStringln(buf);

    //8: Name of Original Track Author (Japanese), ignore
    readGd3Field(buf, BUF_SIZE);
    
    //9: Date of game's release
    Serial.print(F("Release:        "));
    readGd3Field(buf, BUF_SIZE);
    printStringln(buf);

    //10: Name of person who converted it to a VGM file
    Serial.print(F("VGM by:         "));
    readGd3Field(buf, BUF_SIZE);
    printStringln(buf);
  }
}

void loadVgmSong(char* fileName) {
  vgmFile = SD.open(fileName, FILE_READ);

  dumpVgmMetadata();

  vgmFile.seek(OFFSET_LOOP_OFFSET); //VGM loop offset, return to this offset when reaching end of song
  relativeLoopOffset  = (uint32_t)vgmFile.read();
  relativeLoopOffset += (uint32_t)vgmFile.read() << 8;
  relativeLoopOffset += (uint32_t)vgmFile.read() << 16;
  relativeLoopOffset += (uint32_t)vgmFile.read() << 24;

  vgmFile.seek(OFFSET_VGM_DATA_OFFSET); //VGM data offset, beginning of song
  uint32_t relativeVgmOffset  = (uint32_t)vgmFile.read();
  relativeVgmOffset          += (uint32_t)vgmFile.read() << 8;
  relativeVgmOffset          += (uint32_t)vgmFile.read() << 16;
  relativeVgmOffset          += (uint32_t)vgmFile.read() << 24;

  vgmFile.seek(OFFSET_VGM_DATA_OFFSET + relativeVgmOffset);
}

void unsupportedCode(byte b)
{
  Serial.print(F("Unsupported code "));
  Serial.print(b);
  Serial.print(F(" offset "));  
  Serial.println(vgmFile.position());
  PlaybackStatus = PLAYBACK_ERROR_UNSUPPORTED_CODE;
}

void playVgmSongYM3812() {

  //Simple stupid implementation, will stop if reading unsupported byte
  
  byte code = vgmFile.read();

  if (code == 0x5A) { //YM3812 (OPL2)
    byte reg  = vgmFile.read();
    byte data = vgmFile.read();
    opl2.write(reg, data);
  }
  else if (code == 0x61) { //Delay 0-65535 samples (1.49 second)
    waitSamples  = (uint16_t)vgmFile.read();
    waitSamples += (uint16_t)vgmFile.read() << 8;
  }
  else if (code == 0x62) { //Delay 735 samples (1/60 second)
    waitSamples  = 735;
  }
  else if (code == 0x63) { //Delay 882 samples (1/50 second)
    waitSamples  = 882;
  }
  else if (code == 0x66) { //End of data
    if (relativeLoopOffset)
      vgmFile.seek(OFFSET_LOOP_OFFSET + relativeLoopOffset);
    else
      PlaybackStatus = PLAYBACK_COMPLETE;
  }
  else if (code >= 0x70 && code <= 0x7F) { //0-15 sample delay
    waitSamples = (code & 0x0F) + 1;
  }
  else {
    unsupportedCode(code);
  }
}
