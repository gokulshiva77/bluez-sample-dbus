/**
 * @file AgentManager.h
 * @brief Agent manager for handling Bluetooth authentication operations
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <thread>
#include <atomic>

#include "AgentManagerProxy.h"

/**
 * @class AgentManager
 * @brief Manages Bluetooth agent operations and authentication requests
 * 
 * This class handles authentication operations for Bluetooth devices including
 * pairing requests, authorization, and other security-related operations.
 * It runs in its own event loop thread to handle asynchronous D-Bus events.
 */
class AgentManager
{
public:
  /**
   * @brief Construct a new Agent Manager object
   * @param connection Reference to D-Bus system bus connection
   * @param path D-Bus object path for the agent manager
   */
  AgentManager(sdbus::IConnection &connection, std::string path);
  
  /**
   * @brief Destroy the Agent Manager object and cleanup resources
   */
  ~AgentManager();
  
  /**
   * @brief Start the agent manager event loop
   * 
   * Begins processing authentication events in a separate thread. This must be called
   * after construction to enable agent operations.
   */
  void StartLooping();

private:
  /**
   * @brief Main event loop function executed in separate thread
   * 
   * Processes D-Bus events and authentication requests continuously until
   * the manager is shut down.
   */
  void RunEventLoop();
  
private:
  std::string m_path;                    ///< D-Bus object path
  AgentManagerProxy m_agentManagerProxy; ///< Proxy for D-Bus communication
  std::atomic<bool> m_running;           ///< Flag to control event loop execution
  std::thread m_eventLoopThread;         ///< Thread for running the event loop
};