/**
 * @file AdapterProxy.h
 * @brief D-Bus proxy for BlueZ Adapter1 interface operations
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <memory>
#include <string>
#include <vector>
#include <map>

#include <sdbus-c++/sdbus-c++.h>

#include "Adapter1-proxy-generated.hpp"

#include "AdapterHelper.h"
#include "IAdapter.h"

/**
 * @class AdapterProxy
 * @brief D-Bus proxy wrapper for BlueZ Adapter1 interface
 * 
 * This class provides a C++ wrapper around the generated D-Bus proxy for
 * BlueZ Adapter1 interface. It handles Bluetooth adapter operations including
 * power management, discoverability, device discovery, and property management.
 * The class also manages HCI commands for low-level adapter control.
 */
class AdapterProxy : public sdbus::ProxyInterfaces<org::bluez::Adapter1_proxy, sdbus::Properties_proxy>
{
public:
    /**
     * @brief Construct a new Adapter Proxy object
     * @param connection Reference to D-Bus system bus connection
     * @param adapter Reference to IAdapter callback interface
     * @param hciDevice HCI device identifier (e.g., "hci0")
     */
    explicit AdapterProxy(sdbus::IConnection& connection, IAdapter& adapter, std::string hciDevice);
    
    /**
     * @brief Destroy the Adapter Proxy object and cleanup D-Bus connections
     */
    ~AdapterProxy();

    // Power management methods
    /**
     * @brief Set adapter power state
     * @param value True to power on, false to power off
     * @throws sdbus::Error if setting power state fails
     */
    void SetPowered(const bool& value);
    
    /**
     * @brief Get current adapter power state
     * @return True if powered on, false if powered off
     * @throws sdbus::Error if property retrieval fails
     */
    bool GetPowered();

    // Discoverability management
    /**
     * @brief Set adapter discoverability state
     * @param value True to make discoverable, false to hide
     * @throws sdbus::Error if setting discoverability fails
     */
    void SetDiscoverable(const bool& value);
    
    /**
     * @brief Get current adapter discoverability state
     * @return True if discoverable, false if hidden
     * @throws sdbus::Error if property retrieval fails
     */
    bool GetDiscoverable();

    /**
     * @brief Get current discovery state
     * @return True if currently discovering devices, false otherwise
     * @throws sdbus::Error if property retrieval fails
     */
    bool GetDiscovering();
    
    /**
     * @brief Set discoverable timeout period
     * @param value Timeout in seconds (0 = never timeout)
     * @throws sdbus::Error if setting timeout fails
     */
    void SetDiscoverableTimeout(const uint32_t& value);
    
    /**
     * @brief Get current discoverable timeout
     * @return Timeout in seconds
     * @throws sdbus::Error if property retrieval fails
     */
    uint32_t GetDiscoverableTimeout();

    // Pairing management
    /**
     * @brief Set adapter pairable state
     * @param value True to allow pairing, false to disable
     * @throws sdbus::Error if setting pairable state fails
     */
    void SetPairable(const bool& value);
    
    /**
     * @brief Get current adapter pairable state
     * @return True if pairable, false if pairing disabled
     * @throws sdbus::Error if property retrieval fails
     */
    bool GetPairable();

    /**
     * @brief Set pairable timeout period
     * @param value Timeout in seconds (0 = never timeout)
     * @throws sdbus::Error if setting timeout fails
     */
    void SetPairableTimeout(const uint32_t& value);
    
    /**
     * @brief Get current pairable timeout
     * @return Timeout in seconds
     * @throws sdbus::Error if property retrieval fails
     */
    uint32_t GetPairableTimeout();
    
    // Adapter properties
    /**
     * @brief Set adapter alias (human-readable name)
     * @param value New alias for the adapter
     * @throws sdbus::Error if setting alias fails
     */
    void SetAlias(const std::string &value);
    
    /**
     * @brief Get current adapter alias
     * @return Adapter alias string
     * @throws sdbus::Error if property retrieval fails
     */
    std::string GetAlias();

    /**
     * @brief Set adapter device class
     * @param value Device class value
     * @throws sdbus::Error if setting class fails
     */
    void SetClass(const uint32_t &value);
    
    /**
     * @brief Get current adapter device class
     * @return Device class value
     * @throws sdbus::Error if property retrieval fails
     */
    uint32_t GetClass();

    // Discovery operations
    /**
     * @brief Start device discovery
     * @throws sdbus::Error if starting discovery fails
     */
    void StartDiscovery();
    
    /**
     * @brief Set discovery filter parameters
     * @param properties Map of filter properties (UUIDs, RSSI, etc.)
     * @throws sdbus::Error if setting filter fails
     */
    void SetDiscoveryFilter(const std::map<std::string, sdbus::Variant>& properties);
    
    /**
     * @brief Stop device discovery
     * @throws sdbus::Error if stopping discovery fails
     */
    void StopDiscovery();
    
    /**
     * @brief Remove a device from the adapter
     * @param device_path D-Bus object path of the device to remove
     * @throws sdbus::Error if device removal fails
     */
    void RemoveDevice(const std::string& device_path);
    
    /**
     * @brief Get supported discovery filter types
     * @return Vector of supported filter type strings
     * @throws sdbus::Error if retrieval fails
     */
    std::vector<std::string> GetDiscoveryFilters();

    // Adapter management
    /**
     * @brief Initialize adapter to default state
     * 
     * Sets up the adapter with default configuration including
     * power state, discoverability, and pairing settings.
     */
    void InitAdapter();
    
    /**
     * @brief Reset adapter status using HCI commands
     * 
     * Performs low-level reset of the adapter using HCI interface
     * to recover from error states.
     */
    void ResetStatus();
    
private:
    /**
     * @brief Handle D-Bus property change notifications
     * @param interface_name Name of the D-Bus interface (org.bluez.Adapter1)
     * @param changed_properties Map of changed properties and their new values
     * @param invalidated_properties List of properties that became invalid
     */
    void onPropertiesChanged(const std::string& interface_name, 
        const std::map<std::string, sdbus::Variant>& changed_properties, 
        const std::vector<std::string>& invalidated_properties) override;
    
    /**
     * @brief Execute a bash command and return exit code
     * @param command Command string to execute
     * @return Exit code of the command (0 = success)
     */
    int ExecuteBashCommand(std::string command);
    
    /**
     * @brief Construct HCI command string for the adapter
     * @param args HCI command arguments
     * @return Complete HCI command string
     */
    std::string ConstructHCICommand(std::string args);
    
private:
    sdbus::IConnection& m_connection; ///< Reference to D-Bus connection
    IAdapter& m_adapter;              ///< Reference to callback interface
    std::string m_hciDevice;          ///< HCI device identifier (e.g., "hci0")
};

