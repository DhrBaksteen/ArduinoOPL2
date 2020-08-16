# Interpret and stream a music file via the serial interface of the Arduino OPL shield.

import sys

import exc
import opl
import imf
import vgm
import vgz


def usage():
  print(
      '''Arduino OPL2 streaming player

Usage:
   %s <serial_port> [<input_file> [<player_specific_options>]]

   - serial_port: the name of the Arduino USB serial port
   - input_file: the path to a supported input file
   - player_specific_options:
     - IMF: [<frequency_hz>]
       - frequency_hz: one of 'wolf', 'duke', or an integer value (Hz)

Examples:

  %s COM4
  - Connect to and initialize the Arduino board on COM4

  %s COM4 k5t06.imf
  - Play k5t06.imf at the default frequency
''' % ((sys.argv[0],) * 3)
  )
  sys.exit()


PLAYERS = {
    'imf': imf,
    'wlf': imf,
    'vgm': vgm,
    'vgz': vgz
}

def handle_arguments():
  portname = sys.argv[1]
  if len(sys.argv) > 2:
    filename = sys.argv[2]
    extension = filename.split('.')[-1].lower()

    try:
      player = PLAYERS[extension]
    except KeyError:
      print('Unrecognized file extension: %s' % extension)
      sys.exit(-1)

    with open(filename, 'rb') as f:
      try:
        player_opts = player.parse_args(extension, sys.argv[3:])
      except AttributeError:
        player_opts = {}
      try:
        print('Play "%s" on %s' % (filename, portname))
        if player_opts:
          print('Options: %s' % player_opts)
        print('(Ctrl-C to stop)')
        device = opl.ArduinoOpl(portname)
        player.play(device, f, **player_opts)
        device.close()
      except (KeyboardInterrupt, SystemExit):
        device.close()
      except exc.InvalidDeviceError as e:
        device.close()
        print(e)


if __name__ == '__main__':
  if len(sys.argv) <= 1:
    usage()
  handle_arguments()
