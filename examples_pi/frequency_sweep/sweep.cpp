/**
 * This is a simple demo for the OPL2 library that demonstrates altering the frequency of an active channel. This demo
 * sweeps the frequency between 250 and 750 Hz on channel 0.
 *
 * The OPL2 board is connected as follows or run `./connecting`.
 *   Pin 13 - Reset
 *   Pin 12 - A0
 *   Pin 11 - Latch
 *   Pin 19 - Data
 *   Pin 23 - Shift
 *
 * Code by Maarten Janssen (maarten@cheerful.nl) 2018-04-30
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */


#include <OPL2.h>
#include <math.h>
#include <wiringPi.h>


OPL2 opl2;

int main(int argc, char **argv) {
	opl2.begin();

	// Setup channel 0 carrier.
	opl2.setMaintainSustain(0, CARRIER, true);
	opl2.setMultiplier(0, CARRIER, 0x01);
	opl2.setAttack    (0, CARRIER, 0x0A);
	opl2.setSustain   (0, CARRIER, 0x04);
	opl2.setVolume    (0, CARRIER, 0x00);

	// Start tone.
	float t = 0.0;
	opl2.setKeyOn(0, true);
	while (true) {
		float freq = sin(t) * 250 + 500;
		opl2.setFrequency(0, freq);

		t += .01;
		delay(10);
	}
}
