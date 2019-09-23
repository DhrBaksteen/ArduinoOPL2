/**
 * This example can be used together with a Teensy 2.0 or later to use the OPL2 Audio Board as a MIDI device. To
 * configure the Teensy as a MIDI device set USB Type to MIDI in the IDE using Tools > USB Type > MIDI. Once connected
 * the board should appear in the device list as 'OPL2 AUdio Board MIDI'. You can now use test the board with, for
 * example, MIDI-OX, your favorite music creation software or DosBox! 
 *
 * OPL2 board is connected as follows:
 *   Pin  8 - Reset
 *   Pin  9 - A0
 *   Pin 10 - Latch
 *   Pin 11 - Data
 *   Pin 13 - Shift
 *
 * Code by Maarten Janssen (maarten@cheerful.nl) 2018-07-02
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */

#include <SPI.h>
#include <OPL2.h>
#include <midi_instruments.h>
#include <midi_drums.h>

#define MIDI_NUM_CHANNELS 16
#define MIDI_DRUM_CHANNEL 10

#define CONTROL_VOLUME          7
#define CONTROL_ALL_SOUND_OFF 120
#define CONTROL_RESET_ALL     121
#define CONTROL_ALL_NOTES_OFF 123

// Channel mapping to keep track of MIDI to OPL2 channel mapping.
struct ChannelMapping {
	byte midiChannel;
	byte midiNote;
	float midiVelocity;
	float op1Level;
	float op2Level;
};


OPL2 opl2;
ChannelMapping channelMap[OPL2_NUM_CHANNELS];
byte oldestChannel[OPL2_NUM_CHANNELS];
byte programMap[MIDI_NUM_CHANNELS];
float channelVolumes[MIDI_NUM_CHANNELS];


/**
 * Register MIDI event handlers and initialize.
 */
void setup() {
	usbMIDI.setHandleNoteOn(onNoteOn);
	usbMIDI.setHandleNoteOff(onNoteOff);
	usbMIDI.setHandleProgramChange(onProgramChange);
	usbMIDI.setHandleControlChange(onControlChange);
	usbMIDI.setHandleSystemReset(onSystemReset);
	onSystemReset();

	opl2.setDeepVibrato(true);
	opl2.setDeepTremolo(true);
}


/**
 * Read and handle MIDI events.
 */
void loop() {
	usbMIDI.read();
}


/**
 * Get a free OPL2 channel to play a note. If all channels are occupied then recycle the oldes one.
 */
byte getFreeChannel(byte midiChannel) {  
	byte opl2Channel = 255;
	byte oldestIndex = 0;

	// Look for a free OPL2 channel starting with the oldest.
	for (byte i = 0; i < OPL2_NUM_CHANNELS; i ++) {
		if (!opl2.getKeyOn(oldestChannel[i])) {
			opl2Channel = oldestChannel[i];
			oldestIndex = i;
			break;
		}
	}

	// If no channels are free then recycle the oldest, where drum channels will be the first to be recycled. Only if
	// no drum channels are left will the actual oldest channel be recycled.
	if (opl2Channel == 255) {
		opl2Channel = oldestChannel[0];
		for (byte i = 0; i < OPL2_NUM_CHANNELS; i ++) {
			if (channelMap[oldestChannel[i]].midiChannel == MIDI_DRUM_CHANNEL) {
				opl2Channel = oldestChannel[i];
				oldestIndex = i;
				break;
			}
		}
		opl2.setKeyOn(opl2Channel, false);
	}

	// Update the list of last used channels by moving the current channel to the bottom so the last updated channel
	// will move to the front of the list. If no more OPL2 channels are free then the last updated one will be recycled.
	for (byte i = oldestIndex; i < OPL2_NUM_CHANNELS - 1; i ++) {
		oldestChannel[i] = oldestChannel[i + 1];
	}
	oldestChannel[OPL2_NUM_CHANNELS - 1] = opl2Channel;

	return opl2Channel;
}


/**
 * Set the volume of operators 1 and 2 of the given OPL2 channel according to the settings of the given MIDI channel.
 */
void setOpl2ChannelVolume(byte opl2Channel, byte midiChannel) {
	float volume = channelMap[opl2Channel].midiVelocity * channelVolumes[midiChannel];
	byte volumeOp1 = round(channelMap[opl2Channel].op1Level * volume * 63.0);
	byte volumeOp2 = round(channelMap[opl2Channel].op2Level * volume * 63.0);
	opl2.setVolume(opl2Channel, OPERATOR1, 63 - volumeOp1);
	opl2.setVolume(opl2Channel, OPERATOR2, 63 - volumeOp2);
}


/**
 * Handle a note on MIDI event to play a note.
 */
void onNoteOn(byte channel, byte note, byte velocity) {
	// Treat notes with a velocity of 0 as note off.
	if (velocity == 0) {
		onNoteOff(channel, note, velocity);
		return;
	}

	// Get an available OPL2 channel and setup instrument parameters.
	byte opl2Channel = getFreeChannel(channel);
	if (channel != MIDI_DRUM_CHANNEL) {
		opl2.setInstrument(opl2Channel, midiInstruments[programMap[channel]]);
	} else {
		if (note >= DRUM_NOTE_BASE && note < DRUM_NOTE_BASE + NUM_MIDI_DRUMS) {
			opl2.setInstrument(opl2Channel, midiDrums[note - DRUM_NOTE_BASE]);
		} else {
			return;
		}
	}

	// Register channel mapping.
	channelMap[opl2Channel].midiChannel  = channel;
	channelMap[opl2Channel].midiNote     = note;
	channelMap[opl2Channel].midiVelocity = min((float)velocity / 64.0, 1.0);
	channelMap[opl2Channel].op1Level     = (float)(63 - opl2.getVolume(opl2Channel, OPERATOR1)) / 63.0;
	channelMap[opl2Channel].op2Level     = (float)(63 - opl2.getVolume(opl2Channel, OPERATOR2)) / 63.0;

	// Set operator output levels based on note velocity.
	setOpl2ChannelVolume(opl2Channel, channel);

	// Calculate octave and note number and play note!
	byte opl2Octave = 4;
	byte opl2Note = NOTE_C;
	if (channel != MIDI_DRUM_CHANNEL) {
		note = max(24, min(note, 119));
		opl2Octave = 1 + (note - 24) / 12;
		opl2Note   = note % 12;
	}
	opl2.playNote(opl2Channel, opl2Octave, opl2Note);
}


/**
 * Handle a note off MIDI event to stop playing a note.
 */
void onNoteOff(byte channel, byte note, byte velocity) {
	channel = channel % 16;
	for (byte i = 0; i < OPL2_NUM_CHANNELS; i ++) {
		if (channelMap[i].midiChannel == channel && channelMap[i].midiNote == note) {
			opl2.setKeyOn(i, false);
			channelMap[i].midiChannel = 0xFF;
			channelMap[i].midiNote = 0x00;

			// Move channel to the back of recently used channels list to prevent it from
			// being reused immediately and the release portion of the note being clubbored.
			byte oldestIndex = 0;
			for (byte j = 0; j < OPL2_NUM_CHANNELS; j ++) {
				if (oldestChannel[j] == i) {
					oldestIndex = j;
					break;
				}
			}
			for (byte j = oldestIndex; j < OPL2_NUM_CHANNELS - 1; j ++) {
				oldestChannel[j] = oldestChannel[j + 1];
			}
			oldestChannel[OPL2_NUM_CHANNELS - 1] = i;

			break;
		}
	}
}


/**
 * Handle instrument change on the given MIDI channel.
 */
void onProgramChange(byte channel, byte program) {
	programMap[channel % 16] = min(program, 127);
}


/**
 * Handle MIDI control changes on the given channel.
 */
void onControlChange(byte channel, byte control, byte value) {
	channel = channel % 16;

	switch (control) {

		// Change volume of a MIDI channel. (Limited to 0.8 to prevent clipping)
		case CONTROL_VOLUME: {
			channelVolumes[channel] = min(value / 127.0, 0.8);
			for (byte i = 0; i < OPL2_NUM_CHANNELS; i ++) {
				if (channelMap[i].midiChannel == channel && opl2.getKeyOn(i)) {
					setOpl2ChannelVolume(i, channel);
				}
			}
			break;
		}

		// Reset all controller values.
		case CONTROL_RESET_ALL:
			for (byte i = 0; i < MIDI_NUM_CHANNELS; i ++) {
				channelVolumes[channel] = 0.8;
			}
		break;

		// Immediately silence all channels.
		// Intentionally cascade into CONTROL_ALL_NOTES_OFF!
		case CONTROL_ALL_SOUND_OFF:
			for (byte i = 0; i < OPL2_NUM_CHANNELS; i ++) {
				opl2.setRelease(i, OPERATOR1, 0);
				opl2.setRelease(i, OPERATOR2, 0);
			}

		// Silence all MIDI channels.
		case CONTROL_ALL_NOTES_OFF: {
			for (byte i = 0; i < OPL2_NUM_CHANNELS; i ++) {
				if (channelMap[i].midiChannel == channel) {
					onNoteOff(channelMap[i].midiChannel, channelMap[i].midiNote, 0);
				}
			}
			break;
		}

		// Ignore any other MIDI controls.
		default:
			break;
	}
}


/**
 * Handle full system reset.
 */
void onSystemReset() {
	opl2.init();

	// Silence all channels and set default instrument.
	for (byte i = 0; i < OPL2_NUM_CHANNELS; i ++) {
		opl2.setKeyOn(i, false);
		opl2.setInstrument(i, midiInstruments[0]);
		oldestChannel[i] = i;
	}

	// Reset default MIDI player parameters.
	for (byte i = 0; i < MIDI_NUM_CHANNELS; i ++) {
		programMap[i] = 0;
		channelVolumes[i] = 0.8;
	}
}
