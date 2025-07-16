/**
 * @file DeviceProxy.cpp
 * @brief Implementation of D-Bus proxy for BlueZ Device1 interface
 * @author Gokul
 * @date 2025
 */

#include <string>
#include <map>
#include <cstdint>
#include <functional>

#include "DeviceProxy.h"

#include "Logger.h"
#include "Utilities.h"


#define TAG "DeviceProxy::"

const std::string DEVICE_WELLKNOWN_NAME = "org.bluez";

std::map<const std::string, const std::function<void(IDevice& callback, sdbus::Variant)>> dispatchDeviceCallbacks{
  {DEVICE_PROPERTY_Address, [](IDevice& callback, sdbus::Variant value) { callback.AddressChanged(getFromSVariant<std::string>(value)); }},
  {DEVICE_PROPERTY_AddressType, [](IDevice& callback, sdbus::Variant value) { callback.AddressTypeChanged(getFromSVariant<std::string>(value)); }},
  {DEVICE_PROPERTY_Name, [](IDevice& callback, sdbus::Variant value) { callback.NameChanged(getFromSVariant<std::string>(value)); }},
  {DEVICE_PROPERTY_UUIDs, [](IDevice& callback, sdbus::Variant value) { callback.UUIDsChanged(getFromSVariant<std::vector<std::string>>(value)); }},
  {DEVICE_PROPERTY_Paired, [](IDevice& callback, sdbus::Variant value) { callback.PairedChanged(getFromSVariant<bool>(value)); }},
  {DEVICE_PROPERTY_Connected, [](IDevice& callback, sdbus::Variant value) { callback.ConnectedChanged(getFromSVariant<bool>(value)); }},
  {DEVICE_PROPERTY_Trusted, [](IDevice& callback, sdbus::Variant value) { callback.TrustedChanged(getFromSVariant<bool>(value)); }},
  {DEVICE_PROPERTY_Blocked, [](IDevice& callback, sdbus::Variant value) { callback.BlockedChanged(getFromSVariant<bool>(value)); }},
  {DEVICE_PROPERTY_Alias, [](IDevice& callback, sdbus::Variant value) { callback.AliasChanged(getFromSVariant<std::string>(value)); }},
  {DEVICE_PROPERTY_Adapter, [](IDevice& callback, sdbus::Variant value) { callback.AdapterChanged(getFromSVariant<std::string>(value)); }},
  {DEVICE_PROPERTY_LegacyPairing, [](IDevice& callback, sdbus::Variant value) { callback.LegacyPairingChanged(getFromSVariant<bool>(value)); }},
  {DEVICE_PROPERTY_ServiceData, [](IDevice& callback, sdbus::Variant value) { }},
  {DEVICE_PROPERTY_ServicesResolved, [](IDevice& callback, sdbus::Variant value) { callback.ServicesResolvedChanged(getFromSVariant<bool>(value)); }},
  {DEVICE_PROPERTY_Icon, [](IDevice& callback, sdbus::Variant value) { callback.IconChanged(getFromSVariant<std::string>(value)); }},
  {DEVICE_PROPERTY_Class, [](IDevice& callback, sdbus::Variant value) { callback.ClassChanged(getFromSVariant<uint32_t>(value)); }},
  {DEVICE_PROPERTY_ManufacturerData, [](IDevice& callback, sdbus::Variant value) {  }}
};

std::map<const std::string, const std::function<void(DeviceProperties& properties, DeviceProxy &proxy)>> dispatchDeviceProperties{
  {DEVICE_PROPERTY_Address, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.Address = proxy.GetAddress(); }},
  {DEVICE_PROPERTY_AddressType, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.AddressType = proxy.GetAddressType(); }},
  {DEVICE_PROPERTY_Name, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.Name = proxy.GetName(); }},
  {DEVICE_PROPERTY_UUIDs, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.UUIDs = proxy.GetUUIDs(); }},
  {DEVICE_PROPERTY_Paired, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.Paired = proxy.GetPaired(); }},
  {DEVICE_PROPERTY_Connected, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.Connected = proxy.GetConnected(); }},
  {DEVICE_PROPERTY_Trusted, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.Trusted = proxy.GetTrusted(); }},
  {DEVICE_PROPERTY_Blocked, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.Blocked = proxy.GetBlocked(); }},
  {DEVICE_PROPERTY_Alias, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.Alias = proxy.GetAlias(); }},
  {DEVICE_PROPERTY_Adapter, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.AdapterPath = std::string(proxy.GetAdapter()); }},
  {DEVICE_PROPERTY_LegacyPairing, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.LegacyPairing = proxy.GetLegacyPairing(); }},
  {DEVICE_PROPERTY_ServiceData, [](DeviceProperties& properties, DeviceProxy &proxy) { }},
  {DEVICE_PROPERTY_ServicesResolved, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.ServicesResolved = proxy.GetServicesResolved(); }},
  {DEVICE_PROPERTY_Icon, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.Icon = proxy.GetIcon(); }},
  {DEVICE_PROPERTY_Class, [](DeviceProperties& properties, DeviceProxy &proxy) { properties.Class = proxy.GetClass(); }},
  {DEVICE_PROPERTY_ManufacturerData, [](DeviceProperties& properties, DeviceProxy &proxy) {  }}
};

DeviceProxy::DeviceProxy(sdbus::IConnection &connection,IDevice &device, std::string devicePath):
ProxyInterfaces(connection, DEVICE_WELLKNOWN_NAME, devicePath),
m_devicePath(devicePath),
m_connection(connection),
m_device(device)
{
  Log("%s%s", TAG,__func__);
  try
  {
    DeviceProperties properites = GetProperties();
    device.PropertiesChanged(properites);
    registerProxy();
  }
  catch(const std::exception& e)
  {
    Log("%s%s Error - %s", TAG,__func__, e.what());
  }
}

DeviceProxy::~DeviceProxy()
{
  Log("%s%s", TAG,__func__);
  unregisterProxy();
}

void DeviceProxy::Connect()
{
  org::bluez::Device1_proxy::Connect();
}

void DeviceProxy::Disconnect()
{
  org::bluez::Device1_proxy::Disconnect();
}

void DeviceProxy::ConnectProfile(std::string uuid)
{
  org::bluez::Device1_proxy::ConnectProfile(uuid);
}

void DeviceProxy::DisconnectProfile(std::string uuid)
{
  org::bluez::Device1_proxy::DisconnectProfile(uuid);
}

void DeviceProxy::Pair()
{
  org::bluez::Device1_proxy::Pair();
}

void DeviceProxy::CancelPairing()
{
  org::bluez::Device1_proxy::CancelPairing();
}

std::string DeviceProxy::GetAddress()
{
  return Address();
}

std::string DeviceProxy::GetAddressType()
{
  return AddressType();
}

std::string DeviceProxy::GetName()
{
  return Name();
}

std::string DeviceProxy::GetIcon()
{
  return Icon();
}

uint32_t DeviceProxy::GetClass()
{
  return Class();
}

std::vector<std::string> DeviceProxy::GetUUIDs()
{
 return UUIDs(); 
}

bool DeviceProxy::GetPaired()
{
  return Paired();
}

bool DeviceProxy::GetConnected()
{
  return Connected();
}

bool DeviceProxy::GetTrusted()
{
  return Trusted();
}

bool DeviceProxy::GetBlocked()
{
  return Blocked();
}

std::string DeviceProxy::GetAlias()
{
  return Alias();
}

sdbus::ObjectPath DeviceProxy::GetAdapter()
{
  return Adapter();
}

bool DeviceProxy::GetLegacyPairing()
{
  return LegacyPairing();
}

std::map<std::string, sdbus::Variant> DeviceProxy::GetServiceData()
{
  return ServiceData();
}

bool DeviceProxy::GetServicesResolved()
{
  return ServicesResolved();
}

void DeviceProxy::SetTrusted(bool value)
{
  Trusted(value);
}

void DeviceProxy::SetBlocked(bool value)
{
  Blocked(value);
}

void DeviceProxy::SetAlias(const std::string &value)
{
  Alias(value);
}

DeviceProperties DeviceProxy::GetProperties()
{
  DeviceProperties properties;
  for (const auto &prop : dispatchDeviceProperties) {
    try
    {
      prop.second(properties, *this);
    }
    catch(const std::out_of_range& e)
    {
      Log("%s%s %s Not Available in List", TAG,__func__, LOG_STRING(prop.first));
    }
  }
  return properties;
}

void DeviceProxy::onPropertiesChanged(const std::string& interface_name, 
    const std::map<std::string, sdbus::Variant>& changed_properties, 
    const std::vector<std::string>& invalidated_properties)
{
  for (const auto &prop : changed_properties) {
    Log("%s%s Name - %s", TAG, __func__, LOG_STRING(prop.first));
    try
    {
      dispatchDeviceCallbacks.at(prop.first)(m_device, prop.second);
    }
    catch(const std::out_of_range& e)
    {
      Log("%s%s %s Not Available in List", TAG,__func__, LOG_STRING(prop.first));
    }
  }
}
