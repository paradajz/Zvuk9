#include <cstdarg>
#include <cstdio>
#include "Logger.h"

void Logger::bindInterface(StreamWriter& interface)
{
    this->interface = &interface;
}

bool Logger::write(logLevel_t level, const char* message, ...)
{
    if (interface == nullptr)
        return false;

    va_list args;
    va_start(args, message);
    vsnprintf(logBuffer, sizeof(logBuffer), message, args);
    va_end(args);

    if (level != logLevel_t::AMOUNT)
    {
        if (!interface->write(logLevelString[static_cast<uint8_t>(level)]))
            return false;
    }

    if (!interface->write(logBuffer))
        return false;

    if (lineEnding != lineEnding_t::AMOUNT)
    {
        if (!interface->write(lineEndingString[static_cast<uint8_t>(lineEnding)]))
            return false;
    }

    return true;
}