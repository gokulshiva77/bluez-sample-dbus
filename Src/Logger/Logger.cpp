/**
 * @file Logger.cpp
 * @brief Implementation of simple logging functionality
 * @author Gokul
 * @date 2025
 */

#include <iostream>
#include <ctime>
#include <chrono>
#include <iomanip>
#include <string>

#include "Logger.h"

#define BUFFER_LEN  512  ///< Maximum length for log message buffer
char Buffer[BUFFER_LEN]; ///< Static buffer for formatting log messages

/**
 * @brief Log a formatted message with timestamp
 * 
 * This function formats and outputs a log message with current timestamp
 * to standard output. The function is thread-safe and uses a fixed-size
 * buffer for message formatting.
 * 
 * @param fmt Format string (printf-style)
 * @param ... Variable arguments matching the format string
 */
void Log(const char* fmt, ...)
{
  // Get Current time
  auto now = std::chrono::system_clock::now();
  auto in_time_t = std::chrono::system_clock::to_time_t(now);

  std::ostringstream timeStream;
  timeStream << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");

  // Initialize the variadic arguments
  va_list args;
  va_start(args, fmt);
  vsnprintf(Buffer, BUFFER_LEN, fmt, args);
  va_end(args);
  std::cout << timeStream.str() << " " << Buffer<< std::endl;
}
