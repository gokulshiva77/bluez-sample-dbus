/**
 * @file AdapterProxy.cpp
 * @brief Implementation of D-Bus proxy for BlueZ Adapter1 interface
 * @author Gokul
 * @date 2025
 */

#include <cstdint>
#include <functional>
#include <cstdlib>
#include <sstream>

#include "AdapterProxy.h"

#include "Logger.h"
#include "Utilities.h"

#define TAG "AdapterProxy::" ///< Tag for logging messages

const std::string ADAPTER_WELLKNOWN_NAME = "org.bluez";                ///< BlueZ D-Bus service name
const std::string ADAPTER_INTERFACE_OBJECT_PATH = "/org/bluez/";       ///< Base path for BlueZ objects

/**
 * @brief Dispatch table for adapter property change callbacks
 * 
 * Maps adapter property names to callback functions that forward
 * property changes to the IAdapter interface.
 */
std::map<const std::string ,const std::function<void(IAdapter& callback, sdbus::Variant)>> dispatchAdapterCallbacks{
  {ADAPTER_PROPERTY_Address,[](IAdapter& callback, sdbus::Variant value){ callback.AddressChanged(getFromSVariant<std::string>(value)); } },
  {ADAPTER_PROPERTY_AddressType,[](IAdapter& callback, sdbus::Variant value){ callback.AddressTypeChanged(getFromSVariant<std::string>(value)); } },
  {ADAPTER_PROPERTY_Name,[](IAdapter& callback, sdbus::Variant value){ callback.NameChanged(getFromSVariant<std::string>(value)); } },
  {ADAPTER_PROPERTY_Alias,[](IAdapter& callback, sdbus::Variant value){ callback.AliasChanged(getFromSVariant<std::string>(value)); } },
  {ADAPTER_PROPERTY_Class,[](IAdapter& callback, sdbus::Variant value){ callback.ClassChanged(getFromSVariant<uint32_t>(value)); } },
  {ADAPTER_PROPERTY_Powered,[](IAdapter& callback, sdbus::Variant value){ callback.PoweredChanged(getFromSVariant<bool>(value)); } },
  {ADAPTER_PROPERTY_Discoverable,[](IAdapter& callback, sdbus::Variant value){ callback.DiscoverableChanged(getFromSVariant<bool>(value)); } },
  {ADAPTER_PROPERTY_DiscoverableTimeout,[](IAdapter& callback, sdbus::Variant value){ callback.DiscoverableTimeoutChanged(getFromSVariant<uint32_t>(value)); } },
  {ADAPTER_PROPERTY_Pairable,[](IAdapter& callback, sdbus::Variant value){ callback.PairableChanged(getFromSVariant<bool>(value)); } },
  {ADAPTER_PROPERTY_PairableTimeout,[](IAdapter& callback, sdbus::Variant value){ callback.PairableTimeoutChanged(getFromSVariant<uint32_t>(value)); } },
  {ADAPTER_PROPERTY_Discovering,[](IAdapter& callback, sdbus::Variant value){ callback.DiscoveringChanged(getFromSVariant<bool>(value)); } },
  {ADAPTER_PROPERTY_UUIDs,[](IAdapter& callback, sdbus::Variant value){ callback.UUIDsChanged(getFromSVariant<std::vector<std::string>>(value)); } },
};

AdapterProxy::AdapterProxy(sdbus::IConnection& connection, IAdapter& adapter, std::string hciDevice):
m_connection(connection),
m_adapter(adapter),
m_hciDevice(hciDevice),
ProxyInterfaces(connection, ADAPTER_WELLKNOWN_NAME, ADAPTER_INTERFACE_OBJECT_PATH + hciDevice)
{
  Log("%s%s HCI - %s, Path - %s", TAG,__func__, LOG_STRING(m_hciDevice), LOG_STRING(ADAPTER_INTERFACE_OBJECT_PATH + hciDevice));
  registerProxy();
}

AdapterProxy::~AdapterProxy()
{
  Log("%s%s", TAG,__func__);
  unregisterProxy();
}

void AdapterProxy::SetPowered(const bool& value)
{
  Log("%s%s Value - %d", TAG, __func__, value);
  if(Powered() != value) {
    Powered(value);
  } else {
    Log("%s%s Already same Value - %d", TAG, __func__, value);
  }  
}

bool AdapterProxy::GetPowered()
{
  return Powered();
}


void AdapterProxy::SetDiscoverable(const bool& value)
{
  Log("%s%s Value - %d", TAG, __func__, value);
  if(Discoverable() != value) {
    Discoverable(value);
  } else {
    Log("%s%s Already same Value - %d", TAG, __func__, value);
  }
}

bool AdapterProxy::GetDiscoverable()
{
  return Discoverable();
}

bool AdapterProxy::GetDiscovering()
{
  return Discovering();
}


void AdapterProxy::SetDiscoverableTimeout(const uint32_t& value)
{
  Log("%s%s Value - %d", TAG, __func__, value);
  DiscoverableTimeout(value);
}

uint32_t AdapterProxy::GetDiscoverableTimeout()
{
  return DiscoverableTimeout();
}


void AdapterProxy::SetPairable(const bool& value)
{
  Log("%s%s Value - %d", TAG, __func__, value);
  if(Pairable() != value) {
    Pairable(value);
  } else {
    Log("%s%s Already same Value - %d", TAG, __func__, value);
  }
}

bool AdapterProxy::GetPairable()
{
  return Pairable();
}

void AdapterProxy::SetPairableTimeout(const uint32_t& value)
{
  Log("%s%s Value - %d", TAG, __func__, value);
  PairableTimeout(value);
}

uint32_t AdapterProxy::GetPairableTimeout()
{
  return PairableTimeout();
}

void AdapterProxy::SetAlias(const std::string &value)
{
  Log("%s%s Value - %s", TAG,__func__, LOG_STRING(value));
  if(Alias() != value) {
    Alias(value);
  } else {
    Log("%s%s Already same Value - %s", TAG,__func__, LOG_STRING(value));
  }
}

std::string AdapterProxy::GetAlias()
{
  return Alias();
}

void AdapterProxy::SetClass(const uint32_t &value)
{
  Log("%s%s Value - %.6x", TAG,__func__, value);
  if(Class() == value) {
    Log("%s%s Already same Value - %.6x", TAG,__func__, value);
    return;
  }
  try
  {
    std::stringstream ss;
    ss << "class 0x" << std::hex << value;
    std::string command = ConstructHCICommand(ss.str());
    ExecuteBashCommand(command);
    Log("%s%s Changed Class %.6x", TAG,__func__, Class());
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

uint32_t AdapterProxy::GetClass()
{
  return Class();
}

void AdapterProxy::StartDiscovery()
{
  org::bluez::Adapter1_proxy::StartDiscovery();
}

void AdapterProxy::SetDiscoveryFilter(const std::map<std::string, sdbus::Variant>& filters)
{
  org::bluez::Adapter1_proxy::SetDiscoveryFilter(filters);
}

void AdapterProxy::StopDiscovery()
{
  org::bluez::Adapter1_proxy::StopDiscovery();
}

void AdapterProxy::RemoveDevice(const std::string& device_path)
{
    org::bluez::Adapter1_proxy::RemoveDevice(sdbus::ObjectPath(device_path));
}

std::vector<std::string> AdapterProxy::GetDiscoveryFilters()
{
  return org::bluez::Adapter1_proxy::GetDiscoveryFilters();
}

void AdapterProxy::ResetStatus()
{
  Log("%s%s", __func__, TAG);
  try
  {
    std::string command = ConstructHCICommand("rstat");
    ExecuteBashCommand(command);
    Log("%s%s Changed Class %.6x", TAG,__func__, Class());
  }
  catch(const std::exception& e)
  {
    std::cerr << e.what() << '\n';
  }
}

void AdapterProxy::onPropertiesChanged(const std::string& interface_name, 
  const std::map<std::string, sdbus::Variant>& changed_properties, 
  const std::vector<std::string>& invalidated_properties)
{
  Log("%s%s Interface Name %s", TAG,LOG_STRING(interface_name));
  for (const auto &prop : changed_properties) {
    Log("%s%s Name - %s", TAG, __func__, LOG_STRING(prop.first));
    try
    {
      dispatchAdapterCallbacks.at(prop.first)(m_adapter, prop.second);
    }
    catch(const std::out_of_range& e)
    {
      std::cerr << e.what() << '\n';
    }
  }
}

int AdapterProxy::ExecuteBashCommand(std::string command)
{
  int ret = std::system(command.c_str());
  if(ret == 0) {
    Log("%s%s Command %s Executed Successfully", TAG,__func__, LOG_STRING(command));
  } else {
    Log("%s%s Command %s Not Executed Successfully, Error: %d", TAG,__func__, LOG_STRING(command), ret);
  }
  return ret;
}

std::string AdapterProxy::ConstructHCICommand(std::string args)
{
  std::string command;
  command = "sudo hciconfig " + m_hciDevice + " " + args;
  return command;
}
