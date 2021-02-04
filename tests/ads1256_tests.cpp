#include <gtest/gtest.h>
#include "ads1256.h"

namespace
{
    TEST(ADS1256, ChipID)
    {
        uint8_t value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.readChipId();

        EXPECT_EQ(value, 3) << "Returned: " << value;
    }

    TEST(ADS1256, GetChannelOne)
    {
        uint32_t value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.value(0);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }

    TEST(ADS1256, GetChannelTwo)
    {
        uint32_t value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.value(1);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }

    TEST(ADS1256, GetChannelThree)
    {
        uint32_t value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.value(2);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }

    TEST(ADS1256, GetChannelFour)
    {
        uint32_t value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.value(3);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }

    TEST(ADS1256, GetChannelFive)
    {
        uint32_t value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.value(4);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }

    TEST(ADS1256, GetChannelSix)
    {
        uint32_t value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.value(5);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }

    TEST(ADS1256, GetChannelSeven)
    {
        uint32_t value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.value(6);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }

    TEST(ADS1256, GetChannelEight)
    {
        uint32_t value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.value(7);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }

    TEST(ADS1256, GetChannelEightVoltage)
    {
        double value = 0;
        ADS1256 ads1256;
        
        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
        
        value = ads1256.voltage(7);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }
}
