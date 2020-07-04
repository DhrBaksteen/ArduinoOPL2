#include "OPL3.h"

#define NUM_OPL3DUO_CHANNELS 36


class OPL3Duo: public OPL3 {
	public:
		OPL3Duo();
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

	protected:
		byte pinUnit = 5;
};
