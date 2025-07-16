/**
 * @file Device.cpp
 * @brief Implementation of Bluetooth device for BlueZ D-Bus interface
 * @author Gokul
 * @date 2025
 */

#include <atomic>
#include <sstream>

#include "Device.h"

#include "Logger.h"

#define TAG "Device::" ///< Tag for logging messages

/**
 * @brief Construct a new Device object
 * 
 * Initializes a Bluetooth device with the specified D-Bus path and
 * sets up the device proxy for D-Bus communication.
 * 
 * @param connection Reference to D-Bus system bus connection
 * @param devicePath D-Bus object path for the device
 */
Device::Device(sdbus::IConnection &connection, std::string devicePath):
m_running(true),
m_deviceProxy(connection, *this, devicePath),
m_devicePath(devicePath),
m_properties() // Initialize m_properties
{
  Log("%s%s", TAG,__func__);
}

Device::~Device()
{
  Log("%s%s", TAG,__func__);
  m_running = false;
  if(m_eventLoopThread.joinable()){
    m_eventLoopThread.join();
  }
}

void Device::StartLooping()
{
  Log("%s%s", TAG,__func__);
  try
  {
    m_eventLoopThread = std::thread(&Device::RunEventLoop, this);
  }
  catch(const std::system_error & e)
  {
    Log("%s%s Error - %s", TAG,__func__, e.what());
  }
}

std::string Device::GetPath()
{
  return m_devicePath;
}

void Device::Connect()
{
  Log("%s%s", TAG,__func__);
  if(m_deviceProxy.GetConnected()) {
    Log("%s%s Device is already connected", TAG,__func__);
    return;
  }
  m_deviceProxy.Connect();
}

void Device::Disconnect()
{
  Log("%s%s", TAG,__func__);
  if(!m_deviceProxy.GetConnected()) {
    Log("%s%s Device is not connected", TAG,__func__);
    return;
  }
  m_deviceProxy.Disconnect();
}

void Device::ConnectProfile(std::string uuid)
{
  Log("%s%s UUID - %s", TAG,__func__, LOG_STRING(uuid));
  m_properties.UUIDs = m_deviceProxy.GetUUIDs();
  if(m_properties.UUIDs.size() == 0) {
    Log("%s%s Error: UUIDs is empty", TAG,__func__);
  }
  PrintUUID();
  try
  {
    m_deviceProxy.ConnectProfile(uuid);
  }
  catch(const sdbus::Error& e)
  {
    Log("%s%s Error: Couldn't connect UUID - %s %s", TAG,__func__, LOG_STRING(uuid), e.what());
  }
  
  
}

void Device::DisconnectProfile(std::string uuid)
{
  Log("%s%s UUID - %s", TAG,__func__, LOG_STRING(uuid));
  m_deviceProxy.DisconnectProfile(uuid);
}

void Device::Pair()
{
  Log("%s%s", TAG,__func__);
  if (m_deviceProxy.GetPaired()) {
    Log("%s%s Device is already paired", TAG,__func__);
    return;
  }
  m_deviceProxy.Pair();
}

void Device::CancelPairing()
{
  Log("%s%s", TAG,__func__);
  if(!m_deviceProxy.GetPaired()) {
    Log("%s%s Device is not paired", TAG,__func__);
    return;
  }
  m_deviceProxy.CancelPairing();
}

void Device::PropertiesChanged(DeviceProperties properties)
{
  m_properties = properties;
}

DeviceProperties Device::GetProperties()
{
  std::lock_guard<std::mutex> lock(m_deviceMutex);
  DeviceProperties properties = m_deviceProxy.GetProperties();
  return properties;
}

void Device::AddressChanged(std::string value)
{
  if (m_properties.Address != value) {
    m_properties.Address = value;
    Log("%s%s Address- %s ", TAG,__func__, LOG_STRING(value));
  }
}

void Device::AddressTypeChanged(std::string value)
{
  if (m_properties.AddressType != value) {
    m_properties.AddressType = value;
    Log("%s%s AddressType: %s", TAG,__func__, LOG_STRING(value));
  }
}

void Device::NameChanged(std::string value)
{
  if (m_properties.Name != value) {
    m_properties.Name = value;
    Log("%s%s Name: %s", TAG,__func__, LOG_STRING(value));
  }
}

void Device::IconChanged(std::string value)
{
  if (m_properties.Icon != value) {
    m_properties.Icon = value;
    Log("%s%s Icon: %s", TAG,__func__, LOG_STRING(value));
  }
}

void Device::ClassChanged(uint32_t value)
{
  if (m_properties.Class != value) {
    m_properties.Class = value;
    Log("%s%s Class: %u", TAG,__func__, value);
  }
}

void Device::UUIDsChanged(std::vector<std::string> value)
{
  if (m_properties.UUIDs != value) {
    m_properties.UUIDs = value;
    std::stringstream ss;
    for (const auto& uuid : value) {
      ss << uuid << " ";
    }
    Log("%s%s UUIDs: %s", TAG, __func__, ss.str().c_str());
  }
}

void Device::PairedChanged(bool value)
{
  if (m_properties.Paired != value) {
    m_properties.Paired = value;
    Log("%s%s Paired - %d", TAG,__func__, value);
  }
}

void Device::ConnectedChanged(bool value)
{
  if (m_properties.Connected != value) {
    m_properties.Connected = value;
    Log("%s%s Connected - %d", TAG,__func__, value);
  }
}

void Device::TrustedChanged(bool value)
{
  if (m_properties.Trusted != value) {
    m_properties.Trusted = value;
    Log("%s%s Trusted - %d", TAG,__func__, value);
  }
}

void Device::BlockedChanged(bool value)
{
  if (m_properties.Blocked != value) {
    m_properties.Blocked = value;
    Log("%s%s Blocked - %d", TAG,__func__, value);
  }
}

void Device::AliasChanged(std::string value)
{
  if (m_properties.Alias != value) {
    m_properties.Alias = value;
    Log("%s%s Alias %s", TAG,__func__, LOG_STRING(value));
  }
}

void Device::AdapterChanged(std::string value)
{
  if (m_properties.AdapterPath != value) {
    m_properties.AdapterPath = value;
    Log("%s%s Adapter %s", TAG,__func__, LOG_STRING(value));
  }
}

void Device::LegacyPairingChanged(bool value)
{
  if (m_properties.LegacyPairing != value) {
    m_properties.LegacyPairing = value;
    Log("%s%s Legacy Pairing - %d", TAG,__func__, value);
  }
}

void Device::ManufacturerDataChanged(std::map<uint16_t, std::map<int, std::string>> value)
{
  if (m_properties.ManufacturerData != value) {
    m_properties.ManufacturerData = value;
    std::stringstream ss;
    for (const auto& [key, val] : value) {
      ss << key << ": ";
      for (const auto& [subKey, subVal] : val) {
        ss << subKey << "=" << subVal << " ";
      }
    }
    Log("%s%s ManufacturerData: %s", TAG, __func__, ss.str().c_str());
  }
}

void Device::ServiceDataChanged(std::map<std::string, std::map<int, std::string>> value)
{
  if (m_properties.ServiceData != value) {
    m_properties.ServiceData = value;
    std::stringstream ss;
    for (const auto& [key, val] : value) {
      ss << key << ": ";
      for (const auto& [subKey, subVal] : val) {
        ss << subKey << "=" << subVal << " ";
      }
    }
    Log("%s%s ServiceData: %s", TAG, __func__, ss.str().c_str());
  }
}

void Device::ServicesResolvedChanged(bool value)
{
  if (m_properties.ServicesResolved != value) {
    m_properties.ServicesResolved = value;
    Log("%s%s ServicesResolved - %d", TAG,__func__, value);
  }
}

void Device::RunEventLoop()
{
  Log("%s%s", TAG,__func__);
  while(m_running) {
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate doing some work
  }
}

void Device::PrintUUID()
{
  Log("%s%s", TAG,__func__);
  int i = 1;
  for (auto uuid : m_properties.UUIDs) {
    Log("%s%s %d UUID - %s", TAG,__func__, i++, LOG_STRING(uuid));
  }
}
