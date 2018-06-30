#include <SPI.h>
#include <OPL2.h>
#include <midi_instruments.h>

#define MIDI_CHANNELS 16
#define OPL2_CHANNELS 9

const unsigned char *instruments[] = {
  PIANO1, PIANO2, PIANO3, HONKTONK, EP1, EP2, HARPSIC, CLAVIC, CELESTA, GLOCK, MUSICBOX, VIBES, MARIMBA, XYLO, TUBEBELL, SANTUR,
  ORGAN1, ORGAN2, ORGAN3, PIPEORG, REEDORG, ACORDIAN, HARMONIC, BANDNEON, NYLONGT, STEELGT, JAZZGT, CLEANGT, MUTEGT, OVERDGT, DISTGT, GTHARMS,
  ACOUBASS, FINGBASS, PICKBASS, FRETLESS, SLAPBAS1, SLAPBAS2, SYNBASS1, SYNBASS2, VIOLIN, VIOLA, CELLO, CONTRAB, TREMSTR, PIZZ, HARP, TIMPANI,
  STRINGS, SLOWSTR, SYNSTR1, SYNSTR2, CHOIR, OOHS, SYNVOX, ORCHIT, TRUMPET, TROMBONE, TUBA, MUTETRP, FRHORN, BRASS1, SYNBRAS1, SYNBRAS2,
  SOPSAX, ALTOSAX, TENSAX, BARISAX, OBOE, ENGLHORN, BASSOON, CLARINET, PICCOLO, FLUTE1, RECORDER, PANFLUTE, BOTTLEB, SHAKU, WHISTLE, OCARINA,
  SQUARWAV, SAWWAV, SYNCALLI, CHIFLEAD, CHARANG, SOLOVOX, FIFTHSAW, BASSLEAD, FANTASIA, WARMPAD, POLYSYN, SPACEVOX, BOWEDGLS, METALPAD, HALOPAD, SWEEPPAD,
  ICERAIN, SOUNDTRK, CRYSTAL, ATMOSPH, BRIGHT, GOBLIN, ECHODROP, STARTHEM, SITAR, BANJO, SHAMISEN, KOTO, KALIMBA, BAGPIPE, FIDDLE, SHANNAI,
  TINKLBEL, AGOGO, STEELDRM, WOODBLOK, TAIKO, MELOTOM, SYNDRUM, REVRSCYM, FRETNOIS, BRTHNOIS, SEASHORE, BIRDS, TELEPHON, HELICOPT, APPLAUSE
};

struct ChannelMapping {
  byte midiChannel;
  byte midiNote;
};


OPL2 opl2;
ChannelMapping channelMap[OPL2_CHANNELS];
byte programMap[MIDI_CHANNELS];

void setup() {
  Serial.begin(115200);

  usbMIDI.setHandleNoteOn(onNoteOn);
  usbMIDI.setHandleNoteOff(onNoteOff);
  usbMIDI.setHandleProgramChange(onProgramChange);

  opl2.init();
  for (byte i = 0; i < OPL2_CHANNELS; i ++) {
    opl2.setInstrument(i, PIANO1);
  }

  for (byte i = 0; i < MIDI_CHANNELS; i ++) {
    programMap[i] = 0;
  }
}


void loop() {
  usbMIDI.read();
}


void onNoteOn(byte channel, byte note, byte velocity) {
  if (channel == 10) return;

  for (byte i = 0; i < OPL2_CHANNELS; i ++) {
    if (!opl2.getKeyOn(i)) {
      channelMap[i].midiChannel = channel;
      channelMap[i].midiNote    = note;
      
      note = max(24, min(note, 119));
      byte opl2Octave = 1 + (note - 24) / 12;
      byte opl2Note   = note % 12;
      byte opl2Volume = ~(velocity >> 1);
      
      opl2.setInstrument(i, instruments[programMap[channel]]);
      opl2.setVolume(i, OPERATOR1, opl2Volume);
      opl2.setVolume(i, OPERATOR2, opl2Volume);
      opl2.playNote(i, opl2Octave, opl2Note);

      break;
    }
  }
}


void onNoteOff(byte channel, byte note, byte velocity) {
  for (byte i = 0; i < OPL2_CHANNELS; i ++) {
    if (channelMap[i].midiChannel == channel && channelMap[i].midiNote == note) {
      channelMap[i].midiChannel = 255;
      channelMap[i].midiNote    = 255;
      opl2.setKeyOn(i, false);
      break;
    }
  }
}
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                     

void onProgramChange(byte channel, byte program) {
  programMap[channel] = program;
}

