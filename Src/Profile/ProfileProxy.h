/**
 * @file ProfileProxy.h
 * @brief D-Bus adaptor for BlueZ Profile1 interface implementation
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <memory>

#include "Profile1-adapter-generated.hpp"

#include "SPPHandler.h"

/**
 * @class ProfileProxy
 * @brief D-Bus adaptor for implementing BlueZ Profile1 interface
 * 
 * This class implements the BlueZ Profile1 interface to handle Bluetooth
 * profile connections, particularly for Serial Port Profile (SPP). It acts
 * as a D-Bus service that BlueZ calls when profile connections are
 * established or need to be released.
 */
class ProfileProxy : public sdbus::AdaptorInterfaces<org::bluez::Profile1_adaptor>
{
public:
  /**
   * @brief Construct a new Profile Proxy object
   * @param connection Reference to D-Bus system bus connection
   * @param profilePath D-Bus object path for this profile instance
   */
  ProfileProxy(sdbus::IConnection &connection, std::string profilePath);
  
  /**
   * @brief Destroy the Profile Proxy object and cleanup resources
   */
  ~ProfileProxy();

protected:
  /**
   * @brief Release profile resources (BlueZ Profile1 interface method)
   * 
   * Called by BlueZ when the profile is being unregistered or when
   * the system is shutting down. Cleans up any active connections
   * and resources.
   */
  void Release() override;
  
  /**
   * @brief Handle new profile connection (BlueZ Profile1 interface method)
   * @param device D-Bus object path of the connecting device
   * @param fd Unix file descriptor for the connection
   * @param fd_properties Map of connection properties
   * 
   * Called by BlueZ when a new profile connection is established.
   * Creates an SPPHandler to manage the connection.
   */
  void NewConnection(const sdbus::ObjectPath& device, 
                     const sdbus::UnixFd& fd, 
                     const std::map<std::string, sdbus::Variant>& fd_properties) override;
                     
  /**
   * @brief Handle disconnection request (BlueZ Profile1 interface method)
   * @param device D-Bus object path of the device to disconnect
   * 
   * Called by BlueZ when a profile connection should be disconnected.
   * Cleans up the associated SPPHandler and connection resources.
   */
  void RequestDisconnection(const sdbus::ObjectPath& device) override;

private:
  sdbus::IConnection &m_connection;       ///< Reference to D-Bus connection
  std::string m_profilePath;              ///< D-Bus object path for this profile
  std::unique_ptr<SPPHandler> m_spp;      ///< SPP connection handler
};
