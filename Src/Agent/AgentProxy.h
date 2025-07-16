/**
 * @file AgentProxy.h
 * @brief D-Bus adaptor for BlueZ Agent1 interface implementation
 * @author Gokul
 * @date 2025
 */

#pragma once

#include "Agent1-adapter-generated.hpp"

#include "IAgent.h"

/**
 * @class AgentProxy
 * @brief D-Bus adaptor for implementing BlueZ Agent1 interface
 * 
 * This class implements the BlueZ Agent1 interface to handle Bluetooth
 * authentication requests. It acts as a D-Bus service that BlueZ calls
 * during pairing operations, including PIN code requests, passkey display,
 * confirmation requests, and authorization operations.
 */
class AgentProxy : public sdbus::AdaptorInterfaces<org::bluez::Agent1_adaptor>
{
public:
  /**
   * @brief Construct a new Agent Proxy object
   * @param connection Reference to D-Bus system bus connection
   * @param path D-Bus object path for this agent instance
   * @param agent Reference to IAgent callback interface
   */
  AgentProxy(sdbus::IConnection &connection, std::string path, IAgent &agent);
  
  /**
   * @brief Destroy the Agent Proxy object and cleanup D-Bus connections
   */
  ~AgentProxy();
  
protected:
  /**
   * @brief Release agent resources (BlueZ Agent1 interface method)
   * 
   * Called by BlueZ when the agent is being unregistered or when
   * the system is shutting down.
   */
  void Release() override;
  
  /**
   * @brief Request PIN code from user (BlueZ Agent1 interface method)
   * @param arg0 D-Bus object path of the device requesting PIN
   * @return PIN code string entered by user
   * 
   * Called during legacy pairing when a PIN code is required.
   */
  std::string RequestPinCode(const sdbus::ObjectPath& arg0) override;
  
  /**
   * @brief Display PIN code to user (BlueZ Agent1 interface method)
   * @param arg0 D-Bus object path of the device
   * @param arg1 PIN code to display
   * 
   * Called when the device provides a PIN code that should be displayed to the user.
   */
  void DisplayPinCode(const sdbus::ObjectPath& arg0, const std::string& arg1) override;
  
  /**
   * @brief Request passkey from user (BlueZ Agent1 interface method)
   * @param arg0 D-Bus object path of the device requesting passkey
   * @return Numeric passkey entered by user
   * 
   * Called during pairing when a numeric passkey is required.
   */
  uint32_t RequestPasskey(const sdbus::ObjectPath& arg0) override;
  
  /**
   * @brief Display passkey to user (BlueZ Agent1 interface method)
   * @param arg0 D-Bus object path of the device
   * @param arg1 Passkey to display
   * @param arg2 Number of characters entered so far
   * 
   * Called when a passkey should be displayed to the user during pairing.
   */
  void DisplayPasskey(const sdbus::ObjectPath& arg0, const uint32_t& arg1, const uint16_t& arg2) override;
  
  /**
   * @brief Request confirmation of passkey (BlueZ Agent1 interface method)
   * @param arg0 D-Bus object path of the device
   * @param arg1 Passkey to confirm
   * 
   * Called when the user needs to confirm that the displayed passkey
   * matches the one shown on the device.
   */
  void RequestConfirmation(const sdbus::ObjectPath& arg0, const uint32_t& arg1) override;
  
  /**
   * @brief Request authorization for connection (BlueZ Agent1 interface method)
   * @param arg0 D-Bus object path of the device requesting authorization
   * 
   * Called when a device requests authorization to connect.
   */
  void RequestAuthorization(const sdbus::ObjectPath& arg0) override;
  
  /**
   * @brief Authorize specific service access (BlueZ Agent1 interface method)
   * @param arg0 D-Bus object path of the device
   * @param arg1 UUID of the service requesting authorization
   * 
   * Called when a device requests access to a specific service.
   */
  void AuthorizeService(const sdbus::ObjectPath& arg0, const std::string& arg1) override;
  
  /**
   * @brief Cancel ongoing authentication operation (BlueZ Agent1 interface method)
   * 
   * Called by BlueZ to cancel any ongoing authentication request.
   */
  void Cancel() override;
  
private:
  sdbus::IConnection &m_connection; ///< Reference to D-Bus connection
  IAgent &m_agent;                  ///< Reference to callback interface
};
