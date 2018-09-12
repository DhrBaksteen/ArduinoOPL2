#include <SPI.h>
#include <OPL2.h>

OPL2 opl2;

void setup() {
  Serial.begin(115200);
  opl2.init();
}

void loop() {
  while (Serial.available() > 1) {
    byte reg = Serial.read();
    byte val = Serial.read();
    opl2.write(reg, val);
  }
}
