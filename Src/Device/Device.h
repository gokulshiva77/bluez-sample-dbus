/**
 * @file Device.h
 * @brief Bluetooth device implementation for BlueZ D-Bus interface
 * @author Gokul
 * @date 2025
 */

#pragma once
#include <thread>
#include <atomic>
#include <mutex>

#include "IDevice.h"

#include "DeviceProxy.h"

/**
 * @class Device
 * @brief Concrete implementation of IDevice interface for Bluetooth devices
 * 
 * This class provides a complete implementation of Bluetooth device operations
 * including connection management, pairing, property monitoring, and profile
 * operations. It runs in its own event loop thread to handle asynchronous
 * D-Bus property change notifications from BlueZ.
 */
class Device : public IDevice
{
public:
  /**
   * @brief Construct a new Device object
   * @param connection Reference to D-Bus system bus connection
   * @param devicePath D-Bus object path for the device
   */
  Device(sdbus::IConnection &connection, std::string devicePath);
  
  /**
   * @brief Destroy the Device object and cleanup resources
   */
  ~Device();
  
  /**
   * @brief Start the device event loop
   * 
   * Begins processing device property change events in a separate thread.
   * This must be called after construction to enable device monitoring.
   */
  void StartLooping();

  /**
   * @brief Get the D-Bus object path for this device
   * @return String containing the device's D-Bus path
   */
  std::string GetPath() override;
  
  /**
   * @brief Initiate a connection to this device
   * @throws sdbus::Error if connection fails
   */
  void Connect();
  
  /**
   * @brief Disconnect from this device
   * @throws sdbus::Error if disconnection fails
   */
  void Disconnect();
  
  /**
   * @brief Connect to a specific profile on this device
   * @param uuid UUID of the profile to connect to (e.g., SPP UUID)
   * @throws sdbus::Error if profile connection fails
   */
  void ConnectProfile(std::string uuid);
  
  /**
   * @brief Disconnect from a specific profile on this device
   * @param uuid UUID of the profile to disconnect from
   * @throws sdbus::Error if profile disconnection fails
   */
  void DisconnectProfile(std::string uuid);
  
  /**
   * @brief Initiate pairing with this device
   * @throws sdbus::Error if pairing fails
   */
  void Pair();
  
  /**
   * @brief Cancel an ongoing pairing operation
   * @throws sdbus::Error if cancellation fails
   */
  void CancelPairing();

  /**
   * @brief Handle bulk property changes from D-Bus
   * @param properties DeviceProperties structure containing updated values
   */
  void PropertiesChanged(DeviceProperties properties) override;
  
  /**
   * @brief Get current device properties
   * @return DeviceProperties structure containing all current property values
   */
  DeviceProperties GetProperties() override ;
  
  // Property change callback methods
  void AddressChanged(std::string value) override;         ///< Handle device address changes
  void AddressTypeChanged(std::string value) override;     ///< Handle address type changes
  void NameChanged(std::string value) override;            ///< Handle device name changes
  void IconChanged(std::string value) override;            ///< Handle device icon changes
  void ClassChanged(uint32_t value) override;              ///< Handle device class changes
  void UUIDsChanged(std::vector<std::string> value) override; ///< Handle UUID list changes
  void PairedChanged(bool value) override;                 ///< Handle pairing status changes
  void ConnectedChanged(bool value) override;              ///< Handle connection status changes
  void TrustedChanged(bool value) override;                ///< Handle trusted status changes
  void BlockedChanged(bool value) override;                ///< Handle blocked status changes
  void AliasChanged(std::string value) override;           ///< Handle device alias changes
  void AdapterChanged(std::string value) override;         ///< Handle adapter changes
  void LegacyPairingChanged(bool value) override;          ///< Handle legacy pairing changes
  void ManufacturerDataChanged(std::map<uint16_t, std::map<int, std::string>> value) override; ///< Handle manufacturer data changes
  void ServiceDataChanged(std::map<std::string, std::map<int, std::string>> value) override;   ///< Handle service data changes
  void ServicesResolvedChanged(bool value) override;       ///< Handle services resolved status changes

private:
  /**
   * @brief Main event loop function executed in separate thread
   * 
   * Processes D-Bus property change events continuously until
   * the device is destroyed.
   */
  void RunEventLoop();
  
  /**
   * @brief Print UUIDs supported by this device
   * 
   * Helper function to display device capabilities and supported services.
   */
  void PrintUUID();
  
private:
    DeviceProxy m_deviceProxy;         ///< Proxy for D-Bus communication
    DeviceProperties m_properties;     ///< Current device properties
    std::string m_devicePath;          ///< D-Bus object path
    std::mutex m_deviceMutex;          ///< Mutex for thread-safe property access
    std::atomic<bool> m_running;       ///< Flag to control event loop execution
    std::thread m_eventLoopThread;     ///< Thread for running the event loop
};;

