/**
 * @file Application.h
 * @brief Main application class for BlueZ D-Bus sample application
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <chrono>
#include <thread>
#include <atomic>

#include "IDeviceManager.h"

#include "Adapter.h"
#include "AgentManager.h"
#include "Agent.h"
#include "DeviceManager.h"
#include "ObjectManagerProxy.h"
#include "ProfileManager.h"

#include "Logger.h"

// #define AGENT_MANAGER_PATH "/org/bluez/Agent1"
#define AGENT_MANAGER_PATH "/org/gokul"  ///< D-Bus path for agent registration
#define SPP_PATH "/org/gokul/spp"        ///< D-Bus path for SPP profile
#define SPP_UUID "00001101-0000-1000-8000-00805f9b34fb"  ///< Standard SPP UUID

/**
 * @class Application
 * @brief Main orchestrator class for the BlueZ D-Bus sample application
 * 
 * This class coordinates all Bluetooth operations by managing the lifecycle of
 * various subsystems including adapters, devices, agents, and profiles. It handles
 * the D-Bus event loop and provides the main entry points for Bluetooth functionality.
 */
class Application
{
public:
  /**
   * @brief Construct a new Application object
   * @param connection Reference to the D-Bus system bus connection
   * @param hcidevice HCI device identifier (e.g., "hci0")
   * @param deviceName Human-readable name for this device
   * @param deviceClass Device class string ("SMARTPHONE" or "HELMET")
   */
  Application(sdbus::IConnection &connection, std::string hcidevice, std::string deviceName, std::string deviceClass);
  
  /**
   * @brief Destroy the Application object and cleanup all resources
   */
  ~Application();

  /**
   * @brief Initialize and start all application subsystems
   * 
   * This method starts the agent manager, device manager, object manager,
   * registers the SPP profile, and begins the D-Bus event loop in a separate thread.
   */
  void StartApplication();

  /**
   * @brief Get reference to the device manager
   * @return Reference to IDeviceManager interface for device operations
   */
  IDeviceManager& GetDeviceManager();

  /**
   * @brief Start device discovery mode
   * 
   * Powers on the adapter, makes it discoverable and pairable, and configures
   * it for accepting incoming connections.
   */
  void StartDiscovery();
  
  /**
   * @brief Start scanning for nearby devices
   * 
   * Powers on the adapter and begins active scanning for discoverable devices.
   */
  void StartScan();
  
private:
  /**
   * @brief Run the D-Bus event loop asynchronously
   * @param connection Reference to the D-Bus connection to process events for
   * 
   * This method runs in a separate thread and processes D-Bus events continuously
   * until the application is stopped.
   */
  void runEventLoopAsync(sdbus::IConnection &connection);
  
private:
  sdbus::IConnection & m_connection;           ///< D-Bus system bus connection
  std::string m_hcidevice;                     ///< HCI device identifier (e.g., "hci0")
  std::string m_deviceName;                    ///< Human-readable device name
  std::string m_deviceClassStr;                ///< Device class string ("SMARTPHONE"/"HELMET")
  uint32_t m_deviceClass;                      ///< Numeric device class value
  std::unique_ptr<AgentManager> m_agentManager;///< Manages pairing agent registration
  std::unique_ptr<Agent> m_agent;              ///< Handles pairing requests and authentication
  std::unique_ptr<Adapter> m_adapter;          ///< Bluetooth adapter management
  std::unique_ptr<DeviceManager> m_deviceManager; ///< Device discovery and lifecycle
  std::unique_ptr<ObjectManagerProxy> m_objProxy; ///< D-Bus object monitoring
  std::unique_ptr<ProfileManager> m_profileManager; ///< Bluetooth profile management
  std::atomic<bool> m_running;                 ///< Application running state flag
  std::thread m_eventLoopThread;               ///< Thread for D-Bus event processing
};