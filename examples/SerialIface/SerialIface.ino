/**
 * This is an example sketch from the OPL2 library for Arduino.
 * It streams commands to the OPL2 via the serial port.
 *
 * For complex music (eg Doom), enlarge the Arduino Rx buffer to 512 bytes or more:
 *
 * - Edit hardware/arduino/avr/cores/arduino/HardwareSerial.h
 * - #define SERIAL_RX_BUFFER_SIZE 512
 *
 * A very simple serial protocol is used.
 *
 * - Initial 3-way handshake to overcome reset delay / serial noise issues.
 * - 2-byte binary commands to write registers.
 *   - (uint8)  OPL2 register address
 *   - (uint8)  OPL2 register data
 *
 * Example session:
 *
 * Arduino: HLO!
 * PC:      BUF?
 * Arduino: 256 (switches to binary mode)
 * PC:      0xb80a (write OPL register)
 * Arduino: k
 *
 * OPL2 board is connected as follows:
 * Pin  8 - Reset
 * Pin  9 - A0
 * Pin 10 - Latch
 * Pin 11 - Data     (Use pin 51 for Arduino Mega)
 * Pin 13 - Shift    (Use pin 52 for Arduino Mega)
 */

#include <SPI.h>
#include <OPL2.h>

// Text command mode
#define STARTUP_MSG   "HLO!\n"
#define BUF_SIZE_CMD  "BUF?\n"

// Binary command mode
#define ACK_RSP         'k'
#define RESET_CMD       0
#define BINARY_CMD_SIZE 2

OPL2 opl2;
bool ready = false;

void setup() {
  opl2.init();
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  Serial.write(STARTUP_MSG);
}

void processBinaryCmds() {
  while (Serial.available() >= BINARY_CMD_SIZE) {
    byte cmd[BINARY_CMD_SIZE];
    Serial.readBytes(cmd, BINARY_CMD_SIZE);

    if (RESET_CMD != cmd[0]) {
      opl2.write(cmd[0], cmd[1]);
    } else {
      opl2.init();
    }
    Serial.write(ACK_RSP);
  }
}

void waitForBufSizeCmd() {
  if (Serial.available() >= 5) {
    String cmd = Serial.readString();
    if (cmd.equals(BUF_SIZE_CMD)) {
      ready = true;
      Serial.print(SERIAL_RX_BUFFER_SIZE);
      Serial.print("\n");
    }
  }
}

void loop() {
  if (ready) {
    processBinaryCmds();
  } else {
    waitForBufSizeCmd();
  }
}

