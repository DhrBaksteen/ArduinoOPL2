import struct

import exc

SAMPLE_RATE = 44100
YM3812 = 0x5a
DELAY_N = 0x61
DELAY_735 = 0x62
DELAY_882 = 0x63
DELAY_N1 = 0x70
END_DATA = 0x66

SKIP = {
  0x30: 4, # YM2151 clock
  0x50: 4, # YM3812 clock
  0x51: 2, # Write YM2413
}

def _samples_to_us(n):
  return 1000000 * n // SAMPLE_RATE

def play(opl, vgm_stream):
  data_offset = parse_header(vgm_stream)
  vgm_stream.seek(data_offset)

  delay_us = 0

  cmd = None
  while True:
    cmd = vgm_stream.read(1)
    if len(cmd) < 1:
      break
    opcode = cmd[0]

    if YM3812 == opcode:
      addr, data = struct.unpack_from('BB', vgm_stream.read(2))
      opl.write_reg(addr, data, delay_us, True)
      delay_us = 0
    elif DELAY_N == opcode:
      delay_samples, = struct.unpack_from('H', vgm_stream.read(2))
      delay_us += _samples_to_us(delay_samples)
    elif DELAY_735 == opcode:
      delay_us += _samples_to_us(735)
    elif DELAY_882 == opcode:
      delay_us += _samples_to_us(882)
    elif opcode >= DELAY_N1:
      delay_samples = 1 + int(opcode & 0xf)
      delay_us += _samples_to_us(delay_samples)
    elif opcode == END_DATA:
      break
    elif opcode in SKIP:
      vgm_stream.read(SKIP[opcode])
    else:
      raise exc.InvalidFormatError('Unrecognized VGM opcode: 0x%02x' % opcode)


def parse_header(vgm_stream):
  ident = vgm_stream.read(4)

  if ident != b'Vgm ':
    raise exc.InvalidFormatError('vgm')

  vgm_stream.seek(8)
  version, = struct.unpack('<I', vgm_stream.read(4))
  ver_major = (version >> 8) & 0xff
  ver_minor = version & 0xff

  data_offset = 0x40
  if ver_major >= 0x1 and ver_minor >= 0x50:
    vgm_stream.seek(0x34)
    data_offset, = struct.unpack('<I', vgm_stream.read(4))

  return 0x100 + data_offset
