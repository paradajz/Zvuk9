/*

Copyright 2017 Igor Petrović

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

*/

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