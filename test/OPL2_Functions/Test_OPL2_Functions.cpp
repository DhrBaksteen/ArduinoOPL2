#include <Arduino.h>
#include <OPL2.h>
#include <unity.h>

OPL2 opl2;


/**
 * Test changing wave form select enable bit.
 */
void test_register0x01() {
    TEST_ASSERT_FALSE(opl2.getWaveFormSelect());

    opl2.setWaveFormSelect(true);
    TEST_ASSERT_TRUE(opl2.getWaveFormSelect());
    TEST_ASSERT_EQUAL_INT8(0x20, opl2.getChipRegister(0x01));

    opl2.setWaveFormSelect(false);
    TEST_ASSERT_FALSE(opl2.getWaveFormSelect());
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChipRegister(0x01));
}


/**
 * Test operator bits and frequency multiplier for each channel operator.
 */
void test_register0x20() {
    for (int ch = 0; ch < 9; ch ++) {
        for (int op = 0; op < 2; op ++) {
            TEST_ASSERT_FALSE(opl2.getTremolo(ch, op));
            TEST_ASSERT_FALSE(opl2.getVibrato(ch, op));
            TEST_ASSERT_FALSE(opl2.getMaintainSustain(ch, op));
            TEST_ASSERT_FALSE(opl2.getEnvelopeScaling(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getMultiplier(ch, op));

            opl2.setTremolo(ch, op, true);
            TEST_ASSERT_TRUE(opl2.getTremolo(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x80, opl2.getOperatorRegister(0x20, ch, op));
            opl2.setTremolo(ch, op, false);
            TEST_ASSERT_FALSE(opl2.getTremolo(ch, op));

            opl2.setVibrato(ch, op, true);
            TEST_ASSERT_TRUE(opl2.getVibrato(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x40, opl2.getOperatorRegister(0x20, ch, op));
            opl2.setVibrato(ch, op, false);
            TEST_ASSERT_FALSE(opl2.getVibrato(ch, op));

            opl2.setMaintainSustain(ch, op, true);
            TEST_ASSERT_TRUE(opl2.getMaintainSustain(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x20, opl2.getOperatorRegister(0x20, ch, op));
            opl2.setMaintainSustain(ch, op, false);
            TEST_ASSERT_FALSE(opl2.getMaintainSustain(ch, op));

            opl2.setEnvelopeScaling(ch, op, true);
            TEST_ASSERT_TRUE(opl2.getEnvelopeScaling(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x10, opl2.getOperatorRegister(0x20, ch, op));
            opl2.setEnvelopeScaling(ch, op, false);
            TEST_ASSERT_FALSE(opl2.getEnvelopeScaling(ch, op));

            opl2.setMultiplier(ch, op, 0x05);
            TEST_ASSERT_EQUAL_INT8(0x05, opl2.getMultiplier(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x05, opl2.getOperatorRegister(0x20, ch, op));
            opl2.setMultiplier(ch, op, 0x0A);
            TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getMultiplier(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getOperatorRegister(0x20, ch, op));
            opl2.setMultiplier(ch, op, 0xFF);
            TEST_ASSERT_EQUAL_INT8(0x0F, opl2.getMultiplier(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x0F, opl2.getOperatorRegister(0x20, ch, op));
        }
    }
}


/**
 * Test setting key scale level and output level on all channel operators.
 */
void test_register0x40() {
    for (int ch = 0; ch < 9; ch ++) {
        for (int op = 0; op < 2; op ++) {
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getScalingLevel(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getVolume(ch, op));

            opl2.setScalingLevel(ch, op, 0x01);
            opl2.setVolume(ch, op, 0x15);

            TEST_ASSERT_EQUAL_INT8(0x01, opl2.getScalingLevel(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x15, opl2.getVolume(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x55, opl2.getOperatorRegister(0x40, ch, op));

            opl2.setScalingLevel(ch, op, 0x02);
            opl2.setVolume(ch, op, 0x2A);

            TEST_ASSERT_EQUAL_INT8(0x02, opl2.getScalingLevel(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x2A, opl2.getVolume(ch, op));
            TEST_ASSERT_EQUAL_INT8(0xAA, opl2.getOperatorRegister(0x40, ch, op));
        }
    }

    // Test that values over 3 are truncated on key scale level.
    opl2.setOperatorRegister(0x40, 0, 0, 0x00);
    opl2.setScalingLevel(0, 0, 0xFF);
    TEST_ASSERT_EQUAL_INT8(0xC0, opl2.getOperatorRegister(0x40, 0, 0));

    // Test that values over 63 are truncated on output level.
    opl2.setOperatorRegister(0x40, 0, 0, 0x00);
    opl2.setVolume(0, 0, 0xFF);
    TEST_ASSERT_EQUAL_INT8(0x3F, opl2.getOperatorRegister(0x40, 0, 0));
}


/**
 * Test setting attack and decay on all channel operators.
 */
void test_register0x60() {
    for (int ch = 0; ch < 9; ch ++) {
        for (int op = 0; op < 2; op ++) {
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getAttack(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getDecay(ch, op));

            opl2.setAttack(ch, op, 0x05);
            opl2.setDecay(ch, op, 0x0A);

            TEST_ASSERT_EQUAL_INT8(0x05, opl2.getAttack(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getDecay(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x5A, opl2.getOperatorRegister(0x60, ch, op));

            opl2.setAttack(ch, op, 0x0A);
            opl2.setDecay(ch, op, 0x05);

            TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getAttack(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x05, opl2.getDecay(ch, op));
            TEST_ASSERT_EQUAL_INT8(0xA5, opl2.getOperatorRegister(0x60, ch, op));
        }
    }

    // Test that values over 15 are truncated on attack.
    opl2.setOperatorRegister(0x60, 0, 0, 0x00);
    opl2.setAttack(0, 0, 0xFA);
    TEST_ASSERT_EQUAL_INT8(0xA0, opl2.getOperatorRegister(0x60, 0, 0));

    // Test that values over 15 are truncated on decay.
    opl2.setOperatorRegister(0x60, 0, 0, 0x00);
    opl2.setDecay(0, 0, 0xFA);
    TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getOperatorRegister(0x60, 0, 0));
}


/**
 * Test setting sustain and release on all channel operators.
 */
void test_register0x80() {
    for (int ch = 0; ch < 9; ch ++) {
        for (int op = 0; op < 2; op ++) {
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getSustain(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getRelease(ch, op));

            opl2.setSustain(ch, op, 0x05);
            opl2.setRelease(ch, op, 0x0A);

            TEST_ASSERT_EQUAL_INT8(0x05, opl2.getSustain(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getRelease(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x5A, opl2.getOperatorRegister(0x80, ch, op));

            opl2.setSustain(ch, op, 0x0A);
            opl2.setRelease(ch, op, 0x05);

            TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getSustain(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x05, opl2.getRelease(ch, op));
            TEST_ASSERT_EQUAL_INT8(0xA5, opl2.getOperatorRegister(0x80, ch, op));
        }
    }

    // Test that values over 15 are truncated on sustain.
    opl2.setOperatorRegister(0x80, 0, 0, 0x00);
    opl2.setSustain(0, 0, 0xF5);
    TEST_ASSERT_EQUAL_INT8(0x50, opl2.getOperatorRegister(0x80, 0, 0));

    // Test that values over 15 are truncated on release.
    opl2.setOperatorRegister(0x80, 0, 0, 0x00);
    opl2.setRelease(0, 0, 0xF5);
    TEST_ASSERT_EQUAL_INT8(0x05, opl2.getOperatorRegister(0x80, 0, 0));
}


/**
 * Test setting the F-number on all channels.
 */
void test_register0xA0() {
    for (int ch = 0; ch < 9; ch ++) {
        TEST_ASSERT_EQUAL_INT16(0x00, opl2.getFNumber(ch));

        opl2.setFNumber(ch, 0x155);
        TEST_ASSERT_EQUAL_INT16(0x155, opl2.getFNumber(ch));
        TEST_ASSERT_EQUAL_INT8(0x55, opl2.getChannelRegister(0xA0, ch));
        TEST_ASSERT_EQUAL_INT8(0x01, opl2.getChannelRegister(0xB0, ch));

        opl2.setFNumber(ch, 0x2AA);
        TEST_ASSERT_EQUAL_INT16(0x2AA, opl2.getFNumber(ch));
        TEST_ASSERT_EQUAL_INT8(0xAA, opl2.getChannelRegister(0xA0, ch));
        TEST_ASSERT_EQUAL_INT8(0x02, opl2.getChannelRegister(0xB0, ch));

        opl2.setFNumber(ch, 0xFFFF);
        TEST_ASSERT_EQUAL_INT16(0x3FF, opl2.getFNumber(ch));
        TEST_ASSERT_EQUAL_INT8(0xFF, opl2.getChannelRegister(0xA0, ch));
        TEST_ASSERT_EQUAL_INT8(0x03, opl2.getChannelRegister(0xB0, ch));

        opl2.setFNumber(ch, 0x0000);
        TEST_ASSERT_EQUAL_INT16(0x000, opl2.getFNumber(ch));
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChannelRegister(0xA0, ch));
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChannelRegister(0xB0, ch));
    }
}


/**
 * Test setting the block number and key on on all channels.
 */
void test_register0xB0() {
    for (int ch = 0; ch < 9; ch ++) {
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getBlock(ch));
        TEST_ASSERT_FALSE(opl2.getKeyOn(ch));

        opl2.setBlock(ch, 0x05);
        TEST_ASSERT_EQUAL_INT8(0x05, opl2.getBlock(ch));
        TEST_ASSERT_EQUAL_INT8(0x14, opl2.getChannelRegister(0xB0, ch));

        opl2.setBlock(ch, 0x02);
        TEST_ASSERT_EQUAL_INT8(0x02, opl2.getBlock(ch));
        TEST_ASSERT_EQUAL_INT8(0x08, opl2.getChannelRegister(0xB0, ch));

        opl2.setBlock(ch, 0xFF);
        TEST_ASSERT_EQUAL_INT8(0x07, opl2.getBlock(ch));
        TEST_ASSERT_EQUAL_INT8(0x1C, opl2.getChannelRegister(0xB0, ch));

        opl2.setBlock(ch, 0x00);
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getBlock(ch));
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChannelRegister(0xB0, ch));

        opl2.setKeyOn(ch, true);
        TEST_ASSERT_TRUE(opl2.getKeyOn(ch));
        TEST_ASSERT_EQUAL_INT8(0x20, opl2.getChannelRegister(0xB0, ch));

        opl2.setKeyOn(ch, false);
        TEST_ASSERT_FALSE(opl2.getKeyOn(ch));
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChannelRegister(0xB0, ch));
    }
}

/**
 * Test the drum register
 */
void test_register0xBD() {
    TEST_ASSERT_FALSE(opl2.getPercussion());
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getDrums());

    opl2.setPercussion(true);
    TEST_ASSERT_TRUE(opl2.getPercussion());
    TEST_ASSERT_EQUAL_INT8(0x20, opl2.getChipRegister(0xBD));

    opl2.setPercussion(false);
    TEST_ASSERT_FALSE(opl2.getPercussion());
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChipRegister(0xBD));

    opl2.setDrums(DRUM_BITS_BASS | DRUM_BITS_TOM | DRUM_BITS_HI_HAT);
    TEST_ASSERT_EQUAL_INT8(0x15, opl2.getDrums());
    TEST_ASSERT_EQUAL_INT8(0x15, opl2.getChipRegister(0xBD));

    opl2.setDrums(DRUM_BITS_SNARE | DRUM_BITS_CYMBAL);
    TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getDrums());
    TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getChipRegister(0xBD));

    opl2.setDrums(0xFF);
    TEST_ASSERT_EQUAL_INT8(0x1F, opl2.getDrums());
    TEST_ASSERT_EQUAL_INT8(0x1F, opl2.getChipRegister(0xBD));

    opl2.setDrums(0x00);
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getDrums());
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChipRegister(0xBD));

    opl2.setDrums(true, false, true, false, true);
    TEST_ASSERT_EQUAL_INT8(0x15, opl2.getDrums());
    TEST_ASSERT_EQUAL_INT8(0x15, opl2.getChipRegister(0xBD));

    opl2.setDrums(false, true, false, true, false);
    TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getDrums());
    TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getChipRegister(0xBD));

    opl2.setDrums(true, true, true, true, true);
    TEST_ASSERT_EQUAL_INT8(0x1F, opl2.getDrums());
    TEST_ASSERT_EQUAL_INT8(0x1F, opl2.getChipRegister(0xBD));

    opl2.setDrums(false, false, false, false, false);
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getDrums());
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChipRegister(0xBD));
}


/**
 * Test setting feedback and synth mode on each channel.
 */
void test_register0xC0() {
    for (int ch = 0; ch < 9; ch ++) {
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getFeedback(ch));
        TEST_ASSERT_FALSE(opl2.getSynthMode(ch));

        opl2.setFeedback(ch, 0x05);
        TEST_ASSERT_EQUAL_INT8(0x05, opl2.getFeedback(ch));
        TEST_ASSERT_EQUAL_INT8(0x0A, opl2.getChannelRegister(0xC0, ch));

        opl2.setFeedback(ch, 0x02);
        TEST_ASSERT_EQUAL_INT8(0x02, opl2.getFeedback(ch));
        TEST_ASSERT_EQUAL_INT8(0x04, opl2.getChannelRegister(0xC0, ch));

        opl2.setFeedback(ch, 0xFF);
        TEST_ASSERT_EQUAL_INT8(0x07, opl2.getFeedback(ch));
        TEST_ASSERT_EQUAL_INT8(0x0E, opl2.getChannelRegister(0xC0, ch));

        opl2.setSynthMode(ch, true);
        TEST_ASSERT_TRUE(opl2.getSynthMode(ch));
        TEST_ASSERT_EQUAL_INT8(0x0F, opl2.getChannelRegister(0xC0, ch));

        opl2.setSynthMode(ch, false);
        TEST_ASSERT_FALSE(opl2.getSynthMode(ch));
        TEST_ASSERT_EQUAL_INT8(0x0E, opl2.getChannelRegister(0xC0, ch));
    }
}


/**
 * Test setting waveform for each channel operator.
 */
void test_register0xE0() {
    for (int ch = 0; ch < 9; ch ++) {
        for (int op = 0; op < 2; op ++) {
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getWaveForm(ch, op));

            opl2.setWaveForm(ch, op, 0x01);
            TEST_ASSERT_EQUAL_INT8(0x01, opl2.getWaveForm(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x01, opl2.getOperatorRegister(0xE0, ch, op));
            opl2.setWaveForm(ch, op, 0x02);
            TEST_ASSERT_EQUAL_INT8(0x02, opl2.getWaveForm(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x02, opl2.getOperatorRegister(0xE0, ch, op));
            opl2.setWaveForm(ch, op, 0xFF);
            TEST_ASSERT_EQUAL_INT8(0x03, opl2.getWaveForm(ch, op));
            TEST_ASSERT_EQUAL_INT8(0x03, opl2.getOperatorRegister(0xE0, ch, op));
        }
    }
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    opl2.begin();
    RUN_TEST(test_register0x01);
    RUN_TEST(test_register0x20);
    RUN_TEST(test_register0x40);
    RUN_TEST(test_register0x60);
    RUN_TEST(test_register0x80);
    RUN_TEST(test_register0xA0);
    RUN_TEST(test_register0xB0);
    RUN_TEST(test_register0xBD);
    RUN_TEST(test_register0xC0);
    RUN_TEST(test_register0xE0);

    UNITY_END();
}


void loop() {
}
