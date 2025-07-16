/**
 * @file Logger.h
 * @brief Simple logging functionality for the Bluetooth application
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <cstdarg>

/**
 * @def LOG_STRING(x)
 * @brief Macro to convert std::string to C-style string for logging
 * @param x std::string object to convert
 * @return C-style string pointer via c_str()
 */
#define LOG_STRING(x) (x.c_str())

/**
 * @brief Log a formatted message to the console
 * 
 * This function provides printf-style formatted logging with timestamp
 * and automatic newline handling. It's thread-safe and can be called
 * from multiple threads simultaneously.
 * 
 * @param fmt Format string (printf-style)
 * @param ... Variable arguments matching the format string
 * 
 * @example
 * Log("Device connected: %s", deviceName.c_str());
 * Log("Connection status: %d", connected);
 */
void Log(const char* fmt, ...);
