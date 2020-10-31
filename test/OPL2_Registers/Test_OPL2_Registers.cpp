#include <Arduino.h>
#include <OPL2.h>
#include <unity.h>

OPL2 opl2;


/**
 * Number of reported channels should be 9 on OPL2.
 */
void test_getNumChannels() {
    TEST_ASSERT_EQUAL_INT8(9, opl2.getNumChannels());
}


/**
 * Test for correct array offsets for chip wide registers.
 */
void test_getChipRegisterOffset() {
    TEST_ASSERT_EQUAL_INT8(0, opl2.getChipRegisterOffset(0x01));
    TEST_ASSERT_EQUAL_INT8(1, opl2.getChipRegisterOffset(0x08));
    TEST_ASSERT_EQUAL_INT8(2, opl2.getChipRegisterOffset(0xBD));

    // Test invalid register to return offset 0.
    TEST_ASSERT_EQUAL_INT8(0, opl2.getChipRegisterOffset(0xFF));
}


/**
 * Test for correct array offsets for channel registers.
 */
void test_getChannelRegisterOffset() {
    for (int i = 0; i < 9; i ++) {
        TEST_ASSERT_EQUAL_INT8(i * 3 + 0, opl2.getChannelRegisterOffset(0xA0, i));
        TEST_ASSERT_EQUAL_INT8(i * 3 + 1, opl2.getChannelRegisterOffset(0xB0, i));
        TEST_ASSERT_EQUAL_INT8(i * 3 + 2, opl2.getChannelRegisterOffset(0xC0, i));
    }

    // Test wrapping of channels if channel > numChannels.
    TEST_ASSERT_EQUAL_INT8(0, opl2.getChannelRegisterOffset(0xA0, 9));
    TEST_ASSERT_EQUAL_INT8(3, opl2.getChannelRegisterOffset(0xA0, 10));

    // Test invalid base register to return offset 0.
    TEST_ASSERT_EQUAL_INT8(0, opl2.getChannelRegisterOffset(0xFF, 0));
}


/**
 * Test for correct array offsets for operator registers.
 */
void test_getOperatorRegisterOffset() {
    for (int i = 0; i < 9; i ++) {
        for (int j = 0; j < 2; j ++) {
            TEST_ASSERT_EQUAL_INT16((i * 10) + (j * 5) + 0, opl2.getOperatorRegisterOffset(0x20, i, j));
            TEST_ASSERT_EQUAL_INT16((i * 10) + (j * 5) + 1, opl2.getOperatorRegisterOffset(0x40, i, j));
            TEST_ASSERT_EQUAL_INT16((i * 10) + (j * 5) + 2, opl2.getOperatorRegisterOffset(0x60, i, j));
            TEST_ASSERT_EQUAL_INT16((i * 10) + (j * 5) + 3, opl2.getOperatorRegisterOffset(0x80, i, j));
            TEST_ASSERT_EQUAL_INT16((i * 10) + (j * 5) + 4, opl2.getOperatorRegisterOffset(0xE0, i, j));
        }
    }

    // Test wrapping of channels if channel > numChannels.
    TEST_ASSERT_EQUAL_INT16(0, opl2.getOperatorRegisterOffset(0x20, 9, 0));
    TEST_ASSERT_EQUAL_INT16(10, opl2.getOperatorRegisterOffset(0x20, 10, 0));

    // Test wrapping of operators if operator > 1.
    TEST_ASSERT_EQUAL_INT16(0, opl2.getOperatorRegisterOffset(0x20, 0, 2));
    TEST_ASSERT_EQUAL_INT16(5, opl2.getOperatorRegisterOffset(0x20, 9, 3));

    // Test invalid base register to return offset 0.
    TEST_ASSERT_EQUAL_INT16(0, opl2.getOperatorRegisterOffset(0xFF, 0, 0));
}


/**
 * Test write / read operations on chip wide registers.
 */
void test_chipRegisterRW() {
    const byte regs[3] = { 0x01, 0x08, 0x55 };
    const byte values[2] = { 0x55, 0xAA };

    for (int i = 0; i < 2; i ++) {
        for (int reg = 0; reg < 3; reg ++) {
            opl2.setChipRegister(regs[reg], values[i]);
            TEST_ASSERT_EQUAL_INT8(values[i], opl2.getChipRegister(regs[reg]));
        }
    }
}


/**
 * Test write / read operations on channel registers.
 */
void test_channelRegisterRW() {
    const byte regs[3] = { 0xA0, 0xB0, 0xC0 };
    const byte values[2] = { 0x55, 0xAA };

    for (int i = 0; i < 2; i ++) {
        for (int ch = 0; ch < 9; ch ++) {
            for (int reg = 0; reg < 3; reg ++) {
                opl2.setChannelRegister(regs[reg], ch, values[i]);
                TEST_ASSERT_EQUAL_INT8(values[i], opl2.getChannelRegister(regs[reg], ch));
            }
        }
    }
}


/**
 * Test write / read operations on operator registers.
 */
void test_operatorRegisterRW() {
    const byte regs[5] = { 0x20, 0x40, 0x60, 0x80, 0xE0 };
    const byte values[2] = { 0x55, 0xAA };

    for (int i = 0; i < 2; i ++) {
        for (int ch = 0; ch < 9; ch ++) {
            for (int op = 0; op < 2; op ++) {
                for (int reg = 0; reg < 3; reg ++) {
                    opl2.setOperatorRegister(regs[reg], ch, op, values[i]);
                    TEST_ASSERT_EQUAL_INT8(values[i], opl2.getOperatorRegister(regs[reg], ch, op));
                }
            }
        }
    }
}


/**
 * After OPL2.begin() or OPL2.reset() all registers should be set to 0.
 */
void test_OPL2Begin() {
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChipRegister(0x01));
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChipRegister(0x08));
    TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChipRegister(0xBD));

    for (int i = 0; i < 1; i ++) {
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChannelRegister(0xA0, i));
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChannelRegister(0xB0, i));
        TEST_ASSERT_EQUAL_INT8(0x00, opl2.getChannelRegister(0xC0, i));

        for (int j = 0; j < 2; j ++) {
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getOperatorRegister(0x20, 0, 1));
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getOperatorRegister(0x40, i, j));
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getOperatorRegister(0x60, i, j));
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getOperatorRegister(0x80, i, j));
            TEST_ASSERT_EQUAL_INT8(0x00, opl2.getOperatorRegister(0xE0, i, j));
        }
    }
}


void setup() {
    delay(2000);
    UNITY_BEGIN();

    RUN_TEST(test_getNumChannels);
    
    RUN_TEST(test_getChipRegisterOffset);
    RUN_TEST(test_getChannelRegisterOffset);
    RUN_TEST(test_getOperatorRegisterOffset);

    opl2.begin();
    RUN_TEST(test_OPL2Begin);
    RUN_TEST(test_chipRegisterRW);
    RUN_TEST(test_channelRegisterRW);
    RUN_TEST(test_operatorRegisterRW);

    opl2.reset();
    RUN_TEST(test_OPL2Begin);

    UNITY_END();
}


void loop() {
}