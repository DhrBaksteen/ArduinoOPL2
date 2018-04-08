import struct
import threading
import time

import serial

class ArduinoOpl:

  STARTUP_MSG = b'HLO!\n'
  READY_CMD = b'BUF?\n'
  ACK_RSP = b'k'
  BINARY_CMD_SIZE = 2
  RESET_CMD = b'\x00' * BINARY_CMD_SIZE


  def __init__(self, portname, baudrate=115200, debug=False):
    self.port = serial.Serial(portname, baudrate, timeout=None)
    self.ready = False
    self.overdelay = 0
    self.debug = debug

    # Opening port resets Arduino. Wait for ready message.
    self.wait_for_rsp(self.STARTUP_MSG)

    self._debug('Tx: %s' % self.READY_CMD)
    self.port.write(self.READY_CMD)
    opl_rx_buf_bytes = int(self.port.readline())
    self._debug('Rx buffer size: %d bytes' % opl_rx_buf_bytes)
    self.buffer_size = opl_rx_buf_bytes // self.BINARY_CMD_SIZE
    self._status(self.READY_CMD)

    self.sem = threading.Semaphore(self.buffer_size)
    self.ack_thread = threading.Thread(target=self.ack)
    self.ack_thread.daemon = True
    self.ack_thread.start()

    self.ready = True

  def ack(self):
    while True:
      rsp = self.port.read()
      if rsp == '':
        break
      if rsp != self.ACK_RSP:
        raise RuntimeError('Expected: %s, received: %s' % (rsp, self.ACK_RSP))
      self.sem.release()

  def wait_for_rsp(self, rsp):
    self._debug('Awaiting: %s' % rsp)
    rx = self.port.readline()
    self._debug('Rx: %s' % rx)
    if not rx.endswith(rsp):
      raise RuntimeError('Expected: %s, received: %s' % (rsp, rx))

  def wait_for_ack(self):
    rsp = self.port.read()
    if rsp != self.ACK_RSP:
      raise RuntimeError('Expected: %s, received: %s' % (rsp, self.ACK_RSP))
    self.n_outstanding -= 1

  def write_reg(self, addr, data, delay_us, predelay=False):
    if predelay and delay_us > 0:
      time.sleep(delay_us / 1000000.)

    cmd = struct.pack('!BB', addr, data)
    if not self.sem.acquire(False):
      self.overdelay += 1
      self.sem.acquire()
    self.port.write(cmd)
    self._debug('Tx: %s' % ['%02x' % b for b in cmd])
    if not predelay and delay_us > 0:
      time.sleep(delay_us / 1000000.)
    self._status(cmd)

  def _status(self, last_tx):
    status = 'Initialized' if self.ready else 'Initializing'
    tx_txt = 'Tx: %s' % ' '.join('%02x' % b for b in last_tx)
    status_str = "STATUS %-12s BUFFER: %5d LATE: %3d   %s" % (status,
                                                              self.buffer_size,
                                                              self.overdelay,
                                                              tx_txt)
    print("%s\r" % status_str.ljust(79), end='')

  def _debug(self, txt):
    if self.debug:
      print(txt)

  def close(self):
    for n in range(self.buffer_size):
      self.sem.acquire()
    self.port.write(self.RESET_CMD)
    self._status(self.RESET_CMD)
    self.port.close()
    print("")
