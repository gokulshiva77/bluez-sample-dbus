/**
 * @file IAgent.h
 * @brief Interface for Bluetooth authentication agent operations
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <string>

/**
 * @class IAgent
 * @brief Abstract interface for Bluetooth authentication agent
 * 
 * This interface defines the contract for handling Bluetooth authentication
 * requests during pairing operations. Implementations should provide
 * user interaction for confirmation requests and other authentication events.
 */
class IAgent
{
public:
  /**
   * @brief Virtual destructor for proper inheritance cleanup
   */
  virtual ~IAgent() = default;
  
  /**
   * @brief Handle pairing confirmation request
   * @param path D-Bus object path of the device requesting confirmation
   * 
   * Called when a device requires user confirmation for pairing.
   * The implementation should prompt the user or automatically
   * confirm based on application policy.
   */
  virtual void RequestConfirmation(std::string path) = 0;
};