/**
 * @file Utilities.h
 * @brief Utility functions for D-Bus variant handling
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <sdbus-c++/sdbus-c++.h>
#include <iostream>
#include <string>
#include <vector>

/**
 * @brief Extract typed value from D-Bus variant
 * 
 * This template function safely extracts a value of the specified type
 * from a D-Bus variant. It provides error handling and type safety for
 * D-Bus property value extraction.
 * 
 * @tparam T The type to extract from the variant
 * @param variant The D-Bus variant containing the value
 * @return The extracted value of type T
 * @throws std::exception if the variant doesn't contain the expected type
 * 
 * @example
 * std::string name = getFromSVariant<std::string>(nameVariant);
 * bool connected = getFromSVariant<bool>(connectedVariant);
 */
template<typename T>
T getFromSVariant(const sdbus::Variant& variant)
{
    try
    {
        return variant.get<T>();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
        throw; // Re-throw the exception to ensure the caller is aware of the error
    }
}