/**
 * @file DeviceHelper.h
 * @brief Constants, structures, and definitions for BlueZ Device1 interface
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <cstdbool>
#include <map>

// BlueZ Device1 interface property names
#define DEVICE_PROPERTY_Address "Address"                   ///< MAC address of the device
#define DEVICE_PROPERTY_AddressType "AddressType"           ///< Address type (public/random)
#define DEVICE_PROPERTY_Name "Name"                         ///< Device name
#define DEVICE_PROPERTY_UUIDs "UUIDs"                       ///< List of supported service UUIDs
#define DEVICE_PROPERTY_Paired "Paired"                     ///< Pairing status
#define DEVICE_PROPERTY_Connected "Connected"               ///< Connection status
#define DEVICE_PROPERTY_Trusted "Trusted"                   ///< Trusted status
#define DEVICE_PROPERTY_Blocked "Blocked"                   ///< Blocked status
#define DEVICE_PROPERTY_Alias "Alias"                       ///< User-friendly alias
#define DEVICE_PROPERTY_Adapter "Adapter"                   ///< Adapter object path
#define DEVICE_PROPERTY_LegacyPairing "LegacyPairing"       ///< Legacy pairing support
#define DEVICE_PROPERTY_ServiceData "ServiceData"           ///< Service-specific data
#define DEVICE_PROPERTY_ServicesResolved "ServicesResolved" ///< Service discovery status
#define DEVICE_PROPERTY_Icon "Icon"                         ///< Device icon name
#define DEVICE_PROPERTY_Class "Class"                       ///< Device class
#define DEVICE_PROPERTY_ManufacturerData "ManufacturerData" ///< Manufacturer-specific data

/**
 * @struct DeviceProperties
 * @brief Structure containing all device properties from BlueZ Device1 interface
 */
typedef struct {
  std::string Address;                                              ///< MAC address
  std::string AddressType;                                          ///< Address type
  std::string Name;                                                 ///< Device name
  uint32_t Class;                                                   ///< Device class
  std::vector<std::string> UUIDs;                                   ///< Supported service UUIDs
  bool Paired;                                                      ///< Pairing status
  bool Connected;                                                   ///< Connection status
  bool Trusted;                                                     ///< Trusted status
  bool Blocked;                                                     ///< Blocked status
  std::string Alias;                                                ///< User alias
  std::string AdapterPath;                                          ///< Adapter D-Bus path
  bool LegacyPairing;                                               ///< Legacy pairing support
  std::map<std::string, std::map<int, std::string>> ServiceData;   ///< Service data
  bool ServicesResolved;                                            ///< Service discovery complete
  std::string Icon;                                                 ///< Device icon
  std::map<uint16_t, std::map<int, std::string>> ManufacturerData; ///< Manufacturer data
} DeviceProperties;

/**
 * @enum BluetoothMajorDeviceClass
 * @brief Major device class values from Bluetooth specification
 */
typedef enum {
  Miscellaneous = 0x00,          ///< Miscellaneous devices
  Computer = 0x01,               ///< Computer devices
  Phone = 0x02,                  ///< Phone devices
  LAN_NetworkAccessPoint = 0x03, ///< LAN/Network access point
  AudioVideo = 0x04,             ///< Audio/Video devices
  Peripheral = 0x05,             ///< Peripheral devices (mouse, keyboard)
  Imaging = 0x06,                ///< Imaging devices (printer, scanner)
  Wearable = 0x07,               ///< Wearable devices
  Toy = 0x08,                    ///< Toy devices
  Health = 0x09,                 ///< Health devices
  Uncategorized = 0x1F           ///< Uncategorized devices
}BluetoothMajorDeviceClass;

/**
 * @struct BluetoothDeviceClass
 * @brief Bluetooth device class bit field structure
 * 
 * Represents the 24-bit device class as defined in the Bluetooth specification.
 * Provides parsing and interpretation of device class information.
 */
struct BluetoothDeviceClass {
  uint32_t format_byte : 2;        ///< Format type (always 00 for current version)
  uint32_t minor_device_class : 6; ///< Minor device class (device-specific)
  uint32_t major_device_class : 5; ///< Major device class (see BluetoothMajorDeviceClass)
  uint32_t service_class : 11;     ///< Service class bit field
  uint32_t reserved : 8;           ///< Reserved bits

  /**
   * @brief Parse device class from 32-bit value
   * @param value 32-bit device class value from BlueZ
   * @return Parsed BluetoothDeviceClass structure
   */
  static BluetoothDeviceClass from_uint32_t(uint32_t value) {
      BluetoothDeviceClass device_class;
      device_class.format_byte = value & 0x3;
      device_class.minor_device_class = (value >> 2) & 0x3F;
      device_class.major_device_class = (value >> 8) & 0xFF;
      device_class.service_class = (value >> 16) & 0x7FF;
      device_class.reserved = (value >> 27) & 0x1F;
      return device_class;
  }
};
