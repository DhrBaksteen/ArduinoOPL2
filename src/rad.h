#pragma once


#define EFFECT_NONE              0x00
#define EFFECT_NOTE_SLIDE_UP     0x01
#define EFFECT_NOTE_SLIDE_DOWN   0x02
#define EFFECT_NOTE_SLIDE_TO     0x03
#define EFFECT_NOTE_SLIDE_VOLUME 0x05
#define EFFECT_VOLUME_SLIDE      0x0A
#define EFFECT_SET_VOLUME        0x0C
#define EFFECT_PATTERN_BREAK     0x0D
#define EFFECT_SET_SPEED         0x0F

#define FNUMBER_MIN 156
#define FNUMBER_MAX 686
#define FNUMBER_RANGE 530

struct Instrument {
	unsigned char carProps = 0;
	unsigned char modProps = 0;
	unsigned char carLevel = 0;
	unsigned char modLevel = 0;
	unsigned char carAttackDecay = 0;
	unsigned char modAttackDecay = 0;
	unsigned char carSustainRelease = 0;
	unsigned char modSustainRelease = 0;
	unsigned char channelProps = 0;
	unsigned char carWaveform = 0;
	unsigned char modWaveform = 0;
};


struct Note {
	unsigned char instrument = 0;
	unsigned char octave = 0;
	unsigned char note = 0;
	unsigned char effect = 0;
	unsigned char parameter = 0;
};


struct Pattern {
	unsigned short fileOffset = 0;
	Note *notes[64][9] = { { NULL } };
};


struct Song {
	Instrument *instruments[31] = { NULL };
	Pattern *patterns[32] = { NULL };
	unsigned char songLength = 0;
	unsigned char orders[128] = { 0 };
	unsigned char initialSpeed = 0;
	bool hasSlowTimer = false;				// Normal speed is 50Hz, slow speed is 18.2Hz
};


unsigned short noteFrequencies[12] = {
	0x16B, 0x181, 0x198, 0x1B0, 0x1CA, 0x1E5,
	0x202, 0x220, 0x241, 0x263, 0x287, 0x2AE
};
