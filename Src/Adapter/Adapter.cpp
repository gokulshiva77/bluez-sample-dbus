
#include "Adapter.h"

#include "Logger.h"

#define TAG "Adapter::"

Adapter::Adapter(sdbus::IConnection& connection, std::string hciDevice, std::string deviceName, uint32_t deviceClass):
m_hciDevice(hciDevice),
m_deviceName(deviceName),
m_deviceClass(deviceClass),
m_adapterProxy(connection, *this, hciDevice),
m_connection(connection)
{
  Log("%s%s", TAG,__func__);
}

Adapter::~Adapter()
{
  Log("%s%s", TAG,__func__);
  StopDiscovery();
}

void Adapter::AddressChanged(std::string value)
{
  Log("%s%s Address- %s ", TAG,__func__, LOG_STRING(value));
}

void Adapter::AddressTypeChanged(std::string value)
{
  Log("%s%s AddressType: %s", TAG,__func__, LOG_STRING(value));
}

void Adapter::NameChanged(std::string value)
{
  Log("%s%s Name: %s", TAG,__func__, LOG_STRING(value));
}

void Adapter::AliasChanged(std::string value)
{
  Log("%s%s Alias %s", TAG,__func__, LOG_STRING(value));
}

void Adapter::ClassChanged(uint32_t value)
{
  Log("%s%s Class - %.6x", TAG,__func__, value);
}

void Adapter::PoweredChanged(bool value)
{
  Log("%s%s Powered - %d", TAG,__func__, value);
}

void Adapter::DiscoverableChanged(bool value)
{
  Log("%s%s Discoverable - %d", TAG,__func__, value);
}

void Adapter::DiscoverableTimeoutChanged(uint32_t value)
{
  Log("%s%s Discoverable Timeout - %d", TAG,__func__, value);
}

void Adapter::PairableChanged(bool value)
{
  Log("%s%s Pairable - %d", TAG,__func__, value);
}

void Adapter::PairableTimeoutChanged(uint32_t value)
{
  Log("%s%s Pairable Timeout - %d", TAG,__func__, value);
}

void Adapter::DiscoveringChanged(bool value)
{
  Log("%s%s Discovering = %d", TAG,__func__, value);
}

void Adapter::UUIDsChanged(std::vector<std::string> value)
{
  for(auto &uuid : value)
  {
    Log("%s%s UUID - %s", TAG,__func__, LOG_STRING(uuid));
  }
}

void Adapter::StartScan()
{
  Log("%s%s", TAG,__func__);
  m_adapterProxy.SetPowered(true);
  m_adapterProxy.ResetStatus();
  m_adapterProxy.SetAlias(m_deviceName);
  m_adapterProxy.SetPairable(true);
  m_adapterProxy.SetPairableTimeout(60);
  m_adapterProxy.SetDiscoverable(true);
  m_adapterProxy.SetDiscoverableTimeout(60);
  m_adapterProxy.SetClass(m_deviceClass);
  m_adapterProxy.StartDiscovery();
}

void Adapter::StartDiscovery()
{
  Log("%s%s", TAG,__func__);
  m_adapterProxy.SetPowered(true);
  m_adapterProxy.ResetStatus();
  m_adapterProxy.SetAlias(m_deviceName);
  m_adapterProxy.SetPairable(true);
  m_adapterProxy.SetPairableTimeout(0);
  m_adapterProxy.SetDiscoverable(true);
  m_adapterProxy.SetDiscoverableTimeout(0);
  m_adapterProxy.SetClass(m_deviceClass);
}

void Adapter::StopDiscovery()
{
  if(m_adapterProxy.GetDiscovering()) {
    m_adapterProxy.StopDiscovery();
  }
  if(m_adapterProxy.GetDiscoverable()) {
    m_adapterProxy.SetDiscoverable(false);
  }
  if(m_adapterProxy.GetPairable()) {
    m_adapterProxy.SetPairable(false);
  }
  
  if(m_adapterProxy.GetPowered()) {
    m_adapterProxy.SetPowered(false);
  }
}

void Adapter::SetClass(uint32_t value)
{
  m_adapterProxy.SetClass(value);
}
