# ADS1256 in C++ for the Pi and clones

This is an attempt at a simple C++ library for use with C++ projects. It is focused on the High Precision AD-DA board from 
Waveshare and provides a simple interface for using that HAT.

DA services will come at some point. This is being used internally by me and may never become easy to use outside of what I 
need from it. Just beware.

```
#include <cstdio>
#include <chrono>
#include <thread>
#include <ads1256.h>

int main(int argc, char *argv[])
{
        ADS1256 ads1256;
        double voltages[8];

        ads1256.reset();
        ads1256.configADC(ADS1256_GAIN_1, ADS1256_30000SPS);

        while (ads1256.voltages(voltages, 0, 8) > 0) {
                for (int i = 0; i < 8; i++) {
                        printf("%d %f \r\n", i, voltages[i]);
                }
                printf("\33[8A");
                std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }

        printf("Got a bad return value...\n");
        exit(0);
}
```

## Origin

https://www.waveshare.com/wiki/High-Precision_AD/DA_Board 

Most of this code is taken directly from the Waveshare example code provided at the link above. That code is provided with an as-is license
and is noted here.

Portions of code were borrowed from

https://github.com/ferbar/raspberry_ads1256_dac8552

## License

This code is licensed under the GPLv3. See LICENSE for details

