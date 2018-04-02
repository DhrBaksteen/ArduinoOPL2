#ifndef OPL2PLAY_H_
	#define OPL2PLAY_H_

	#include <stdio.h>


	typedef struct DRO {
		FILE *file;
		unsigned long songLength;
		unsigned char codeShortDelay;
		unsigned char codeLongDelay;
		unsigned char registerMapLength;
		unsigned char registerMap[256];
	} DRO;


	typedef struct IMF {
		FILE *file;
		unsigned long songLength;
		int speed;
	} IMF;
	
	
	typedef struct VGM {
		FILE *file;
		unsigned long songLength;
		unsigned long loopOffset;
		unsigned long loopLength;
	} VGM;


	int main(int argc, char **argv);
	DRO loadDro(FILE *droFile);
	IMF loadImf(FILE *imfFile, int speed);
	VGM loadVgm(FILE *vgmFile);
	VGM loadVgz(FILE *vgzFile);
	void playDroMusic(DRO dro);
	void playImfMusic(IMF imf);
	void playVgmMusic(VGM vgm);
	unsigned char fileRead(FILE *file);
	unsigned int readWord(FILE *file);
	unsigned int readDWord(FILE *file);
	int spiError();
	int fileError();
	void printHeader();
	void showHelp();
	void showConnections();
	
	int inflate(FILE *source, FILE *dest);
#endif
