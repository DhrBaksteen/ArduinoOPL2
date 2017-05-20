#ifndef OPL2PLAY_H_
	#define OPL2PLAY_H_

	#include <stdio.h>


	struct DRO {
		FILE *file;
		long songLength;
		unsigned char codeShortDelay;
		unsigned char codeLongDelay;
		unsigned char registerMapLength;
		unsigned char registerMap[256];
	};


	struct IMF {
		FILE *file;
		long songLength;
		int speed;
	};


	int main(int argc, char **argv);
	struct DRO loadDro(FILE *droFile);
	struct IMF loadImf(FILE *imfFile, int speed);
	void playDroMusic(struct DRO dro);
	void playImfMusic(struct IMF imf);
	int fileRead(FILE *file);
	int spiError();
	int fileError();
	void printHeader();
	void showHelp();
	void showConnections();
#endif
