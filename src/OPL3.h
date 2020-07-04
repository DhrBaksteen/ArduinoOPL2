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
		virtual byte getNumChannels();

		virtual byte getChipRegister(short reg);
		virtual byte getChannelRegister(byte baseRegister, byte channel);
		virtual byte getOperatorRegister(byte baseRegister, byte channel, byte op);
		virtual void setChipRegister(short reg, byte value);
		virtual void setChannelRegister(byte baseRegister, byte channel, byte value);
		virtual void setOperatorRegister(byte baseRegister, byte channel, byte op, byte value);
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

		byte get4OPControlChannel(byte channel4OP);
		bool isOPL3Enabled();
		bool isPannedLeft (byte channel);
		bool isPannedRight(byte channel);
		void enableOPL3(bool enable);
		void setPanning(byte channel, bool left, bool right);
		bool is4OPChannelEnabled(byte channel4OP);
		void enable4OPChannel(byte channel4OP, bool enable);

	protected:
		byte pinBank = 6;

		const byte channelPairs[2][6] = {
			{ 0, 1, 2, 9, 10, 11},
			{ 3, 4, 5, 12, 13, 14}
		};
};
