/**
 * Though the OPL3 can produce stereo sound, it's panning capabilities are only limited. You can eather play the sound
 * of a channel on the left speaker only, the right speaker only or on both. Gradual panning between the left and right
 * speaker is not possible. This demonstration shows how gradual panning can still be achieved using two channels.
 * Channel 0 will output to the left speaker and channel 1 will output to the right speaker. By inverting the volume of
 * channel 0 on channel 1 we can achieve 63 steps of panning as this demo will show with a ping sound that drifts
 * between the left and the right speaker.
 */


#include <SPI.h>
#include <OPL3Duo.h>


OPL3Duo opl3Duo;
int t = 0;


void setup() {
	// Initialize the OPL3Duo! and enable OPL3 features (requered to enable panning support).
	opl3Duo.begin();
	opl3Duo.setOPL3Enabled(true);

	// Define a simple ping sound for channels 0 and 1.
	for (byte i = 0; i < 2; i++) {
		for (byte j = 0; j < 2; j ++) {
			opl3Duo.setAttack(i, j, 5);
			opl3Duo.setDecay(i, j, 5);
			opl3Duo.setSustain(i, j, 3);
			opl3Duo.setRelease(i, j, 4);
			opl3Duo.setMaintainSustain(i, j, false);
			opl3Duo.setEnvelopeScaling(i, j, true);
			opl3Duo.setMultiplier(i, j, 1);
			opl3Duo.setScalingLevel(i, j, 1);
			opl3Duo.setVolume(i, j, 0);
		}
		opl3Duo.setSynthMode(i, true);
	}

	// Set panning for channels 0 (left) and 1 (right).
	opl3Duo.setPanning(0, true, false);
	opl3Duo.setPanning(1, false, true);
}


void loop() {
	// Restart the tone every 300 cycles.
	if (t % 300 == 0) {
	opl3Duo.playNote(0, 5, NOTE_A);
	opl3Duo.playNote(1, 5, NOTE_A);
	}

	// Define channel volume as a sine wave so it will gradually increase and decrease.
	byte volume = (sin((float)t / 10) * 31) + 31;

	// Apply volume to operators of channel 0.
	opl3Duo.setVolume(0, 0, volume);
	opl3Duo.setVolume(0, 1, volume);

	// Apply inverse of volume to operators of channel 1.
	opl3Duo.setVolume(1, 0, 63 - volume);
	opl3Duo.setVolume(1, 1, 63 - volume);

	delay(10);
	t ++;
}
