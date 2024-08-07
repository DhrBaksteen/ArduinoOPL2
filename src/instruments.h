/**
 * This file contains instrument defenitions from the Adlib standard instrument library for use with the OPL2 Audio
 * Board library.
 *
 * Instrument definition is based on Adlib instrument bank format.
 *  0 - Transpose
 *		Tells the number of semi-tones a note that is played using this instrument should be pitched up or down. For
 *		drum instruments that are played in melodic mode this sets the absolute note to be played for the drum sound.
 *
 *  1 - Channel c, operator 1, register 0x20
 *		Tremolo(1) | Vibrato(1) | Sustain(1) | KSR(1) | Frequency multiplier (4)
 *
 *  2 - Channel c, operator 1, register 0x40
 *		Key scale level(2) | Output level(6)
 *
 *  3 - Channel c, operator 1, register 0x60
 *		Attack(4) | Decay(4)
 *
 *  4 - Channel c, operator 1, register 0x80
 *		Sustain(4) | Release(4)
 *
 *  5 - Channel c, register 0xC0
 *		Undefined(4) | Modulation feedback factor(3) | Synth type(1)
 *
 *  6 - Channel c, operator 2, register 0x20
 *  7 - Channel c, operator 2, register 0x40
 *  8 - Channel c, operator 2, register 0x60
 *  9 - Channel c, operator 2, register 0x80
 *
 * 10 - Channel c, operators 1 + 2, register 0xE0
 *		OP1 WaveForm(3) | OP2 Waveform(3)
 */

#ifndef _ADLIB_INST_H_
	#define _ADLIB_INST_H_

	// Melodic instruments.
	const unsigned char INSTRUMENT_ACCORDN[11]  PROGMEM = { 0x00, 0x24, 0x4F, 0xF2, 0x0B, 0x0E, 0x31, 0x00, 0x52, 0x0B, 0x00 };
	const unsigned char INSTRUMENT_BAGPIPE1[11] PROGMEM = { 0x00, 0x31, 0x43, 0x6E, 0x17, 0x02, 0x22, 0x05, 0x8B, 0x0C, 0x21 };
	const unsigned char INSTRUMENT_BAGPIPE2[11] PROGMEM = { 0x00, 0x30, 0x00, 0xFF, 0xA0, 0x00, 0xA3, 0x00, 0x65, 0x0B, 0x23 };
	const unsigned char INSTRUMENT_BANJO1[11]   PROGMEM = { 0x00, 0x31, 0x87, 0xA1, 0x11, 0x08, 0x16, 0x80, 0x7D, 0x43, 0x00 };
	const unsigned char INSTRUMENT_BASS1[11]    PROGMEM = { 0x00, 0x01, 0x15, 0x25, 0x2F, 0x0A, 0x21, 0x80, 0x65, 0x6C, 0x00 };
	const unsigned char INSTRUMENT_BASS2[11]    PROGMEM = { 0x00, 0x01, 0x1D, 0xF2, 0xEF, 0x0A, 0x01, 0x00, 0xF5, 0x78, 0x00 };
	const unsigned char INSTRUMENT_BASSHARP[11] PROGMEM = { 0x00, 0xC0, 0x6D, 0xF9, 0x01, 0x0E, 0x41, 0x00, 0xF2, 0x73, 0x01 };
	const unsigned char INSTRUMENT_BASSOON1[11] PROGMEM = { 0x00, 0x30, 0xC8, 0xD5, 0x19, 0x0C, 0x71, 0x80, 0x61, 0x1B, 0x00 };
	const unsigned char INSTRUMENT_BASSTRLG[11] PROGMEM = { 0x00, 0xC1, 0x4F, 0xB1, 0x53, 0x06, 0xE0, 0x00, 0x12, 0x74, 0x33 };
	const unsigned char INSTRUMENT_BELLONG[11]  PROGMEM = { 0x00, 0x64, 0xDB, 0xFF, 0x01, 0x04, 0x3E, 0xC0, 0xF3, 0x62, 0x00 };
	const unsigned char INSTRUMENT_BELLS[11]    PROGMEM = { 0x00, 0x07, 0x4F, 0xF2, 0x60, 0x08, 0x12, 0x00, 0xF2, 0x72, 0x00 };
	const unsigned char INSTRUMENT_BELSHORT[11] PROGMEM = { 0x00, 0x64, 0xDB, 0xFF, 0x01, 0x04, 0x3E, 0xC0, 0xF5, 0xF3, 0x00 };
	const unsigned char INSTRUMENT_BNCEBASS[11] PROGMEM = { 0x00, 0x20, 0x4B, 0x7B, 0x04, 0x0E, 0x21, 0x00, 0xF5, 0x72, 0x01 };
	const unsigned char INSTRUMENT_BRASS1[11]   PROGMEM = { 0x00, 0x21, 0x16, 0x71, 0xAE, 0x0E, 0x21, 0x00, 0x81, 0x9E, 0x00 };
	const unsigned char INSTRUMENT_CBASSOON[11] PROGMEM = { 0x00, 0x30, 0xC5, 0x52, 0x11, 0x00, 0x31, 0x80, 0x31, 0x2E, 0x00 };
	const unsigned char INSTRUMENT_CELESTA[11]  PROGMEM = { 0x00, 0x33, 0x87, 0x01, 0x10, 0x08, 0x14, 0x80, 0x7D, 0x33, 0x00 };
	const unsigned char INSTRUMENT_CLAR1[11]    PROGMEM = { 0x00, 0x32, 0x16, 0x73, 0x24, 0x0E, 0x21, 0x80, 0x75, 0x57, 0x00 };
	const unsigned char INSTRUMENT_CLAR2[11]    PROGMEM = { 0x00, 0x31, 0x1C, 0x41, 0x1B, 0x0C, 0x60, 0x80, 0x42, 0x3B, 0x00 };
	const unsigned char INSTRUMENT_CLARINET[11] PROGMEM = { 0x00, 0x32, 0x9A, 0x51, 0x1B, 0x0C, 0x61, 0x82, 0xA2, 0x3B, 0x00 };
	const unsigned char INSTRUMENT_CLAVECIN[11] PROGMEM = { 0x00, 0x11, 0x0D, 0xF2, 0x01, 0x0A, 0x15, 0x0D, 0xF2, 0xB1, 0x00 };
	const unsigned char INSTRUMENT_CROMORNE[11] PROGMEM = { 0x00, 0x00, 0x02, 0xF0, 0xFF, 0x06, 0x11, 0x80, 0xF0, 0xFF, 0x00 };
	const unsigned char INSTRUMENT_ELCLAV1[11]  PROGMEM = { 0x00, 0x05, 0x8A, 0xF0, 0x7B, 0x08, 0x01, 0x80, 0xF4, 0x7B, 0x00 };
	const unsigned char INSTRUMENT_ELCLAV2[11]  PROGMEM = { 0x00, 0x01, 0x49, 0xF1, 0x53, 0x06, 0x11, 0x00, 0xF1, 0x74, 0x21 };
	const unsigned char INSTRUMENT_ELECFL[11]   PROGMEM = { 0x00, 0xE0, 0x6D, 0x57, 0x04, 0x0E, 0x61, 0x00, 0x67, 0x7D, 0x01 };
	const unsigned char INSTRUMENT_ELECVIBE[11] PROGMEM = { 0x00, 0x13, 0x97, 0x9A, 0x12, 0x0E, 0x91, 0x80, 0x9B, 0x11, 0x02 };
	const unsigned char INSTRUMENT_ELGUIT1[11]  PROGMEM = { 0x00, 0xF1, 0x01, 0x97, 0x17, 0x08, 0x21, 0x0D, 0xF1, 0x18, 0x00 };
	const unsigned char INSTRUMENT_ELGUIT2[11]  PROGMEM = { 0x00, 0x13, 0x96, 0xFF, 0x21, 0x0A, 0x11, 0x80, 0xFF, 0x03, 0x00 };
	const unsigned char INSTRUMENT_ELGUIT3[11]  PROGMEM = { 0x00, 0x07, 0x8F, 0x82, 0x7D, 0x0C, 0x14, 0x80, 0x82, 0x7D, 0x00 };
	const unsigned char INSTRUMENT_ELGUIT4[11]  PROGMEM = { 0x00, 0x05, 0x8F, 0xDA, 0x15, 0x0A, 0x01, 0x80, 0xF9, 0x14, 0x20 };
	const unsigned char INSTRUMENT_ELORGAN1[11] PROGMEM = { 0x00, 0xB2, 0xCD, 0x91, 0x2A, 0x09, 0xB1, 0x80, 0x91, 0x2A, 0x12 };
	const unsigned char INSTRUMENT_ELPIANO1[11] PROGMEM = { 0x00, 0x01, 0x4F, 0xF1, 0x50, 0x06, 0x01, 0x04, 0xD2, 0x7C, 0x00 };
	const unsigned char INSTRUMENT_ELPIANO2[11] PROGMEM = { 0x00, 0x02, 0x22, 0xF2, 0x13, 0x0E, 0x02, 0x00, 0xF5, 0x43, 0x00 };
	const unsigned char INSTRUMENT_EPIANO1A[11] PROGMEM = { 0x00, 0x81, 0x63, 0xF3, 0x58, 0x00, 0x01, 0x80, 0xF2, 0x58, 0x00 };
	const unsigned char INSTRUMENT_EPIANO1B[11] PROGMEM = { 0x00, 0x07, 0x1F, 0xF5, 0xFA, 0x0E, 0x01, 0x57, 0xF5, 0xFA, 0x00 };
	const unsigned char INSTRUMENT_FLUTE[11]    PROGMEM = { 0x00, 0x21, 0x83, 0x74, 0x17, 0x07, 0xA2, 0x8D, 0x65, 0x17, 0x00 };
	const unsigned char INSTRUMENT_FLUTE1[11]   PROGMEM = { 0x00, 0xA1, 0x27, 0x74, 0x8F, 0x02, 0xA1, 0x80, 0x65, 0x2A, 0x00 };
	const unsigned char INSTRUMENT_FLUTE2[11]   PROGMEM = { 0x00, 0xE0, 0xEC, 0x6E, 0x8F, 0x0E, 0x61, 0x00, 0x65, 0x2A, 0x00 };
	const unsigned char INSTRUMENT_FRHORN1[11]  PROGMEM = { 0x00, 0x21, 0x9F, 0x53, 0x5A, 0x0C, 0x21, 0x80, 0xAA, 0x1A, 0x00 };
	const unsigned char INSTRUMENT_FRHORN2[11]  PROGMEM = { 0x00, 0x20, 0x8E, 0xA5, 0x8F, 0x06, 0x21, 0x00, 0x36, 0x3D, 0x02 };
	const unsigned char INSTRUMENT_FSTRP1[11]   PROGMEM = { 0x00, 0xF0, 0x18, 0x55, 0xEF, 0x00, 0xE0, 0x80, 0x87, 0x1E, 0x32 };
	const unsigned char INSTRUMENT_FSTRP2[11]   PROGMEM = { 0x00, 0x70, 0x16, 0x55, 0x2F, 0x0C, 0xE0, 0x80, 0x87, 0x1E, 0x32 };
	const unsigned char INSTRUMENT_FUZGUIT1[11] PROGMEM = { 0x00, 0xF1, 0x00, 0x97, 0x13, 0x0A, 0x25, 0x0D, 0xF1, 0x18, 0x10 };
	const unsigned char INSTRUMENT_FUZGUIT2[11] PROGMEM = { 0x00, 0x31, 0x48, 0xF1, 0x53, 0x06, 0x32, 0x00, 0xF2, 0x27, 0x20 };
	const unsigned char INSTRUMENT_GUITAR1[11]  PROGMEM = { 0x00, 0x01, 0x11, 0xF2, 0x1F, 0x0A, 0x01, 0x00, 0xF5, 0x88, 0x00 };
	const unsigned char INSTRUMENT_HARP1[11]    PROGMEM = { 0x00, 0x02, 0x29, 0xF5, 0x75, 0x00, 0x01, 0x83, 0xF2, 0xF3, 0x00 };
	const unsigned char INSTRUMENT_HARP2[11]    PROGMEM = { 0x00, 0x02, 0x99, 0xF5, 0x55, 0x00, 0x01, 0x80, 0xF6, 0x53, 0x00 };
	const unsigned char INSTRUMENT_HARP3[11]    PROGMEM = { 0x00, 0x02, 0x57, 0xF5, 0x56, 0x00, 0x01, 0x80, 0xF6, 0x54, 0x00 };
	const unsigned char INSTRUMENT_HARPE1[11]   PROGMEM = { 0x00, 0x02, 0x29, 0xF5, 0x75, 0x00, 0x01, 0x03, 0xF2, 0xF3, 0x00 };
	const unsigned char INSTRUMENT_HARPSI1[11]  PROGMEM = { 0x00, 0x32, 0x87, 0xA1, 0x10, 0x08, 0x16, 0x80, 0x7D, 0x33, 0x00 };
	const unsigned char INSTRUMENT_HARPSI2[11]  PROGMEM = { 0x00, 0x33, 0x87, 0xA1, 0x10, 0x06, 0x15, 0x80, 0x7D, 0x43, 0x00 };
	const unsigned char INSTRUMENT_HARPSI3[11]  PROGMEM = { 0x00, 0x35, 0x84, 0xA8, 0x10, 0x08, 0x18, 0x80, 0x7D, 0x33, 0x00 };
	const unsigned char INSTRUMENT_HARPSI4[11]  PROGMEM = { 0x00, 0x11, 0x0D, 0xF2, 0x01, 0x0A, 0x15, 0x0D, 0xF2, 0xB1, 0x00 };
	const unsigned char INSTRUMENT_HARPSI5[11]  PROGMEM = { 0x00, 0x36, 0x87, 0x8A, 0x00, 0x08, 0x1A, 0x80, 0x7F, 0x33, 0x00 };
	const unsigned char INSTRUMENT_HELICPTR[11] PROGMEM = { 0x00, 0xF0, 0x00, 0x1E, 0x11, 0x08, 0xE2, 0xC0, 0x11, 0x11, 0x11 };
	const unsigned char INSTRUMENT_JAVAICAN[11] PROGMEM = { 0x00, 0x87, 0x4D, 0x78, 0x42, 0x0A, 0x94, 0x00, 0x85, 0x54, 0x00 };
	const unsigned char INSTRUMENT_JAZZGUIT[11] PROGMEM = { 0x00, 0x03, 0x5E, 0x85, 0x51, 0x0E, 0x11, 0x00, 0xD2, 0x71, 0x01 };
	const unsigned char INSTRUMENT_JEWSHARP[11] PROGMEM = { 0x00, 0x00, 0x50, 0xF2, 0x70, 0x0E, 0x13, 0x00, 0xF2, 0x72, 0x00 };
	const unsigned char INSTRUMENT_KEYBRD1[11]  PROGMEM = { 0x00, 0x00, 0x02, 0xF0, 0xFA, 0x06, 0x11, 0x80, 0xF2, 0xFA, 0x11 };
	const unsigned char INSTRUMENT_KEYBRD2[11]  PROGMEM = { 0x00, 0x01, 0x8F, 0xF2, 0xBD, 0x08, 0x14, 0x80, 0x82, 0xBD, 0x00 };
	const unsigned char INSTRUMENT_KEYBRD3[11]  PROGMEM = { 0x00, 0x01, 0x00, 0xF0, 0xF0, 0x00, 0xE4, 0x03, 0xF3, 0x36, 0x00 };
	const unsigned char INSTRUMENT_LOGDRUM1[11] PROGMEM = { 0x00, 0x32, 0x44, 0xF8, 0xFF, 0x0E, 0x11, 0x00, 0xF5, 0x7F, 0x00 };
	const unsigned char INSTRUMENT_MARIMBA1[11] PROGMEM = { 0x00, 0x05, 0x4E, 0xDA, 0x25, 0x0A, 0x01, 0x00, 0xF9, 0x15, 0x00 };
	const unsigned char INSTRUMENT_MARIMBA2[11] PROGMEM = { 0x00, 0x85, 0x4E, 0xDA, 0x15, 0x0A, 0x81, 0x80, 0xF9, 0x13, 0x00 };
	const unsigned char INSTRUMENT_MDRNPHON[11] PROGMEM = { 0x00, 0x30, 0x00, 0xFE, 0x11, 0x08, 0xAE, 0xC0, 0xF1, 0x19, 0x11 };
	const unsigned char INSTRUMENT_MOOGSYNT[11] PROGMEM = { 0x00, 0x20, 0x90, 0xF5, 0x9E, 0x0C, 0x11, 0x00, 0xF4, 0x5B, 0x32 };
	const unsigned char INSTRUMENT_NOISE1[11]   PROGMEM = { 0x00, 0x0E, 0x40, 0xD1, 0x53, 0x0E, 0x0E, 0x00, 0xF2, 0x7F, 0x30 };
	const unsigned char INSTRUMENT_OBOE1[11]    PROGMEM = { 0x00, 0xB1, 0xC5, 0x6E, 0x17, 0x02, 0x22, 0x05, 0x8B, 0x0E, 0x00 };
	const unsigned char INSTRUMENT_ORGAN1[11]   PROGMEM = { 0x00, 0x65, 0xD2, 0x81, 0x03, 0x02, 0x71, 0x80, 0xF1, 0x05, 0x00 };
	const unsigned char INSTRUMENT_ORGAN2[11]   PROGMEM = { 0x00, 0x24, 0x80, 0xFF, 0x0F, 0x01, 0x21, 0x80, 0xFF, 0x0F, 0x00 };
	const unsigned char INSTRUMENT_ORGAN3[11]   PROGMEM = { 0x00, 0x03, 0x5B, 0xF0, 0x1F, 0x0A, 0x01, 0x80, 0xF0, 0x1F, 0x00 };
	const unsigned char INSTRUMENT_ORGAN3A[11]  PROGMEM = { 0x00, 0x03, 0x5B, 0xF0, 0x1F, 0x0A, 0x01, 0x8D, 0xF0, 0x13, 0x00 };
	const unsigned char INSTRUMENT_ORGAN3B[11]  PROGMEM = { 0x00, 0x03, 0x5B, 0xF0, 0x1F, 0x0A, 0x01, 0x92, 0xF0, 0x12, 0x00 };
	const unsigned char INSTRUMENT_ORGNPERC[11] PROGMEM = { 0x00, 0x0C, 0x00, 0xF8, 0xB5, 0x01, 0x00, 0x00, 0xD6, 0x4F, 0x00 };
	const unsigned char INSTRUMENT_PHONE1[11]   PROGMEM = { 0x00, 0x17, 0x4F, 0xF2, 0x61, 0x08, 0x12, 0x08, 0xF1, 0xB2, 0x00 };
	const unsigned char INSTRUMENT_PHONE2[11]   PROGMEM = { 0x00, 0x17, 0x4F, 0xF2, 0x61, 0x08, 0x12, 0x0A, 0xF1, 0xB4, 0x00 };
	const unsigned char INSTRUMENT_PIAN1A[11]   PROGMEM = { 0x00, 0x81, 0x63, 0xF3, 0x58, 0x00, 0x01, 0x80, 0xF2, 0x58, 0x00 };
	const unsigned char INSTRUMENT_PIAN1B[11]   PROGMEM = { 0x00, 0x07, 0x1F, 0xF5, 0xFA, 0x0E, 0x01, 0x26, 0xF5, 0xFA, 0x00 };
	const unsigned char INSTRUMENT_PIAN1C[11]   PROGMEM = { 0x00, 0x07, 0x1F, 0xF5, 0xFA, 0x0E, 0x01, 0x57, 0xF5, 0xFA, 0x00 };
	const unsigned char INSTRUMENT_PIANO[11]    PROGMEM = { 0x00, 0x03, 0x4F, 0xF1, 0x53, 0x06, 0x17, 0x00, 0xF2, 0x74, 0x00 };
	const unsigned char INSTRUMENT_PIANO1[11]   PROGMEM = { 0x00, 0x01, 0x4F, 0xF1, 0x53, 0x06, 0x11, 0x00, 0xD2, 0x74, 0x00 };
	const unsigned char INSTRUMENT_PIANO2[11]   PROGMEM = { 0x00, 0x41, 0x9D, 0xF2, 0x51, 0x06, 0x13, 0x00, 0xF2, 0xF1, 0x00 };
	const unsigned char INSTRUMENT_PIANO3[11]   PROGMEM = { 0x00, 0x01, 0x4F, 0xF1, 0x50, 0x06, 0x01, 0x04, 0xD2, 0x7C, 0x00 };
	const unsigned char INSTRUMENT_PIANO4[11]   PROGMEM = { 0x00, 0x01, 0x4D, 0xF1, 0x60, 0x08, 0x11, 0x00, 0xD2, 0x7B, 0x00 };
	const unsigned char INSTRUMENT_PIANOBEL[11] PROGMEM = { 0x00, 0x03, 0x4F, 0xF1, 0x53, 0x06, 0x17, 0x03, 0xF2, 0x74, 0x00 };
	const unsigned char INSTRUMENT_PIANOF[11]   PROGMEM = { 0x00, 0x01, 0xCF, 0xF1, 0x53, 0x02, 0x12, 0x00, 0xF2, 0x83, 0x00 };
	const unsigned char INSTRUMENT_POPBASS1[11] PROGMEM = { 0x00, 0x10, 0x00, 0x75, 0x93, 0x00, 0x01, 0x00, 0xF5, 0x82, 0x11 };
	const unsigned char INSTRUMENT_SAX1[11]     PROGMEM = { 0x00, 0x01, 0x4F, 0x71, 0x53, 0x0A, 0x12, 0x00, 0x52, 0x7C, 0x00 };
	const unsigned char INSTRUMENT_SCRATCH[11]  PROGMEM = { 0x00, 0x07, 0x00, 0xF0, 0xF0, 0x0E, 0x00, 0x00, 0x5C, 0xDC, 0x00 };
	const unsigned char INSTRUMENT_SCRATCH4[11] PROGMEM = { 0x00, 0x07, 0x00, 0xF0, 0xF0, 0x0E, 0x00, 0x00, 0x5C, 0xDC, 0x00 };
	const unsigned char INSTRUMENT_SDRUM2[11]   PROGMEM = { 0x00, 0x06, 0x00, 0xF0, 0xF0, 0x0E, 0x00, 0x00, 0xF6, 0xB4, 0x00 };
	const unsigned char INSTRUMENT_SHRTVIBE[11] PROGMEM = { 0x00, 0xE4, 0x0E, 0xFF, 0x3F, 0x00, 0xC0, 0x00, 0xF3, 0x07, 0x01 };
	const unsigned char INSTRUMENT_SITAR1[11]   PROGMEM = { 0x00, 0x01, 0x40, 0xF1, 0x53, 0x00, 0x08, 0x40, 0xF1, 0x53, 0x00 };
	const unsigned char INSTRUMENT_SITAR2[11]   PROGMEM = { 0x00, 0x01, 0x40, 0xF1, 0x53, 0x00, 0x08, 0x40, 0xF1, 0x53, 0x10 };
	const unsigned char INSTRUMENT_SNAKEFL[11]  PROGMEM = { 0x00, 0x61, 0x0C, 0x81, 0x03, 0x08, 0x71, 0x80, 0x61, 0x0C, 0x00 };
	const unsigned char INSTRUMENT_SNRSUST[11]  PROGMEM = { 0x00, 0x06, 0x00, 0xF0, 0xF0, 0x0E, 0xC4, 0x03, 0xC4, 0x34, 0x00 };
	const unsigned char INSTRUMENT_SOLOVLN[11]  PROGMEM = { 0x00, 0x70, 0x1C, 0x51, 0x03, 0x0E, 0x20, 0x00, 0x54, 0x67, 0x22 };
	const unsigned char INSTRUMENT_STEELGT1[11] PROGMEM = { 0x00, 0x01, 0x46, 0xF1, 0x83, 0x06, 0x61, 0x03, 0x31, 0x86, 0x00 };
	const unsigned char INSTRUMENT_STEELGT2[11] PROGMEM = { 0x00, 0x01, 0x47, 0xF1, 0x83, 0x06, 0x61, 0x03, 0x91, 0x86, 0x00 };
	const unsigned char INSTRUMENT_STRINGS1[11] PROGMEM = { 0x00, 0xB1, 0x8B, 0x71, 0x11, 0x06, 0x61, 0x40, 0x42, 0x15, 0x10 };
	const unsigned char INSTRUMENT_STRNLONG[11] PROGMEM = { 0x00, 0xE1, 0x4F, 0xB1, 0xD3, 0x06, 0x21, 0x00, 0x12, 0x74, 0x13 };
	const unsigned char INSTRUMENT_SYN1[11]     PROGMEM = { 0x00, 0x55, 0x97, 0x2A, 0x02, 0x00, 0x12, 0x80, 0x42, 0xF3, 0x00 };
	const unsigned char INSTRUMENT_SYN2[11]     PROGMEM = { 0x00, 0x13, 0x97, 0x9A, 0x12, 0x0E, 0x11, 0x80, 0x9B, 0x14, 0x00 };
	const unsigned char INSTRUMENT_SYN3[11]     PROGMEM = { 0x00, 0x11, 0x8A, 0xF1, 0x11, 0x06, 0x01, 0x40, 0xF1, 0xB3, 0x00 };
	const unsigned char INSTRUMENT_SYN4[11]     PROGMEM = { 0x00, 0x21, 0x0D, 0xE9, 0x3A, 0x0A, 0x22, 0x80, 0x65, 0x6C, 0x00 };
	const unsigned char INSTRUMENT_SYN5[11]     PROGMEM = { 0x00, 0x01, 0x4F, 0x71, 0x53, 0x06, 0x19, 0x00, 0x52, 0x7C, 0x00 };
	const unsigned char INSTRUMENT_SYN6[11]     PROGMEM = { 0x00, 0x24, 0x0F, 0x41, 0x7E, 0x0A, 0x21, 0x00, 0xF1, 0x5E, 0x00 };
	const unsigned char INSTRUMENT_SYN9[11]     PROGMEM = { 0x00, 0x07, 0x87, 0xF0, 0x05, 0x04, 0x01, 0x80, 0xF0, 0x05, 0x00 };
	const unsigned char INSTRUMENT_SYNBAL1[11]  PROGMEM = { 0x00, 0x26, 0x03, 0xE0, 0xF0, 0x08, 0x1E, 0x00, 0xFF, 0x31, 0x00 };
	const unsigned char INSTRUMENT_SYNBAL2[11]  PROGMEM = { 0x00, 0x28, 0x03, 0xE0, 0xF0, 0x04, 0x13, 0x00, 0xE8, 0x11, 0x00 };
	const unsigned char INSTRUMENT_SYNBASS1[11] PROGMEM = { 0x00, 0x30, 0x88, 0xD5, 0x19, 0x0C, 0x71, 0x80, 0x61, 0x1B, 0x00 };
	const unsigned char INSTRUMENT_SYNBASS2[11] PROGMEM = { 0x00, 0x81, 0x86, 0x65, 0x01, 0x0C, 0x11, 0x00, 0x32, 0x74, 0x00 };
	const unsigned char INSTRUMENT_SYNBASS4[11] PROGMEM = { 0x00, 0x81, 0x83, 0x65, 0x05, 0x0A, 0x51, 0x00, 0x32, 0x74, 0x00 };
	const unsigned char INSTRUMENT_SYNSNR1[11]  PROGMEM = { 0x00, 0x06, 0x00, 0xF0, 0xF0, 0x0E, 0x00, 0x00, 0xF8, 0xB6, 0x00 };
	const unsigned char INSTRUMENT_SYNSNR2[11]  PROGMEM = { 0x00, 0x06, 0x00, 0xF0, 0xF0, 0x0E, 0x00, 0x00, 0xF6, 0xB4, 0x00 };
	const unsigned char INSTRUMENT_TINCAN1[11]  PROGMEM = { 0x00, 0x8F, 0x81, 0xEF, 0x01, 0x04, 0x01, 0x00, 0x98, 0xF1, 0x00 };
	const unsigned char INSTRUMENT_TRAINBEL[11] PROGMEM = { 0x00, 0x17, 0x4F, 0xF2, 0x61, 0x08, 0x12, 0x08, 0xF2, 0x74, 0x00 };
	const unsigned char INSTRUMENT_TRIANGLE[11] PROGMEM = { 0x00, 0x26, 0x03, 0xE0, 0xF0, 0x08, 0x1E, 0x00, 0xFF, 0x31, 0x00 };
	const unsigned char INSTRUMENT_TROMB1[11]   PROGMEM = { 0x00, 0xB1, 0x1C, 0x41, 0x1F, 0x0E, 0x61, 0x80, 0x92, 0x3B, 0x00 };
	const unsigned char INSTRUMENT_TROMB2[11]   PROGMEM = { 0x00, 0x21, 0x1C, 0x53, 0x1D, 0x0C, 0x61, 0x80, 0x52, 0x3B, 0x00 };
	const unsigned char INSTRUMENT_TRUMPET1[11] PROGMEM = { 0x00, 0x31, 0x1C, 0x41, 0x0B, 0x0E, 0x61, 0x80, 0x92, 0x3B, 0x00 };
	const unsigned char INSTRUMENT_TRUMPET2[11] PROGMEM = { 0x00, 0x31, 0x1C, 0x23, 0x1D, 0x0C, 0x61, 0x80, 0x52, 0x3B, 0x00 };
	const unsigned char INSTRUMENT_TRUMPET3[11] PROGMEM = { 0x00, 0x31, 0x1C, 0x41, 0x01, 0x0E, 0x61, 0x80, 0x92, 0x3B, 0x00 };
	const unsigned char INSTRUMENT_TRUMPET4[11] PROGMEM = { 0x00, 0x31, 0x1C, 0x41, 0x0B, 0x0C, 0x61, 0x80, 0x92, 0x3B, 0x00 };
	const unsigned char INSTRUMENT_TUBA1[11]    PROGMEM = { 0x00, 0x21, 0x19, 0x43, 0x8C, 0x0C, 0x21, 0x80, 0x85, 0x2F, 0x00 };
	const unsigned char INSTRUMENT_VIBRA1[11]   PROGMEM = { 0x00, 0x84, 0x53, 0xF5, 0x33, 0x06, 0xA0, 0x80, 0xFD, 0x25, 0x00 };
	const unsigned char INSTRUMENT_VIBRA2[11]   PROGMEM = { 0x00, 0x06, 0x73, 0xF6, 0x54, 0x00, 0x81, 0x03, 0xF2, 0xB3, 0x00 };
	const unsigned char INSTRUMENT_VIBRA3[11]   PROGMEM = { 0x00, 0x93, 0x97, 0xAA, 0x12, 0x0E, 0x91, 0x80, 0xAC, 0x21, 0x02 };
	const unsigned char INSTRUMENT_VIOLIN1[11]  PROGMEM = { 0x00, 0x31, 0x1C, 0x51, 0x03, 0x0E, 0x61, 0x80, 0x54, 0x67, 0x00 };
	const unsigned char INSTRUMENT_VIOLIN2[11]  PROGMEM = { 0x00, 0xE1, 0x88, 0x62, 0x29, 0x0C, 0x22, 0x80, 0x53, 0x2C, 0x00 };
	const unsigned char INSTRUMENT_VIOLIN3[11]  PROGMEM = { 0x00, 0xE1, 0x88, 0x64, 0x29, 0x06, 0x22, 0x83, 0x53, 0x2C, 0x00 };
	const unsigned char INSTRUMENT_VLNPIZZ1[11] PROGMEM = { 0x00, 0x31, 0x9C, 0xF1, 0xF9, 0x0E, 0x31, 0x80, 0xF7, 0xE6, 0x00 };
	const unsigned char INSTRUMENT_WAVE[11]     PROGMEM = { 0x00, 0x00, 0x02, 0x00, 0xF0, 0x0E, 0x14, 0x80, 0x1B, 0xA2, 0x00 };
	const unsigned char INSTRUMENT_XYLO1[11]    PROGMEM = { 0x00, 0x11, 0x2D, 0xC8, 0x2F, 0x0C, 0x31, 0x00, 0xF5, 0xF5, 0x00 };
	const unsigned char INSTRUMENT_XYLO3[11]    PROGMEM = { 0x00, 0x06, 0x00, 0xFF, 0xF0, 0x0E, 0xC4, 0x00, 0xF8, 0xB5, 0x00 };

	// Drum instruments for percussive mode.
	// DRUM_BASS:
	const unsigned char INSTRUMENT_BDRUM1[11]   PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0B, 0xA8, 0x4C, 0x00 };
	const unsigned char INSTRUMENT_BDRUM2[11]   PROGMEM = { 0x00, 0x00, 0x0B, 0xA8, 0x4C, 0x00, 0x00, 0x00, 0xA8, 0x4C, 0x00 };
	const unsigned char INSTRUMENT_XYLO2[11]    PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2E, 0x00, 0xFF, 0x0F, 0x00 };

	// DRUM_SNARE:
	const unsigned char INSTRUMENT_MLTRDRUM[11] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0xC8, 0xB6, 0x10 };
	const unsigned char INSTRUMENT_RKSNARE1[11] PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0xC7, 0xB4, 0x00 };
	const unsigned char INSTRUMENT_SNARE1[11]   PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0C, 0x00, 0xF8, 0xB5, 0x00 };

	// DRUM_TOM:
	const unsigned char INSTRUMENT_TOM1[11]     PROGMEM = { 0x00, 0x04, 0x00, 0xF7, 0xB5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	const unsigned char INSTRUMENT_TOM2[11]     PROGMEM = { 0x00, 0x02, 0x00, 0xC8, 0x97, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

	// DRUM_CYMBAL:
	const unsigned char INSTRUMENT_CYMBAL1[11]  PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x00, 0xF5, 0xB5, 0x00 };
	const unsigned char INSTRUMENT_LASER[11]    PROGMEM = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xE6, 0x00, 0x25, 0xB5, 0x00 };

	// DRUM_HI_HAT:
	const unsigned char INSTRUMENT_HIHAT1[11]   PROGMEM = { 0x00, 0x01, 0x00, 0xF7, 0xB5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
	const unsigned char INSTRUMENT_HIHAT2[11]   PROGMEM = { 0x00, 0x01, 0x03, 0xDA, 0x18, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

#endif
