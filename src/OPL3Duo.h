#include "OPL3.h"

class OPL3Duo: public OPL3 {
	public:
		OPL3Duo();
		virtual void begin();
        virtual void reset();
		virtual void createShadowRegisters();

		virtual byte getChipRegister(byte synthUnit, short reg);
		virtual void setChipRegister(byte synthUnit, short reg, byte value);
		virtual void setChannelRegister(byte baseRegister, byte channel, byte value);
		virtual void setOperatorRegister(byte baseRegister, byte channel, byte op, byte value);
		virtual void write(byte bank, byte reg, byte value);

		virtual byte getNumChannels();
		virtual byte getNum4OPChannels();
		virtual byte get4OPControlChannel(byte channel4OP, byte index2OP = 0);

		virtual bool isOPL3Enabled();
		virtual bool isOPL3Enabled(byte synthUnit);
		virtual void setOPL3Enabled(bool enable);
		virtual void setOPL3Enabled(byte synthUnit, bool enable);
	protected:
		byte pinUnit = 5;

		byte numChannels = 36;
		byte num4OPChannels = 12;

		byte channelPairs[12][2] = {
			{  0,  3 }, {  1,  4 }, {  2,  5 },
			{  9, 12 }, { 10, 13 }, { 11, 14 },
			{ 18, 21 }, { 19, 22 }, { 20, 23 },
			{ 27, 30 }, { 28, 31 }, { 29, 32 }
		};
};
