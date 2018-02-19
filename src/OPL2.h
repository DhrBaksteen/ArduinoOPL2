#ifndef TRUE
	#define TRUE 1
#endif


#ifndef FALSE
	#define FALSE 0
#endif


#ifndef NULL
	#define NULL 0
#endif


#ifndef OPL2_LIB_H_
	#define OPL2_LIB_H_

	#define ARDUINO      0
	#define RASPBERRY_PI 1
	#define ORANGE_PI    2

	// !!! IMPORTANT !!!
	// In order to correctly compile the library for your platform be sure to set the correct BOARD_TYPE below.
	#define BOARD_TYPE ORANGE_PI

	#if BOARD_TYPE == ARDUINO
		#define PIN_LATCH 10
		#define PIN_ADDR   9
		#define PIN_RESET  8
	#elif BOARD_TYPE == RASPBERRY_PI or BOARD_TYPE == ORANGE_PI
		#define PIN_LATCH 1				// GPIO header pin 12
		#define PIN_ADDR  4				// GPIO header pin 16
		#define PIN_RESET 5				// GPIO header pin 18
	#endif

	// SPI setup for WiringPi.
	#define SPI_SPEED   8000000
	#define SPI_CHANNEL 0

	// Operator definitions.
	#define OPERATOR1 0
	#define OPERATOR2 1
	#define MODULATOR 0
	#define CARRIER   1

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
	#define NOTE_CS  1
	#define NOTE_D   2
	#define NOTE_DS  3
	#define NOTE_E   4
	#define NOTE_F   5
	#define NOTE_FS  6
	#define NOTE_G   7
	#define NOTE_GS  8
	#define NOTE_A   9
	#define NOTE_AS 10
	#define NOTE_B  11


	#if BOARD_TYPE == ARDUINO
		#include <Arduino.h>
	#else
		#include <stdint.h>
		typedef uint8_t byte;
		#define min(a, b) ((a) < (b) ? (a) : (b))
		#define max(a, b) ((a) > (b) ? (a) : (b))
		#define PROGMEM 
	#endif


	class OPL2 {
		public:
			OPL2();
			OPL2(byte reset, byte address, byte latch);
			void init();
			void reset();
			void write(byte, byte);

			byte getRegisterOffset(byte, byte);
			short getNoteFNumber(byte channel, byte octave, byte note);
			short getFrequencyFNumber(byte channel, float frequency);
			short getFNumberForBlock(float frequency, unsigned char block);
			float getNoteFrequency(byte octave, byte note);
			float getFrequencyStep(byte channel);
			byte getFrequencyBlock(float frequency);

			byte getRegister(byte);
			bool getWaveFormSelect();
			bool getTremolo(byte, byte);
			bool getVibrato(byte, byte);
			bool getMaintainSustain(byte, byte);
			bool getEnvelopeScaling(byte, byte);
			byte getMultiplier(byte, byte);
			byte getScalingLevel(byte, byte);
			byte getVolume(byte, byte);
			byte getAttack(byte, byte);
			byte getDecay(byte, byte);
			byte getSustain(byte, byte);
			byte getRelease(byte, byte);
			short getFNumber(byte);
			float getFrequency(byte channel);
			byte getBlock(byte);
			bool getKeyOn(byte);
			byte getFeedback(byte);
			bool getSynthMode(byte);
			bool getDeepTremolo();
			bool getDeepVibrato();
			bool getPercussion();
			byte getDrums();
			byte getWaveForm(byte, byte);

			void setInstrument(byte, const unsigned char*);
			void playNote(byte channel, byte octave, byte note);
			byte setRegister(byte, byte);
			byte setWaveFormSelect(bool);
			byte setTremolo(byte, byte, bool);
			byte setVibrato(byte, byte, bool);
			byte setMaintainSustain(byte, byte, bool);
			byte setEnvelopeScaling(byte, byte, bool);
			byte setMultiplier(byte, byte, byte);
			byte setScalingLevel(byte, byte, byte);
			byte setVolume(byte, byte, byte);
			byte setAttack(byte, byte, byte);
			byte setDecay(byte, byte, byte);
			byte setSustain(byte, byte, byte);
			byte setRelease(byte, byte, byte);
			byte setFNumber(byte, short);
			byte setFrequency(byte channel, float frequency);
			byte setBlock(byte, byte);
			byte setKeyOn(byte, bool);
			byte setFeedback(byte, byte);
			byte setSynthMode(byte, bool);
			byte setDeepTremolo(bool);
			byte setDeepVibrato(bool);
			byte setPercussion(bool);
			byte setDrums(bool, bool, bool, bool, bool);
			byte setWaveForm(byte, byte, byte);

		private:
			byte pinReset   = PIN_RESET;
			byte pinAddress = PIN_ADDR;
			byte pinLatch   = PIN_LATCH;
		
			const float fIntervals[8] = {
				0.048, 0.095, 0.190, 0.379, 0.759, 1.517, 3.034, 6.069
			};
			const float notes[12] = {
				261.63, 277.18,		// C, C#
				293.66, 311.13,		// D, D#
				329.63,				// E
				349.23, 369.99,		// F, F#
				392.00, 415.30,		// G, G#
				440.00, 466.16,		// A, A#
				493.88				// B
			};
			const float blockFrequencies[8] = {
				 48.503,   97.006,  194.013,  388.026,
				776.053, 1552.107, 3104.215, 6208.431
			};
			const byte offset[2][9] = {  
				{0x00, 0x01, 0x02, 0x08, 0x09, 0x0A, 0x10, 0x11, 0x12} ,   /*  initializers for operator 0 */
				{0x03, 0x04, 0x05, 0x0B, 0x0C, 0x0D, 0x13, 0x14, 0x15} ,   /*  initializers for operator 1 */
			};
			const byte drumOffset[6] = {
				0x10, 0x13, 0x14, 0x12, 0x15, 0x11
			};
			const byte instrumentBaseRegs[11] = {
				0x20, 0x40, 0x60, 0x80, 0xE0, 0xC0
			};
			byte oplRegisters[256];
	};
#endif

