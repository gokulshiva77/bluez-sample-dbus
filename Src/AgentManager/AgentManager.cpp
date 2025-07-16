
/**
 * @file AgentManager.cpp
 * @brief Implementation of agent manager for handling Bluetooth authentication
 * @author Gokul
 * @date 2025
 */

#include "AgentManager.h"

#include "Logger.h"

#define TAG "AgentManager::" ///< Tag for logging messages

/**
 * @brief Construct a new Agent Manager object
 * 
 * Initializes the agent manager, registers the agent with BlueZ,
 * and requests it to be the default agent for authentication operations.
 * 
 * @param connection Reference to D-Bus system bus connection
 * @param path D-Bus object path for the agent
 */
AgentManager::AgentManager(sdbus::IConnection &connection, std::string path):
m_running(true),
m_path(path),
m_agentManagerProxy(connection)
{
  Log("%s%s Path - %s", TAG,__func__, LOG_STRING(path));
  m_agentManagerProxy.RegisterAgent(sdbus::ObjectPath(m_path), "KeyboardDisplay");
  m_agentManagerProxy.RequestDefaultAgent(sdbus::ObjectPath(m_path)); 
}

AgentManager::~AgentManager()
{
  Log("%s%s", TAG,__func__);
  m_agentManagerProxy.UnregisterAgent(sdbus::ObjectPath(m_path));
  m_running = false;
  if(m_eventLoopThread.joinable()) {
    m_eventLoopThread.join();
  }
}

void AgentManager::StartLooping()
{
  Log("%s%s", TAG,__func__);
  m_eventLoopThread = std::thread(&AgentManager::RunEventLoop, this);
}

void AgentManager::RunEventLoop()
{
  Log("%s%s", TAG,__func__);
  while(m_running) {
    std::this_thread::sleep_for(std::chrono::seconds(1));
  }
  
}
