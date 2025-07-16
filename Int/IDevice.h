/**
 * @file IDevice.h
 * @brief Interface for Bluetooth device operations and event handling
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <cstdbool>

#include "DeviceHelper.h"

/**
 * @class IDevice
 * @brief Abstract interface for Bluetooth device management and operations
 * 
 * This interface defines the contract for Bluetooth device operations including
 * connection management, pairing, profile operations, and property change notifications.
 * Implementations handle D-Bus communication with BlueZ device interfaces.
 */
class IDevice
{
public:
  /**
   * @brief Get the D-Bus object path for this device
   * @return String containing the device's D-Bus path
   */
  virtual std::string GetPath() = 0;

  /**
   * @brief Initiate a connection to this device
   * @throws sdbus::Error if connection fails
   */
  virtual void Connect() = 0;
  
  /**
   * @brief Disconnect from this device
   * @throws sdbus::Error if disconnection fails
   */
  virtual void Disconnect() = 0;
  
  /**
   * @brief Connect to a specific profile on this device
   * @param uuid UUID of the profile to connect to (e.g., SPP UUID)
   * @throws sdbus::Error if profile connection fails
   */
  virtual void ConnectProfile(std::string uuid) = 0;
  
  /**
   * @brief Disconnect from a specific profile on this device
   * @param uuid UUID of the profile to disconnect from
   * @throws sdbus::Error if profile disconnection fails
   */
  virtual void DisconnectProfile(std::string uuid) = 0;
  
  /**
   * @brief Initiate pairing with this device
   * @throws sdbus::Error if pairing fails or is already paired
   */
  virtual void Pair() = 0;
  
  /**
   * @brief Cancel an ongoing pairing operation
   * @throws sdbus::Error if no pairing is in progress
   */
  virtual void CancelPairing() = 0;

  /**
   * @brief Handle device properties change notification
   * @param properties Updated device properties structure
   */
  virtual void PropertiesChanged(DeviceProperties properties) = 0;
  
  /**
   * @brief Get current device properties
   * @return DeviceProperties structure containing all current properties
   */
  virtual DeviceProperties GetProperties() = 0;

  /**
   * @brief Callback for device MAC address changes
   * @param value New MAC address of the device
   */
  virtual void AddressChanged(std::string value) = 0;
  
  /**
   * @brief Callback for device address type changes
   * @param value New address type (e.g., "public", "random")
   */
  virtual void AddressTypeChanged(std::string value) = 0;
  
  /**
   * @brief Callback for device name changes
   * @param value New device name
   */
  virtual void NameChanged(std::string value) = 0;
  
  /**
   * @brief Callback for device icon changes
   * @param value New device icon identifier
   */
  virtual void IconChanged(std::string value) = 0;
  
  /**
   * @brief Callback for device class changes
   * @param value New device class value (24-bit integer)
   */
  virtual void ClassChanged(uint32_t value) = 0;
  
  /**
   * @brief Callback for supported UUIDs changes
   * @param value List of supported service UUIDs
   */
  virtual void UUIDsChanged(std::vector<std::string> value) = 0;
  
  /**
   * @brief Callback for paired state changes
   * @param value True if device is paired, false otherwise
   */
  virtual void PairedChanged(bool value) = 0;
  
  /**
   * @brief Callback for connection state changes
   * @param value True if device is connected, false otherwise
   */
  virtual void ConnectedChanged(bool value) = 0;
  
  /**
   * @brief Callback for trusted state changes
   * @param value True if device is trusted, false otherwise
   */
  virtual void TrustedChanged(bool value) = 0;
  
  /**
   * @brief Callback for blocked state changes
   * @param value True if device is blocked, false otherwise
   */
  virtual void BlockedChanged(bool value) = 0;
  
  /**
   * @brief Callback for device alias changes
   * @param value New device alias (friendly name)
   */
  virtual void AliasChanged(std::string value) = 0;
  
  /**
   * @brief Callback for adapter path changes
   * @param value D-Bus path of the associated adapter
   */
  virtual void AdapterChanged(std::string value) = 0;
  
  /**
   * @brief Callback for legacy pairing state changes
   * @param value True if legacy pairing is required, false otherwise
   */
  virtual void LegacyPairingChanged(bool value) = 0;
  
  /**
   * @brief Callback for manufacturer data changes
   * @param value Map of manufacturer ID to data
   */
  virtual void ManufacturerDataChanged(std::map<uint16_t, std::map<int, std::string>> value) = 0;
  
  /**
   * @brief Callback for service data changes
   * @param value Map of service UUID to data
   */
  virtual void ServiceDataChanged(std::map<std::string, std::map<int, std::string>> value) = 0;
  
  /**
   * @brief Callback for services resolved state changes
   * @param value True if services are resolved, false otherwise
   */
  virtual void ServicesResolvedChanged(bool value) = 0;

  /**
   * @brief Virtual destructor for proper inheritance cleanup
   */
  virtual ~IDevice() = default;
};
