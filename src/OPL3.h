#include "OPL2.h"

#define NUM_OPL3_CHANNELS 18
#define NUM_4OP_CHANNELS   6


struct Instrument4OP {
	Instrument subInstrument[2];		// Definition of the 2 sub instruments for each channel.
};


class OPL3: public OPL2 {
	public:
		OPL3();
		virtual void begin();
		virtual void reset();
		virtual void createShadowRegisters();

		virtual void setChannelRegister(byte baseRegister, byte channel, byte value);
		virtual void setOperatorRegister(byte baseRegister, byte channel, byte operatorNum, byte value);
		virtual byte getChipRegisterOffset(short reg);
		virtual void write(byte bank, byte reg, byte value);

		Instrument4OP createInstrument4OP();
		#if BOARD_TYPE == OPL2_BOARD_TYPE_ARDUINO
			Instrument4OP loadInstrument4OP(const unsigned char *instrument, bool fromProgmem = INSTRUMENT_DATA_PROGMEM);
		#else
			Instrument4OP loadInstrument4OP(const unsigned char *instrument);
		#endif
		Instrument4OP getInstrument4OP(byte channel4OP);
		void setInstrument4OP(byte channel4OP, Instrument4OP instrument, float volume = 1.0);

		virtual bool getWaveFormSelect();
		virtual void setWaveFormSelect(bool enable = false);

		virtual bool isOPL3Enabled();
		virtual void setOPL3Enabled(bool enable);
		bool isPannedLeft (byte channel);
		bool isPannedRight(byte channel);
		void setPanning(byte channel, bool left, bool right);
		bool is4OPChannelEnabled(byte channel4OP);
		void enable4OPChannel(byte channel4OP, bool enable);
		byte get4OPControlChannel(byte channel4OP, byte index2OP = 0);

	protected:
		byte pinBank = 6;

		byte numChannels = 18;

		byte channelPairs[6][2] = {
			{ 0,  3 }, {  1,  4 }, {  2,  5 },
			{ 9, 12 }, { 10, 13 }, { 11, 14 }
		};
};
