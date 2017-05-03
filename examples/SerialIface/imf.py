import collections
import struct

IMF_FREQUENCIES = collections.defaultdict(lambda: 560)
IMF_FREQUENCIES.update({
    'duke': 280,
    'wolf': 700,
    'wlf': 700
})


def parse_args(file_extension, args):
  frequency_hz = IMF_FREQUENCIES['default']
  if not args:
    frequency_hz = IMF_FREQUENCIES[file_extension]
  else:
    try:
      frequency_hz = int(args[0])
    except ValueError:
      frequency_hz = IMF_FREQUENCIES[args[0].lower()]
  return {'frequency_hz': frequency_hz}


def play(opl, imf_stream, **kwargs):
  song_length, = struct.unpack('H', imf_stream.read(2))

  if song_length == 0:
    imf_type = 0
    imf_stream.read(2)
  else:
    imf_type = 1

  i = 0
  while imf_type == 0 or imf_type == 1 and i < song_length:
    cmd = imf_stream.read(4)
    if len(cmd) < 4:
      break

    addr, data, delay_cycles = struct.unpack_from('BBH', cmd)
    delay_us = 1000000 * delay_cycles // kwargs['frequency_hz']
    opl.write_reg(addr, data, delay_us)
    i += 4
