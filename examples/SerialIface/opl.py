import struct

import serial

class ArduinoOpl:

  STARTUP_MSG = b'HLO!\n'
  READY_CMD = b'RDY?\n'
  READY_RSP = b'RDY!\n'
  ACK_RSP = b'k'
  RESET_CMD = b'\x00' * 4

  def __init__(self, portname, baudrate=115200, max_write_ahead=5, debug=False):
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

  def write_reg(self, addr, data, delay_ms=0):
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
    status_str = "STATUS %-14s BUFFER %-14s %s" % (status, buf_status, tx_txt)
    print("%s\r" % status_str.ljust(79), end='')

  def _debug(self, txt):
    if self.debug:
      print(txt)

  def close(self):
    self.port.write(self.RESET_CMD)
    self._status(self.RESET_CMD)
    self.port.close()
