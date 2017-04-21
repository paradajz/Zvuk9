#pragma once

#include "Config.h"

///
/// \brief Various functions used to manipulate strings more easily.
/// \defgroup strings_core Strings
/// \ingroup core
/// @{
///

///
/// \brief Appends single number to stringBuffer.
/// @param [in] number Number to append.
/// @param [in, out] stringSize Current stringBuffer size. Size gets incremented after number is appended.
///
void addNumberToCharArray(int32_t number, uint8_t &stringSize, bool temp = false);

///
/// \brief Appends raw byte value to stringBuffer.
/// @param [in] byte Byte value to append.
/// @param [in, out] stringSize Current stringBuffer size. Size gets incremented after byte is appended.
///
void addByteToCharArray(uint8_t byte, uint8_t &stringSize, bool temp = false);

///
/// \brief Appends text to stringBuffer.
/// @param [in] text Text to append.
/// @param [in, out] stringSize Current stringBuffer size. Size gets incremented after text is appended.
///
void appendText(const char *text, uint8_t &stringSize, bool temp = false);

///
/// \brief Appends space to stringBuffer.
/// @param [in] numberOfSpaces  Number of spaces to append to stringBuffer.
/// @param [in, out] stringSize Current stringBuffer size. Size gets incremented after spaces are appended.
///
void addSpaceToCharArray(uint8_t numberOfSpaces, uint8_t &stringSize, bool temp = false);

///
/// \brief Clears contents of stringBuffer.
///
void startLine(bool temp = false);

///
/// \brief Ends stringBuffer by appending '\0' char to current index.
///
void endLine(uint8_t &stringSize, bool temp = false);

/// @}