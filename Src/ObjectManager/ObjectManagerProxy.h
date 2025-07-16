/**
 * @file ObjectManagerProxy.h
 * @brief D-Bus proxy for BlueZ ObjectManager interface operations
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <thread>
#include <atomic>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <map>

#include <sdbus-c++/sdbus-c++.h>

#include "IDeviceManager.h"

/**
 * @struct InterfaceAddedStruct
 * @brief Structure for queuing interface addition events
 */
typedef struct {
  sdbus::ObjectPath path;                                                      ///< D-Bus object path
  std::map<std::string, std::map<std::string, sdbus::Variant>> interfacesAndProperties; ///< Interface properties
}InterfaceAddedStruct;

/**
 * @struct InterfaceRemovedStruct
 * @brief Structure for queuing interface removal events
 */
typedef struct {
  sdbus::ObjectPath path;           ///< D-Bus object path
  std::vector<std::string> interfaces; ///< List of removed interfaces
}InterfaceRemovedStruct;

/**
 * @class ObjectManagerProxy
 * @brief D-Bus proxy for monitoring BlueZ object lifecycle
 * 
 * This class monitors D-Bus objects managed by BlueZ, particularly focusing
 * on Device1 interfaces. It processes InterfacesAdded and InterfacesRemoved
 * signals to track device discovery and removal events. The class filters
 * devices based on their device class and forwards relevant events to
 * the device manager.
 */
class ObjectManagerProxy : public sdbus::ProxyInterfaces<sdbus::ObjectManager_proxy>
{
public:
  /**
   * @brief Construct a new Object Manager Proxy object
   * @param connection Reference to D-Bus system bus connection
   * @param deviceManager Reference to device manager for event forwarding
   */
  ObjectManagerProxy(sdbus::IConnection& connection, IDeviceManager &deviceManager);
  
  /**
   * @brief Destroy the Object Manager Proxy object and cleanup resources
   */
  ~ObjectManagerProxy();

  /**
   * @brief Start the object manager event loop
   * 
   * Begins processing interface addition/removal events in a separate thread.
   * This must be called after construction to enable object monitoring.
   */
  void StartLooping();

  /**
   * @brief Handle D-Bus InterfacesAdded signal
   * @param objectPath D-Bus object path of the added interface
   * @param interfacesAndProperties Map of interfaces and their properties
   * 
   * Called when new D-Bus objects are added to BlueZ. Filters for Device1
   * interfaces and queues them for processing.
   */
  void onInterfacesAdded( const sdbus::ObjectPath& objectPath
    , const std::map<std::string, std::map<std::string, sdbus::Variant>>& interfacesAndProperties) override;
    
  /**
   * @brief Handle D-Bus InterfacesRemoved signal
   * @param objectPath D-Bus object path of the removed interface
   * @param interfaces List of removed interface names
   * 
   * Called when D-Bus objects are removed from BlueZ. Filters for Device1
   * interfaces and queues them for processing.
   */
  void onInterfacesRemoved( const sdbus::ObjectPath& objectPath
      , const std::vector<std::string>& interfaces) override;

private:
  /**
   * @brief Main event loop function executed in separate thread
   * 
   * Processes queued interface addition/removal events and forwards
   * device events to the device manager.
   */
  void RunEventLoop();

  /**
   * @brief Extract device class from interface properties
   * @param interfaces Map of interface properties
   * @return Device class value (24-bit)
   */
  uint32_t GetClass(std::map<std::string, sdbus::Variant> interfaces);
  
  /**
   * @brief Validate if device class is acceptable
   * @param device_class Device class value to validate
   * @return True if device class should be processed, false otherwise
   */
  bool ValidateClass(uint32_t device_class);
  
  /**
   * @brief Get and validate device class from interface properties
   * @param interfaces Map of interface properties
   * @return True if device should be processed, false otherwise
   */
  bool GetAndValidateClass(std::map<std::string, sdbus::Variant> interfaces);

private:
    sdbus::IConnection& m_connection;                          ///< Reference to D-Bus connection
    IDeviceManager &m_deviceManager;                           ///< Reference to device manager
    std::atomic<bool> m_running;                               ///< Flag to control event loop execution
    std::thread m_eventLoopThread;                             ///< Thread for running the event loop
    std::mutex m_obj_manager_mutex;                            ///< Mutex for thread-safe queue access
    std::queue<InterfaceAddedStruct> m_interface_added_queue;  ///< Queue for interface addition events
    std::queue<InterfaceRemovedStruct> m_interface_removed_queue; ///< Queue for interface removal events
    std::condition_variable m_queueCV;                         ///< Condition variable for queue synchronization
};