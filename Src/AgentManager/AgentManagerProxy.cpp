#include "AgentManagerProxy.h"
#include "Logger.h"
#include <sdbus-c++/Error.h>

#define TAG "AgentManagerProxy::"

std::string AGENT_MANAGER_WELLKNOWN_NAME = "org.bluez";
std::string AGENT_MANAGER_INTERFACE_OBJECT_PATH = "/org/bluez";

AgentManagerProxy::AgentManagerProxy(sdbus::IConnection &connection):
m_connection(connection),
ProxyInterfaces(connection, AGENT_MANAGER_WELLKNOWN_NAME, AGENT_MANAGER_INTERFACE_OBJECT_PATH)
{
  Log("%s%s", TAG,__func__);
  registerProxy();
}

AgentManagerProxy::~AgentManagerProxy()
{
  Log("%s%s", TAG,__func__);
  unregisterProxy();
}

void AgentManagerProxy::RegisterAgent(const sdbus::ObjectPath& agent, const std::string& capability)
{
  Log("%s%s", TAG,__func__);
  try {
    org::bluez::AgentManager1_proxy::RegisterAgent(agent, capability);
  } catch (const sdbus::Error& e) {
    Log("%s%s: D-Bus error: %s", TAG, __func__, e.what());
    return;
  }
}

void AgentManagerProxy::UnregisterAgent(const sdbus::ObjectPath& agent)
{
  Log("%s%s", TAG,__func__);
  try {
    org::bluez::AgentManager1_proxy::UnregisterAgent(agent);
  } catch (const sdbus::Error& e) {
    Log("%s%s: D-Bus error: %s", TAG, __func__, e.what());
    return;
  }
}

void AgentManagerProxy::RequestDefaultAgent(const sdbus::ObjectPath& agent)
{
  Log("%s%s", TAG,__func__);
  try {
    org::bluez::AgentManager1_proxy::RequestDefaultAgent(agent);
  } catch (const sdbus::Error& e) {
    Log("%s%s: D-Bus error: %s", TAG, __func__, e.what());
    return;
  }
}
