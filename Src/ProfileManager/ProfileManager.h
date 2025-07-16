/**
 * @file ProfileManager.h
 * @brief Manager for Bluetooth profile registration and lifecycle
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <memory>

#include "ProfileManagerProxy.h"

#include "ProfileProxy.h"

/**
 * @class ProfileManager
 * @brief Manages Bluetooth profile registration with BlueZ
 * 
 * This class handles the registration and management of Bluetooth profiles
 * with the BlueZ profile manager. It creates and manages ProfileProxy
 * instances and handles their lifecycle including registration and
 * unregistration with the BlueZ daemon.
 */
class ProfileManager
{
public:
  /**
   * @brief Construct a new Profile Manager object
   * @param connection Reference to D-Bus system bus connection
   */
  ProfileManager(sdbus::IConnection &connection);
  
  /**
   * @brief Destroy the Profile Manager object and cleanup resources
   * 
   * Unregisters any active profiles and cleans up resources.
   */
  ~ProfileManager();

  /**
   * @brief Register a Bluetooth profile with BlueZ
   * @param profile D-Bus object path for the profile implementation
   * @param UUID Service UUID for the profile (e.g., SPP UUID)
   * @param options Map of profile options and configuration
   * @throws sdbus::Error if profile registration fails
   */
  void RegisterProfile(const sdbus::ObjectPath& profile, 
                       const std::string& UUID, 
                       const std::map<std::string, sdbus::Variant>& options);
                       
  /**
   * @brief Unregister a Bluetooth profile from BlueZ
   * @param profile D-Bus object path of the profile to unregister
   * @throws sdbus::Error if profile unregistration fails
   */
  void UnregisterProfile(const sdbus::ObjectPath& profile);

private:
  sdbus::IConnection &m_connection;              ///< Reference to D-Bus connection
  ProfileManagerProxy m_profileManagerProxy;    ///< Proxy for BlueZ ProfileManager1 interface
  std::unique_ptr<ProfileProxy> m_profileProxy; ///< Profile implementation instance
};
