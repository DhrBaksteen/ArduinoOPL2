/**
 * This demonstration sketch for the OPL3 Duo shows how you can use an Arduino to receive serial data to control the
 * OPL3 Duo board.
 *
 * By default this sketch is configured to use Nuke.YKT's encoding for the serial data. The OPL bank, register and value
 * are encoded into a triple of byes with the advantage of a synchronization bit that tells the code where the start of
 * a new OPL register write is. More information about encoding and decoding can be found here:
 * https://github.com/DhrBaksteen/ArduinoOPL2/wiki/Nuke.YKT's-OPL3-protocol
 * 
 * An alternative straight forward 'protocol' is also implemented in this sketch. It expects 3 bytes [BANK, REGISTER,
 * DATA] for every register write to one of the chips. See below how to switch between implementations.
 *
 * BANK - Defined as follows:
 *    bits 7 .. 2 - Unused
 *    bit  1      - Chip 0 or 1
 *    bit  0      - Bank 0 or 1
 * REGISTER - The register of the selected chip and bank to write to.
 * DATA - The data to write to the selected register.
 *
 * The OPL3 Duo can be reset by writing 165 (0xA5) to register 255 (0xFF) of bank 0. After resetting there must be a
 * delay of at least 2ms before sending more data to the OPL2 as to not flood the Arduino's serial buffer and loosing
 * data!
 *
 * Code by Maarten Janssen, 2020-11-07
 * WWW.CHEERFUL.NL
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */

#include <SPI.h>
#include <OPL3Duo.h>

// If NUKEYKTPROTO is defined then Nuke.YKT's protocol is used. It provides a basic packet validation to avoid
// syncronisation issues. Also it's implemented and ready to use with Wohlstand's OPL3BankEditor.
//
// Remove the line below to use simple unsynchronized implementation.
#define NUKEYKT

OPL3Duo opl3Duo;
byte bank;
byte reg;
byte val;

void setup() {
	Serial.begin(115200);
	opl3Duo.begin();
}

void loop() {

#ifdef NUKEYKT
	// Nuke.YKT encoded data.
	if (Serial.available()) {
		int x1 = Serial.read();

		// Check if first byte of the transferred packet is valid.
		if (x1 & 0x80) {
			while (Serial.available() < 2);
			byte x2 = Serial.read();
			byte x3 = Serial.read();

			// Decode byte triplet.
			bank = x1 >> 2;
			reg = ((x1 << 6) & 0x0C0) | ((x2 >> 1) & 0x3F);
			val = ((x2 << 7) & 0x80) | (x3 & 0x7F);

			if (bank == 0x00 && reg == 0xFF && val == 0xA5) {
				opl3Duo.reset();
			} else {
				opl3Duo.write(bank, reg, val);
			}
		}
	}
#else
	// Unencoded serial data.
	while (Serial.available() > 2) {
		bank = Serial.read() & 0x03;
		reg = Serial.read();
		val = Serial.read();

		if (bank == 0x00 && reg == 0xFF && val == 0xA5) {
			opl3Duo.reset();
		} else {
			opl3Duo.write(bank, reg, val);
		}
	}
#endif
}
