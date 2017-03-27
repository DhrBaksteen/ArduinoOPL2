/**
 * This is an example sketch from the OPL2 library for Arduino.
 * It streams commands to the OPL2 via the serial port.
 *
 * A very simple serial protocol is used.
 *
 * - Initial 3-way handshake to overcome reset delay / serial noise issues.
 * - 4-byte binary commands to write registers.
 *   - (uint8)  OPL2 register address
 *   - (uint8)  OPL2 register data
 *   - (uint16) delay (milliseconds)
 *
 * Example session:
 *
 * Arduino: HLO!
 * PC:      RDY?
 * Arduino: RDY! (switches to binary mode)
 * PC:      0xb82e4c01 (write OPL register and delay)
 * Arduino: k
 *
 * OPL2 board is connected as follows:
 * Pin  8 - Reset
 * Pin  9 - A0
 * Pin 10 - Latch
 * Pin 11 - Data
 * Pin 13 - Shift
 */

#include <SPI.h>
#include <OPL2.h>

// Text command mode
#define STARTUP_MSG   "HLO!\n"
#define READY_CMD     "RDY?\n"
#define READY_RSP     "RDY!\n"

// Binary command mode
#define ACK_RSP       'k'
#define RESET_CMD     0x00

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
  while (Serial.available() >= 4) {
    byte cmd[4];
    Serial.readBytes(cmd, 4);

    if (RESET_CMD != cmd[0]) {
      opl2.write(cmd[0], cmd[1]);
      unsigned short delayMs = (cmd[3] << 8) + cmd[2];

      if (delayMs > 0) {
        delay(delayMs);
      }
    } else {
      opl2.init();
    }
    Serial.write(ACK_RSP);
    Serial.flush();
  }
}

String readyCmdStr = String(READY_CMD);
void waitForReadyCmd() {
  if (Serial.available() >= 5) {
    String cmd = Serial.readString();
    if (cmd.equals(READY_CMD)) {
      ready = true;
      Serial.write(READY_RSP);
    }
  }
}

void loop() {
  if (ready) {
    processBinaryCmds();
  } else {
    waitForReadyCmd();
  }
}

