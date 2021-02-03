include <gtest/gtest.h>
#include "ads1256.h"

namespace
{
    TEST(AnalogValues, GetChannelOne)
    {
        uint32_t value = 0;
        ADS1256 ads1256;
        value = ads1256.value(0);

        EXPECT_EQ(value, 0) << "Returned: " << value;
    }
}
