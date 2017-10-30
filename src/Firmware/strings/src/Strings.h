#pragma once

///
/// \ingroup coreStrings
/// @{

#if defined(USE_STRINGS) || defined(__DOXYGEN__)

#ifndef STRING_BUFFER_SIZE
#error STRING_BUFFER_SIZE undefined
#endif

///
/// \brief String buffer used by all string functions. Must be declared externally.
///
extern char stringBuffer[STRING_BUFFER_SIZE];

///
/// \brief Temporary buffer. Must be declared externally.
///
extern char tempBuffer[STRING_BUFFER_SIZE_TEMP];

///
/// \brief Appends single number to stringBuffer.
/// @param [in] number              Number to append.
/// @param [in, out] stringSize     Current stringBuffer size. Size gets incremented after number is appended.
/// @param [in] temp                If set to true, tempBuffer is used, otherwise, stringBuffer is used.
///
void addNumberToCharArray(int32_t number, uint8_t &stringSize, bool temp = false);

///
/// \brief Appends raw byte value to stringBuffer.
/// @param [in] byte                Byte value to append.
/// @param [in, out] stringSize     Current stringBuffer size. Size gets incremented after byte is appended.
/// @param [in] temp                If set to true, tempBuffer is used, otherwise, stringBuffer is used.
///
void addByteToCharArray(uint8_t byte, uint8_t &stringSize, bool temp = false);

///
/// \brief Appends text to stringBuffer.
/// @param [in] text                Text to append.
/// @param [in, out] stringSize     Current stringBuffer size. Size gets incremented after text is appended.
/// @param [in] temp                If set to true, tempBuffer is used, otherwise, stringBuffer is used.
///
void appendText(const char *text, uint8_t &stringSize, bool temp = false);

///
/// \brief Appends space to stringBuffer.
/// @param [in] numberOfSpaces      Number of spaces to append to stringBuffer.
/// @param [in, out] stringSize     Current stringBuffer size. Size gets incremented after spaces are appended.
/// @param [in] temp                If set to true, tempBuffer is used, otherwise, stringBuffer is used.
///
void addSpaceToCharArray(uint8_t numberOfSpaces, uint8_t &stringSize, bool temp = false);

///
/// \brief Clears contents of stringBuffer.
/// @param [in] temp    If set to true, tempBuffer is used, otherwise, stringBuffer is used.
///
void startLine(bool temp = false);

///
/// \brief Ends stringBuffer by appending '\0' char to current index.
/// @param [in] stringSize  Current string size.
/// @param [in] temp        If set to true, tempBuffer is used, otherwise, stringBuffer is used.
///
void endLine(uint8_t stringSize, bool temp = false);

/// @}

#endif