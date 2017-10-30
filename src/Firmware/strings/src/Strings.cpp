#if defined(USE_STRINGS) || defined(__DOXYGEN__)

#include "Strings.h"

void addNumberToCharArray(int32_t number, uint8_t &stringSize, bool temp)
{
    char intToCharArray[7];
    itoa(number, intToCharArray, 10);
    stringSize += strlen(intToCharArray);

    if (temp)
    {
        strcat(tempBuffer, intToCharArray);
        tempBuffer[stringSize] = '\0';
    }
    else
    {
        strcat(stringBuffer, intToCharArray);
        stringBuffer[stringSize] = '\0';
    }
}

void addByteToCharArray(uint8_t byte, uint8_t &stringSize, bool temp)
{
    if (temp)
    {
        tempBuffer[stringSize] = byte;
        stringSize++;
        tempBuffer[stringSize] = '\0';
    }
    else
    {
        stringBuffer[stringSize] = byte;
        stringSize++;
        stringBuffer[stringSize] = '\0';
    }
}

void appendText(const char *text, uint8_t &stringSize, bool temp)
{
    if (temp)
    {
        stringSize += strlen(text);
        strcat(tempBuffer, text);
        tempBuffer[stringSize] = '\0';
    }
    else
    {
        stringSize += strlen(text);
        strcat(stringBuffer, text);
        stringBuffer[stringSize] = '\0';
    }
}

void addSpaceToCharArray(uint8_t numberOfSpaces, uint8_t &stringSize, bool temp)
{
    if (temp)
    {
        for (int i=0; i<numberOfSpaces; i++)
            tempBuffer[stringSize+i] = ' ';

        stringSize += numberOfSpaces;
        tempBuffer[stringSize] = '\0';
    }
    else
    {
        for (int i=0; i<numberOfSpaces; i++)
            stringBuffer[stringSize+i] = ' ';

        stringSize += numberOfSpaces;
        stringBuffer[stringSize] = '\0';
    }
}

void startLine(bool temp)
{
    if (temp)
        tempBuffer[0] = '\0';
    else
        stringBuffer[0] = '\0';
}

void endLine(uint8_t stringSize, bool temp)
{
    if (temp)
        tempBuffer[stringSize] = '\0';
    else
        stringBuffer[stringSize] = '\0';
}

#endif