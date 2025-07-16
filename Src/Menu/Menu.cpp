/**
 * @file Menu.cpp
 * @brief Implementation of interactive command-line menu for Bluetooth operations
 * @author Gokul
 * @date 2025
 */

#include <iostream>
#include <string>
#include <map>
#include <functional>

#include <cstdint>

#include "Menu.h"
#include "main.h"

#include "Logger.h"

#define TAG "Menu::"    ///< Tag for logging messages

#define SPP_UUID "00001101-0000-1000-8000-00805f9b34fb" ///< Serial Port Profile UUID

/**
 * @enum MenuOptions
 * @brief Enumeration of available menu options
 */
typedef enum
{
  START_DISCOVERY = 1,
  START_SCAN,
  LIST_DEVICES,
  GET_DEVICE,
  PRINT_PROPERITES,
  AUTO_CONNECT_SPP,
  CONNECT,
  DISCONNECT,
  CONNECT_PROFILE,
  CONNECT_SPP_PROFILE,
  DISCONNECT_SPP_PROFILE,
  PAIR,
  CANCEL_PAIRING,
  EXIT,
  MAX_MENU
} MenuEnum;

std::map<uint8_t, std::string> menu_map = {
    {START_DISCOVERY, "Start Discovery"},
    {START_SCAN, "Start Scan"},
    {LIST_DEVICES, "List Devices"},
    {GET_DEVICE, "Get Device"},
    {PRINT_PROPERITES, "Print Properties"},
    {AUTO_CONNECT_SPP, "Auto Connect SPP"},
    {CONNECT, "Connect"},
    {DISCONNECT, "Disconnect"},
    {CONNECT_PROFILE, "Connect Profile"},
    {CONNECT_SPP_PROFILE, "Connect SPP Profile"},
    {DISCONNECT_SPP_PROFILE, "Disconnect SPP Profile"},
    {PAIR, "Pair"},
    {CANCEL_PAIRING, "Cancel Pairing"},
    {EXIT, "Exit"}};

std::map<std::string, std::string> UUIDDescription{
    {"00001101-0000-1000-8000-00805f9b34fb", "Serial Port Profile (SPP)"},
    {"00001105-0000-1000-8000-00805f9b34fb", "Dial-Up Networking (DUN)"},
    {"00001106-0000-1000-8000-00805f9b34fb", "IrMC Sync"},
    {"00001107-0000-1000-8000-00805f9b34fb", "OBEX Object Push"},
    {"00001108-0000-1000-8000-00805f9b34fb", "OBEX File Transfer"},
    {"00001109-0000-1000-8000-00805f9b34fb", "IrMC Sync Command"},
    {"0000110a-0000-1000-8000-00805f9b34fb", "Headset Profile (HSP)"},
    {"0000110b-0000-1000-8000-00805f9b34fb", "Audio Gateway (AG)"},
    {"0000110c-0000-1000-8000-00805f9b34fb", "Audio/Video Remote Control Profile (AVRCP)"},
    {"0000110d-0000-1000-8000-00805f9b34fb", "Hands-Free Profile (HFP)"},
    {"0000110e-0000-1000-8000-00805f9b34fb", "Hands-Free Profile (HFP)"},
    {"0000110f-0000-1000-8000-00805f9b34fb", "Basic Imaging Profile (BIP)"},
    {"00001110-0000-1000-8000-00805f9b34fb", "Basic Imaging Profile (BIP)"},
    {"00001111-0000-1000-8000-00805f9b34fb", "Basic Imaging Profile (BIP)"},
    {"00001112-0000-1000-8000-00805f9b34fb", "Basic Imaging Profile (BIP)"},
    {"00001113-0000-1000-8000-00805f9b34fb", "Basic Imaging Profile (BIP)"},
    {"00001114-0000-1000-8000-00805f9b34fb", "Basic Imaging Profile (BIP)"},
    {"00001115-0000-1000-8000-00805f9b34fb", "Basic Imaging Profile (BIP)"},
};

std::map<uint8_t, std::function<void(Menu* callback)>> dispatchMenuCallbacks = {
  {START_DISCOVERY,         [](Menu* callback) { callback->StartDiscovery(); }},
  {START_SCAN,              [](Menu* callback) { callback->StartScan(); }},
  {LIST_DEVICES,            [](Menu* callback) { callback->ListDevices(); }},
  {GET_DEVICE,              [](Menu* callback) { callback->GetDevice(); }},
  {PRINT_PROPERITES,        [](Menu* callback) { callback->PrintProperties(); }},
  {AUTO_CONNECT_SPP,        [](Menu* callback) { callback->AutoConnectSPP(); }},
  {CONNECT,                 [](Menu* callback) { callback->Connect(); }},
  {DISCONNECT,              [](Menu* callback) { callback->Disconnect(); }},
  {CONNECT_PROFILE,         [](Menu* callback) { callback->ConnectProfile(); }},
  {CONNECT_SPP_PROFILE,     [](Menu* callback) { callback->ConnectSPP(); }},
  {DISCONNECT_SPP_PROFILE,  [](Menu* callback) { callback->DisconnectSPP(); }},
  {PAIR,                    [](Menu* callback) { callback->Pair(); }},
  {CANCEL_PAIRING,          [](Menu* callback) { callback->CancelPairing(); }},
  {EXIT,                    [](Menu* callback) { callback->StopApplication(); }},
};
Menu::Menu(std::shared_ptr<Application> app) : m_application(app)
{
}

Menu::~Menu()
{
}

void Menu::PrintMenu()
{
  for (auto menu : menu_map)
  {
    Log("%d. %s", menu.first, menu.second.c_str());
  }
}

void Menu::ProcessMenu(std::string menu)
{
  try
  {
    uint8_t option = std::stoi(menu);
    dispatchMenuCallbacks.at(option)(this);
  }
  catch (const std::out_of_range &e)
  {
    Log("%s Invalid Option %s", TAG, LOG_STRING(menu));
  }
  catch (const std::invalid_argument &e)
  {
    Log("%s Invalid Option %s", TAG, LOG_STRING(menu));
  }
}

void Menu::StartDiscovery()
{
  m_application->StartDiscovery();
}

void Menu::StartScan()
{
  Log("%s%s", TAG,__func__);
  m_application->StartScan();
}

void Menu::GetDevice()
{
  Log("%s%s", TAG,__func__);
  auto mac = GetMAC();
  auto device = m_application->GetDeviceManager().GetDevice(mac);
  if (!device)
  {
    Log("Device is null");
    return;
  }
  m_device = device;
}

void Menu::ListDevices()
{
  Log("%s%s", TAG,__func__);
  auto devices = GetDevicesMac();
  for (auto device : devices) {
    Log("%s%s Device - %s", TAG, __func__, LOG_STRING(device));
  }
}

void Menu::PrintProperties()
{
  Log("%s%s", TAG,__func__);
  if (!m_device)
  {
    Log("Device is null");
    return;
  }
  DeviceProperties properties = m_device->GetProperties();
  Log("Properties: ");
  Log("Name: %s", LOG_STRING(properties.Name));
  Log("Class: %d", properties.Class);
  Log("Paired: %d", properties.Paired);
  Log("Connected: %d", properties.Connected);
  int i = 1;
  for (auto uuid : properties.UUIDs)
  {
    try
    {
      auto it = UUIDDescription.at(uuid);
      Log("%d UUID: %s - %s", i++, LOG_STRING(uuid), LOG_STRING(it));
    }
    catch (std::out_of_range &e)
    {
      Log("%d UUID: %s", i++, LOG_STRING(uuid));
    }
  }
}

void Menu::AutoConnectSPP()
{
  Log("%s%s", TAG,__func__);
  auto devices_mac = GetDevicesMac();
  for (auto mac : devices_mac) {
    auto device = m_application->GetDeviceManager().GetDevice(mac);
    if (!device)
    {
      Log("Device is null");
      continue;
    }
    DeviceProperties properties = device->GetProperties();
    if (properties.Paired && IsSPPAvailable(properties.UUIDs))
    {
      device->ConnectProfile(SPP_UUID);
    }
  }
}

void Menu::Connect()
{
  Log("%s%s", TAG,__func__);
  if (!m_device)
  {
    Log("Device is null");
    return;
  }
  m_device->Connect();
}

void Menu::Disconnect()
{
  Log("%s%s", TAG,__func__);
  if (!m_device)
  {
    Log("Device is null");
    return;
  }
  m_device->Disconnect();
}

void Menu::ConnectProfile()
{
  Log("%s%s", TAG,__func__);
  std::string uuid = GetUUID();
  if (uuid.empty())
  {
    Log("UUID is empty");
    return;
  }
  if (!m_device)
  {
    Log("Device is null");
    return;
  }
  m_device->ConnectProfile(uuid);
}

void Menu::DisconnectProfile()
{
  Log("%s%s", TAG,__func__);
  std::string uuid = GetUUID();
  if (uuid.empty())
  {
    Log("UUID is empty");
    return;
  }
  if (!m_device)
  {
    Log("Device is null");
    return;
  }
  m_device->DisconnectProfile(uuid);
}

void Menu::ConnectSPP()
{
  Log("%s%s", TAG,__func__);
  if (!m_device)
  {
    Log("Device is null");
    return;
  }
  m_device->ConnectProfile(SPP_UUID);
}

void Menu::DisconnectSPP()
{
  Log("%s%s", TAG,__func__);
  if (!m_device)
  {
    Log("Device is null");
    return;
  }
  m_device->DisconnectProfile(SPP_UUID);
}

void Menu::Pair()
{
  Log("%s%s", TAG,__func__);
  if (!m_device)
  {
    Log("Device is null");
    return;
  }
  m_device->Pair();
}

void Menu::CancelPairing()
{
  Log("%s%s", TAG,__func__);
  if (!m_device)
  {
    Log("Device is null");
    return;
  }
  m_device->CancelPairing();
}

void Menu::StopApplication()
{
  Log("%s%s", TAG,__func__);
  StopApp();
}

std::string Menu::GetMAC()
{
  std::string mac;
  Log("Enter MAC Address");
  std::cin >> mac;
  return mac;
}

std::string Menu::GetUUID()
{
  std::string uuid;
  Log("Enter UUID");
  std::cin >> uuid;
  return uuid;
}

std::vector<std::string> Menu::GetDevicesMac()
{
  std::vector<std::string> devices_mac;
  if(m_application) {
    devices_mac = m_application->GetDeviceManager().GetDevicesMAC();
  }
  return devices_mac;
}

bool Menu::IsSPPAvailable(std::vector<std::string> UUIDs)
{
  bool out = false;
  auto it = std::find_if(UUIDs.begin(), UUIDs.end(), [](const std::string &u) { return u == SPP_UUID; });
  if (it != UUIDs.end()) {
    out = true;
  }
  return out;
}
