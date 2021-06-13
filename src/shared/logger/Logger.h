#pragma once

#include <cstdio>

class Logger
{
    public:
    enum class logLevel_t : uint8_t
    {
        info,
        warning,
        error,
        AMOUNT
    };

    enum class lineEnding_t : uint8_t
    {
        none,
        lf,      ///<    Line feed
        cr,      ///<    Carriage return
        crlf,    ///<    Carriage return + line feed
        AMOUNT
    };

    class StreamWriter
    {
        public:
        virtual bool write(const char* message) = 0;
    };

    Logger(lineEnding_t lineEnding)
    {
        this->lineEnding = lineEnding;
    }

    void bindInterface(StreamWriter& interface);
    bool write(logLevel_t level, const char* message, ...);

    private:
    static constexpr size_t bufferSize                                               = 512;
    StreamWriter*           interface                                                = nullptr;
    const char*             logLevelString[static_cast<uint8_t>(logLevel_t::AMOUNT)] = {
        "[INFO] ",
        "[WARNING] ",
        "[ERROR] ",
    };

    const char* lineEndingString[static_cast<uint8_t>(lineEnding_t::AMOUNT)] = {
        "",
        "\n",
        "\r",
        "\r\n",
    };

    char         logBuffer[bufferSize];
    lineEnding_t lineEnding = lineEnding_t::AMOUNT;
};

extern Logger logger;

#define LOG(...)           logger.write(Logger::logLevel_t::AMOUNT, __VA_ARGS__)
#define LOG_OVERWRITE(...) logger.overwrite(Logger::logLevel_t::AMOUNT, __VA_ARGS__)
#define LOG_INFO(...)      logger.write(Logger::logLevel_t::info, __VA_ARGS__)
#define LOG_WARNING(...)   logger.write(Logger::logLevel_t::warning, __VA_ARGS__)
#define LOG_ERROR(...)     logger.write(Logger::logLevel_t::error, __VA_ARGS__)