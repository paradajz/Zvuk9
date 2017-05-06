#include "Board.h"

#define PAD_SAMPLE_BUFFER_SIZE  3

const uint8_t       padIDArray[] =
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

const uint8_t       muxCommonPinsAnalogRead[] =
{
    MUX_COMMON_PIN_0_PIN,
    MUX_COMMON_PIN_1_PIN,
    MUX_COMMON_PIN_2_PIN,
    MUX_COMMON_PIN_3_PIN
};

uint8_t             adcPinReadOrder_board[] =
{
    muxCommonPinsAnalogRead[2], //pressure, first reading
    muxCommonPinsAnalogRead[0], //pressure, second reading
    muxCommonPinsAnalogRead[0], //x coordinate
    muxCommonPinsAnalogRead[2]  //y coordinate
};

//four readings - two for pressure, one for x, one for y
//PAD_SAMPLE_BUFFER_SIZE total samples
//for each pad
volatile uint16_t   samples[PAD_SAMPLE_BUFFER_SIZE][PAD_READINGS][NUMBER_OF_PADS];

//three coordinates
uint16_t            extractedSamples[PAD_READINGS][NUMBER_OF_PADS];

bool                dataReady[PAD_SAMPLE_BUFFER_SIZE];

volatile uint8_t    pad_sample_buffer_head;
volatile uint8_t    pad_sample_buffer_tail;

uint8_t             activePad;
uint8_t             readIndex;

void                (*valueSetup[PAD_READINGS]) (void);

inline void setMuxInput(uint8_t muxInput)
{
    bitRead(muxInput, 0) ? setHigh(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN) : setLow(MUX_SELECT_PIN_0_PORT, MUX_SELECT_PIN_0_PIN);
    bitRead(muxInput, 1) ? setHigh(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN) : setLow(MUX_SELECT_PIN_1_PORT, MUX_SELECT_PIN_1_PIN);
    bitRead(muxInput, 2) ? setHigh(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN) : setLow(MUX_SELECT_PIN_2_PORT, MUX_SELECT_PIN_2_PIN);
    bitRead(muxInput, 3) ? setHigh(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN) : setLow(MUX_SELECT_PIN_3_PORT, MUX_SELECT_PIN_3_PIN);
}

void setupPressure()
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
}

void setupX()
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
}

void setupY()
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
}

void Board::initPads()
{
    setupPressure();
    setADCchannel(adcPinReadOrder_board[0]);
    setMuxInput(0);

    //place setup z/x/y in function pointer for simpler access in isr
    valueSetup[readPressure0] = setupPressure;
    valueSetup[readPressure1] = setupPressure;
    valueSetup[readX] = setupX;
    valueSetup[readY] = setupY;

    for (int i=0; i<PAD_SAMPLE_BUFFER_SIZE; i++)
    {
        for (int j=0; j<PAD_READINGS; j++)
        {
            for (int k=0; k<NUMBER_OF_PADS; k++)
            {
                samples[i][j][k] = 0;
            }
        }
    }

    adcInterruptEnable();
}

uint16_t Board::getPadPressure(uint8_t pad)
{
    uint16_t value1, value2;

    value1 = extractedSamples[readPressure0][pad];
    value2 = extractedSamples[readPressure1][pad];

    return (1023 - (value2 - value1));
}

uint16_t Board::getPadX(uint8_t pad)
{
    return extractedSamples[readY][pad];
}

uint16_t Board::getPadY(uint8_t pad)
{
    return extractedSamples[readX][pad];
}

ISR(ADC_vect)
{
    static bool insert = true;

    if (insert)
    {
        uint8_t index = pad_sample_buffer_head + 1;

        if (index >= PAD_SAMPLE_BUFFER_SIZE)
            index = 0;

        //if buffer is full, wait until there is some space
        if (pad_sample_buffer_tail == index)
        {
            return;
        }

        pad_sample_buffer_head = index;
        insert = false;
    }

    samples[pad_sample_buffer_head][readIndex][activePad] = ADC;

    activePad++;

    if (activePad == NUMBER_OF_PADS)
    {
        activePad = 0;

        //switch to next pad value readout
        readIndex++;

        if (readIndex == PAD_READINGS)
        {
            readIndex = 0;
            insert = true;
            dataReady[pad_sample_buffer_head] = true;
        }

        (*valueSetup[readIndex])();

        //switch adc channel
        setADCchannel(adcPinReadOrder_board[readIndex]);
    }

    //always switch mux input
    setMuxInput(activePad);
}

///
/// \brief Checks if there's sampled data in pad sample buffer.
/// \returns True if there's data, false otherwise.
///
bool Board::padDataAvailable()
{
    bool returnValue = true;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        if ((pad_sample_buffer_head == pad_sample_buffer_tail) || !dataReady[pad_sample_buffer_head])
        {
            //buffer is empty
            returnValue = false;
        }
    }

    if (!returnValue)
        return false;

    //printf_P(PSTR("Head: %d\n"), pad_sample_buffer_head);
    //printf_P(PSTR("Tail: %d\n"), pad_sample_buffer_tail);
    //printf_P(PSTR("Sample counter: %d\n"), sampleCounter);
    //printf_P(PSTR("Active pad: %d\n"), activePad);
    //printf_P(PSTR("Read index: %d\n\n"), readIndex);

    uint8_t index;

    ATOMIC_BLOCK(ATOMIC_RESTORESTATE)
    {
        //there is something in buffer
        index = pad_sample_buffer_tail + 1;

        if (index >= PAD_SAMPLE_BUFFER_SIZE)
            index = 0;

        pad_sample_buffer_tail = index;

        for (int i=0; i<PAD_READINGS; i++)
        {
            for (int j=0; j<NUMBER_OF_PADS; j++)
            {
                extractedSamples[i][j] = samples[index][i][j];
            }
        }

        dataReady[pad_sample_buffer_head] = false;
    }

    return returnValue;
}