/**
 * @file IAdapter.h
 * @brief Interface for Bluetooth adapter operations and event handling
 * @author Gokul
 * @date 2025
 */

#pragma once
#include <string>
#include <vector>
#include <cstdint>
#include <cstdbool>

/**
 * @class IAdapter
 * @brief Abstract interface for Bluetooth adapter management and event handling
 * 
 * This interface defines the contract for Bluetooth adapter operations including
 * property change notifications, power management, and device class configuration.
 * Implementations handle D-Bus communication with the BlueZ adapter interface.
 */
class IAdapter
{
public:
    /**
     * @brief Virtual destructor for proper inheritance cleanup
     */
    virtual ~IAdapter() = default;
    
    /**
     * @brief Callback for adapter MAC address changes
     * @param value New MAC address of the adapter
     */
    virtual void AddressChanged(std::string value) = 0;
    
    /**
     * @brief Callback for adapter address type changes
     * @param value New address type (e.g., "public", "random")
     */
    virtual void AddressTypeChanged(std::string value) = 0;
    
    /**
     * @brief Callback for adapter name changes
     * @param value New adapter name
     */
    virtual void NameChanged(std::string value) = 0;
    
    /**
     * @brief Callback for adapter alias changes
     * @param value New adapter alias (friendly name)
     */
    virtual void AliasChanged(std::string value) = 0;
    
    /**
     * @brief Callback for adapter device class changes
     * @param value New device class value (24-bit integer)
     */
    virtual void ClassChanged(uint32_t value) = 0;
    
    /**
     * @brief Callback for adapter power state changes
     * @param value True if adapter is powered on, false otherwise
     */
    virtual void PoweredChanged(bool value) = 0;
    
    /**
     * @brief Callback for adapter discoverability changes
     * @param value True if adapter is discoverable, false otherwise
     */
    virtual void DiscoverableChanged(bool value) = 0;
    
    /**
     * @brief Callback for discoverable timeout changes
     * @param value Timeout in seconds (0 means no timeout)
     */
    virtual void DiscoverableTimeoutChanged(uint32_t value) = 0;
    
    /**
     * @brief Callback for adapter pairable state changes
     * @param value True if adapter accepts pairing requests, false otherwise
     */
    virtual void PairableChanged(bool value) = 0;
    
    /**
     * @brief Callback for pairable timeout changes
     * @param value Timeout in seconds (0 means no timeout)
     */
    virtual void PairableTimeoutChanged(uint32_t value) = 0;
    
    /**
     * @brief Callback for discovery state changes
     * @param value True if discovery is active, false otherwise
     */
    virtual void DiscoveringChanged(bool value) = 0;
    
    /**
     * @brief Callback for supported UUIDs changes
     * @param value List of supported service UUIDs
     */
    virtual void UUIDsChanged(std::vector<std::string> value) = 0;

    /**
     * @brief Set the device class for this adapter
     * @param value 24-bit device class value (major, minor, service classes combined)
     */
    virtual void SetClass(uint32_t value) = 0;
};
