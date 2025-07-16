/**
 * @file DeviceProxy.h
 * @brief D-Bus proxy for BlueZ Device1 interface operations
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <string>
#include <vector>
#include <map>
#include <cstdint>
#include <cstdbool>

#include <sdbus-c++/sdbus-c++.h>
#include "Device1-proxy-generated.hpp"

#include "IDevice.h"

#include "DeviceHelper.h"

/**
 * @class DeviceProxy
 * @brief D-Bus proxy wrapper for BlueZ Device1 interface
 * 
 * This class provides a C++ wrapper around the generated D-Bus proxy for
 * BlueZ Device1 interface. It handles property change notifications,
 * method calls, and property access for Bluetooth devices. The class
 * automatically forwards property changes to the registered IDevice callback.
 */
class DeviceProxy : sdbus::ProxyInterfaces<org::bluez::Device1_proxy, sdbus::Properties_proxy>
{
public:
  /**
   * @brief Construct a new Device Proxy object
   * @param connection Reference to D-Bus system bus connection
   * @param device Reference to IDevice callback interface
   * @param devicePath D-Bus object path for the device
   */
  DeviceProxy(sdbus::IConnection &connection,IDevice &device, std::string devicePath);
  
  /**
   * @brief Destroy the Device Proxy object and cleanup D-Bus connections
   */
  ~DeviceProxy();

  // Device operation methods
  /**
   * @brief Connect to the device
   * @throws sdbus::Error if connection operation fails
   */
  void Connect();
  
  /**
   * @brief Disconnect from the device
   * @throws sdbus::Error if disconnection operation fails
   */
  void Disconnect();
  
  /**
   * @brief Connect to a specific profile
   * @param uuid UUID of the profile to connect to
   * @throws sdbus::Error if profile connection fails
   */
  void ConnectProfile(std::string uuid);
  
  /**
   * @brief Disconnect from a specific profile
   * @param uuid UUID of the profile to disconnect from
   * @throws sdbus::Error if profile disconnection fails
   */
  void DisconnectProfile(std::string uuid);
  
  /**
   * @brief Initiate pairing with the device
   * @throws sdbus::Error if pairing operation fails
   */
  void Pair();
  
  /**
   * @brief Cancel ongoing pairing operation
   * @throws sdbus::Error if cancellation fails
   */
  void CancelPairing();

  // Property getter methods
  std::string GetAddress();                                    ///< Get device MAC address
  std::string GetAddressType();                               ///< Get device address type (public/random)
  std::string GetName();                                       ///< Get device name
  std::string GetIcon();                                       ///< Get device icon name
  uint32_t GetClass();                                         ///< Get device class
  std::vector<std::string> GetUUIDs();                        ///< Get list of supported service UUIDs
  bool GetPaired();                                            ///< Get pairing status
  bool GetConnected();                                         ///< Get connection status
  bool GetTrusted();                                           ///< Get trusted status
  bool GetBlocked();                                           ///< Get blocked status
  std::string GetAlias();                                      ///< Get device alias
  sdbus::ObjectPath GetAdapter();                             ///< Get adapter object path
  bool GetLegacyPairing();                                     ///< Get legacy pairing status
  std::map<std::string, sdbus::Variant> GetServiceData();     ///< Get service data
  bool GetServicesResolved();                                  ///< Get services resolved status
  
  // Property setter methods
  /**
   * @brief Set device trusted status
   * @param value True to trust the device, false otherwise
   * @throws sdbus::Error if setting property fails
   */
  void SetTrusted(bool value);
  
  /**
   * @brief Set device blocked status
   * @param value True to block the device, false otherwise
   * @throws sdbus::Error if setting property fails
   */
  void SetBlocked(bool value);
  
  /**
   * @brief Set device alias
   * @param value New alias for the device
   * @throws sdbus::Error if setting property fails
   */
  void SetAlias(const std::string &value);

  /**
   * @brief Get all device properties at once
   * @return DeviceProperties structure containing all current property values
   * @throws sdbus::Error if property retrieval fails
   */
  DeviceProperties GetProperties();
  
  /**
   * @brief Handle D-Bus property change notifications
   * @param interface_name Name of the D-Bus interface (org.bluez.Device1)
   * @param changed_properties Map of changed properties and their new values
   * @param invalidated_properties List of properties that became invalid
   */
  void onPropertiesChanged(const std::string& interface_name, 
    const std::map<std::string, sdbus::Variant>& changed_properties, 
    const std::vector<std::string>& invalidated_properties) override;

  private:
    sdbus::IConnection &m_connection; ///< Reference to D-Bus connection
    IDevice &m_device;                ///< Reference to callback interface
    std::string m_devicePath;         ///< D-Bus object path for this device
};
