/**
 * ________ __________.____    ________      _____            .___.__         .____    ._____.    
 * \_____  \\______   \    |   \_____  \    /  _  \  __ __  __| _/|__| ____   |    |   |__\_ |__  
 *  /   |   \|     ___/    |    /  ____/   /  /_\  \|  |  \/ __ | |  |/  _ \  |    |   |  || __ \ 
 * /    |    \    |   |    |___/       \  /    |    \  |  / /_/ | |  (  <_> ) |    |___|  || \_\ \
 * \_______  /____|   |_______ \_______ \ \____|__  /____/\____ | |__|\____/  |_______ \__||___  /
 *         \/                 \/       \/         \/           \/                     \/       \/ 
 *                 _____                 _____            .___    .__                             
 *               _/ ____\___________    /  _  \_______  __| _/_ __|__| ____   ____                
 *               \   __\/  _ \_  __ \  /  /_\  \_  __ \/ __ |  |  \  |/    \ /  _ \               
 *                |  | (  <_> )  | \/ /    |    \  | \/ /_/ |  |  /  |   |  (  <_> )              
 *                |__|  \____/|__|    \____|__  /__|  \____ |____/|__|___|  /\____/               
 *                                            \/           \/             \/                      
 * YM3812 OPL2 Audio Library for Arduino v1.0.1
 * Code by Maarten Janssen (maarten@cheerful.nl) 2016-12-18
 *
 * Look for example sketches on how to use this library in the examples folder.
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 * Details about the YM3812 and OPL chips can be found at http://www.shikadi.net/moddingwiki/OPL_chip
 *
 * OPL2 board is connedted as follows:
 * Pin  8 - Reset
 * Pin  9 - A0
 * Pin 10 - Latch
 * Pin 11 - Data
 * Pin 13 - Shift
 *
 * This library is open source and provided as is under the MIT software license, a copy of which is provided as part of
 * the project's repository.
 * Last updated 2017-03-08
 */


#include <SPI.h>
#include <Arduino.h>
#include "OPL2.h"


OPL2::OPL2() {
}


/**
 * Initialize the YM3812.
 */
void OPL2::init() {
	pinMode(PIN_DATA,  OUTPUT);
	pinMode(PIN_CLOCK, OUTPUT);
	pinMode(PIN_LATCH, OUTPUT);
	pinMode(PIN_A0,    OUTPUT);
	pinMode(PIN_RESET, OUTPUT);

	digitalWrite(PIN_LATCH, HIGH);
	digitalWrite(PIN_RESET, HIGH);
	digitalWrite(PIN_A0,    LOW);
	SPI.begin();

	reset();
}


/**
 * Send the given byte of data to the given register of the OPL2 chip.
 */
void OPL2::write(byte reg, byte data) {
	digitalWrite(PIN_A0, LOW);
	SPI.transfer(reg);
	digitalWrite(PIN_LATCH, LOW);
	digitalWrite(PIN_LATCH, HIGH);
	delayMicroseconds(4);

	digitalWrite(PIN_A0, HIGH);
	SPI.transfer(data);
	digitalWrite(PIN_LATCH, LOW);
	digitalWrite(PIN_LATCH, HIGH);
	delayMicroseconds(23);
}


/**
 * Calculate register offet based on channel and operator.
 */
byte OPL2::getRegisterOffset(byte ch, bool op2) {
	ch = max(0, min(ch, 8));
	return offset[op2][ch];
}


/**
 * Send the data of the given register to the OPL2 chip.
 */
void OPL2::updateRegister(byte reg) {
	write(reg, oplRegisters[reg]);
}


/**
 * Hard reset the OPL2 chip. This should be done before sending any register data to the chip.
 */
void OPL2::reset() {
	digitalWrite(PIN_RESET, LOW);
	delay(1);
	digitalWrite(PIN_RESET, HIGH);

	for(int i = 0; i < 256; i ++) {
		oplRegisters[i] = 0x00;
	}
}


/**
 * Get the F-number for the given note for a given channel. Note the the channel must have an appropriate block set
 * before calling this function in order to get a useful F-number!
 */
short OPL2::getNoteFrequency(byte channel, byte octave, byte note) {
	octave = max(0, min(octave, 7));
	float fInterval = fIntervals[getBlock(channel)];
	float freq = notes[max(0, min(note, 11))];

	if (octave < 4) {
		for (int i = 0; i < 4 - octave; i ++) {
			freq /= 2;
		}
	} else if (octave > 4) {
		for (int i = 0; i < octave - 4; i ++) {
			freq *= 2;
		}
	}

	return (short)max(0, min(freq / fInterval, 1023));
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
	return reg;
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
		oplRegisters[0x01] |= 0x20;
	} else {
		oplRegisters[0x01] &= 0xDF;
	}
	return 0x01;
}


/**
 * Is amplitude modulation enabled for the given operator?
 */
bool OPL2::getTremolo(byte ch, bool op) {
	return oplRegisters[0x20 + getRegisterOffset(ch, op)] & 0x80;
}


/**
 * Apply amplitude modulation when set to true. Modulation depth is controlled globaly by the AM-depth flag in the 0xBD
 * register.
 */
byte OPL2::setTremolo(byte ch, bool op, bool enable) {
	byte reg = 0x20 + getRegisterOffset(ch, op);
	if (enable) {
		oplRegisters[reg] |= 0x80;
	} else {
		oplRegisters[reg] &= 0x7F;
	}
	return reg;
}


/**
 * Is vibrator enabled for the given channel?
 */
bool OPL2::getVibrato(byte ch, bool op) {
	return oplRegisters[0x20 + getRegisterOffset(ch, op)] & 0x40;
}


/**
 * Apply vibrato when set to true. Vibrato depth is controlled globally by the VIB-depth flag in the 0xBD register.
 */
byte OPL2::setVibrato(byte ch, bool op, bool enable) {
	byte reg = 0x20 + getRegisterOffset(ch, op);
	if (enable) {
		oplRegisters[reg] |= 0x40;
	} else {
		oplRegisters[reg] &= 0xBF;
	}
	return reg;
}


/**
 * Is sustain being maintained for the given channel?
 */
bool OPL2::getMaintainSustain(byte ch, bool op) {
	return oplRegisters[0x20 + getRegisterOffset(ch, op)] & 0x20;
}


/**
 * When set to true the sustain level of the voice is maintained until released. When false the sound begins to decay
 * immediately after hitting the sustain phase.
 */
byte OPL2::setMaintainSustain(byte ch, bool op, bool enable) {
	byte reg = 0x20 + getRegisterOffset(ch, op);
	if (enable) {
		oplRegisters[reg] |= 0x20;
	} else {
		oplRegisters[reg] &= 0xDF;
	}
	return reg;
}


/**
 * Is envelope scaling being applied to the given channel?
 */
bool OPL2::getEnvelopeScaling(byte ch, bool op) {
	return oplRegisters[0x20 + getRegisterOffset(ch, op)] & 0x10;
}


/**
 * Enable or disable envelope scaling. When set to true higher notes will be shorter than lower ones.
 */
byte OPL2::setEnvelopeScaling(byte ch, bool op, bool enable) {
	byte reg = 0x20 + getRegisterOffset(ch, op);
	if (enable) {
		oplRegisters[reg] |= 0x10;
	} else {
		oplRegisters[reg] &= 0xEF;
	}
	return reg;
}


/**
 * Get the frequency multiplier for the given channel.
 */
byte OPL2::getMultiplier(byte ch, bool op) {
	return oplRegisters[0x20 + getRegisterOffset(ch, op)] & 0x0F;
}


/**
 * Set frequency multiplier for the given channel. Note that a multiplier of 0 will apply a 0.5 multiplication.
 */
byte OPL2::setMultiplier(byte ch, bool op, byte multiplier) {
	byte reg = 0x20 + getRegisterOffset(ch, op);
	oplRegisters[reg] &= 0xF0;
	oplRegisters[reg] |= multiplier & 0x0F;
	return reg;
}


/**
 * Get the scaling level for the given channel.
 */
byte OPL2::getScalingLevel(byte ch, bool op) {
	return (oplRegisters[0x40 + getRegisterOffset(ch, op)] & 0xC0) >> 6;
}


/**
 * Decrease output levels as frequency increases.
 * 00 - No change
 * 01 - 1.5 dB/oct
 * 10 - 3.0 dB/oct
 * 11 - 6.0 dB/oct
 */
byte OPL2::setScalingLevel(byte ch, bool op, byte scaling) {
	byte reg = 0x40 + getRegisterOffset(ch, op);
	oplRegisters[reg] &= 0x3F;
	oplRegisters[reg] |= (scaling & 0x03) << 6;
	return reg;
}


/**
 * Get the volume of the given channel. 0x00 is laudest, 0x3F is softest.
 */
byte OPL2::getVolume(byte ch, bool op) {
	return oplRegisters[0x40 + getRegisterOffset(ch, op)] & 0x3F;
}


/**
 * Set the volume of the channel.
 * Note that the scale is inverted! 0x00 for loudest, 0x3F for softest.
 */
byte OPL2::setVolume(byte ch, bool op, byte volume) {
	byte reg = 0x40 + getRegisterOffset(ch, op);
	oplRegisters[reg] &= 0xC0;
	oplRegisters[reg] |= volume & 0x3F;
	return reg;
}


/**
 * Get the attack rate of the given channel.
 */
byte OPL2::getAttack(byte ch, bool op) {
	return (oplRegisters[0x60 + getRegisterOffset(ch, op)] & 0xF0) >> 4;
}


/**
 * Attack rate. 0x00 is slowest, 0x0F is fastest.
 */
byte OPL2::setAttack(byte ch, bool op, byte attack) {
	byte reg = 0x60 + getRegisterOffset(ch, op);
	oplRegisters[reg] &= 0x0F;
	oplRegisters[reg] |= (attack & 0x0F) << 4;
	return reg;
}


/**
 * Get the decay rate of the given channel.
 */
byte OPL2::getDecay(byte ch, bool op) {
	return oplRegisters[0x60 + getRegisterOffset(ch, op)] & 0x0F;
}


/**
 * Decay rate. 0x00 is slowest, 0x0F is fastest.
 */
byte OPL2::setDecay(byte ch, bool op, byte decay) {
	byte reg = 0x60 + getRegisterOffset(ch, op);
	oplRegisters[reg] &= 0xF0;
	oplRegisters[reg] |= decay & 0x0F;
	return reg;
}


/**
 * Get the sustain level of the given channel. 0x00 is laudest, 0x0F is softest.
 */
byte OPL2::getSustain(byte ch, bool op) {
	return (oplRegisters[0x80 + getRegisterOffset(ch, op)] & 0xF0) >> 4;
}


/**
 * Sustain level. 0x00 is laudest, 0x0F is softest.
 */
byte OPL2::setSustain(byte ch, bool op, byte sustain) {
	byte reg = 0x80 + getRegisterOffset(ch, op);
	oplRegisters[reg] &= 0x0F;
	oplRegisters[reg] |= (sustain & 0x0F) << 4;
	return reg;
}


/**
 * Get the release rate of the given channel.
 */
byte OPL2::getRelease(byte ch, bool op) {
	return oplRegisters[0x80 + getRegisterOffset(ch, op)] & 0x0F;
}


/**
 * Release rate. 0x00 is flowest, 0x0F is fastest.
 */
byte OPL2::setRelease(byte ch, bool op, byte release) {
	byte reg = 0x80 + getRegisterOffset(ch, op);
	oplRegisters[reg] &= 0xF0;
	oplRegisters[reg] |= release & 0x0F;
	return reg;
}


/**
 * Get the frequenct F-number of the given channel.
 */
short OPL2::getFrequency(byte ch) {
	byte offset = max(0x00, min(ch, 0x08));
	return (oplRegisters[0xB0 + offset] & 0x03) << 8 + oplRegisters[0xA0 + offset];
}
	

/**
 * Set frequency F-number. 
 */
byte OPL2::setFrequency(byte ch, short frequency) {
	byte reg = 0xA0 + max(0x00, min(ch, 0x08));
	oplRegisters[reg] = frequency & 0x00FF;
	oplRegisters[reg + 0x10] &= 0xFC;
	oplRegisters[reg + 0x10] |= (frequency & 0x0300) >> 8;
	return reg;
}


/**
 * Get the frequency block of the given channel.
 */
byte OPL2::getBlock(byte ch) {
	byte offset = max(0x00, min(ch, 0x08));
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
byte OPL2::setBlock(byte ch, byte octave) {
	byte reg = 0xB0 + max(0x00, min(ch, 0x08));
	oplRegisters[reg] &= 0xE3;
	oplRegisters[reg] |= (octave & 0x07) << 2;
	return reg;
}


/**
 * Is the voice of the given channel currently enabled?
 */
bool OPL2::getKeyOn(byte ch) {
	byte offset = max(0x00, min(ch, 0x08));
	return oplRegisters[0xB0 + offset] & 0x20;
}


/**
 * Enable voice on channel.
 */
byte OPL2::setKeyOn(byte ch, bool keyOn) {
	byte reg = 0xB0 + max(0x00, min(ch, 0x08));
	if (keyOn) {
		oplRegisters[reg] |= 0x20;
	} else {
		oplRegisters[reg] &= 0xDF;
	}
	return reg;
}


/**
 * Get the feedback strength of the given channel.
 */
byte OPL2::getFeedback(byte ch) {
	byte offset = max(0x00, min(ch, 0x08));
	return (oplRegisters[0xC0 + offset] & 0xE0) >> 1;
}


/**
 * Set feedback strength. 0x00 is no feedback, 0x07 is strongest.
 */
byte OPL2::setFeedback(byte ch, byte feedback) {
	byte reg = 0xC0 + max(0x00, min(ch, 0x08));
	oplRegisters[reg] &= 0x01;
	oplRegisters[reg] |= (feedback & 0x07) << 1;
	return reg;
}


/**
 * Is the decay algorythm enabled for the given channel?
 */
bool OPL2::getSynthMode(byte ch) {
	byte offset = max(0x00, min(ch, 0x08));
	return oplRegisters[0xC0 + offset] & 0x01;
}


/**
 * Set decay algorithm. When false operator 1 modulates operator 2 (operator 2 is the only one to produce sounde). If
 * set to true both operator 1 and operator 2 will produce sound.
 */
byte OPL2::setSynthMode(byte ch, bool isAdditive) {
	byte reg = 0xC0 + max(0x00, min(ch, 0x08));
	if (isAdditive) {
		oplRegisters[reg] |= 0x01;
	} else {
		oplRegisters[reg] &= 0xFE;
	}
	return reg;
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
		oplRegisters[0xBD] |= 0x80;
	} else {
		oplRegisters[0xBD] &= 0x7F;
	}
	return 0xBD;
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
		oplRegisters[0xBD] |= 0x40;
	} else {
		oplRegisters[0xBD] &= 0xBF;
	}
	return 0xBD;
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
		oplRegisters[0xBD] |= 0x20;
	} else {
		oplRegisters[0xBD] &= 0xDF;
	}
	return 0xBD;
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
	drums |= bass   ? DRUM_BASS   : 0x00;
	drums |= snare  ? DRUM_SNARE  : 0x00;
	drums |= tom    ? DRUM_TOM    : 0x00;
	drums |= cymbal ? DRUM_CYMBAL : 0x00;
	drums |= hihat  ? DRUM_HI_HAT : 0x00;
	oplRegisters[0xBD] &= 0xE0;
	oplRegisters[0xBD] |= drums;
	return 0xBD;
}


/**
 * Get the wave form currently set for the given channel.
 */
byte OPL2::getWaveForm(byte ch, bool op) {
	return oplRegisters[0xE0 + getRegisterOffset(ch, op)] & 0x03;
}


/**
 * Select the wave form to use.
 */
byte OPL2::setWaveForm(byte ch, bool op, byte waveForm) {
	byte reg = 0xE0 + getRegisterOffset(ch, op);
	oplRegisters[reg] &= 0xFC;
	oplRegisters[reg] |= waveForm & 0x03;
	return reg;
}
