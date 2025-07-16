/**
 * @file Agent.h
 * @brief Bluetooth authentication agent implementation
 * @author Gokul
 * @date 2025
 */

#pragma once

#include "IAgent.h"
#include "IDeviceManager.h"
#include "AgentProxy.h"

/**
 * @class Agent
 * @brief Concrete implementation of Bluetooth authentication agent
 * 
 * This class implements the IAgent interface to handle Bluetooth authentication
 * requests from BlueZ. It processes pairing confirmations and other authentication
 * events, working with the device manager to coordinate device operations.
 */
class Agent : public IAgent
{
public:
  /**
   * @brief Construct a new Agent object
   * @param connection Reference to D-Bus system bus connection
   * @param Path D-Bus object path for this agent instance
   * @param deviceManager Reference to device manager for coordination
   */
  Agent(sdbus::IConnection &connection, std::string Path, IDeviceManager &deviceManager);
  
  /**
   * @brief Destroy the Agent object and cleanup resources
   */
  ~Agent();

  /**
   * @brief Handle pairing confirmation request
   * @param path D-Bus object path of the device requesting confirmation
   * 
   * Called by BlueZ when a device requires confirmation for pairing.
   * This method should prompt the user or automatically confirm based
   * on application policy.
   */
  void RequestConfirmation(std::string path) override;
  
private:
  sdbus::IConnection &m_connection;  ///< Reference to D-Bus connection
  AgentProxy m_agentProxy;           ///< D-Bus proxy for agent operations
  IDeviceManager &m_deviceManager;  ///< Reference to device manager
};

