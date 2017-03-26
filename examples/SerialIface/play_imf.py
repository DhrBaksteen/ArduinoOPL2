# Stream an IMF file via the serial interface of the Arduino OPL shield.

import struct, sys, time
import serial

class ArduinoOpl:

  STARTUP_MSG = b'HLO!\n'
  READY_CMD = b'RDY?\n'
  READY_RSP = b'RDY!\n'
  ACK_RSP = b'k'
  RESET_CMD = b'\x00' * 4

  def __init__(self, portname, baudrate=115200, max_write_ahead=10, debug=False):
    self.port = serial.Serial(portname, baudrate, timeout=None)
    self.ready = False
    self.max_write_ahead = max_write_ahead
    self.n_outstanding = 0
    self.debug = debug

    self._status(b'')
    # Opening port resets Arduino. Wait for ready message.
    self.wait_for_rsp(self.STARTUP_MSG)

    self._debug('Tx: %s' % self.READY_CMD)
    self._status(self.READY_CMD)
    self.port.write(self.READY_CMD)
    self.wait_for_rsp(self.READY_RSP)

    self.ready = True

  def wait_for_rsp(self, rsp):
    self._debug('Awaiting: %s' % rsp)
    rx = self.port.readline()
    self._debug('Rx: %s' % rx)
    if not rx.endswith(rsp):
      raise RuntimeError('Expected: %s, received: %s' % (rsp, rx))

  def write_reg(self, addr, data, delay_ms):
    if self.n_outstanding >= self.max_write_ahead:
      rsp = self.port.read()
      if rsp != self.ACK_RSP:
        raise RuntimeError('Expected: %s, received: %s' % (rsp, self.ACK_RSP))
      self.n_outstanding -= 1
    self.write_reg_unbuffered(addr, data, delay_ms)

  def write_reg_unbuffered(self, addr, data, delay_ms):
    cmd = struct.pack('BBH', addr, data, delay_ms)
    self.n_outstanding += 1
    self.port.write(cmd)
    self._debug('Tx: %s' % ['%02x' % b for b in cmd])
    if delay_ms > 0:
      self._debug('%8d ms' % delay_ms)
    self._status(cmd)

  def _status(self, last_tx):
    status = 'Initialized' if self.ready else 'Initializing'
    buf_status = '[ %s ]' % ('#' * self.n_outstanding).ljust(self.max_write_ahead)
    tx_txt = 'Tx: %s' % ['%02x' % b for b in last_tx]
    print("%-19s %-29s %-29s\r" % (status, buf_status, tx_txt), end='')

  def _debug(self, txt):
    if self.debug:
      print(txt)

  def close(self):
    self.port.write(self.RESET_CMD)
    self._status(self.RESET_CMD)
    self.port.close()


DEFAULT_IMF_FREQUENCY_HZ = 560

def play_imf(opl, imf_stream, frequency_hz=DEFAULT_IMF_FREQUENCY_HZ):
  # Type-1 format
  song_length, = struct.unpack('H', f.read(2))
  # Type-2 format
  if song_length == 0:
    song_length = 65535
    f.read(2)

  i = 0
  while i < song_length:
    cmd = f.read(4)

    addr, data, delay_cycles = struct.unpack_from('BBH', cmd)
    delay_us = 1000000 * delay_cycles // frequency_hz
    opl.write_reg(addr, data, delay_us // 1000)
    i += 4


if __name__ == '__main__':
  try:
    portname = sys.argv[1]
    opl = ArduinoOpl(portname)
    if len(sys.argv) > 2:
      filename = sys.argv[2]

      frequency_hz = DEFAULT_IMF_FREQUENCY_HZ
      if len(sys.argv) > 3:
        frequency_hz = int(sys.argv[3])

      with open(sys.argv[2], 'rb') as f:
        print('Playing "%s" on %s @ %d Hz' % (filename, portname, frequency_hz))
        play_imf(opl, f, frequency_hz)

  except KeyboardInterrupt:
    print('Shutting down..')
    opl.close()
