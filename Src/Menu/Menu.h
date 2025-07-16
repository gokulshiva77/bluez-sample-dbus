/**
 * @file Menu.h
 * @brief Interactive command-line menu for Bluetooth operations
 * @author Gokul
 * @date 2025
 */

#pragma once

#include "Application.h"

/**
 * @class Menu
 * @brief Interactive command-line interface for Bluetooth operations
 * 
 * This class provides a user-friendly menu system for performing various
 * Bluetooth operations including device discovery, connection management,
 * pairing, and profile operations. It acts as a front-end to the Application class.
 */
class Menu
{
public:
  /**
   * @brief Construct a new Menu object
   * @param app Shared pointer to the Application instance
   */
  explicit Menu(std::shared_ptr<Application> app);
  
  /**
   * @brief Destroy the Menu object
   */
  ~Menu();

  /**
   * @brief Display the main menu options to the user
   */
  void PrintMenu();
  
  /**
   * @brief Process user menu selection
   * @param menu String containing the user's menu choice
   */
  void ProcessMenu(std::string menu);

  /**
   * @brief Start device discovery mode
   */
  void StartDiscovery();
  
  /**
   * @brief Start scanning for nearby devices
   */
  void StartScan();
  
  /**
   * @brief Get and select a specific device by MAC address
   */
  void GetDevice();
  
  /**
   * @brief List all discovered devices
   */
  void ListDevices();
  
  /**
   * @brief Print properties of the currently selected device
   */
  void PrintProperties();
  
  /**
   * @brief Automatically connect to SPP-capable devices
   */
  void AutoConnectSPP();
  
  /**
   * @brief Connect to the currently selected device
   */
  void Connect();
  
  /**
   * @brief Disconnect from the currently selected device
   */
  void Disconnect();
  
  /**
   * @brief Connect to a specific profile on the selected device
   */
  void ConnectProfile();
  
  /**
   * @brief Disconnect from a specific profile on the selected device
   */
  void DisconnectProfile();
  
  /**
   * @brief Connect to SPP profile on the selected device
   */
  void ConnectSPP();
  
  /**
   * @brief Disconnect from SPP profile on the selected device
   */
  void DisconnectSPP();
  
  /**
   * @brief Initiate pairing with the selected device
   */
  void Pair();
  
  /**
   * @brief Cancel ongoing pairing operation
   */
  void CancelPairing();
  
  /**
   * @brief Stop the application gracefully
   */
  void StopApplication();
  
private:
  /**
   * @brief Get MAC address input from user
   * @return String containing the MAC address
   */
  std::string GetMAC();
  
  /**
   * @brief Get UUID input from user
   * @return String containing the UUID
   */
  std::string GetUUID();
  
  /**
   * @brief Get list of available device MAC addresses
   * @return Vector containing MAC addresses of discovered devices
   */
  std::vector<std::string> GetDevicesMac();
  
  /**
   * @brief Check if Serial Port Profile is available in UUID list
   * @param UUIDs Vector of service UUIDs to check
   * @return True if SPP UUID is found, false otherwise
   */
  bool IsSPPAvailable(std::vector<std::string> UUIDs);
  
private:
  std::shared_ptr<Application> m_application; ///< Reference to main application instance
  std::shared_ptr<IDevice> m_device;          ///< Currently selected device for operations
};
