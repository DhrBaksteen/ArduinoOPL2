/**
 * ________ __________.____    ________      _____            .___.__         .____    ._____.    
 * \_____  \\______   \    |   \_____  \    /  _  \  __ __  __| _/|__| ____   |    |   |__\_ |__  
 *  /   |   \|     ___/    |    /  ____/   /  /_\  \|  |  \/ __ | |  |/  _ \  |    |   |  || __ \ 
 * /    |    \    |   |    |___/       \  /    |    \  |  / /_/ | |  (  <_> ) |    |___|  || \_\ \
 * \_______  /____|   |_______ \_______ \ \____|__  /____/\____ | |__|\____/  |_______ \__||___  /
 *         \/                 \/       \/ _____   \/           \/                     \/       \/ 
 *                                      _/ ____\___________                                       
 *                                      \   __\/  _ \_  __ \                                      
 *                                       |  | (  <_> )  | \/                                      
 *                                       |__|  \____/|__|                                         
 *               _____            .___    .__                  ____    __________.__              
 *              /  _  \_______  __| _/_ __|__| ____   ____    /  _ \   \______   \__|             
 *             /  /_\  \_  __ \/ __ |  |  \  |/    \ /  _ \   >  _ </\  |     ___/  |             
 *            /    |    \  | \/ /_/ |  |  /  |   |  (  <_> ) /  <_\ \/  |    |   |  |             
 *            \____|__  /__|  \____ |____/|__|___|  /\____/  \_____\ \  |____|   |__|             
 *                    \/           \/             \/                \/                            
 *
 * YM3812 OPL2 Audio Library for Arduino, Raspberry Pi and Orange Pi v1.4.2
 * Code by Maarten Janssen (maarten@cheerful.nl) 2016-12-18
 *
 * Look for example code on how to use this library in the examples folder.
 *
 * Connect the OPL2 Audio Board as follows:
 *               |         | Raspberry Pi 
 *    OPL2 Board | Arduino |   Orange Pi
 *   ------------+---------+--------------
 *      Reset    |    8    |      18
 *      A0       |    9    |      16
 *      Latch    |   10    |      12
 *      Data     |   11*   |      19
 *      Shift    |   13*   |      23
 *
 * Arduino pins 11 and 13 are based on Arduino Uno. For Arduino Mega use pins 51 and 52 respectively, or refer to
 * https://www.arduino.cc/en/Reference/SPI for your Arduino board.
 *
 * IMPORTANT: Make sure you set the correct BOARD_TYPE in OPL2.h. Default is set to Arduino.
 *
 *
 * Last updated 2018-07-02
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 * Details about the YM3812 and OPL chips can be found at http://www.shikadi.net/moddingwiki/OPL_chip
 *
 * This library is open source and provided as is under the MIT software license, a copy of which is provided as part of
 * the project's repository. This library makes use of Gordon Henderson's Wiring Pi.
 */


#include "OPL2.h"

#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
	#include <SPI.h>
	#include <Arduino.h>
#else
	#include <wiringPi.h>
	#include <wiringPiSPI.h>
#endif


/**
 * Instantiate the OPL2 library with default pin setup.
 */
OPL2::OPL2() {
}


/**
 * Instantiate the OPL2 library with custom pin setup.
 */
OPL2::OPL2(byte reset, byte address, byte latch) {
	pinReset   = reset;
	pinAddress = address;
	pinLatch   = latch;	
}


/**
 * Initialize the YM3812.
 */
void OPL2::init() {
	#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
		SPI.begin();
	#else
		wiringPiSetup();
		wiringPiSPISetup(SPI_CHANNEL, SPI_SPEED);
	#endif

	pinMode(pinLatch,   OUTPUT);
	pinMode(pinAddress, OUTPUT);
	pinMode(pinReset,   OUTPUT);

	digitalWrite(pinLatch,   HIGH);
	digitalWrite(pinReset,   HIGH);
	digitalWrite(pinAddress, LOW);

	reset();
}


/**
 * Hard reset the OPL2 chip. This should be done before sending any register data to the chip.
 */
void OPL2::reset() {
	digitalWrite(pinReset, LOW);
	delay(1);
	digitalWrite(pinReset, HIGH);

	for(int i = 0; i < 256; i ++) {
		oplRegisters[i] = 0x00;
	}
}


/**
 * Send the given byte of data to the given register of the OPL2 chip.
 */
void OPL2::write(byte reg, byte data) {
	digitalWrite(pinAddress, LOW);
	#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
		SPI.transfer(reg);
	#else
		wiringPiSPIDataRW(SPI_CHANNEL, &reg, 1);
	#endif
	digitalWrite(pinLatch, LOW);
	delayMicroseconds(1);
	digitalWrite(pinLatch, HIGH);
	delayMicroseconds(4);

	digitalWrite(pinAddress, HIGH);
	#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
		SPI.transfer(data);
	#else
		wiringPiSPIDataRW(SPI_CHANNEL, &data, 1);
	#endif
	digitalWrite(pinLatch, LOW);
	delayMicroseconds(1);
	digitalWrite(pinLatch, HIGH);
	delayMicroseconds(23);
}


/**
 * Get the current value of the given register.
 */
byte OPL2::getRegister(byte reg) {
	return oplRegisters[reg];
}


/**
 * Sets the given register to the given value.
 */
byte OPL2::setRegister(byte reg, byte value) {
	oplRegisters[reg] = value;
	write(reg, value);
	return reg;
}


/**
 * Calculate register offet based on channel and operator.
 */
byte OPL2::getRegisterOffset(byte channel, byte operatorNum) {
	channel = max(0, min(channel, 8));
	operatorNum = max(0, min(operatorNum, 1));
	return registerOffsets[operatorNum][channel];
}


/**
 * Get the F-number for the given frequency for a given channel. When the F-number is calculated the current frequency
 * block of the channel is taken into account.
 */
short OPL2::getFrequencyFNumber(byte channel, float frequency) {
	float fInterval = getFrequencyStep(channel);
	return (short)max(0, min(frequency / fInterval, 1023));
}


/**
 * Get the F-Number for the given note. In this case the block is assumed to be the octave.
 */
short OPL2::getNoteFNumber(byte note) {
	return noteFNumbers[max(0, min(note, 11))];
}

/**
 * Get the frequency step per F-number for the current block on the given channel.
 */
float OPL2::getFrequencyStep(byte channel) {
	return fIntervals[getBlock(channel)];
}


/**
 * Get the optimal frequency block for the given frequency.
 */
byte OPL2::getFrequencyBlock(float frequency) {
	for (byte i = 0; i < 8; i ++) {
		if (frequency < blockFrequencies[i]) {
			return i;
		}
	}
	return 7;
}


/**
 * Load an instrument and apply it to the given channel. If the instrument to be loaded is a percussive instrument then
 * the channel will depend on the type of drum and the channel parameter will be ignored.
 * See instruments.h for instrument definition format.
 */
void OPL2::setInstrument(byte channel, const unsigned char *instrument) {
	#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
		unsigned char percussionChannel = pgm_read_byte_near(instrument);
	#else
		unsigned char percussionChannel = instrument[0];
	#endif

	setWaveFormSelect(true);
	switch (percussionChannel) {
		case 6:		// Base drum...
			for (byte i = 0; i < 5; i ++) {
				setRegister(
					instrumentBaseRegs[i] + drumOffsets[0],
					#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
						pgm_read_byte_near(instrument + i + 1)
					#else
						instrument[i + 1]
					#endif
				);
				setRegister(
					instrumentBaseRegs[i] + drumOffsets[1],
					#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
						pgm_read_byte_near(instrument + i + 1)
					#else
						instrument[i + 1]
					#endif
				);
			}
			break;

		case 7:		// Snare drum...
		case 8:		// Tom tom...
		case 9:		// Top cymbal...
		case 10:	// Hi hat...
			for (byte i = 0; i < 5; i ++) {
				setRegister(
					instrumentBaseRegs[i] + drumOffsets[percussionChannel - 5],
					#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
						pgm_read_byte_near(instrument + i + 1)
					#else
						instrument[i + 1]
					#endif
				);
			}
			break;

		default:	// Melodic instruments...
			for (byte i = 0; i < 11; i ++) {
				setRegister(
					instrumentBaseRegs[i % 6] + getRegisterOffset(channel, i > 5),
					#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
						pgm_read_byte_near(instrument + i + 1)
					#else
						instrument[i + 1]
					#endif
				);
			}
			break;
	}
}


/**
 * Play a note of a certain octave on the given channel.
 */
void OPL2::playNote(byte channel, byte octave, byte note) {
	setKeyOn(channel, false);
	setBlock(channel, max(0, min(octave, 7)));
	setFNumber(channel, noteFNumbers[max(0, min(note, 11))]);
	setKeyOn(channel, true);
}


/**
 * Is wave form selection currently enabled.
 */
bool OPL2::getWaveFormSelect() {
	return oplRegisters[0x01] & 0x20;
}


/**
 * Enable wave form selection for each operator.
 */
byte OPL2::setWaveFormSelect(bool enable) {
	if (enable) {
		return setRegister(0x01, oplRegisters[0x01] | 0x20);
	} else {
		return setRegister(0x01, oplRegisters[0x01] & 0xDF);
	}
}


/**
 * Is amplitude modulation enabled for the given operator?
 */
bool OPL2::getTremolo(byte channel, byte operatorNum) {
	return oplRegisters[0x20 + getRegisterOffset(channel, operatorNum)] & 0x80;
}


/**
 * Apply amplitude modulation when set to true. Modulation depth is controlled globaly by the AM-depth flag in the 0xBD
 * register.
 */
byte OPL2::setTremolo(byte channel, byte operatorNum, bool enable) {
	byte reg = 0x20 + getRegisterOffset(channel, operatorNum);
	if (enable) {
		return setRegister(reg, oplRegisters[reg] | 0x80);
	} else {
		return setRegister(reg, oplRegisters[reg] & 0x7F);
	}
}


/**
 * Is vibrator enabled for the given channel?
 */
bool OPL2::getVibrato(byte channel, byte operatorNum) {
	return oplRegisters[0x20 + getRegisterOffset(channel, operatorNum)] & 0x40;
}


/**
 * Apply vibrato when set to true. Vibrato depth is controlled globally by the VIB-depth flag in the 0xBD register.
 */
byte OPL2::setVibrato(byte channel, byte operatorNum, bool enable) {
	byte reg = 0x20 + getRegisterOffset(channel, operatorNum);
	if (enable) {
		return setRegister(reg, oplRegisters[reg] | 0x40);
	} else {
		return setRegister(reg, oplRegisters[reg] & 0xBF);
	}
}


/**
 * Is sustain being maintained for the given channel?
 */
bool OPL2::getMaintainSustain(byte channel, byte operatorNum) {
	return oplRegisters[0x20 + getRegisterOffset(channel, operatorNum)] & 0x20;
}


/**
 * When set to true the sustain level of the voice is maintained until released. When false the sound begins to decay
 * immediately after hitting the sustain phase.
 */
byte OPL2::setMaintainSustain(byte channel, byte operatorNum, bool enable) {
	byte reg = 0x20 + getRegisterOffset(channel, operatorNum);
	if (enable) {
		return setRegister(reg, oplRegisters[reg] | 0x20);
	} else {
		return setRegister(reg, oplRegisters[reg] & 0xDF);
	}
}


/**
 * Is envelope scaling being applied to the given channel?
 */
bool OPL2::getEnvelopeScaling(byte channel, byte operatorNum) {
	return oplRegisters[0x20 + getRegisterOffset(channel, operatorNum)] & 0x10;
}


/**
 * Enable or disable envelope scaling. When set to true higher notes will be shorter than lower ones.
 */
byte OPL2::setEnvelopeScaling(byte channel, byte operatorNum, bool enable) {
	byte reg = 0x20 + getRegisterOffset(channel, operatorNum);
	if (enable) {
		return setRegister(reg, oplRegisters[reg] | 0x10);
	} else {
		return setRegister(reg, oplRegisters[reg] & 0xEF);
	}
	return reg;
}


/**
 * Get the frequency multiplier for the given channel.
 */
byte OPL2::getMultiplier(byte channel, byte operatorNum) {
	return oplRegisters[0x20 + getRegisterOffset(channel, operatorNum)] & 0x0F;
}


/**
 * Set frequency multiplier for the given channel. Note that a multiplier of 0 will apply a 0.5 multiplication.
 */
byte OPL2::setMultiplier(byte channel, byte operatorNum, byte multiplier) {
	byte reg = 0x20 + getRegisterOffset(channel, operatorNum);
	return setRegister(reg, (oplRegisters[reg] & 0xF0) | (multiplier & 0x0F));
}


/**
 * Get the scaling level for the given channel.
 */
byte OPL2::getScalingLevel(byte channel, byte operatorNum) {
	return (oplRegisters[0x40 + getRegisterOffset(channel, operatorNum)] & 0xC0) >> 6;
}


/**
 * Decrease output levels as frequency increases.
 * 00 - No change
 * 01 - 1.5 dB/oct
 * 10 - 3.0 dB/oct
 * 11 - 6.0 dB/oct
 */
byte OPL2::setScalingLevel(byte channel, byte operatorNum, byte scaling) {
	byte reg = 0x40 + getRegisterOffset(channel, operatorNum);
	return setRegister(reg, (oplRegisters[reg] & 0x3F) | ((scaling & 0x03) << 6));
}


/**
 * Get the volume of the given channel. 0x00 is laudest, 0x3F is softest.
 */
byte OPL2::getVolume(byte channel, byte operatorNum) {
	return oplRegisters[0x40 + getRegisterOffset(channel, operatorNum)] & 0x3F;
}


/**
 * Set the volume of the channel.
 * Note that the scale is inverted! 0x00 for loudest, 0x3F for softest.
 */
byte OPL2::setVolume(byte channel, byte operatorNum, byte volume) {
	byte reg = 0x40 + getRegisterOffset(channel, operatorNum);
	return setRegister(reg, (oplRegisters[reg] & 0xC0) | (volume & 0x3F));
}


/**
 * Get the attack rate of the given channel.
 */
byte OPL2::getAttack(byte channel, byte operatorNum) {
	return (oplRegisters[0x60 + getRegisterOffset(channel, operatorNum)] & 0xF0) >> 4;
}


/**
 * Attack rate. 0x00 is slowest, 0x0F is fastest.
 */
byte OPL2::setAttack(byte channel, byte operatorNum, byte attack) {
	byte reg = 0x60 + getRegisterOffset(channel, operatorNum);
	return setRegister(reg, (oplRegisters[reg] & 0x0F) | ((attack & 0x0F) << 4));
}


/**
 * Get the decay rate of the given channel.
 */
byte OPL2::getDecay(byte channel, byte operatorNum) {
	return oplRegisters[0x60 + getRegisterOffset(channel, operatorNum)] & 0x0F;
}


/**
 * Decay rate. 0x00 is slowest, 0x0F is fastest.
 */
byte OPL2::setDecay(byte channel, byte operatorNum, byte decay) {
	byte reg = 0x60 + getRegisterOffset(channel, operatorNum);
	return setRegister(reg, (oplRegisters[reg] & 0xF0) | (decay & 0x0F));
}


/**
 * Get the sustain level of the given channel. 0x00 is laudest, 0x0F is softest.
 */
byte OPL2::getSustain(byte channel, byte operatorNum) {
	return (oplRegisters[0x80 + getRegisterOffset(channel, operatorNum)] & 0xF0) >> 4;
}


/**
 * Sustain level. 0x00 is laudest, 0x0F is softest.
 */
byte OPL2::setSustain(byte channel, byte operatorNum, byte sustain) {
	byte reg = 0x80 + getRegisterOffset(channel, operatorNum);
	return setRegister(reg, (oplRegisters[reg] & 0x0F) | ((sustain & 0x0F) << 4));
}


/**
 * Get the release rate of the given channel.
 */
byte OPL2::getRelease(byte channel, byte operatorNum) {
	return oplRegisters[0x80 + getRegisterOffset(channel, operatorNum)] & 0x0F;
}


/**
 * Release rate. 0x00 is flowest, 0x0F is fastest.
 */
byte OPL2::setRelease(byte channel, byte operatorNum, byte release) {
	byte reg = 0x80 + getRegisterOffset(channel, operatorNum);
	return setRegister(reg, (oplRegisters[reg] & 0xF0) | (release & 0x0F));
}


/**
 * Get the frequenct F-number of the given channel.
 */
short OPL2::getFNumber(byte channel) {
	byte offset = max(0x00, min(channel, 0x08));
	return ((oplRegisters[0xB0 + offset] & 0x03) << 8) + oplRegisters[0xA0 + offset];
}


/**
 * Set frequency F-number [0, 1023] for the given channel. 
 */
byte OPL2::setFNumber(byte channel, short fNumber) {
	byte reg = 0xA0 + max(0x00, min(channel, 0x08));
	setRegister(reg, fNumber & 0x00FF);
	setRegister(reg + 0x10, (oplRegisters[reg + 0x10] & 0xFC) | ((fNumber & 0x0300) >> 8));
	return reg;
}


/**
 * Get the frequency for the given channel.
 */
float OPL2::getFrequency(byte channel) {
	float fInterval = getFrequencyStep(channel);
	return getFNumber(channel) * fInterval;
}


/**
 * Set the frequenct of the given channel and if needed switch to a different block.
 */
byte OPL2::setFrequency(byte channel, float frequency) {
	unsigned char block = getFrequencyBlock(frequency);
	if (getBlock(channel) != block) {
		setBlock(channel, block);
	}
	short fNumber = getFrequencyFNumber(channel, frequency);
	return setFNumber(channel, fNumber);
}


/**
 * Get the frequency block of the given channel.
 */
byte OPL2::getBlock(byte channel) {
	byte offset = max(0x00, min(channel, 0x08));
	return (oplRegisters[0xB0 + offset] & 0x1C) >> 2;
}


/**
 * Set frequency block. 0x00 is lowest, 0x07 is highest. This determines the frequency interval between notes.
 * 0 - 0.048 Hz, Range: 0.047 Hz ->   48.503 Hz
 * 1 - 0.095 Hz, Range: 0.094 Hz ->   97.006 Hz
 * 2 - 0.190 Hz, Range: 0.189 Hz ->  194.013 Hz
 * 3 - 0.379 Hz, Range: 0.379 Hz ->  388.026 Hz
 * 4 - 0.759 Hz, Range: 0.758 Hz ->  776.053 Hz
 * 5 - 1.517 Hz, Range: 1.517 Hz -> 1552.107 Hz
 * 6 - 3.034 Hz, Range: 3.034 Hz -> 3104.215 Hz
 * 7 - 6.069 Hz, Range: 6.068 Hz -> 6208.431 Hz
 */
byte OPL2::setBlock(byte channel, byte block) {
	byte reg = 0xB0 + max(0x00, min(channel, 0x08));
	return setRegister(reg, (oplRegisters[reg] & 0xE3) | ((block & 0x07) << 2));
}


/**
 * Is the voice of the given channel currently enabled?
 */
bool OPL2::getKeyOn(byte channel) {
	byte offset = max(0x00, min(channel, 0x08));
	return oplRegisters[0xB0 + offset] & 0x20;
}


/**
 * Enable voice on channel.
 */
byte OPL2::setKeyOn(byte channel, bool keyOn) {
	byte reg = 0xB0 + max(0x00, min(channel, 0x08));
	if (keyOn) {
		return setRegister(reg, oplRegisters[reg] | 0x20);
	} else {
		return setRegister(reg, oplRegisters[reg] & 0xDF);
	}
}


/**
 * Get the feedback strength of the given channel.
 */
byte OPL2::getFeedback(byte channel) {
	byte offset = max(0x00, min(channel, 0x08));
	return (oplRegisters[0xC0 + offset] & 0xE0) >> 1;
}


/**
 * Set feedback strength. 0x00 is no feedback, 0x07 is strongest.
 */
byte OPL2::setFeedback(byte channel, byte feedback) {
	byte reg = 0xC0 + max(0x00, min(channel, 0x08));
	return setRegister(reg, (oplRegisters[reg] & 0x01) | ((feedback & 0x07) << 1));
}


/**
 * Is the decay algorythm enabled for the given channel?
 */
bool OPL2::getSynthMode(byte channel) {
	byte offset = max(0x00, min(channel, 0x08));
	return oplRegisters[0xC0 + offset] & 0x01;
}


/**
 * Set decay algorithm. When false operator 1 modulates operator 2 (operator 2 is the only one to produce sounde). If
 * set to true both operator 1 and operator 2 will produce sound.
 */
byte OPL2::setSynthMode(byte channel, bool isAdditive) {
	byte reg = 0xC0 + max(0x00, min(channel, 0x08));
	if (isAdditive) {
		return setRegister(reg, oplRegisters[reg] | 0x01);
	} else {
		return setRegister(reg, oplRegisters[reg] & 0xFE);
	}
}


/**
 * Is deeper amplitude modulation enabled?
 */
bool OPL2::getDeepTremolo() {
	return oplRegisters[0xBD] & 0x80;
}


/**
 * Set deeper aplitude modulation depth. When false modulation depth is 1.0 dB, when true modulation depth is 4.8 dB.
 */
byte OPL2::setDeepTremolo(bool enable) {
	if (enable) {
		return setRegister(0xBD, oplRegisters[0xBD] | 0x80);
	} else {
		return setRegister(0xBD, oplRegisters[0xBD] & 0x7F);
	}
}


/**
 * Is deeper vibrato depth enabled?
 */
bool OPL2::getDeepVibrato() {
	return oplRegisters[0xBD] & 0x40;
}


/**
 * Set deeper vibrato depth. When false vibrato depth is 7/100 semi tone, when true vibrato depth is 14/100.
 */
byte OPL2::setDeepVibrato(bool enable) {
	if (enable) {
		return setRegister(0xBD, oplRegisters[0xBD] | 0x40);
	} else {
		return setRegister(0xBD, oplRegisters[0xBD] & 0xBF);
	}
}


/**
 * Is percussion mode currently enabled?
 */
bool OPL2::getPercussion() {
	return oplRegisters[0xBD] & 0x20;
}


/**
 * Enable or disable percussion mode. When set to false there are 9 melodic voices, when true there are 6 melodic
 * voices and channels 6 through 8 are used for drum sounds. KeyOn for these channels must be off.
 */
byte OPL2::setPercussion(bool enable) {
	if (enable) {
		return setRegister(0xBD, oplRegisters[0xBD] | 0x20);
	} else {
		return setRegister(0xBD, oplRegisters[0xBD] & 0xDF);
	}
}


/**
 * Return which drum sounds are enabled.
 */
byte OPL2::getDrums() {
	return oplRegisters[0xBD] & 0x1F;
}


/**
 * Enable or disable various drum sounds.
 * Note that keyOn for channels 6, 7 and 8 must be false in order to use rhythms.
 */
byte OPL2::setDrums(bool bass, bool snare, bool tom, bool cymbal, bool hihat) {
	byte drums = 0;
	drums += bass   ? DRUM_BASS   : 0x00;
	drums += snare  ? DRUM_SNARE  : 0x00;
	drums += tom    ? DRUM_TOM    : 0x00;
	drums += cymbal ? DRUM_CYMBAL : 0x00;
	drums += hihat  ? DRUM_HI_HAT : 0x00;
	setRegister(0xBD, oplRegisters[0xBD] & ~drums);
	return setRegister(0xBD, oplRegisters[0xBD] | drums);
}


/**
 * Get the wave form currently set for the given channel.
 */
byte OPL2::getWaveForm(byte channel, byte operatorNum) {
	return oplRegisters[0xE0 + getRegisterOffset(channel, operatorNum)] & 0x03;
}


/**
 * Select the wave form to use.
 */
byte OPL2::setWaveForm(byte channel, byte operatorNum, byte waveForm) {
	byte reg = 0xE0 + getRegisterOffset(channel, operatorNum);
	return setRegister(reg, (oplRegisters[reg] & 0xFC) | (waveForm & 0x03));
}
