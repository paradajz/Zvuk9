#include "Board.h"

#ifdef BOARD_R1
uint32_t mcpData = 0;

void write_I2C_reg(uint8_t address, uint8_t reg, uint8_t value)
{
    i2c_start((address << 1) + I2C_WRITE);
    i2c_write(reg);
    i2c_write(value);
    i2c_stop();
}

uint8_t read_I2C_reg(uint8_t address, uint8_t reg)
{
    uint8_t value;

    i2c_start((address << 1) + I2C_WRITE);
    i2c_write(reg);
    i2c_stop();

    i2c_start((address << 1) + I2C_READ);
    value = i2c_read_nack();
    i2c_stop();

    return value;
}

void Board::initButtons()
{
    i2c_init();

    //ensure that we know the configuration
    write_I2C_reg(expanderAddress[0], 0x0A, 0x00);              //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[0], 0x05, 0x00);              //IOCON=0x00 if BANK=1
    write_I2C_reg(expanderAddress[1], 0x0A, 0x00);              //IOCON=0x00 if BANK=0
    write_I2C_reg(expanderAddress[1], 0x05, 0x00);              //IOCON=0x00 if BANK=1

    write_I2C_reg(expanderAddress[0], iodirAddress[0], 0xFF);   //expander 1, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[0], iodirAddress[1], 0xFF);   //expander 1, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[1], iodirAddress[0], 0xFF);   //expander 2, set all pins on PORTA to input mode
    write_I2C_reg(expanderAddress[1], iodirAddress[1], 0xFF);   //expander 2, set all pins on PORTB to input mode

    write_I2C_reg(expanderAddress[0], gppuAddress[0], 0xFF);    //expander 1, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[0], gppuAddress[1], 0xFF);    //expander 1, turn on pull-ups, PORTB

    write_I2C_reg(expanderAddress[1], gppuAddress[0], 0xFF);    //expander 2, turn on pull-ups, PORTA
    write_I2C_reg(expanderAddress[1], gppuAddress[1], 0xFF);    //expander 2, turn on pull-ups, PORTB
}
#endif

bool Board::buttonDataAvailable()
{
    #ifdef BOARD_R1
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[0], gpioAddress[0]);     //expander A, GPIOA
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[0], gpioAddress[1]);     //expander A, GPIOB
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[1], gpioAddress[0]);     //expander B, GPIOA
    mcpData <<= 8;
    mcpData |= read_I2C_reg(expanderAddress[1], gpioAddress[1]);     //expander B, GPIOB
    #endif

    return true;
}

bool Board::getButtonState(uint8_t buttonID)
{
    #ifdef BOARD_R1
    return !((mcpData >> buttonID) & 0x01);
    #elif defined (BOARD_R2)
    return false;
    #endif
}