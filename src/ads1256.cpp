/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2021  <copyright holder> <email>
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "ads1256.h"

ADS1256::ADS1256()
{
    wiringPiSetupGpio();
    pinMode(DEV_RST_PIN, OUTPUT);
    pinMode(DEV_CS_PIN, OUTPUT);
    pinMode(DEV_DRDY_PIN, INPUT);
    
    digitalWrite(DEV_CS_PIN, HIGH);
    digitalWrite(DEV_RST_PIN, HIGH);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    digitalWrite(DEV_RST_PIN, LOW);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    digitalWrite(DEV_RST_PIN, HIGH)
    
    std::cout << "__FUNCTION__: Talking to chipid " << readChipId << std::endl;
    configADC(ADS1256_GAIN_1, ADS1256_30000SPS);
}

ADS1256::~ADS1256()
{
    digitalWrite(DEV_RST_PIN, HIGH);
}

uint32_t ADS1256::currentTime()
{
    double ret;
    struct timespec spec;

    clock_gettime(CLOCK_REALTIME, &spec);
    ret  = spec.tv_sec + ( spec.tv_nsec / 1.0e9 );
    return ret;
}

void ADS1256::configADC(ADS1256_GAIN gain, ADS1256_DRATE drate)
{
    if (waitDReady()) {
        m_gain = gain;
        m_rate = drate;
        
        uint8_t buf[4] = {0,0,0,0};
        buf[0] = (0 << 3) | (1 << 2) | (0 << 1);
        buf[1] = 0x08;
        buf[2] = (0 << 5) | (0 << 3) | (gain << 0);
        buf[3] = ADS1256_DRATE_E[drate];
        digitalWrite(DEV_CS_PIN, 0);
        write(CMD_WREG | 0);
        write(0x03);

        write(buf[0]);
        write(buf[1]);
        write(buf[2]);
        write(buf[3]);
        digitalWrite(DEV_CS_PIN, 1);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

bool ADS1256::waitDReady()
{
    double start = getCurrentTime();
    // Waits for DRDY to go to zero or TIMEOUT seconds to pass
    while (getCurrentTime() - start < 0.5) {
        if (digitalRead(DEV_DRDY_PIN) == LOW) //  drdy_level = pi.read(17) # ADS1256 /DRDY
            return;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}

void ADS1256::setMode(int mode)
{
    if (!mode)
        m_mode = 0;
    else
        m_mode = 1;
}

uint32_t ADS1256::readADC()
{
    uint32_t read = 0;
    uint8_t buf[3] = {0,0,0};

    if (waitDReady()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        digitalWrite(DEV_CS_PIN, 0);
        write(CMD_RDATA);
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
        buf[0] = readByte();
        buf[1] = readByte();
        buf[2] = readByte();
        digitalWrite(DEV_CS_PIN, 1);
        read = (buf[0] << 16) & 0x00FF0000;
        read |= buf[1] << 8;  /* Pay attention to It is wrong   read |= (buf[1] << 8) */
        
        read |= buf[2];
        //printf("%d  %d  %d \r\n",buf[0],buf[1],buf[2]);
        if (read & 0x800000)
            read &= 0xFF000000;
        
        return read;
    }
    return 0;
}

int ADS1256::values(uint32_t *array, int first, int count)
{
    int i = 0;
    
    memset(array, 0, (sizeof(uint32_t) * count));
    
    if (array == nullptr)
        return 0;
    
    if ((first > 7) || (count > 8))
        return 0;
    
    for (i = first; i < count; i++) {
        array[i] = value(i);
    }
    
    return i;
}

uint32_t ADS1256::value(uint8_t channel)
{
    uint32_t value = 0;
    
    if (waitDReady()) {
        if (!m_mode) {// 0  Single-ended input  8 channel1 Differential input  4 channe
            if (channel >= 8) {
                return 0;
            }
            
            setChannel(channel);
            write(CMD_SYNC);
            write(CMD_WAKEUP);
            value = readADC();
        }
    }
    else{
        if (channel >= 4){
            return 0;
        }
        setDifferentialChannel(channel);
        write(CMD_SYNC);
        write(CMD_WAKEUP);
        value = readADC();
    }
    return value;
}

uint8_t ADS1256::readChipId()
{
    uint8_t id;
    
    waitDReady();
    id = read(REG_STATUS);
    return id >> 4;
}

void ADS1256::setChannel(uint8_t channel)
{
    if(channel > 7){
        return;
    }
    
    writeReg(REG_MUX, (channel << 4) | (1 << 3));
}

void ADS1256::setDifferentialChannel(uint8_t channel)
{
    if (channel == 0){
        writeReg(REG_MUX, (0 << 4) | 1);        //DiffChannal  AIN0-AIN1
    }
    else if(channel == 1){
        writeReg(REG_MUX, (2 << 4) | 3);        //DiffChannal   AIN2-AIN3
    }
    else if(channel == 2){
        writeReg(REG_MUX, (4 << 4) | 5);        //DiffChannal    AIN4-AIN5
    }
    else if(channel == 3){
        writeReg(REG_MUX, (6 << 4) | 7);        //DiffChannal   AIN6-AIN7
    }
}

void ADS1256::reset()
{
    digitalWrite(DEV_RST_PIN, HIGH);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    digitalWrite(DEV_RST_PIN, LOW);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    digitalWrite(DEV_RST_PIN, HIGH);
}

void ADS1256::write(uint8_t cmd)
{
    digitalWrite(DEV_CS_PIN, LOW);
    writeByte(cmd);
    digitalWrite(DEV_CS_PIN, HIGH);
}

void ADS1256::writeReg(uint8_t reg, uint8_t data)
{
    digitalWrite(DEV_CS_PIN, LOW);
    writeByte(CMD_WREG | reg);
    writeByte(0x00);
    writeByte(data);
    digitalWrite(DEV_CS_PIN, HIGH);
}

uint8_t ADS1256::read(uint8_t reg)
{
    uint8_t temp = 0;
    
    digitalWrite(DEV_CS_PIN, LOW);
    write(CMD_RREG | reg);
    write(0x00);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
    temp = readByte();
    digitalWrite(DEV_CS_PIN, HIGH);
    
    return temp;
}

void ADS1256::writeByte(uint8_t value)
{
    int read_data;
    
    read_data = wiringPiSPIDataRW(0, &value, 1);
    if(read_data < 0)
        std::cerr << __FUNCTION__ << ": error: " << read_data << std::endl;
}

uint8_t ADS1256::readByte()
{
    unsigned char value = 0xff;
    int read_data;
    
    read_data = wiringPiSPIDataRW(0, &value, 1);
    if(read_data < 0)
        std::cerr << __FUNCTION__ << ": error: " << read_data << std::endl;

    return value;
}
