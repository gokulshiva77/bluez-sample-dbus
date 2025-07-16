/**
 * @file Adapter.h
 * @brief Bluetooth adapter implementation using BlueZ D-Bus interface
 * @author Gokul
 * @date 2025
 */

#pragma once

#include "IAdapter.h"

#include "AdapterProxy.h"

/**
 * @class Adapter
 * @brief Concrete implementation of Bluetooth adapter management
 * 
 * This class implements the IAdapter interface and provides concrete
 * Bluetooth adapter functionality including power management, discovery
 * control, and property change handling. It uses AdapterProxy for
 * D-Bus communication with BlueZ.
 */
class Adapter : public IAdapter
{
public:
  /**
   * @brief Construct a new Adapter object
   * @param connection Reference to D-Bus system bus connection
   * @param hciDevice HCI device identifier (e.g., "hci0")
   * @param deviceName Human-readable name for this device
   * @param deviceClass Numeric device class value (24-bit)
   */
  explicit Adapter(sdbus::IConnection& connection, std::string hciDevice, std::string deviceName, uint32_t deviceClass);
  
  /**
   * @brief Destroy the Adapter object and stop discovery
   */
  ~Adapter();

  /**
   * @brief Handle adapter MAC address change notification
   * @param value New MAC address
   */
  void AddressChanged(std::string value) override;
  
  /**
   * @brief Handle adapter address type change notification
   * @param value New address type
   */
  void AddressTypeChanged(std::string value) override;
  
  /**
   * @brief Handle adapter name change notification
   * @param value New adapter name
   */
  void NameChanged(std::string value) override;
  
  /**
   * @brief Handle adapter alias change notification
   * @param value New adapter alias
   */
  void AliasChanged(std::string value) override;
  
  /**
   * @brief Handle adapter device class change notification
   * @param value New device class
   */
  void ClassChanged(uint32_t value) override;
  
  /**
   * @brief Handle adapter power state change notification
   * @param value New power state
   */
  void PoweredChanged(bool value) override;
  
  /**
   * @brief Handle adapter discoverability change notification
   * @param value New discoverability state
   */
  void DiscoverableChanged(bool value) override;
  
  /**
   * @brief Handle discoverable timeout change notification
   * @param value New timeout value in seconds
   */
  void DiscoverableTimeoutChanged(uint32_t value) override;
  
  /**
   * @brief Handle adapter pairable state change notification
   * @param value New pairable state
   */
  void PairableChanged(bool value) override;
  
  /**
   * @brief Handle pairable timeout change notification
   * @param value New timeout value in seconds
   */
  void PairableTimeoutChanged(uint32_t value) override;
  
  /**
   * @brief Handle discovery state change notification
   * @param value New discovery state
   */
  void DiscoveringChanged(bool value) override;
  
  /**
   * @brief Handle supported UUIDs change notification
   * @param value New list of supported UUIDs
   */
  void UUIDsChanged(std::vector<std::string> value) override;

  /**
   * @brief Start scanning for nearby devices
   * 
   * Powers on the adapter and begins active scanning for discoverable devices.
   */
  void StartScan();
  
  /**
   * @brief Start discovery mode (make this device discoverable)
   * 
   * Powers on the adapter, makes it discoverable and pairable, and configures
   * it for accepting incoming connections.
   */
  void StartDiscovery();
  
  /**
   * @brief Stop discovery and make adapter non-discoverable
   */
  void StopDiscovery();

  /**
   * @brief Set the device class for this adapter
   * @param value 24-bit device class value
   */
  void SetClass(uint32_t value) override;
  private:
  AdapterProxy m_adapterProxy;
  sdbus::IConnection& m_connection;
  std::string m_hciDevice;
  std::string m_deviceName;
  uint32_t m_deviceClass;
};
