import struct

import exc

SAMPLE_RATE = 44100
YM3812 = 0x5a
DELAY_N = 0x61
DELAY_735 = 0x62
DELAY_882 = 0x63
DELAY_N1 = 0x70
END_DATA = 0x66

def _samples_to_ms(n):
  return 1000 * n // SAMPLE_RATE

def play(opl, vgm_stream):
  data_offset = parse_header(vgm_stream)
  vgm_stream.seek(data_offset)

  delay_ms = 0

  cmd = None
  while cmd != END_DATA:
    cmd = vgm_stream.read(1)
    if len(cmd) < 1:
      break
    opcode = cmd[0]

    if YM3812 == opcode:
      addr, data = struct.unpack_from('BB', vgm_stream.read(2))
      opl.write_reg(addr, data, delay_ms)
      delay_ms = 0
    elif DELAY_N == opcode:
      delay_samples, = struct.unpack_from('H', vgm_stream.read(2))
      delay_ms = _samples_to_ms(delay_samples)
    elif DELAY_735 == opcode:
      delay_ms = _samples_to_ms(735)
    elif DELAY_882 == opcode:
      delay_ms = _samples_to_ms(882)
    elif opcode >= DELAY_N1:
      delay_samples = 1 + int(opcode & 0xf)
      delay_ms = _samples_to_ms(delay_samples)
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
