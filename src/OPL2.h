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

	#define OPL2_BOARD_TYPE_ARDUINO      0
	#define OPL2_BOARD_TYPE_RASPBERRY_PI 1

	// !!! IMPORTANT !!!
	// In order to correctly compile the library for your platform be sure to set the correct BOARD_TYPE below.
	#define BOARD_TYPE OPL2_BOARD_TYPE_ARDUINO

	#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
		#define PIN_LATCH 10
		#define PIN_ADDR   9
		#define PIN_RESET  8
	#else
		#define PIN_LATCH 1				// GPIO header pin 12
		#define PIN_ADDR  4				// GPIO header pin 16
		#define PIN_RESET 5				// GPIO header pin 18

		// SPI setup for WiringPi.
		#define SPI_SPEED   8000000
		#define SPI_CHANNEL 0
	#endif

	// General OPL2 definitions.
	#define OPL2_NUM_CHANNELS 9

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


	#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
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
			void write(byte reg, byte data);
			byte getRegisterOffset(byte channel, byte operatorNum);

			byte getFrequencyBlock(float frequency);
			short getFrequencyFNumber(byte channel, float frequency);
			short getNoteFNumber(byte note);
			float getFrequencyStep(byte channel);

			byte getRegister(byte reg);
			bool getWaveFormSelect();
			bool getTremolo(byte channel, byte operatorNum);
			bool getVibrato(byte channel, byte operatorNum);
			bool getMaintainSustain(byte channel, byte operatorNum);
			bool getEnvelopeScaling(byte channel, byte operatorNum);
			byte getMultiplier(byte channel, byte operatorNum);
			byte getScalingLevel(byte channel, byte operatorNum);
			byte getVolume(byte channel, byte operatorNum);
			byte getAttack(byte channel, byte operatorNum);
			byte getDecay(byte channel, byte operatorNum);
			byte getSustain(byte channel, byte operatorNum);
			byte getRelease(byte channel, byte operatorNum);
			short getFNumber(byte channel);
			float getFrequency(byte channel);
			byte getBlock(byte channel);
			bool getKeyOn(byte channel);
			byte getFeedback(byte channel);
			bool getSynthMode(byte channel);
			bool getDeepTremolo();
			bool getDeepVibrato();
			bool getPercussion();
			byte getDrums();
			byte getWaveForm(byte channel, byte operatorNum);

			void setInstrument(byte channel, const unsigned char *instrument);
			void playNote(byte channel, byte octave, byte note);
			void playDrum(byte drum, byte octave, byte note);
			byte setRegister(byte reg, byte value);
			byte setWaveFormSelect(bool enable);
			byte setTremolo(byte channel, byte operatorNum, bool enable);
			byte setVibrato(byte channel, byte operatorNum, bool enable);
			byte setMaintainSustain(byte channel, byte operatorNum, bool enable);
			byte setEnvelopeScaling(byte channel, byte operatorNum, bool enable);
			byte setMultiplier(byte channel, byte operatorNum, byte multiplier);
			byte setScalingLevel(byte channel, byte operatorNum, byte scaling);
			byte setVolume(byte channel, byte operatorNum, byte volume);
			byte setAttack(byte channel, byte operatorNum, byte attack);
			byte setDecay(byte channel, byte operatorNum, byte decay);
			byte setSustain(byte channel, byte operatorNum, byte sustain);
			byte setRelease(byte channel, byte operatorNum, byte release);
			byte setFNumber(byte channel, short fNumber);
			byte setFrequency(byte channel, float frequency);
			byte setBlock(byte channel, byte block);
			byte setKeyOn(byte channel, bool keyOn);
			byte setFeedback(byte channel, byte feedback);
			byte setSynthMode(byte channel, bool isAdditive);
			byte setDeepTremolo(bool enable);
			byte setDeepVibrato(bool enable);
			byte setPercussion(bool enable);
			byte setDrums(bool bass, bool snare, bool tom, bool cymbal, bool hihat);
			byte setWaveForm(byte channel, byte operatorNum, byte waveForm);

		private:
			byte pinReset   = PIN_RESET;
			byte pinAddress = PIN_ADDR;
			byte pinLatch   = PIN_LATCH;

			const float fIntervals[8] = {
				0.048, 0.095, 0.190, 0.379, 0.759, 1.517, 3.034, 6.069
			};
			const unsigned int noteFNumbers[12] = {
				0x16B, 0x181, 0x198, 0x1B0, 0x1CA, 0x1E5,
				0x202, 0x220, 0x241, 0x263, 0x287, 0x2AE
			};
			const float blockFrequencies[8] = {
				 48.503,   97.006,  194.013,  388.026,
				776.053, 1552.107, 3104.215, 6208.431
			};
			const byte registerOffsets[2][9] = {  
				{0x00, 0x01, 0x02, 0x08, 0x09, 0x0A, 0x10, 0x11, 0x12} ,   /*  initializers for operator 1 */
				{0x03, 0x04, 0x05, 0x0B, 0x0C, 0x0D, 0x13, 0x14, 0x15} ,   /*  initializers for operator 2 */
			};
			const byte drumOffsets[6] = {
				0x10, 0x13, 0x14, 0x12, 0x15, 0x11
			};
			const byte drumBits[5] = {
				0x10, 0x08, 0x04, 0x02, 0x01
			};
			const byte instrumentBaseRegs[6] = {
				0x20, 0x40, 0x60, 0x80, 0xE0, 0xC0
			};
			byte oplRegisters[256];
	};
#endif

