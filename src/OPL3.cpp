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
 Create 2 banks of shadow registers to hold the values writtent to the OPL3 chip for later access.
 */
void OPL3::createShadowRegisters() {
	oplRegisters = new byte[2 * 256];
}


/**
 * Hard reset the OPL3 chip. This should be done before sending any register data to the chip.
 */
void OPL3::reset() {
	digitalWrite(pinReset, LOW);
	delay(1);
	digitalWrite(pinReset, HIGH);

	for(byte bank = 0; bank < 2; bank ++) {
		for(short reg = 0; reg < 256; reg ++) {
			write(bank, reg, 0x00);
		}
	}
}


/**
 * Return the number of channels for the OPL3.
 */
byte OPL3::getNumChannels() {
	return NUM_OPL3_CHANNELS;
}


/**
 * Get the current value of a chip wide register from the shadow registers.
 *
 * @param reg - The 9-bit address of the register.
 * @return The value of the register from shadow registers.
 */
byte OPL3::getChipRegister(short reg) {
	return oplRegisters[reg & 0x01FF];
}


/**
 * Write a given value to a chip wide register.
 *
 * @param reg - The 9-bit register to write to.
 * @param value - The value to write to the register.
 */
void OPL3::setChipRegister(short reg, byte value) {
	byte bank = reg > 0xFF ? 1 : 0;
	reg = reg & 0xFF;
	write(bank, reg, value);
}


/**
 * Get the value of a channel register.
 *
 * @param baseAddress - The base address of the register.
 * @param channel - The channel for which to get the register value.
 * @return The current value of the from the shadow register.
 */
byte OPL3::getChannelRegister(byte baseRegister, byte channel) {
	byte bank = channel > 8 ? 1 : 0;
	byte reg = baseRegister + (channel % CHANNELS_PER_BANK);
	return oplRegisters[bank * 256 + reg];
}


/**
 * Write a given value to a channel based register.
 *
 * @param baseRegister - The base address of the register.
 * @param channel - The channel to address [0, 17]
 * @param value - The value to write to the register.
 */
void OPL3::setChannelRegister(byte baseRegister, byte channel, byte value) {
	byte bank = channel > 8 ? 1 : 0;
	byte reg = baseRegister + (channel % CHANNELS_PER_BANK);
	write(bank, reg, value);
}


/**
 * Get the current value of an operator register of a channel from the shadow registers.
 *
 * @param baseRegister - The base address of the register.
 * @param channel - The channel of the operatpr [0, 17].
 * @param op - The operator [0, 1].
 * @return The operator register value from shadow registers.
 */
byte OPL3::getOperatorRegister(byte baseRegister, byte channel, byte op) {
	byte bank = channel > 8 ? 1 : 0;
	byte reg = baseRegister + getRegisterOffset(channel % CHANNELS_PER_BANK, op);
	return oplRegisters[bank * 256 + reg];
}


/**
 * Write a given value to an operator register for a channel.
 *
 * @param baseRegister - The base address of the register.
 * @param channel - The channel of the operator [0, 17]
 * @param op - The operator to change [0, 1].
 * @param value - The value to write to the operator's register.
 */
void OPL3::setOperatorRegister(byte baseRegister, byte channel, byte op, byte value) {
	byte bank = channel > 8 ? 1 : 0;
	byte reg = baseRegister + getRegisterOffset(channel % CHANNELS_PER_BANK, op);
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
	oplRegisters[bank * 256 + reg] = value;

	digitalWrite(pinAddress, LOW);
	digitalWrite(pinBank, bank & 0x01 ? HIGH : LOW);
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
	delayMicroseconds(2);
	digitalWrite(pinLatch, HIGH);
	delayMicroseconds(46);
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
	channel4OP = channel4OP % NUM_4OP_CHANNELS;

	Instrument4OP instrument;
	instrument.subInstrument[0] = getInstrument(channelPairs[channel4OP][0]);
	instrument.subInstrument[1] = getInstrument(channelPairs[channel4OP][1]);

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
	channel4OP = channel4OP % NUM_4OP_CHANNELS;
	setInstrument(channelPairs[channel4OP][0], instrument.subInstrument[0], volume);
	setInstrument(channelPairs[channel4OP][1], instrument.subInstrument[1], volume);
}


/**
 * Enable or disable OPL3 mode. This function must be called in order to use any of the OPL3 functions. It will also
 * set panning for all channels to enable both left and right speakers when OPL3 mode is enabled.
 *
 * @param enable - When set to true enables OPL3 mode.
 */
void OPL3::enableOPL3(bool enable) {
	setChipRegister(0x0105, enable ? 0x01 : 0x00);

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
	return oplRegisters[0x0105] & 0x01;
}


/**
 * Get the 2-OP channel that is associated with the given 4 operator channel.
 *
 * @param channel4OP - The 4 operator channel [0, 5] for wich we want to get the associated OPL channel.
 * @param index2OP - Then 2 operator channel index [0, 1], defaults to 0.
 * @return The OPL3 channel number that controls the 4 operator channel.
 */
byte OPL3::get4OPControlChannel(byte channel4OP, byte index2OP) {
	return channelPairs[channel4OP % NUM_4OP_CHANNELS][index2OP % 2];
}


/**
 * Set the panning of the givven channel.
 *
 * @param channel - The channel for which to set panning [0, 17].
 * @param left - When true the left speaker will output audio.
 * @param right - When true the right speaker will output audio.
 */
void OPL3::setPanning(byte channel, bool left, bool right) {
	channel = channel % getNumChannels();

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
	return getChannelRegister(0xC0, channel % getNumChannels()) & 0x10;
}


/**
 * Does audio output of the given channel go to the right speaker?
 *
 * @return True if audio output on the right speaker is enabled.
 */
bool OPL3::isPannedRight(byte channel) {
	return getChannelRegister(0xC0, channel % getNumChannels()) & 0x20;
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
 * @return True if the given 4-OP channel is in 4-operator mode.
 */
bool OPL3::is4OPChannelEnabled(byte channel4OP) {
	byte channelMask = 0x01 << (channel4OP % NUM_4OP_CHANNELS);
	return getChipRegister(0x0104) & channelMask;
}


/**
 * Enable or disable 4-operator mode on the given 4-OP channel.
 *
 * @param channel4OP - The 4-OP channel [0, 5] for which to enable or disbale 4=operator mode.
 * @param enable - Enables or disable 4 operator mode.
 */
void OPL3::enable4OPChannel(byte channel4OP, bool enable) {
	byte channelMask = 0x01 << (channel4OP % NUM_4OP_CHANNELS);
	byte value = getChipRegister(0x0104) & ~channelMask;
	setChipRegister(0x0104, value + (enable ? channelMask : 0));
}
