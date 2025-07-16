/**
 * @file ProfileManagerProxy.h
 * @brief D-Bus proxy for BlueZ ProfileManager1 interface operations
 * @author Gokul
 * @date 2025
 */

#pragma once

#include "ProfileManager1-proxy-generated.hpp"

/**
 * @class ProfileManagerProxy
 * @brief D-Bus proxy wrapper for BlueZ ProfileManager1 interface
 * 
 * This class provides a C++ wrapper around the generated D-Bus proxy for
 * BlueZ ProfileManager1 interface. It handles profile registration and
 * unregistration operations with the BlueZ daemon, allowing applications
 * to implement custom Bluetooth profiles.
 */
class ProfileManagerProxy : public sdbus::ProxyInterfaces<org::bluez::ProfileManager1_proxy>
{
public:
  /**
   * @brief Construct a new Profile Manager Proxy object
   * @param connection Reference to D-Bus system bus connection
   */
  ProfileManagerProxy(sdbus::IConnection &connection);
  
  /**
   * @brief Destroy the Profile Manager Proxy object and cleanup D-Bus connections
   */
  ~ProfileManagerProxy();

  /**
   * @brief Register a profile with BlueZ ProfileManager
   * @param profile D-Bus object path of the profile implementation
   * @param UUID Service UUID for the profile (e.g., "00001101-0000-1000-8000-00805f9b34fb" for SPP)
   * @param options Map of profile configuration options (Name, Channel, etc.)
   * @throws sdbus::Error if profile registration fails
   */
  void RegisterProfile(const sdbus::ObjectPath& profile, 
                       const std::string& UUID, 
                       const std::map<std::string, sdbus::Variant>& options);

  /**
   * @brief Unregister a profile from BlueZ ProfileManager
   * @param profile D-Bus object path of the profile to unregister
   * @throws sdbus::Error if profile unregistration fails
   */
  void UnregisterProfile(const sdbus::ObjectPath& profile);

private:
  sdbus::IConnection &m_connection; ///< Reference to D-Bus connection
};

