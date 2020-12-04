/**
 * This is a simple demo for the OPL2 library. It plays a musical scale on channels 0, 1 and 2 of the YM3812 using a
 * bell sound.
 *
 * The OPL2 board is connected as follows or run `./connecting`.
 *   Pin 13 - Reset
 *   Pin 12 - A0
 *   Pin 11 - Latch
 *   Pin 19 - Data
 *   Pin 23 - Shift
 *
 * IMPORTANT: Make sure you set the correct BOARD_TYPE in OPL2.h. Default is set to Arduino.
 *
 * Code by Maarten Janssen (maarten@cheerful.nl) 2017-05-04
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */

#include <OPL2.h>
#include <wiringPi.h>


OPL2 opl2;


int main(int argc, char **argv) {
	opl2.begin();

	// Setup channels 0, 1 and 2 to produce a bell sound.
	for (byte i = 0; i < 3; i ++) {
		opl2.setTremolo   (i, CARRIER, true);
		opl2.setVibrato   (i, CARRIER, true);
		opl2.setMultiplier(i, CARRIER, 0x04);
		opl2.setAttack    (i, CARRIER, 0x0A);
		opl2.setDecay     (i, CARRIER, 0x04);
		opl2.setSustain   (i, CARRIER, 0x0F);
		opl2.setRelease   (i, CARRIER, 0x0F);
		opl2.setVolume    (i, CARRIER, 0x00);
	}

	// Play notes on alternating channels.
	for (byte i = 0; i < 24; i ++) {
		byte octave = 3 + (i / 12);
		byte note = i % 12;
		opl2.playNote(i % 3, octave, note);
		delay(300);
	}

	return 0;
}
