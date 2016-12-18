#include <Arduino.h>

// Arduino pin definitions.
#define PIN_RESET  8
#define PIN_A0     9
#define PIN_LATCH 10
#define PIN_DATA  11
#define PIN_CLOCK 13

// Operator definitions.
#define OPERATOR1 false
#define OPERATOR2 true
#define MODULATOR false
#define CARRIER   true

// Synthesis type definitions.
#define FREQ_MODULATION false
#define ADDITIVE_SYNTH  true

// Drum sounds.
#define DRUM_BASS   0x10
#define DRUM_SNARE  0x08
#define DRUM_TOM    0x04
#define DRUM_CYMBAL 0x02
#define DRUM_HI_HAT 0x01

// Note to frequency mapping.
#define NOTE_C   0
#define NOTE_CS  8
#define NOTE_D   1
#define NOTE_DS  9
#define NOTE_E   2
#define NOTE_F   3
#define NOTE_FS 10
#define NOTE_G   4
#define NOTE_GS 11
#define NOTE_A   5
#define NOTE_AS 12
#define NOTE_B   6


class OPL2 {
	public:
		OPL2();
		void init();
		void reset();
		void write(byte, byte);
		void updateRegister(byte);

		short getNoteFrequency(byte, byte, byte);
		byte getRegister(byte);
		bool getWaveFormSelect();
		bool getTremolo(byte, bool);
		bool getVibrato(byte, bool);
		bool getMaintainSustain(byte, bool);
		bool getEnvelopeScaling(byte, bool);
		byte getMultiplier(byte, bool);
		byte getScalingLevel(byte, bool);
		byte getVolume(byte, bool);
		byte getAttack(byte, bool);
		byte getDecay(byte, bool);
		byte getSustain(byte, bool);
		byte getRelease(byte, bool);
		short getFrequency(byte);
		byte getBlock(byte);
		bool getKeyOn(byte);
		byte getFeedback(byte);
		bool getSynthMode(byte);
		bool getDeepTremolo();
		bool getDeepVibrato();
		bool getPercussion();
		byte getDrums();
		byte getWaveForm(byte, bool);

		byte setRegister(byte, byte);
		byte setWaveFormSelect(bool);
		byte setTremolo(byte, bool, bool);
		byte setVibrato(byte, bool, bool);
		byte setMaintainSustain(byte, bool, bool);
		byte setEnvelopeScaling(byte, bool, bool);
		byte setMultiplier(byte, bool, byte);
		byte setScalingLevel(byte, bool, byte);
		byte setVolume(byte, bool, byte);
		byte setAttack(byte, bool, byte);
		byte setDecay(byte, bool, byte);
		byte setSustain(byte, bool, byte);
		byte setRelease(byte, bool, byte);
		byte setFrequency(byte, short);
		byte setBlock(byte, byte);
		byte setKeyOn(byte, bool);
		byte setFeedback(byte, byte);
		byte setSynthMode(byte, bool);
		byte setDeepTremolo(bool);
		byte setDeepVibrato(bool);
		byte setPercussion(bool);
		byte setDrums(bool, bool, bool, bool, bool);
		byte setWaveForm(byte, bool, byte);

	private:
		const float fIntervals[8] = {
			0.048, 0.095, 0.190, 0.379, 0.759, 1.517, 3.034, 6.069
		};
		const float notes[13] = {
			261.63, 293.66, 329.63, 349.23, 392.00, 440.00, 493.88, 523.26,
			277.18, 311.13, 369.99, 415.30, 466.16
		};
		byte oplRegisters[256];
		byte getRegisterOffset(byte, bool);
};

