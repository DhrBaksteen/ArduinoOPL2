import struct

import serial

from time import sleep

class ArduinoOpl:

  def __init__(self, portname, baudrate=115200, debug=False):
    self.port = serial.Serial(portname, baudrate, timeout=None)
    self.debug = debug
    self.status = 'Initializing'
    self.reset()
    self.status = 'Initialized'

  def write_reg(self, addr, data, delay_us=0, predelay=False):
    delay_ms = delay_us // 1000
    delay_remainder = delay_us % 1000
    cmd = struct.pack('>BB', addr, data)
    if predelay and delay_ms > 0:
        sleep(delay_us / 1000000.0)
    self.port.write(cmd)
    self._status(cmd)
    self._debug('Tx: %s' % ['%02x' % b for b in cmd])
    if (not predelay) and delay_ms > 0:
      sleep(delay_us / 1000000.0)

  def _status(self, last_tx):
    tx_txt = 'Tx: %s' % ' '.join('%02x' % b for b in last_tx)
    status_str = "STATUS %-12s    %s" % (self.status, tx_txt)
    print("%s\r" % status_str.ljust(79), end='')

  def _debug(self, txt):
    if self.debug:
      print(txt)

  def reset(self):
      for reg in range(0, 256):
          self.write_reg(reg, 0x00)

  def close(self):
    self.status = 'Closing'
    self.reset()
    self.port.close()
    print("")
