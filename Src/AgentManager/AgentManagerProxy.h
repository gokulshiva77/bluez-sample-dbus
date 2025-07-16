/**
 * @file AgentManagerProxy.h
 * @brief D-Bus proxy for BlueZ AgentManager1 interface operations
 * @author Gokul
 * @date 2025
 */

#pragma once

#include "AgentManager1-proxy-generated.hpp"

/**
 * @class AgentManagerProxy
 * @brief D-Bus proxy wrapper for BlueZ AgentManager1 interface
 * 
 * This class provides a C++ wrapper around the generated D-Bus proxy for
 * BlueZ AgentManager1 interface. It handles agent registration, unregistration,
 * and default agent management for Bluetooth authentication operations.
 */
class AgentManagerProxy : public sdbus::ProxyInterfaces<org::bluez::AgentManager1_proxy>
{
public:
  /**
   * @brief Construct a new Agent Manager Proxy object
   * @param connection Reference to D-Bus system bus connection
   */
  AgentManagerProxy(sdbus::IConnection &connection);
  
  /**
   * @brief Destroy the Agent Manager Proxy object and cleanup D-Bus connections
   */
  ~AgentManagerProxy();

  /**
   * @brief Register an agent with the agent manager
   * @param agent D-Bus object path of the agent to register
   * @param capability Capability string (e.g., "KeyboardDisplay", "NoInputNoOutput")
   * @throws sdbus::Error if registration fails
   */
  void RegisterAgent(const sdbus::ObjectPath& agent, const std::string& capability);
  
  /**
   * @brief Unregister an agent from the agent manager
   * @param agent D-Bus object path of the agent to unregister
   * @throws sdbus::Error if unregistration fails
   */
  void UnregisterAgent(const sdbus::ObjectPath& agent);
  
  /**
   * @brief Request to make an agent the default agent
   * @param agent D-Bus object path of the agent to set as default
   * @throws sdbus::Error if request fails
   */
  void RequestDefaultAgent(const sdbus::ObjectPath& agent);

  private:
    sdbus::IConnection &m_connection; ///< Reference to D-Bus connection
};