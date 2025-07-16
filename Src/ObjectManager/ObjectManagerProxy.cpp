#include "ObjectManagerProxy.h"

#include "Logger.h"
#include "DeviceHelper.h"

#define TAG "ObjectManagerProxy::"


const std::string OBJECT_MANAGER_WELLKNOWN_NAME = "org.bluez";
const std::string OBJECT_MANAGER_INTERFACE_OBJECT_PATH = "/";

const std::string DEVICE_INTERFACE = "org.bluez.Device1";
const std::string DBUS_INTERFACE = "org.freedesktop.DBus";

ObjectManagerProxy::ObjectManagerProxy(sdbus::IConnection& connection, IDeviceManager &deviceManager):
m_running(true),
m_connection(connection),
m_deviceManager(deviceManager),
ProxyInterfaces(connection, OBJECT_MANAGER_WELLKNOWN_NAME, OBJECT_MANAGER_INTERFACE_OBJECT_PATH)
{
  Log("%s%s", TAG,__func__);
}

ObjectManagerProxy::~ObjectManagerProxy()
{
  Log("%s%s", TAG,__func__);
  unregisterProxy();
  {
    std::lock_guard<std::mutex> lock(m_obj_manager_mutex);
    m_running = false;
  }
  m_queueCV.notify_one();
  if(m_eventLoopThread.joinable()){
    m_eventLoopThread.join();
  }
}

void ObjectManagerProxy::StartLooping()
{
  Log("%s%s", TAG,__func__);
  registerProxy();
  m_eventLoopThread = std::thread(&ObjectManagerProxy::RunEventLoop, this);
}

void ObjectManagerProxy::onInterfacesAdded(const sdbus::ObjectPath& objectPath
    , const std::map<std::string, std::map<std::string, sdbus::Variant>>& interfacesAndProperties)
{
  Log("%s%s Object Path - %s", TAG, __func__, LOG_STRING(std::string(objectPath)));
  std::lock_guard<std::mutex> lock(m_obj_manager_mutex);
  m_interface_added_queue.push({objectPath, interfacesAndProperties});
  bool toAdd = false;
  m_queueCV.notify_one();
}

void ObjectManagerProxy::onInterfacesRemoved(const sdbus::ObjectPath& objectPath
      , const std::vector<std::string>& interfaces)
{
  Log("%s%s Object Path - %s", TAG, __func__, LOG_STRING(objectPath));
  for (const auto& interface : interfaces)
  {
    if(DEVICE_INTERFACE == interface) {
      m_deviceManager.DeviceRemoved(std::string(interface));
    }
  }
}

void ObjectManagerProxy::RunEventLoop()
{
  Log("%s%s", TAG,__func__);
  while(m_running) {
    std::unique_lock<std::mutex> lock(m_obj_manager_mutex);
    m_queueCV.wait(lock, [this] { return !m_running || !m_interface_added_queue.empty(); });
    
    if(!m_running) {
      Log("%s%s Exiting RunEventLoop", TAG,__func__);
      break;
    }
    if (!m_interface_added_queue.empty()) {
      auto interfaceAdded = m_interface_added_queue.front();
      m_interface_added_queue.pop();
      lock.unlock();
      for (const auto& interface : interfaceAdded.interfacesAndProperties)
      {
        Log("%s%s Interface - %s", TAG,__func__, LOG_STRING(interface.first));
        if(DEVICE_INTERFACE == interface.first) {
          if(GetAndValidateClass(interface.second)) {
            m_deviceManager.DeviceAdded(std::string(interfaceAdded.path), false);
          }
        }
      }
      lock.lock();
    }

    // if (!m_interface_removed_queue.empty()) {
    //   auto interfaceRemoved = m_interface_removed_queue.front();
    //   m_interface_removed_queue.pop();
    //   lock.unlock();
    //   for (const auto& interface : interfaceRemoved.interfaces)
    //   {
    //     if(DEVICE_INTERFACE == interface) {
    //       m_deviceManager.DeviceRemoved(std::string(interfaceRemoved.path));
    //     }
    //   }
    // }
    // lock.lock();
  }
}

uint32_t ObjectManagerProxy::GetClass(std::map<std::string, sdbus::Variant> interfaces)
{
  uint32_t device_class = BluetoothMajorDeviceClass::Uncategorized;
  auto it = std::find_if(interfaces.begin(), interfaces.end(), [](const std::pair<std::string, sdbus::Variant>& interface) {
    return interface.first == "Class";
  });
  if(it != interfaces.end()) {
    device_class = it->second.get<uint32_t>();
  }
  return device_class;
}

bool ObjectManagerProxy::ValidateClass(uint32_t device_class)
{
  bool valid = false;
  BluetoothDeviceClass device_class_obj = BluetoothDeviceClass::from_uint32_t(device_class);
  if( device_class_obj.major_device_class == BluetoothMajorDeviceClass::Phone
      || device_class_obj.major_device_class == BluetoothMajorDeviceClass::AudioVideo) {
        valid = true;
  }
  return valid;
}

bool ObjectManagerProxy::GetAndValidateClass(std::map<std::string, sdbus::Variant> interfaces)
{
  uint32_t device_class = GetClass(interfaces);
  return ValidateClass(device_class);
}