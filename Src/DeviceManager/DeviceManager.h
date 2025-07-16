/**
 * @file DeviceManager.h
 * @brief Device manager class for handling Bluetooth device lifecycle
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <map>
#include <memory>
#include <atomic>
#include <thread>
#include <mutex>
#include <queue>
#include <condition_variable>

#include <sdbus-c++/sdbus-c++.h>

#include "IDeviceManager.h"

#include "Device.h"

/// Type alias for mapping MAC addresses to Device objects
typedef std::map<std::string, std::shared_ptr<Device>> DevicesMap;

/**
 * @struct DeviceStruct
 * @brief Structure for queuing device operations
 */
typedef struct{
  std::string path;   ///< D-Bus path of the device
  bool enableLoop;    ///< Whether to enable event loop for this device
}DeviceStruct;

/**
 * @class DeviceManager
 * @brief Manages Bluetooth device lifecycle and registry
 * 
 * This class maintains a registry of discovered Bluetooth devices, handles
 * device addition/removal events, and provides thread-safe access to device
 * operations. It processes device events in a dedicated thread.
 */
class DeviceManager : public IDeviceManager
{
public:
  /**
   * @brief Construct a new Device Manager object
   * @param connection Reference to D-Bus system bus connection
   */
  explicit DeviceManager(sdbus::IConnection &connection);
  
  /**
   * @brief Destroy the Device Manager object and cleanup resources
   */
  ~DeviceManager();

  /**
   * @brief Start the device management event loop
   * 
   * Begins processing device events in a separate thread. This must be called
   * after construction to enable device discovery and management.
   */
  void StartLooping();

  /**
   * @brief Handle device addition event
   * @param devicePath D-Bus object path of the added device
   * @param enableLoop Whether to start event loop for this device
   */
  void DeviceAdded(std::string devicePath, bool enableLoop) override;
  
  /**
   * @brief Handle device removal event
   * @param devicePath D-Bus object path of the removed device
   */
  void DeviceRemoved(std::string devicePath) override;

  /**
   * @brief Get device instance by MAC address
   * @param mac MAC address of the device to retrieve
   * @return Shared pointer to IDevice instance, or nullptr if not found
   */
  std::shared_ptr<IDevice> GetDevice(std::string mac) override;
  
  /**
   * @brief Get list of all device MAC addresses
   * @return Vector containing MAC addresses of all managed devices
   */
  std::vector<std::string> GetDevicesMAC() override;
  
private:
  /**
   * @brief Main event loop function executed in separate thread
   * 
   * Processes device addition/removal events from the queue continuously
   * until the manager is shut down.
   */
  void RunEventLoop();
  
  /**
   * @brief Remove all devices from the manager
   * 
   * Cleanup function called during destruction to properly remove
   * all managed devices and their event loops.
   */
  void RemoveDevices();
  
  /**
   * @brief Extract MAC address from D-Bus device path
   * @param path D-Bus object path of the device
   * @return MAC address string extracted from the path
   */
  std::string GetMACFromPath(const std::string &path);
  
private:
  sdbus::IConnection &m_connection;         ///< Reference to D-Bus connection
  DevicesMap m_devicesMap;                  ///< Map of MAC addresses to Device objects
  std::mutex m_deviceManagerMutex;          ///< Mutex for thread-safe access
  std::atomic<bool> m_running;              ///< Flag to control event loop execution
  std::thread m_eventLoopThread;            ///< Thread for running the event loop
  std::queue<DeviceStruct> m_deviceQueue;   ///< Queue for device operations
  std::condition_variable m_deviceQueueCV;  ///< Condition variable for queue synchronization
};
