import gzip

import vgm

def play(opl, vgz_stream):
  gzf = gzip.GzipFile(fileobj=vgz_stream, mode='rb')
  vgm.play(opl, gzf)
