/**
 * This is a simple demo for the OPL2 library. It plays a drum loop using instrument settings form the Adlib standard
 * instrument library. Note that channel 6 is used by the base drum, 7 is used by snare and hi-hat and 8 is used by tom
 * tom and cymbal.
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
#include "instruments.h"


OPL2 opl2;
int i = 0;


int main(int argc, char **argv) {
	opl2.begin();

	// Set percussion mode and load instruments.
	Instrument bass = opl2.loadInstrument(INSTRUMENT_BDRUM2);
	Instrument snare = opl2.loadInstrument(INSTRUMENT_RKSNARE1);
	Instrument tom = opl2.loadInstrument(INSTRUMENT_TOM2);
	Instrument cymbal = opl2.loadInstrument(INSTRUMENT_CYMBAL1);
	Instrument hihat = opl2.loadInstrument(INSTRUMENT_HIHAT2);

	opl2.setPercussion(true);
	opl2.setDrumInstrument(bass, DRUM_BASS);
	opl2.setDrumInstrument(snare, DRUM_SNARE);
	opl2.setDrumInstrument(tom, DRUM_TOM);
	opl2.setDrumInstrument(cymbal, DRUM_CYMBAL);
	opl2.setDrumInstrument(hihat, DRUM_HI_HAT);

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


	// Play drum loop
	while (true) {
		bool bass   = i % 4 == 0;           // Bass drum every 1st tick
		bool snare  = i % 4 == 2;           // Snare drum every 3rd tick
		bool tom    = i % 8 > 3;            // Tom tom
		bool cymbal = i % 32 == 0;          // Cymbal every 32nd tick
		bool hiHat  = true;                 // Hi-hat every tick

		opl2.setDrums(bass, snare, tom, cymbal, hiHat);

		i ++;
		delay(200);
	}

	return 0;
}
