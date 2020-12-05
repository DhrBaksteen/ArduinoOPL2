#include <OPL2.h>
#include <wiringPi.h>
#include <zlib.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "opl2play.h"

#define CHUNK 16384


FILE *oplFile;
OPL2 opl2;
int repeat = FALSE;
int silent = FALSE;


int main(int argc, char **argv) {
	if (argc < 2) {
		showHelp();
		return 0;
	}

	opl2.begin();

	for (int i = 1; i < argc; i ++) {
		if (strcmp(argv[i], "-r") == 0 || strcmp(argv[i], "--repeat") == 0) {
			repeat = TRUE;
		} else if (strcmp(argv[i], "-s") == 0 || strcmp(argv[i], "--silent") == 0) {
			silent = TRUE;
		} else if (strcmp(argv[i], "-k") == 0 || strcmp(argv[i], "--kill") == 0) {
			opl2.reset();
			return 0;
		} else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
			showHelp();
			return 0;
		} else if (strcmp(argv[i], "-c") == 0 || strcmp(argv[i], "--connect") == 0) {
			showConnections();
			return 0;
		}
	}

	printHeader();

	for (int i = 1; i < argc; i ++) {
		if (argv[i][0] != '-') {
			char *ext = strrchr(argv[i], '.');
			for (int i = 0; ext[i]; i ++) {
				ext[i] = tolower(ext[i]);
			}

			oplFile = fopen(argv[i], "rb");
			if (oplFile == NULL) return fileError();
			if (!silent) printf("Playing %s\n", argv[i]);

			opl2.reset();
			if (strcmp(ext, ".dro") == 0) {
				DRO dro = loadDro(oplFile);
				if (dro.file == NULL) return fileError();
				playDroMusic(dro);
			} else if (strcmp(ext, ".imf") == 0) {
				int speed = 560;
				if (i < argc - 1 && atoi(argv[i + 1])) speed = atoi(argv[++i]);
				IMF imf = loadImf(oplFile, speed);
				if (imf.file == NULL) return fileError();
				playImfMusic(imf);
			} else if (strcmp(ext, ".vgm") == 0) {
				VGM vgm = loadVgm(oplFile);
				if (vgm.file == NULL) return fileError();
				playVgmMusic(vgm);
			} else if ((strcmp(ext, ".vgz") == 0) || (strcmp(ext, ".gz") == 0)) {
				FILE *tempFile = fopen("./temp.vgm", "w");
				int result = inflate(oplFile, tempFile);
				fclose(tempFile);

				if (result == 0) {
					fclose(oplFile);
					oplFile = fopen("./temp.vgm", "rb");
					VGM vgm = loadVgm(oplFile);
					if (vgm.file == NULL) return fileError();
					playVgmMusic(vgm);
				} else {
					printf("Decompression error %d\n", result);
				}
			}

			fclose(oplFile);
		}

		if (i == argc -1 && repeat) {
			i = 0;
		}
	}

	opl2.reset();
	printf("\n");
	return 0;
}


void playDroMusic(DRO dro) {
	// Load file data into memory...
	unsigned long dataOffset = ftell(dro.file);
	fseek(dro.file, 0, SEEK_END);
	unsigned long dataSize = ftell(dro.file) - dataOffset;
	fseek(dro.file, dataOffset, SEEK_SET);
	unsigned char fileData[dataSize];
	for (unsigned long i = 0; i < dataSize; i ++) {
		fileData[i] = fileRead(dro.file);
	}

	// Play!
	unsigned long offset = 0;
	unsigned char registerCode;
	unsigned char reg;
	unsigned char value;

	while(offset < dro.songLength * 2) {
		registerCode = fileData[offset ++];
		value        = fileData[offset ++];

		if (registerCode == dro.codeShortDelay) {
			delay(value + 1);
		} else if (registerCode == dro.codeLongDelay) {
			delay((value + 1) << 8);
		} else if (registerCode < 128) {
			reg = dro.registerMap[registerCode];
			opl2.write(reg, value);
		}
	}
}


DRO loadDro(FILE *droFile) {
	DRO dro;
	dro.file = NULL;

	if (fseek(droFile, 12, SEEK_SET) == 0) {
		dro.songLength = readDWord(droFile);

		if (fseek(droFile, 23, SEEK_SET) == 0) {
			dro.codeShortDelay    = fileRead(droFile);
			dro.codeLongDelay     = fileRead(droFile);
			dro.registerMapLength = fileRead(droFile);

			for (int i = 0; i < dro.registerMapLength; i ++) {
				dro.registerMap[i] = fileRead(droFile);
			}

			dro.file = droFile;
		}
	}

	return dro;
}


void playImfMusic(IMF imf) {
	// Load file data into memory...
	unsigned long dataOffset = ftell(imf.file);
	fseek(imf.file, 0, SEEK_END);
	unsigned long dataSize = ftell(imf.file) - dataOffset;
	fseek(imf.file, dataOffset, SEEK_SET);
	unsigned char fileData[dataSize];
	for (unsigned long i = 0; i < dataSize; i ++) {
		fileData[i] = fileRead(imf.file);
	}

	// Play!
	unsigned long offset = 0;
	unsigned char reg;
	unsigned char value;
	unsigned int delayCycles;

	while (offset < imf.songLength) {
		reg   = fileData[offset ++];
		value = fileData[offset ++];;
		delayCycles  = fileData[offset ++];
		delayCycles += fileData[offset ++] << 8;

		opl2.write(reg, value);
		delay((int)(delayCycles * (1000.0 / imf.speed)));
	}
}


IMF loadImf(FILE *imfFile, int speed) {
	IMF imf;

	imf.file  = imfFile;
	imf.speed = speed;
	imf.songLength = readWord(imfFile);

	// If song length is 0 we're probably dealing with a type 0 file. In that
	// case we want to play the file until EOF, so set songLength to max.
	if (imf.songLength == 0) {
		imf.songLength = 65535;
		fseek(imfFile, 0, SEEK_SET);
	}

	return imf;
}


void playVgmMusic(VGM vgm) {
	// Load file data into memory...
	unsigned long dataOffset = ftell(vgm.file);
	fseek(vgm.file, 0, SEEK_END);
	unsigned long dataSize = ftell(vgm.file) - dataOffset;
	fseek(vgm.file, dataOffset, SEEK_SET);
	unsigned char fileData[dataSize];
	for (unsigned long i = 0; i < dataSize; i ++) {
		fileData[i] = fileRead(vgm.file);
	}

	// Play!
	unsigned long offset = 0;
	unsigned char command;
	unsigned char reg;
	unsigned char data;
	unsigned int sampleDelay;

	while(offset < dataSize) {
		command = fileData[offset ++];

		switch (command) {
			// YM3812 command.
			case 0x5A: {
				reg = fileData[offset ++];
				data = fileData[offset ++];
				opl2.write(reg, data);
				break;
			}

			// Sample delay long.
			case 0x61: {
				sampleDelay  = fileData[offset ++];
				sampleDelay += fileData[offset ++] << 8;
				delay(sampleDelay * 0.023);
				break;
			}

			// 60 Hz delay.
			case 0x62: {
				delay(17);
				break;
			}

			// 50 Hz delay.
			case 0x63: {
				delay(20);
				break;
			}

			// End of song data. Stop or loop.
			case 0x66: {
				if (repeat && vgm.loopOffset > 0) {
					offset = vgm.loopOffset;
				} else {
					offset = dataSize;
				}
				break;
			}

			// Sample dalay short.
			case 0x70 ... 0x7F: {
				sampleDelay = (command % 0x0F) + 1;
				delay(sampleDelay * 0.023);
				break;
			}

			// Unsupported sample
			case 0x51 ... 0x59:
			case 0x5B ... 0x5F: {
				offset += 2;
				break;
			}
		}

		// Do we need to loop?
		if (repeat && vgm.loopOffset >= 0 && vgm.loopLength > 0 && offset > vgm.loopLength) {
			offset = vgm.loopOffset;
		}
	}
}


VGM loadVgm(FILE *vgmFile) {
	VGM vgm;
	vgm.file = NULL;

	if (fileRead(vgmFile) == 0x56 && fileRead(vgmFile) == 0x67 && fileRead(vgmFile) == 0x6D) {
		fseek(vgmFile, 0x50, SEEK_SET);
		unsigned long clockSpeed = readDWord(vgmFile);

		if (clockSpeed > 0) {
			fseek(vgmFile, 0x18, SEEK_SET);
			vgm.songLength = readDWord(vgmFile);
			vgm.loopOffset = readDWord(vgmFile);
			vgm.loopLength = readDWord(vgmFile);

			// Calculate loop offset relative to song data offset.
			if (vgm.loopOffset > 0) {
				vgm.loopOffset -= 0xE4;
			}

			fseek(vgmFile, 0x100, SEEK_SET);
			vgm.file = vgmFile;
		} else {
			printf("No YM3812 sample data!\n");
		}
	}

	return vgm;
}


unsigned char fileRead(FILE *file) {
	unsigned char byte[1];
	if (fread(byte, sizeof(byte), 1, file)) {
		return byte[0];
	}
	return 0;
}


unsigned int readWord(FILE *file) {
	unsigned int value = fileRead(file);
	value += fileRead(file) << 8;
	return value;
}


unsigned int readDWord(FILE *file) {
	unsigned int value = fileRead(file);
	value += fileRead(file) << 8;
	value += fileRead(file) << 16;
	value += fileRead(file) << 24;
	return value;
}


int spiError() {
	printf("Cannot initialize SPI! Are you running this as root?\n\n");
	return 1;
}


int fileError() {
	printf("Please provide a .DRO, .IMF, .VGM or .VGZ file as the first parameter.\n\n");
	return 1;
}


/* Decompress from file source to file dest until stream ends or EOF.
   inf() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_DATA_ERROR if the deflate data is
   invalid or incomplete, Z_VERSION_ERROR if the version of zlib.h and
   the version of the library linked do not match, or Z_ERRNO if there
   is an error reading or writing the files. */
int inflate(FILE *source, FILE *dest) {
	int ret;
	unsigned int have;
	z_stream strm;
	unsigned char in[CHUNK];
	unsigned char out[CHUNK];

		/* allocate inflate state */
	strm.zalloc = Z_NULL;
	strm.zfree = Z_NULL;
	strm.opaque = Z_NULL;
	strm.avail_in = 0;
	strm.next_in = Z_NULL;
	ret = inflateInit2(&strm, 16 + MAX_WBITS);
	if (ret != Z_OK)
		return ret;


	/* decompress until deflate stream ends or end of file */
	do {
		strm.avail_in = fread(in, 1, CHUNK, source);
		if (ferror(source)) {
			(void)inflateEnd(&strm);
			return Z_ERRNO;
		}
		if (strm.avail_in == 0)
			break;
		strm.next_in = in;

		/* run inflate() on input until output buffer not full */
		do {
			strm.avail_out = CHUNK;
			strm.next_out = out;

			ret = inflate(&strm, Z_NO_FLUSH);
			//assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
			switch (ret) {
			case Z_NEED_DICT:
				ret = Z_DATA_ERROR;     /* and fall through */
			case Z_DATA_ERROR:
			case Z_MEM_ERROR:
				(void)inflateEnd(&strm);
				return ret;
			}

			have = CHUNK - strm.avail_out;
			if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
				(void)inflateEnd(&strm);
				return Z_ERRNO;
			}

		} while (strm.avail_out == 0);
		/* done when inflate() says it's done */
	} while (ret != Z_STREAM_END);

	/* clean up and return */
	(void)inflateEnd(&strm);
	return ret == Z_STREAM_END ? Z_OK : Z_DATA_ERROR;
}


void printHeader() {
	if (!silent) {
		printf("\033[2J\033[1;1H\033[0m");
		printf("\033[1;33m________ __________.____    ________             .__                \n");
		printf("\033[1;33m\\_____  \\\\______   \\    |   \\_____  \\     ______ |  | _____  ___.__.\n");
		printf("\033[1;32m /   |   \\|     ___/    |    /  ____/     \\____ \\|  | \\__  \\<   |  |\n");
		printf("\033[1;36m/    |    \\    |   |    |___/       \\     |  |_> >  |__/ __ \\\\___  |\n");
		printf("\033[1;34m\\_______  /____|   |_______ \\_______ \\ /\\ |   __/|____(____  / ____|\n");
		printf("\033[0;34m        \\/                 \\/       \\/ \\/ |__|             \\/\\/     \n");
		printf("\033[0mOPL2.play v1.1.1 for the OPL2 Audio Board\n");
		printf("By Maarten Janssen in 2017 - 2019\n");
		printf("Visit \033[1;34mhttp://github.com/DhrBaksteen/ArduinoOPL2\033[0m to learn more!\n\n");
	}
}


void showHelp() {
	printHeader();
	printf("This is a demo utility for the OPL2 Audio Board on the Raspberry Pi and other Pi\n");
	printf("variants that lets you play various audio formats for YM3512. OPL2Play supports\n");
	printf("the following file formats:\n");
	printf("    *.DRO        - Raw Adlib register captures from DosBox\n");
	printf("    *.IMF        - id Software music files\n");
	printf("    *.VGM, *.VGZ - Video Game Music files\n");
	printf("\n");
	printf("Usage: opl2play <file> [imf_speed] [<file_n> [imf_speed_n]]\n");
	printf("                [--help] [--kill] [--silent] [--repeat]\n");
	printf("\n");
	printf("file             The music file to play. Multiple files may be provided to play\n");
	printf("                 one after the other\n");
	printf("\n");
	printf("imf_speed        The playback speed in Hz for an IMF file. If this argument is\n");
	printf("                 omitted then the default speed of 560 Hz will be used.\n");
	printf("                 Common values are:\n");
	printf("                     280 - Duke Nukem II\n");
	printf("                     560 - Bio Menace, Commander Keen, Monster Bash\n");
	printf("                     700 - Blake Stone, Major Stryker, Wolfenstein 3D\n");
	printf("\n");
	printf("--help, -h       Show this help screen and exit.\n");
	printf("\n");
	printf("--connect, -c    Show how to connect the board to your Raspberry Pi and exit.\n");
	printf("\n");
	printf("--kill, -k       Stops all sound, reset the OPL2 and exit.\n");
	printf("\n");
	printf("--silent, -s     Hide all messages, except errors.\n");
	printf("\n");
	printf("--repeat, -r     Repeats when all songs have been played.\n");
	printf("\n");
}


void showConnections() {
	printHeader();
	printf("Connect the OPL2 Audio Board to your Raspberry or Orange Pi as follows:\n");
	printf("    +5v   - 2\n");
	printf("    GND   - 6\n");
	printf("    A0    - 16\n");
	printf("    Data  - 19\n");
	printf("    Latch - 12\n");
	printf("    Shift - 23\n");
	printf("    Reset - 18\n");
	printf("\n");
}
