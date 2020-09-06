#include "OPL3.h"

#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
	#include <SPI.h>
	#include <Arduino.h>
#else
	#include <wiringPi.h>
	#include <wiringPiSPI.h>
#endif


OPL3::OPL3() : OPL2() {
}


/**
 * Initialize the OPL3 library and reset the chip.
 */
void OPL3::begin() {
	pinMode(pinBank, OUTPUT);
	digitalWrite(pinBank, LOW);
	OPL2::begin();
}


/**
 * Initialize the OPL3 library with custom pins.
 *
 * @param a1 - Pin number to use for A1.
 * @param a0 - Pin number to use for A0.
 * @param latch - Pin number to use for LATCH.
 * @param reset - Pin number to use for RESET.
 */
void OPL3::begin(byte a1, byte a0, byte latch, byte reset) {
	pinBank = a1;
	pinMode(pinBank, OUTPUT);
	digitalWrite(pinBank, LOW);
	OPL2::begin(a0, latch, reset);
}


/**
 * Create shadow registers to hold the values written to the OPL3 chip for later access. Only those registers that are
 * are valid on the YMF262 are created to be as memory friendly as possible for platforms with limited RAM such as the
 * Arduino Uno / Nano. Registers consume 239 bytes.
 */
void OPL3::createShadowRegisters() {
	chipRegisters = new byte[5];					//   5
	channelRegisters = new byte[3 * numChannels];	//  54
	operatorRegisters = new byte[10 * numChannels];	// 180
}


/**
 * Hard reset the YMF262 chip and initialize all registers to 0x00. This should be called before sending any data to the
 * chip.
 */
void OPL3::reset() {
	digitalWrite(pinReset, LOW);
	delay(1);
	digitalWrite(pinReset, HIGH);

	// Initialize chip registers.
	setChipRegister(0x00, 0x00);
	setChipRegister(0x08, 0x00);
	setChipRegister(0xBD, 0x00);
	setChipRegister(0x104, 0x00);
	setChipRegister(0x105, 0x00);

	// Initialize all channel and operator registers.
	for (byte i = 0; i < getNumChannels(); i ++) {
		setChannelRegister(0xA0, i, 0x00);
		setChannelRegister(0xB0, i, 0x00);
		setChannelRegister(0xC0, i, 0x00);

		for (byte j = OPERATOR1; j <= OPERATOR2; j ++) {
			setOperatorRegister(0x20, i, j, 0x00);
			setOperatorRegister(0x40, i, j, 0x00);
			setOperatorRegister(0x60, i, j, 0x00);
			setOperatorRegister(0x80, i, j, 0x00);
			setOperatorRegister(0xE0, i, j, 0x00);
		}
	}
}


/**
 * Get the internal register offset for a chip wide register.
 *
 * @param reg - The 9-bit register for which we wnat to know the internal offset.
 * @return The offset to the internal shadow register or 0 if an illegal chip register was requested.
 */
byte OPL3::getChipRegisterOffset(short reg) {
	switch (reg & 0xFF) {
		case 0x04:
			return 1;
		case 0x05:
			return 2;
		case 0x08:
			return 3;
		case 0xBD:
			return 4;
		case 0x01:
		default:
			return 0;
	}
}


/**
 * Write a given value to a channel based register.
 *
 * @param baseRegister - The base address of the register.
 * @param channel - The channel to address [0, 17]
 * @param value - The value to write to the register.
 */
void OPL3::setChannelRegister(byte baseRegister, byte channel, byte value) {
	channelRegisters[getChannelRegisterOffset(baseRegister, channel)] = value;

	byte bank = (channel >> 8) & 0x01;
	byte reg = baseRegister + (channel % CHANNELS_PER_BANK);
	write(bank, reg, value);
}


/**
 * Write a given value to an operator register for a channel.
 *
 * @param baseRegister - The base address of the register.
 * @param channel - The channel of the operator [0, 17]
 * @param operatorNum - The operator to change [0, 1].
 * @param value - The value to write to the operator's register.
 */
void OPL3::setOperatorRegister(byte baseRegister, byte channel, byte operatorNum, byte value) {
	operatorRegisters[getOperatorRegisterOffset(baseRegister, channel, operatorNum)] = value;

	byte bank = (channel >> 8) & 0x01;
	byte reg = baseRegister + getRegisterOffset(channel % CHANNELS_PER_BANK, operatorNum);
	write(bank, reg, value);
}


/**
 * Write a given value to a register of the OPL3 chip.
 *
 * @param bank - The bank (A1) of the register [0, 1].
 * @param reg - The register to be changed.
 * @param value - The value to write to the register.
 */
void OPL3::write(byte bank, byte reg, byte value) {
	digitalWrite(pinAddress, LOW);
	digitalWrite(pinBank, (bank & 0x01) ? HIGH : LOW);
	#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
		SPI.transfer(reg);
	#else
		wiringPiSPIDataRW(SPI_CHANNEL, &reg, 1);
	#endif
	digitalWrite(pinLatch, LOW);
	delayMicroseconds(8);

	digitalWrite(pinLatch, HIGH);
	delayMicroseconds(8);

	digitalWrite(pinAddress, HIGH);
	#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
		SPI.transfer(value);
	#else
		wiringPiSPIDataRW(SPI_CHANNEL, &value, 1);
	#endif
	digitalWrite(pinLatch, LOW);
	delayMicroseconds(8);
	digitalWrite(pinLatch, HIGH);
	delayMicroseconds(8);
}


/**
 * Get the number of 2OP channels for this implementation.
 *
 * @return The number of 2OP channels.
 */
byte OPL3::getNumChannels() {
	return numChannels;
}


/**
 * Get the number of 4OP channels for this implementation.
 *
 * @return The number of 4OP channels.
 */
byte OPL3::getNum4OPChannels() {
	return num4OPChannels;
}


/**
 * Get the 2-OP channel that is associated with the given 4 operator channel.
 *
 * @param channel4OP - The 4 operator channel [0, 5] for wich we want to get the associated OPL channel.
 * @param index2OP - Then 2 operator channel index [0, 1], defaults to 0 for control channel.
 * @return The OPL3 channel number that controls the 4 operator channel.
 */
byte OPL3::get4OPControlChannel(byte channel4OP, byte index2OP) {
	return channelPairs[channel4OP % getNum4OPChannels()][index2OP % 2];
}


/**
 * Create a new and empty 4-OP instrument. By default the new 4-OP instrument will be an OPL2 compatible pair of 2-OP
 * Instruments.
 *
 * @return A new, empty 4-OP instrument.
 */
Instrument4OP OPL3::createInstrument4OP() {
	Instrument4OP instrument4OP;
	instrument4OP.subInstrument[0] = createInstrument();
	instrument4OP.subInstrument[1] = createInstrument();
	return instrument4OP;
}


#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
	/**
	 * Create a 4-OP instrument and load it with instrument parameters from the given data pointer. Instrument data must
	 * be 24 bytes of contiguous instrument data to define the two sub instruments.
	 *
	 * @param instrumentData - Pointer to the offset of instrument data.
	 * @param fromProgmem - On Arduino defines to load instrument data from PROGMEM (when true (default)) or SRAM.
	 * @return The 4-OP instrument defined by the parameters at the given location in memory.
	 */
	Instrument4OP OPL3::loadInstrument4OP(const unsigned char *instrumentData, bool fromProgmem) {
		Instrument4OP instrument4OP = createInstrument4OP();

		instrument4OP.subInstrument[0] = loadInstrument(instrumentData, fromProgmem);
		instrument4OP.subInstrument[1] = loadInstrument(instrumentData + 12, fromProgmem);

		return instrument4OP;
	}
#else
	/**
	 * Create a 4-OP instrument and load it with instrument parameters from the given data pointer. Instrument data must
	 * be 24 bytes of contiguous instrument data to define the two sub instruments.
	 *
	 * @param instrumentData - Pointer to the offset of instrument data.
	 * @return The 4-OP instrument defined by the parameters at the given location in memory.
	 */
	Instrument4OP OPL3::loadInstrument4OP(const unsigned char *instrumentData) {
		Instrument4OP instrument4OP = createInstrument4OP();

		instrument4OP.subInstrument[0] = loadInstrument(instrumentData);
		instrument4OP.subInstrument[1] = loadInstrument(instrumentData + 12);

		return instrument4OP;
	}
#endif


/**
 * Create a new 4-operator instrument from the current settings of the four operators of the given 4-op channel.
 *
 * @param channel4OP - The 4-OP channel [0, 5] from which to create the instrument.
 * @return The Instrument4OP containing the current 4-OP channel operator settings.
 */
Instrument4OP OPL3::getInstrument4OP(byte channel4OP) {
	channel4OP = channel4OP % getNum4OPChannels();

	Instrument4OP instrument;
	instrument.subInstrument[0] = getInstrument(get4OPControlChannel(channel4OP, 0));
	instrument.subInstrument[1] = getInstrument(get4OPControlChannel(channel4OP, 1));

	return instrument;
}


/**
 * Assign the given 4-operator instrument to a 4-OP channel. An optional volume may be provided.
 *
 * @param channel4OP - The 4-op channel [0, 5] to assign the instrument to.
 * @param instrument - The Instrument4OP to assign to the channel.
 * @param volume - Optional volume [0.0, 1.0] that will be assigned to the operators. If omitted volume is set to 1.0.
 */
void OPL3::setInstrument4OP(byte channel4OP, Instrument4OP instrument, float volume) {
	channel4OP = channel4OP % getNum4OPChannels();
	setInstrument(get4OPControlChannel(channel4OP, 0), instrument.subInstrument[0], volume);
	setInstrument(get4OPControlChannel(channel4OP, 1), instrument.subInstrument[1], volume);
}


/**
 * Enable or disable OPL3 mode. This function must be called in order to use any of the OPL3 functions. It will also
 * set panning for all channels to enable both left and right speakers when OPL3 mode is enabled.
 *
 * @param enable - When set to true enables OPL3 mode.
 */
void OPL3::setOPL3Enabled(bool enable) {
	setChipRegister(0x105, enable ? 0x01 : 0x00);

	// For ease of use enable both the left and the right speaker on all channels when going into OPL3 mode.
	for (byte i = 0; i < getNumChannels(); i ++) {
		setPanning(i, enable, enable);
	}
}


/**
 * Returns whether OPL3 mode is currently enabled or not.
 *
 * @return True if OPL3 mode is enabled.
 */
bool OPL3::isOPL3Enabled() {
	return getChipRegister(0x105) & 0x01;
}


/**
 * Set the panning of the givven channel.
 *
 * @param channel - The channel for which to set panning [0, 17].
 * @param left - When true the left speaker will output audio.
 * @param right - When true the right speaker will output audio.
 */
void OPL3::setPanning(byte channel, bool left, bool right) {
	byte value = getChannelRegister(0xC0, channel) & 0xCF;
	value += left ? 0x10 : 0x00;
	value += right ? 0x20 : 0x00;

	setChannelRegister(0xC0, channel, value);
}


/**
 * Does audio output of the given channel go to the left speaker?
 *
 * @return True if audio output on the left speaker is enabled.
 */
bool OPL3::isPannedLeft (byte channel) {
	return getChannelRegister(0xC0, channel) & 0x10;
}


/**
 * Does audio output of the given channel go to the right speaker?
 *
 * @return True if audio output on the right speaker is enabled.
 */
bool OPL3::isPannedRight(byte channel) {
	return getChannelRegister(0xC0, channel) & 0x20;
}


/**
 * OPL3 always has waveform selection enabled. This will override the OPL2 function.
 *
 * @return Always true
 */
bool OPL3::getWaveFormSelect() {
	return true;
}


/**
 * Enabling or disabling waveform select is not implemented on OPL3. Waveform selection is always enabled and the WSE
 * bit must remain disabled! Calling this function will simply clear chip register 0x01.
 * 
 * @param enable - Dummy parameter vor OPL2 compatibility that may be ignored.
 */
void OPL3::setWaveFormSelect(bool enable) {
	setChipRegister(0x01, 0x00);
}


/**
 * Is the given 4-OP channel enabled?
 *
 * @param channel4OP -The 4-OP cahnnel [0, 5] for which we want to know if 4-operator mode is enabled.
 * @return True if the given 4-OP channel is in 4-operator mode.
 */
bool OPL3::is4OPChannelEnabled(byte channel4OP) {
	byte channelMask = 0x01 << (channel4OP % getNum4OPChannels());
	return getChipRegister(0x0104) & channelMask;
}


/**
 * Enable or disable 4-operator mode on the given 4-OP channel.
 *
 * @param channel4OP - The 4-OP channel [0, 5] for which to enable or disbale 4-operator mode.
 * @param enable - Enables or disable 4 operator mode.
 */
void OPL3::set4OPChannelEnabled(byte channel4OP, bool enable) {
	byte channelMask = 0x01 << (channel4OP % getNum4OPChannels());
	byte value = getChipRegister(0x0104) & ~channelMask;
	setChipRegister(0x0104, value + (enable ? channelMask : 0));
}


/**
 * Enables or disables all 4-OP channels.
 *
 * @param enable - Enables 4-OP channels when true.
 */
void OPL3::setAll4OPChannelsEnabled(bool enable) {
	for (byte i = 0; i < getNum4OPChannels(); i ++) {
		set4OPChannelEnabled(i, enable);
	}
}