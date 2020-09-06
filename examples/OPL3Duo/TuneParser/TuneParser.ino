#include <OPL3Duo.h>
#include <SPI.h>
#include <midi_instruments_4op.h>


#define SONG_END '\0'
#define SONG_OCTAVE 'O'
#define SONG_OCTAVE_UP '>'
#define SONG_OCTAVE_DOWN '<'
#define SONG_TEMPO 'T'
#define SONG_NOTE_LENGTH 'L'
#define SONG_INSTRUMENT 'I'
#define SONG_REST 'R'
#define SONG_PAUSE 'P'
#define SONG_NOTE_A 'A'
#define SONG_NOTE_G 'G'
#define SONG_NOTE_SHARP '#'
#define SONG_NOTE_SHARP2 '+'
#define SONG_NOTE_FLAT '-'
#define SONG_NOTE_DOUBLE '.'
#define SONG_VOLUME 'V'


#define NUM_CHANNELS 12
#define NAN 255


struct Voice {
  const char* pattern;
  long position;
  byte ticks;
  Instrument4OP instrument;
  float volume;
  byte octave;
  byte defaultNoteLength;
  bool ended;
  byte channel;
};

struct Song {
  unsigned long tickDuration;
  Voice voice[3];
};

bool channelInUse[NUM_CHANNELS] = { 
	false, false, false, false, false, false,
	false, false, false, false, false, false
};
byte oplChannel4OP = 0;

const byte notes[3][7] = {{ NOTE_A,  NOTE_B,  NOTE_C,  NOTE_D,  NOTE_E,  NOTE_F,  NOTE_G  },
                          { NOTE_GS, NOTE_AS, NOTE_B,  NOTE_CS, NOTE_DS, NOTE_E,  NOTE_FS },
                          { NOTE_AS, NOTE_C,  NOTE_CS, NOTE_DS, NOTE_F,  NOTE_FS, NOTE_GS }};

const char pattern1[] PROGMEM = "i14t150o5l8egredgrdcerc<b>er<ba>a<a>agdefefedr4.regredgrdcerc<b>er<ba>a<a>agdedcr4.c<g>cea>cr<ag>cr<gfarfearedgrdcfrc<bagab>cdfegredgrdcerc<b>er<ba>a<a>agdedcr4.cro3c2r2";
const char pattern2[] PROGMEM = "i14o4l8crer<br>dr<ar>cr<grbrfr>cr<grbr>crer<gb>dgcrer<br>dr<ar>cr<grbrfr>cr<grbr>ceger4.rfrafergedrfdcrec<br>d<bar>c<agrgd<gr4.o4crer<br>dr<ar>cr<grbrfr>cr<grbr>cege";
const char pattern3[] PROGMEM = "i2o3l8r4gr4.gr4.er4.err4fr4.gr4.gr4.grr4gr4.er4.er4.frr4gr4>ccr4ccr4<aarraar4ggr4ffr4.ro4gab>dr4.r<gr4.gr4.err4er4.fr4.g";


OPL3Duo opl3;
Song song;

void setup() {
	opl3.begin();
	opl3.setOPL3Enabled(true);
	for (byte i = 0; i < opl3.getNum4OPChannels(); i ++) {
		opl3.set4OPChannelEnabled(i, true);
	}

	song.tickDuration = 60000 / (100 * 16);		// duration of a 64th note @ 100 bmp.
	song.voice[0].pattern = pattern1;
	song.voice[0].ended = false;
	song.voice[0].position = 0;
	song.voice[0].ticks = 1;
	song.voice[0].octave = 4;
	song.voice[0].volume = 0.8;
	song.voice[0].instrument = opl3.loadInstrument4OP(midiInstruments[0]);
	song.voice[0].defaultNoteLength = 16;

	song.voice[1].pattern = pattern2;
	song.voice[1].ended = false;
	song.voice[1].position = 0;
	song.voice[1].ticks = 1;
	song.voice[1].octave = 4;
	song.voice[1].volume = 0.8;
	song.voice[1].instrument = opl3.loadInstrument4OP(midiInstruments[0]);
	song.voice[1].defaultNoteLength = 16;

	song.voice[2].pattern = pattern3;
	song.voice[2].ended = false;
	song.voice[2].position = 0;
	song.voice[2].ticks = 1;
	song.voice[2].octave = 4;
	song.voice[2].volume = 0.8;
	song.voice[2].instrument = opl3.loadInstrument4OP(midiInstruments[0]);
	song.voice[2].defaultNoteLength = 16;


//   song.voice[0].pattern = "i13t180o4l32cceg>c<g>ceg>c<g<a->ce-a-ea >ce-a-e <<b->df b-fb >dfb-f";		// Magic mushroom!
//  song.voice[0].pattern = "i100t120l16o5eg>ecdgp1";		// 1up
//   song.voice[1].pattern = "";
//   song.voice[2].pattern = "";
}


void loop() {
	unsigned long t = millis();

	byte numEnded = 0;
	for (byte i = 0; i < 3; i ++) {
		if (!song.voice[i].ended) {
			song.voice[i].ticks --;

			if (song.voice[i].ticks == 0) {
				opl3.setKeyOn(opl3.get4OPControlChannel(song.voice[i].channel), false);
				channelInUse[song.voice[i].channel] = false;

				bool playingNote = false;
				while (!playingNote) {
					playingNote = parse(song, i);
				}

				if (song.voice[i].ended) {
					numEnded ++;
				}
			}
		}
	}

	// Song ended. Let's hang out here indefinitely :)
	if (numEnded == 3) {
		while (true);
	}

	unsigned long wait = song.tickDuration - (millis() - t);
	delay(wait);
}


/**
 * parse the next command in the command string and leave the data pointer at the next command.
 */
bool parse(Song& song, byte voiceIndex) {
	bool isPlaying = false;
	Voice& voice = song.voice[voiceIndex];

	// Get command character and convert to upper case.
	char command = pgm_read_byte_near(voice.pattern + voice.position);
	if (command >= 'a') {
		command -= 32;
	}

	switch (command) {
		// Handle end of song for this voice.
		case SONG_END: {
			voice.ended = true;
			isPlaying = true;
			break;
		}

		// Parse note and cascade into handling rest for note delay.
		case SONG_NOTE_A ... SONG_NOTE_G: {
			parseNote(voice);
		}

		// Handle a rest or pause in the tune.
		case SONG_REST:
		case SONG_PAUSE: {
			parseRest(voice);
			isPlaying = true;
			break;
		}

		// Handle 'On' to change octave.
		case SONG_OCTAVE: {
			byte octave = parseNumber(voice, 0, 7);
			if (octave != NAN) {
				voice.octave = octave;
			}
			break;
		}

		// Handle '>' to increment octave.
		case SONG_OCTAVE_UP: {
			if (voice.octave < 7) {
				voice.octave ++;
			}
			break;
		}

		// Handle '<' to decrement octave.
		case SONG_OCTAVE_DOWN: {
			if (voice.octave > 0) {
				voice.octave --;
			}
			break;
		}

		// Handle 'Tnnn' to change sone temop.
		case SONG_TEMPO: {
			byte tempo = parseNumber(voice, 40, 250);
			if (tempo != NAN) {
				song.tickDuration = 60000 / (tempo * 16);
			}
			break;
		}

		// Handle 'Lnn' to change the default note length.
		case SONG_NOTE_LENGTH: {
			voice.defaultNoteLength = parseNoteLength(voice);
			break;
		}

		// Handle 'Innn' to change the current instrument.
		case SONG_INSTRUMENT: {
			byte instrumentIndex = parseNumber(voice, 0, 127);
			if (instrumentIndex != NAN) {
				voice.instrument = opl3.loadInstrument4OP(midiInstruments[instrumentIndex]);
			}
			break;
		}

		// Handle 'Vnn' to change volume.
		case SONG_VOLUME: {
			byte volume = parseNumber(voice, 0, 15);
			if (volume != NAN) {
				voice.volume = (float)volume / 15.0;
			}
			break;
		}

		// ignore anything else.
		default:
			break;
	}

	voice.position ++;
	return isPlaying;
}



void parseNote(Voice& voice) {
	byte noteIndex = pgm_read_byte_near(voice.pattern + voice.position);
	if (noteIndex >= 'a' && noteIndex <= 'g') {
		noteIndex = noteIndex - 'a';
	} else if (noteIndex >= 'A' && noteIndex <= 'G') {
		noteIndex = noteIndex - 'A';
	} else {
		return;
	}

	byte octave = voice.octave;
	byte note = notes[0][noteIndex];

	// Handle sharp and flat notes.
	char sharpFlat = pgm_read_byte_near(voice.pattern + voice.position + 1);
	if (sharpFlat == SONG_NOTE_FLAT) {
		voice.position ++;
		note = notes[1][noteIndex];
		if (note == NOTE_B) {
			octave = max(octave - 1, 0);
		}
	} else if (sharpFlat == SONG_NOTE_SHARP || sharpFlat == SONG_NOTE_SHARP2) {
		voice.position ++;
		note = notes[2][noteIndex];
		if (note == NOTE_C) {
			octave = min(octave + 1, 7);
		}
	}

	// Find the next free channel to play the note on.
	for (byte i = 1; i <= NUM_CHANNELS; i ++) {
		oplChannel4OP = (oplChannel4OP + i) % NUM_CHANNELS;
		if (!channelInUse[oplChannel4OP]) {
			break;
		}
	}

	// Do some administartion and play the note!
	channelInUse[oplChannel4OP] = true;
	voice.channel = oplChannel4OP;
	opl3.setInstrument4OP(oplChannel4OP, voice.instrument, voice.volume);
	opl3.playNote(opl3.get4OPControlChannel(oplChannel4OP), octave, note);
}


void parseRest(Voice& voice) {
	byte ticks = parseNoteLength(voice);

	// Step through pattern data until we no longer find a digit.
	char digit;
	do {
		voice.position ++;
		digit = pgm_read_byte_near(voice.pattern + voice.position);
	} while (digit >= '0' && digit <= '9');

	// If note has a dot then add half of its duration.
	if (pgm_read_byte_near(voice.pattern + voice.position) == '.') {
		ticks += ticks / 2;
	}

	// Move position back one byte to not skip next command.
	voice.position --;

	// Set the number of ticks to hold the note.
	voice.ticks = ticks;
}


/**
 * Get note length in ticks.
 */
byte parseNoteLength(Voice voice) {
	byte length = parseNumber(voice, 1, 64);

	if (length == NAN) {
		return voice.defaultNoteLength;
	}

	// Reverse the bit order, so if duration is 16th note return 4 ticks.
	for (byte i = 6; i >= 0; i --) {
		if (length & 1 << i) {
			return 1 << (6 - i);
		}
	}

	return 16;
}


/**
 * Parse a number from 1 up to nDigits digits startting from the current pattern data position + 1.
 */
byte parseNumber(Voice voice, byte nMin, byte nMax) {
	char nextDigit = pgm_read_byte_near(voice.pattern + voice.position + 1);
	if (nextDigit < '0' || nextDigit > '9') {
		return NAN;
	}

	int number = 0;
	while(nextDigit >= '0' && nextDigit <= '9') {
		number *= 10;
		voice.position ++;
		number = number + pgm_read_byte_near(voice.pattern + voice.position) - '0';
		nextDigit = pgm_read_byte_near(voice.pattern + voice.position + 1);
	}

	return (byte)max(nMin, min(number, nMax));
}