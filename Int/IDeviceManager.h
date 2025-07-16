/**
 * @file IDeviceManager.h
 * @brief Interface for Bluetooth device manager operations
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <string>
#include <cstdbool>
#include <memory>
#include <vector>

#include "IDevice.h"

/**
 * @class IDeviceManager
 * @brief Abstract interface for managing Bluetooth device lifecycle
 * 
 * This interface defines the contract for device discovery, registration,
 * and management operations. Implementations handle device addition/removal
 * events and provide access to discovered devices.
 */
class IDeviceManager
{
public:
  /**
   * @brief Virtual destructor for proper inheritance cleanup
   */
  virtual ~IDeviceManager() = default;
  
  /**
   * @brief Handle device addition event
   * @param devicePath D-Bus object path of the added device
   * @param enableLoop Whether to start event loop for this device
   */
  virtual void DeviceAdded(std::string devicePath, bool enableLoop) = 0;
  
  /**
   * @brief Handle device removal event
   * @param devicePath D-Bus object path of the removed device
   */
  virtual void DeviceRemoved(std::string devicePath) = 0;

  /**
   * @brief Get device instance by MAC address
   * @param mac MAC address of the device to retrieve
   * @return Shared pointer to IDevice instance, or nullptr if not found
   */
  virtual std::shared_ptr<IDevice> GetDevice(std::string mac) = 0;
  
  /**
   * @brief Get list of all device MAC addresses
   * @return Vector containing MAC addresses of all managed devices
   */
  virtual std::vector<std::string> GetDevicesMAC() = 0;
};
