/**
 * This sketch tests if your OPL3 Duo board is functioning correctly. Open the serial terminal and follow the
 * instructions all tests indicate what you should hear when your board is functioning correctly.
 *
 * Code by Maarten Janssen, 2020-10-27
 * WWW.CHEERFUL.NL
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */


#include <stdio.h>
#include <wiringPi.h>
#include <OPL3Duo.h>
#include "instruments.h"


OPL3Duo opl3Duo;
Instrument testInstrument;
Instrument testInstrument2;


/**
 * Play a tone of 250 Hz on the given channel.
 */
void playTone(byte channel) {
	opl3Duo.setInstrument(channel, testInstrument2);
	opl3Duo.playNote(channel, 4, NOTE_C);
}


void testSynthUnit1() {
	printf("TEST 1: Synth unit 1...\n");
	printf("- You should hear a steady tone and a continuous scale through both speakers.\n");

	// Play a 250 Hz tone on channel 0 of synth unit 2.
	playTone(18);

	// Play a scale on all channels of synth unit 1.
	byte channel = 0;
	byte note = 0;
	for (int i = 0; i < 18; i ++) {
		opl3Duo.setInstrument(channel, testInstrument);
		opl3Duo.playNote(channel, 5, note);
		delay(200);
		opl3Duo.setKeyOn(channel, false);

		channel = (channel + 1) % 18;
		note = (note + 1) % 12;
	}

	// Stop background tone.
	opl3Duo.setKeyOn(18, false);
	while (getchar() != '\n');
}


void testUnit1Panning() {
	printf("TEST 2: Synth unit 1 left channel...\n");
	printf("- You should hear a steady tone through the left speaker only.\n");

	opl3Duo.setInstrument(0, testInstrument2);
	opl3Duo.setPanning(0, true, false);
	opl3Duo.playNote(0, 4, NOTE_C);
	while (getchar() != '\n');
	opl3Duo.setKeyOn(0, false);

	printf("Test 3: Synth unit 1 right channel...\n");
	printf("- You should hear a steady tone through the right speaker only.\n");

	opl3Duo.setPanning(0, false, true);
	opl3Duo.playNote(0, 4, NOTE_C);
	while (getchar() != '\n');

	opl3Duo.setKeyOn(0, false);
	opl3Duo.setPanning(0, true, true);
}


void testSynthUnit2() {
	printf("TEST 4: Synth unit 2...\n");
	printf("- You should hear a steady tone and a continuous scale through both speakers.\n");

	// Play a 250 Hz tone on channel 0 of synth unit 1.
	playTone(0);

	// Play a scale on all channels of synth unit 2.
	byte channel = 0;
	byte note = 0;
	for (int i = 0; i < 18; i ++) {
		opl3Duo.setInstrument(18 + channel, testInstrument);
		opl3Duo.playNote(18 + channel, 5, note);
		delay(200);
		opl3Duo.setKeyOn(18 + channel, false);

		channel = (channel + 1) % 18;
		note = (note + 1) % 12;
	}

	// Stop background tone.
	opl3Duo.setKeyOn(0, false);
	while (getchar() != '\n');
}


void testUnit2Panning() {
	printf("TEST 5: Synth unit 2 left channel...\n");
	printf("- You should hear a steady tone through the left speaker only.\n");

	opl3Duo.setInstrument(18, testInstrument2);
	opl3Duo.setPanning(18, true, false);
	opl3Duo.playNote(18, 4, NOTE_C);
	while (getchar() != '\n');
	opl3Duo.setKeyOn(18, false);

	printf("Test 6: Synth unit 2 right channel...\n");
	printf("- You should hear a steady tone through the right speaker only.\n");

	opl3Duo.setPanning(18, false, true);
	opl3Duo.playNote(18, 4, NOTE_C);
	while (getchar() != '\n');
	opl3Duo.setKeyOn(18, false);

	opl3Duo.setPanning(18, true, true);
}


void testReset() {
	printf("TEST 7: OPL3 Duo reset...\n");
	printf("- There should be no sound.\n");

	playTone(0);
	playTone(18);
	opl3Duo.reset();

	while (getchar() != '\n');
}


int main(int argc, char **argv) {
    opl3Duo.begin();

    testInstrument = opl3Duo.loadInstrument(INSTRUMENT_SYN1);
    testInstrument2 = opl3Duo.loadInstrument(INSTRUMENT_FLUTE);

    printf("This sketch tests the basic functions of the OPL3 Duo board.\n");
    printf("The expected test reulst is shown for each test.\n");
    printf("On each test press ENTER to advance to the next test.\n");
    printf("Press ENTER to begin...\n\n");
	while (getchar() != '\n');

	testSynthUnit1();
	opl3Duo.setOPL3Enabled(true);
	testUnit1Panning();
	opl3Duo.setOPL3Enabled(false);
	testSynthUnit2();
	opl3Duo.setOPL3Enabled(true);
	testUnit2Panning();
	opl3Duo.setOPL3Enabled(false);
	testReset();

	printf("Done.\n");
	return 0;
}

