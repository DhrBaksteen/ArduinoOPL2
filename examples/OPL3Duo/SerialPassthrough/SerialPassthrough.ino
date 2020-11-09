/**
 * This demonstration sketch for the OPL3 Duo shows how you can use an Arduino to receive serial data to control the
 * OPL3 Duo board. The 'protocol' implemented in this sketch is very simple. It expects 3 bytes [BANK, REGISTER, DATA]
 * for every register write to one of the chips.
 *
 * The BANK is defined as follows:
 *    bits 7 .. 2 - Unused
 *    bit  1      - Chip 0 or 1
 *    bit  0      - Bank 0 or 1
 * The REGISTER sets the register of the selected chip and bank to write to.
 * The DATA defines the data to write to the selected register.
 *
 * You can change and improve the serial transfer to suit your application.
 *
 * Code by Maarten Janssen, 2020-11-07
 * WWW.CHEERFUL.NL
 * Most recent version of the library can be found at my GitHub: https://github.com/DhrBaksteen/ArduinoOPL2
 */

#include <SPI.h>
#include <OPL3Duo.h>

OPL3Duo opl3Duo;

void setup() {
	Serial.begin(115200);
	opl3Duo.begin();
}

void loop() {
	while (Serial.available() > 2) {
		byte bank = Serial.read() & 0x03;
		byte reg = Serial.read();
		byte val = Serial.read();

		opl3Duo.write(bank, reg, val);
	}
}
