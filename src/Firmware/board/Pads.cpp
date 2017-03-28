#include "Board.h"

volatile uint16_t padPressure1[MAX_PADS];
volatile uint16_t padPressure2[MAX_PADS];
volatile uint16_t padX[MAX_PADS];
volatile uint16_t padY[MAX_PADS];

volatile uint16_t *readingPointer;

volatile uint8_t activePad;
volatile uint8_t activeReadout;
volatile uint8_t sampleCounter;

#define SAMPLE_SHIFT_AMOUNT     3
#define PAD_SAMPLES             8

//multiplexer pins
const uint8_t muxCommonPinsAnalogRead[] =
{
    MUX_COMMON_PIN_0_PIN,
    MUX_COMMON_PIN_1_PIN,
    MUX_COMMON_PIN_2_PIN,
    MUX_COMMON_PIN_3_PIN
};

const uint8_t padIDArray[] =
{
    PAD_0,
    PAD_1,
    PAD_2,
    PAD_3,
    PAD_4,
    PAD_5,
    PAD_6,
    PAD_7,
    PAD_8
};

uint8_t adcPinReadOrder_board[] =
{
    muxCommonPinsAnalogRead[2], //pressure, first reading
    muxCommonPinsAnalogRead[0], //pressure, second reading
    muxCommonPinsAnalogRead[0], //x coordinate
    muxCommonPinsAnalogRead[2]  //y coordinate
};

typedef enum
{
    readPressure0,
    readPressure1,
    readX,
    readY
} padReadOrder;

inline void nextMuxInput()
{
    bitRead(activePad, 0) ? setHigh(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN) : setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    bitRead(activePad, 1) ? setHigh(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN) : setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    bitRead(activePad, 2) ? setHigh(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN) : setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    bitRead(activePad, 3) ? setHigh(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN) : setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);

    //add NOPs to compensate for propagation delay
    _NOP(); _NOP(); _NOP();
    _NOP(); _NOP(); _NOP();
    _NOP(); _NOP(); _NOP();
    _NOP(); _NOP(); _NOP();
}

inline void setupPressure()
{
    //pressure is read from x+/y+
    //set 0/5V across x-/y-
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    _NOP(); _NOP(); _NOP();
}

inline void setupX()
{
    //x is read from y+
    //set 0/5V across x+/x-
    setInput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setInput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setOutput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setOutput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setLow(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setHigh(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    _NOP(); _NOP(); _NOP();
}

inline void setupY()
{
    //y is read from x+
    //set 0/5V across y+/y-
    setOutput(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setOutput(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setInput(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setInput(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    setLow(MUX_COMMON_PIN_0_PORT, MUX_COMMON_PIN_0_PIN);
    setHigh(MUX_COMMON_PIN_1_PORT, MUX_COMMON_PIN_1_PIN);
    setLow(MUX_COMMON_PIN_2_PORT, MUX_COMMON_PIN_2_PIN);
    setLow(MUX_COMMON_PIN_3_PORT, MUX_COMMON_PIN_3_PIN);

    _NOP(); _NOP(); _NOP();
}

void Board::initPads()
{
    setUpADC();
    setupPressure();
    nextMuxInput();
    setADCchannel(adcPinReadOrder_board[readPressure0]);
    readingPointer = padPressure1;
    startADCconversion();
}

bool Board::padDataAvailable()
{
    return sampleCounter == PAD_SAMPLES;
}

uint16_t Board::getPadPressure(uint8_t pad)
{
    uint16_t value1, value2;

    value1 = padPressure1[pad];
    value2 = padPressure2[pad];

    return (1023 - (value2 - value1)) >> SAMPLE_SHIFT_AMOUNT;
}

uint16_t Board::getPadX(uint8_t pad)
{
    return padX[pad];
}

uint16_t Board::getPadY(uint8_t pad)
{
    return padY[pad];
}

ISR(ADC_vect)
{   return;
    //now we have first pressure value
    readingPointer[activePad] += ADC;
    activePad++;

    if (activePad == MAX_PADS)
    {
        activeReadout++;

        if (activeReadout == 4)
        {
            sampleCounter++;
            activeReadout = 0;
        }

        switch(activeReadout)
        {
            case 0:
            setupPressure();
            setADCchannel(adcPinReadOrder_board[readPressure0]);
            readingPointer = padPressure1;
            break;

            case 1:
            //pressure already setup
            setADCchannel(adcPinReadOrder_board[readPressure1]);
            readingPointer = padPressure2;
            break;

            case 2:
            setupX();
            setADCchannel(adcPinReadOrder_board[readX]);
            readingPointer = padX;
            break;

            case 3:
            setupY();
            setADCchannel(adcPinReadOrder_board[readY]);
            readingPointer = padY;
            break;
        }

        activePad = 0;
    }

    nextMuxInput();

    if (sampleCounter < PAD_SAMPLES)
        startADCconversion();
}

void Board::samplePads()
{
    for (int i=0; i<MAX_PADS; i++)
    {
        padPressure1[i] = 0;
        padPressure2[i] = 0;
        padX[i] = 0;
        padY[i] = 0;
    }

    activePad = 0;
    activeReadout = 0;
    sampleCounter = 0;
    startADCconversion();
}