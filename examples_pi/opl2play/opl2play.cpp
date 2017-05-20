#include <OPL2.h>
#include <wiringPi.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "opl2play.h"


FILE *oplFile;
OPL2 opl2;
int repeat = FALSE;
int silent = FALSE;


int main(int argc, char **argv) {
	if (argc < 2) {
		showHelp();
		return 0;
	}

	opl2.init();

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
				struct DRO dro = loadDro(oplFile);
				if (dro.file == NULL) return fileError();
				playDroMusic(dro);
			} else {
				int speed = 560;
				if (i < argc - 1 && atoi(argv[i + 1])) speed = atoi(argv[++i]);
				struct IMF imf = loadImf(oplFile, speed);
				if (imf.file == NULL) return fileError();
				playImfMusic(imf);
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


void playDroMusic(struct DRO dro) {
	unsigned char registerCode;
	unsigned char reg;
	unsigned char data;

	for (long i = 0; i < dro.songLength; i ++) {
		if (feof(dro.file) || ferror(dro.file)) break;
		registerCode = fileRead(dro.file);
		data = fileRead(dro.file);

		if (registerCode == dro.codeShortDelay) {
			delay(data + 1);
		} else if (registerCode == dro.codeLongDelay) {
			delay((data + 1) << 8);
		} else {
			reg = dro.registerMap[registerCode];
			opl2.write(reg, data);
		}
	}
}


struct DRO loadDro(FILE *droFile) {
	struct DRO dro;
	dro.file = NULL;

	if (fseek(droFile, 12, SEEK_SET) == 0) {
		dro.songLength  = fileRead(droFile);
		dro.songLength += fileRead(droFile) << 8;
		dro.songLength += fileRead(droFile) << 16;
		dro.songLength += fileRead(droFile) << 24;

		if (fseek(droFile, 23, SEEK_SET) == 0) {
			dro.codeShortDelay = fileRead(droFile);
			dro.codeLongDelay  = fileRead(droFile);
			dro.registerMapLength = fileRead(droFile);

			for (int i = 0; i < dro.registerMapLength; i ++) {
				dro.registerMap[i] = fileRead(droFile);
			}

			dro.file = droFile;
		}
	}

	return dro;
}


void playImfMusic(struct IMF imf) {
	unsigned char reg;
	unsigned char data;
	unsigned int delayCycles;

	for (int i = 0; i < imf.songLength; i += 3) {
		if (feof(imf.file) || ferror(imf.file)) break;
		reg  = fileRead(imf.file);
		data = fileRead(imf.file);
		delayCycles  = fileRead(imf.file);
		delayCycles += fileRead(imf.file);

		opl2.write(reg, data);
		delay((int)(delayCycles * (1000.0 / imf.speed)));
	}
}


struct IMF loadImf(FILE *imfFile, int speed) {
	struct IMF imf;

	imf.file  = imfFile;
	imf.speed = speed;
	imf.songLength  = fileRead(imfFile);
	imf.songLength += fileRead(imfFile) << 8;

	// If song length is 0 we're probably dealing with a type 0 file. In that
	// case we want to play the file until EOF, so set songLength to max.
	if (imf.songLength == 0) {
		imf.songLength = 65535;
		fseek(imfFile, 0, SEEK_SET);
	}

	return imf;
}


int fileRead(FILE *file) {
	unsigned char byte[1];
	if (fread(byte, sizeof(byte), 1, file)) {
		return byte[0];
	}
	return 0;
}


int spiError() {
	printf("Cannot initialize SPI! Are you running this as root?\n\n");
	return 1;
}


int fileError() {
	printf("Please provide a .IMF or .DRO file as the first parameter.\n\n");
	return 1;
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
		printf("\033[0mOPL2.play v1.0.0 for the OPL2 Audio Board by Maarten Janssen in 2017\n");
		printf("Visit \033[1;34mhttp://github.com/DhrBaksteen/ArduinoOPL2\033[0m to learn more!\n\n");
	}
}


void showHelp() {
	printHeader();
	printf("OPL2.play is a demo utility of the OPL2 Audio Board for the Arduino and\n");
	printf("Raspberry Pi. It lets you playback *.DRO and *.IMF music files that are either\n");
	printf("raw Adlib register captures from DOXBox or id Software game music files.\n");
	printf("\n");
	printf("Usage: opl2play <file> [imf_speed] [<file_n> [imf_speed_n]]\n");
	printf("                [--help] [--kill] [--silent] [--repeat]\n");
	printf("\n");
	printf("file             The music files to play. If the file extension is '.dro' then\n");
	printf("                 the player will interpret the file as a DOSBox DRO capture\n");
	printf("                 file. Any other files are assumed to be IMF files and may\n");
	printf("                 declare the playback speed in Hz as the next argument.\n");
	printf("\n");
	printf("imf_speed        The playback speed in Hz for the IMF file. If this argument is\n");
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
	printf("-- repeat, -r    Repeats when all songs have been played.\n");
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
