#include <OPL2.h>
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include "rad.h"


void* operator new(size_t size) { return malloc(size); }
void operator delete(void* ptr) { if (ptr) free(ptr); }

FILE *oplFile;
OPL2 opl2;


unsigned char fileRead(FILE *file) {
	unsigned char byte[1];
	if (fread(byte, sizeof(byte), 1, file)) {
		return byte[0];
	}
	return 0;
}


unsigned short fileReadWord(FILE *file) {
	unsigned short value = fileRead(file);
	value += fileRead(file) << 8;
	return value;
}


Song loadRAD(FILE *radFile) {
	Song song;

	fseek(radFile, 0x11, SEEK_SET);
	unsigned char songProps = fileRead(radFile);
	song.hasSlowTimer = (songProps & 0x40) != 0x00;
	song.initialSpeed = songProps & 0x1F;

	// Skip song description block.
	if ((songProps & 0x80) != 0x00) {
		while (fileRead(radFile) != 0x00);
	}

	// Read instruments.
	unsigned char instrumentNum = 0;
	while ((instrumentNum = fileRead(radFile)) != 0x00) {
		Instrument *instrument = new Instrument;
		instrument->carProps = fileRead(radFile);
		instrument->modProps = fileRead(radFile);
		instrument->carLevel = fileRead(radFile);
		instrument->modLevel = fileRead(radFile);
		instrument->carAttackDecay = fileRead(radFile);
		instrument->modAttackDecay = fileRead(radFile);
		instrument->carSustainRelease = fileRead(radFile);
		instrument->modSustainRelease = fileRead(radFile);
		instrument->channelProps = fileRead(radFile);
		instrument->carWaveform = fileRead(radFile) & 0x07;
		instrument->modWaveform = fileRead(radFile) & 0x07;
		song.instruments[instrumentNum - 1] = instrument;
	}

	// Read order list.
	song.songLength = fileRead(radFile);
	for (int i = 0; i < song.songLength; i++) {
		song.orders[i] = fileRead(radFile);
	}

	// Read pattern offsets.
	for (int i = 0; i < 32; i++) {
		Pattern *pattern = new Pattern;
		pattern->fileOffset = fileReadWord(radFile);
		song.patterns[i] = pattern;
	}

	// Read pattern data.
	for (int i = 0; i < 32; i++) {
		Pattern *pattern = song.patterns[i];
		if (pattern->fileOffset != 0x00) {

			// Clear the pattern.
			for (int l = 0; l < 64; l++) {
				for (int c = 0; c < 9; c++) {
					Note *note = new Note;
					pattern->notes[l][c] = note;
				}
			}

			// Load pattern data.
			unsigned char line     = 0;
			unsigned char channel  = 0;
			unsigned char noteData = 0;
			fseek(radFile, pattern->fileOffset, SEEK_SET);
			do {
				line = fileRead(radFile);

				do {
					channel = fileRead(radFile);
					Note *note = pattern->notes[line & 0x3F][channel & 0x0F];

					noteData = fileRead(radFile);
					note->instrument = (noteData & 0x80) >> 3;
					note->octave     = (noteData & 0x70) >> 4;
					note->note       = noteData & 0x0F;

					noteData = fileRead(radFile);
					note->instrument += (noteData & 0xF0) >> 4;
					note->effect     = noteData & 0x0F;
					note->parameter  = note->effect ? fileRead(radFile) : 0x00;
				} while ((channel & 0x80) == 0x00);
			} while ((line & 0x80) == 0x00);
		}
	}

	return song;
}


unsigned char tick  = 0;
unsigned char line  = 0;
unsigned char order = 0;
unsigned char speed = 6;
unsigned char slideToBlock[9]    = { 0 };
unsigned short slideToFNumber[9] = { 0 };
unsigned char slideSpeed[9]      = { 0 };
unsigned int overTime = 0;
Song song;
Pattern *currPattern;


void setInstrument(unsigned char channel, Instrument *instrument) {
	unsigned char registerOffset;

	registerOffset = opl2.getRegisterOffset(channel, CARRIER);
	opl2.setRegister(0x20 + registerOffset, instrument->carProps);
	opl2.setRegister(0x40 + registerOffset, instrument->carLevel);
	opl2.setRegister(0x60 + registerOffset, instrument->carAttackDecay);
	opl2.setRegister(0x80 + registerOffset, instrument->carSustainRelease);
	opl2.setRegister(0xE0 + registerOffset, instrument->carWaveform);

	registerOffset = opl2.getRegisterOffset(channel, MODULATOR);
	opl2.setRegister(0x20 + registerOffset, instrument->modProps);
	opl2.setRegister(0x40 + registerOffset, instrument->modLevel);
	opl2.setRegister(0x60 + registerOffset, instrument->modAttackDecay);
	opl2.setRegister(0x80 + registerOffset, instrument->modSustainRelease);
	opl2.setRegister(0xE0 + registerOffset, instrument->modWaveform);

	opl2.setRegister(0xC0 + channel, instrument->channelProps);
}


void playNote(unsigned char channel, unsigned char octave, unsigned char note) {
	opl2.setKeyOn(channel, false);
	opl2.setBlock(channel, octave);
	opl2.setFNumber(channel, noteFrequencies[note - 1]);
	opl2.setKeyOn(channel, true);
}


/**
 * Slide the pitch of a channel by a given F-number amount.
 */
void pitchAdjust(unsigned char channel, short amount) {
	short fNumber = opl2.getFNumber(channel);
	fNumber = max(0x000, min(fNumber + amount, 0x3FF));

	if (fNumber < 0x156) {
		unsigned char block = opl2.getBlock(channel);
		if (block > 0) {
			opl2.setBlock(channel, block - 1);
			fNumber = 0x2AE + (fNumber - 0x156);
		}
	} else if (fNumber > 0x2AE) {
		unsigned char block = opl2.getBlock(channel);
		if (block < 7) {
			opl2.setBlock(channel, block + 1);
			fNumber = 0x156 + (fNumber - 0x2AE);
		}
	}

	opl2.setFNumber(channel, fNumber);
}


void pitchAdjustToNote(unsigned char channel) {
	unsigned char block   = opl2.getBlock(channel);
	unsigned char fNumber = opl2.getFNumber(channel);

	if (block < slideToBlock[channel] || fNumber < slideToFNumber[channel]) {
		pitchAdjust(channel, slideSpeed[channel]);
		if (opl2.getBlock(channel) >= slideToBlock[channel] && opl2.getFNumber(channel) > slideToFNumber[channel]) {
			opl2.setBlock(channel, slideToBlock[channel]);
			opl2.setFNumber(channel, slideToFNumber[channel]);
		}
	} else if (block > slideToBlock[channel] || fNumber > slideToFNumber[channel]) {
		pitchAdjust(channel, -slideSpeed[channel]);
		if (opl2.getBlock(channel) <= slideToBlock[channel] && opl2.getFNumber(channel) < slideToFNumber[channel]) {
			opl2.setBlock(channel, slideToBlock[channel]);
			opl2.setFNumber(channel, slideToFNumber[channel]);
		}
	}
}


void volumeAdjust(unsigned char channel, unsigned char amount) {
	unsigned char volume = opl2.getVolume(channel, CARRIER);

	if (amount > 0 && amount < 50) {
		volume = min(63, volume + amount);
	} else if (amount > 50 && amount < 100) {
		volume = max(0, volume - (amount - 50));
	}

	opl2.setVolume(channel, CARRIER, volume);
}


void initPlayer() {
	opl2.init();
	opl2.setWaveFormSelect(true);
	opl2.setPercussion(false);
	speed = song.initialSpeed;

	order = 0;
	line = 0;
	tick = 0;
	currPattern = song.patterns[song.orders[order]];
}


void nextOrder(unsigned char startLine = 0) {
	order ++;
	if (song.orders[order] >= 128) {
		order = song.orders[order] % 128;
	}

	unsigned char nextOrder = song.orders[order];
	currPattern = song.patterns[nextOrder];
	line = startLine;
}


void playMusic() {
	unsigned long time = millis();

	for (int channel = 0; channel < 9; channel ++) {
		Note *note = currPattern->notes[line][channel];

		if (tick == 0) {

			// Stop note.
			if (note->note == 0x0F) {
				opl2.setKeyOn(channel, false);
			}

			// Trigger note.
			else if (note->instrument > 0x00) {
				Instrument *instrument = song.instruments[note->instrument - 1];
				if (instrument != NULL) {
					setInstrument(channel, instrument);
				}
				if (note->effect != EFFECT_NOTE_SLIDE_TO) {
					playNote(channel, note->octave, note->note);
				}
			}

			// Process line effects.
			switch(note->effect) {
				case EFFECT_NOTE_SLIDE_TO: {
					if (note->note > 0x00 && note->note < 0x0F) {
						slideToBlock[channel]   = note->octave;
						slideToFNumber[channel] = noteFrequencies[note->note - 1];
						slideSpeed[channel]     = note->parameter;
					}
					break;
				}

				case EFFECT_SET_VOLUME: {
					opl2.setVolume(channel, CARRIER, max(0, min(64 - note->parameter, 63)));
					break;
				}

				case EFFECT_PATTERN_BREAK: {
					nextOrder(note->parameter);
					break;
				}

				case EFFECT_SET_SPEED: {
					speed = note->parameter;
					break;
				}
			}
		}

		// Process tick effects.
		switch(note->effect) {
			case EFFECT_NOTE_SLIDE_UP: {
				pitchAdjust(channel, note->parameter);
				break;
			}

			case EFFECT_NOTE_SLIDE_DOWN: {
				pitchAdjust(channel, -note->parameter);
				break;
			}

			case EFFECT_NOTE_SLIDE_VOLUME: {
				pitchAdjustToNote(channel);
				volumeAdjust(channel, note->parameter);
				break;
			}

			case EFFECT_NOTE_SLIDE_TO: {
				pitchAdjustToNote(channel);
				break;
			}

			case EFFECT_VOLUME_SLIDE: {
				volumeAdjust(channel, note->parameter);
				break;
			}
		}
	}

	tick = (tick + 1) % speed;
	if (tick == 0) {
		line = (line + 1) % 64;
		if (line == 0) {
			nextOrder();
		}
	}

	int timeSpent = (millis() - time) + overTime;
	int wait = (song.hasSlowTimer ? 55 : 20) - timeSpent;
	overTime = -min(wait, 0);
	delay(max(0, wait));
}


int main(int argc, char **argv) {
	oplFile = fopen("./tunes/RASTER.RAD", "rb");
	song = loadRAD(oplFile);

	initPlayer();
	while (order < song.songLength) {
		playMusic();
	}

	return 0;
}

