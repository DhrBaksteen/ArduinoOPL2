#include "OPL3Duo.h"

#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
	#include <SPI.h>
	#include <Arduino.h>
#else
	#include <wiringPi.h>
	#include <wiringPiSPI.h>
#endif


OPL3Duo::OPL3Duo() : OPL3() {
}


/**
 * Initialize the OPL3 library and reset the chips.
 */
void OPL3Duo::begin() {
    pinMode(pinUnit, OUTPUT);
	digitalWrite(pinUnit, LOW);

	OPL3::begin();
}


/**
 * Create 4 banks of shadow registers to hold the values writtent to the OPL3 chips for later access.
 */
void OPL3Duo::createShadowRegisters() {
	oplRegisters = new byte[4 * 256];
}


/**
 * Hard reset the OPL3 chip. This should be done before sending any register data to the chip.
 */
void OPL3Duo::reset() {
    for (byte unit = 0; unit < 2; unit ++) {
        digitalWrite(pinUnit, unit == 1);
        digitalWrite(pinReset, LOW);
        delay(1);
        digitalWrite(pinReset, HIGH);
    }

    for(byte bank = 0; bank < 4; bank ++) {
        for(short reg = 0; reg < 256; reg ++) {
            write(bank, reg, 0x00);
        }
    }

    digitalWrite(pinUnit, LOW);
}


/**
 * Return the number of channels for the OPL3.
 */
byte OPL3Duo::getNumChannels() {
	return NUM_OPL3DUO_CHANNELS;
}


/**
 * Get the current value of a chip wide register from the shadow registers.
 * TODO: Specify what chip! --> Currently only accesses unit 0 data!
 * 
 * @param reg - The 9-bit address of the register.
 * @return The value of the register from shadow registers.
 */
byte OPL3Duo::getChipRegister(short reg) {
	return oplRegisters[reg & 0x01FF];
}


/**
 * Write a given value to a chip wide register.
 *
 * @param reg - The 9-bit register to write to.
 * @param value - The value to write to the register.
 */
void OPL3Duo::setChipRegister(short reg, byte value) {
	byte bank = (reg >> 8) & 0x01;
	reg = reg & 0xFF;
	write(bank, reg, value);
    write(bank | 0x02, reg, value);
}


/**
 * Get the value of a channel register.
 *
 * @param baseAddress - The base address of the register.
 * @param channel - The channel for which to get the register value.
 * @return The current value of the from the shadow register.
 */
byte OPL3Duo::getChannelRegister(byte baseRegister, byte channel) {
	byte bank = (channel / CHANNELS_PER_BANK) & 0x03;
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
void OPL3Duo::setChannelRegister(byte baseRegister, byte channel, byte value) {
	byte bank = (channel / CHANNELS_PER_BANK) & 0x03;
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
byte OPL3Duo::getOperatorRegister(byte baseRegister, byte channel, byte op) {
	byte bank = (channel / CHANNELS_PER_BANK) & 0x03;
	byte registerOffset = registerOffsets[op][channel % CHANNELS_PER_BANK];
	byte reg = baseRegister + (channel % CHANNELS_PER_BANK) + registerOffset;
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
void OPL3Duo::setOperatorRegister(byte baseRegister, byte channel, byte op, byte value) {
	byte bank = (channel / CHANNELS_PER_BANK) & 0x03;
	byte registerOffset = registerOffsets[op][channel % CHANNELS_PER_BANK];
	byte reg = baseRegister + (channel % CHANNELS_PER_BANK) + registerOffset;
	write(bank, reg, value);
}


/**
 * Write a given value to a register of the OPL3 chip.
 *
 * @param bank - The bank + unit (A1 + A2) of the register [0, 3].
 * @param reg - The register to be changed.
 * @param value - The value to write to the register.
 */
void OPL3Duo::write(byte bank, byte reg, byte value) {
    digitalWrite(pinUnit, bank & 0x02);
    OPL3::write(bank, reg, value);
}
