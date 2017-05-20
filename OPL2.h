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
	#define BOARD_TYPE ARDUINO

	#if BOARD_TYPE == ARDUINO
		#define PIN_LATCH 10
		#define PIN_A0     9
		#define PIN_RESET  8
	#elif BOARD_TYPE == RASPBERRY_PI or BOARD_TYPE == ORANGE_PI
		#define PIN_LATCH 1				// GPIO header pin 12
		#define PIN_A0    4				// GPIO header pin 16
		#define PIN_RESET 5				// GPIO header pin 18
	#endif

	// SPI setup for WiringPi.
	#define SPI_SPEED   8000000
	#define SPI_CHANNEL 0

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
			void init();
			void reset();
			void write(byte, byte);

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

			void setInstrument(byte, const unsigned char*);
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
			const float notes[12] = {
				261.63, 277.18,		// C, C#
				293.66, 311.13,		// D, D#
				329.63,				// E
				349.23, 369.99,		// F, F#
				392.00, 415.30,		// G, G#
				440.00, 466.16,		// A, A#
				493.88				// B
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
			byte getRegisterOffset(byte, bool);
	};
#endif

