/**
 * This is a simple demo for the OPL2 library. It plays a drum loop using instrument settings form the Adlib standard
 * instrument library. Note that channel 6 is used by the base drum, 7 is used by snare and hi-hat and 8 is used by tom
 * tom and cymbal.
 *
 * The OPL2 board is connedted as follows:
 *   OPL2 Board | Raspberry Pi | Orange Pi
 *  ------------+--------------+-----------
 *     Reset    |      13      |    18     
 *     A0       |      12      |    16     
 *     Latch    |      11      |    12     
 *     Data     |      19      |    19     
 *     Shift    |      23      |    23     
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
	opl2.init();

	opl2.setPercussion(true);
	opl2.setInstrument(0, BDRUM1);
	opl2.setInstrument(0, RKSNARE1);
	opl2.setInstrument(0, TOM2);
	opl2.setInstrument(0, CYMBAL1);
	opl2.setInstrument(0, HIHAT2);
	opl2.setBlock     (6, 4);
	opl2.setFNumber   (6, opl2.getNoteFNumber(6, 4, NOTE_C));
	opl2.setBlock     (7, 4);
	opl2.setFNumber   (7, opl2.getNoteFNumber(7, 3, NOTE_C));
	opl2.setBlock     (8, 4);
	opl2.setFNumber   (8, opl2.getNoteFNumber(8, 3, NOTE_A));

	while (true) {
		opl2.setDrums(i % 4 == 0, (i + 2) % 4 == 0, 0, i % 32 == 0 , i % 2);
		i ++;
		delay(250);
	}

	return 0;
}
