
/**
 * @file DeviceManager.cpp
 * @brief Implementation of device manager for handling Bluetooth device lifecycle
 * @author Gokul
 * @date 2025
 */

#include "Logger.h"

#include "DeviceManager.h"

#define TAG "DeviceManager::" ///< Tag for logging messages

/**
 * @brief Construct a new Device Manager object
 * 
 * Initializes the device manager with a D-Bus connection and sets up
 * the running state for the event loop thread.
 * 
 * @param connection Reference to D-Bus system bus connection
 */
DeviceManager::DeviceManager(sdbus::IConnection &connection) : m_running(true),
                                                               m_connection(connection)
{
  Log("%s%s", TAG, __func__);
}

DeviceManager::~DeviceManager()
{
  Log("%s%s", TAG, __func__);
  {
    std::lock_guard<std::mutex> lock(m_deviceManagerMutex);
    m_running = false;
  }
  m_deviceQueueCV.notify_one();
  if (m_eventLoopThread.joinable())
  {
    m_eventLoopThread.join();
  }
  RemoveDevices();
}

void DeviceManager::StartLooping()
{
  Log("%s%s", TAG, __func__);
  m_eventLoopThread = std::thread(&DeviceManager::RunEventLoop, this);
}

void DeviceManager::DeviceAdded(std::string devicePath, bool enableLoop)
{
  std::lock_guard<std::mutex> lock(m_deviceManagerMutex);
  m_deviceQueue.push({devicePath, enableLoop});
  m_deviceQueueCV.notify_one();
  Log("%s%s Device - %s added to queue", TAG, __func__, LOG_STRING(devicePath));
}

void DeviceManager::DeviceRemoved(std::string devicePath)
{
  std::string deviceMAC = GetMACFromPath(devicePath);
  Log("%s%s Device - %s MAC - %s", TAG, __func__, LOG_STRING(devicePath), LOG_STRING(GetMACFromPath(devicePath)));
  try
  {
    m_devicesMap.at(deviceMAC).reset();
    m_devicesMap.erase(deviceMAC);
  }
  catch (const std::exception &e)
  {
    Log("%s%s Device - %s Seleting %s, Error - %s", TAG, __func__, LOG_STRING(devicePath), LOG_STRING(deviceMAC), e.what());
  }
}

std::shared_ptr<IDevice> DeviceManager::GetDevice(std::string mac)
{
  std::shared_ptr<IDevice> device = nullptr;
  try
  {
    std::lock_guard<std::mutex> lock(m_deviceManagerMutex);
    auto it = m_devicesMap.at(mac);
    device = it;
  }
  catch(const std::out_of_range& e)
  {
    Log("%s%s Out of Range Error", TAG, __func__, e.what());
  }
  return device;
}

std::vector<std::string> DeviceManager::GetDevicesMAC()
{
  std::vector<std::string> DevicesMAC;
  std::lock_guard<std::mutex> lock(m_deviceManagerMutex);
  for(const auto& device : m_devicesMap)
  {
    DevicesMAC.push_back(device.first);
  }
  return DevicesMAC;
}

void DeviceManager::RunEventLoop()
{
  try
  {
    while (m_running)
    {
      std::unique_lock<std::mutex> lock(m_deviceManagerMutex);
      m_deviceQueueCV.wait(lock, [this]
                           { return !m_running || !m_deviceQueue.empty(); });
      if (!m_running)
      {
        Log("%s%s Exiting RunEventLoop", TAG, __func__);
        break;
      }

      while (!m_deviceQueue.empty())
      {
        auto devicePath = m_deviceQueue.front();
        m_deviceQueue.pop();
        std::string deviceMAC = GetMACFromPath(devicePath.path);
        Log("%s%s Processing Device - %s MAC - %s", TAG, __func__, LOG_STRING(devicePath.path), LOG_STRING(deviceMAC));

        if (devicePath.path.empty() || deviceMAC.empty())
        {
          Log("%s%s Error: devicePath or deviceMAC is empty", TAG, __func__);
          continue;
        }
        if (m_devicesMap.find(deviceMAC) != m_devicesMap.end())
        {
          Log("%s%s Device - %s already exists", TAG, __func__, LOG_STRING(deviceMAC));
          continue;
        }
        try
        {
          m_devicesMap[deviceMAC] = std::make_shared<Device>(m_connection, devicePath.path);
          Log("%s%s Device Count - %d", TAG, __func__, m_devicesMap.size());
          if (lock.owns_lock())
          {
            lock.unlock();
          }
        }
        catch (const sdbus::Error &e)
        {
          Log("%s%s Error creating device for devicePath - %s, Error - %s", TAG, __func__, LOG_STRING(devicePath.path), e.what());
        }
        // No need to re-lock the mutex here
      }
    }
  }
  catch (const std::system_error &e)
  {
    Log("%s%s System Error", TAG, __func__, e.what());
  }
}

void DeviceManager::RemoveDevices()
{
  Log("%s%s", TAG, __func__);
  try
  {
    for (auto it = m_devicesMap.begin(); it != m_devicesMap.end();)
    {
      if (it->second == nullptr)
      {
        ++it;
        continue;
      }
      auto properties = it->second->GetProperties();
      if (properties.Connected)
      {
        it->second->Disconnect();
      }
      if (properties.Paired)
      {
        it->second->CancelPairing();
      }
      it->second.reset();
      it = m_devicesMap.erase(it);
    }
    if (!m_devicesMap.empty())
    {
      m_devicesMap.clear();
    }
  }
  catch (const sdbus::Error &e)
  {
    Log("%s%s Error - %s", TAG, __func__, e.what());
  }
}

std::string DeviceManager::GetMACFromPath(const std::string &path)
{
  // Extract the MAC address part from the path
  std::string toFind("dev_");
  size_t pos = path.find(toFind);
  if (pos == std::string::npos)
  {
    return "";
  }
  std::string mac = path.substr(pos + toFind.length());

  // Replace underscores with colons
  std::replace(mac.begin(), mac.end(), '_', ':');

  return mac;
}
