#include "ProfileManagerProxy.h"

#include "Logger.h"

#define TAG "ProfileManagerProxy::"

const std::string PROFILE_MANAGER_WELLKNOWN_NAME = "org.bluez";
const std::string PROFILE_MANAGER_INTERFACE_OBJECT_PATH = "/org/bluez";

ProfileManagerProxy::ProfileManagerProxy(sdbus::IConnection &connection):
ProxyInterfaces(connection, PROFILE_MANAGER_WELLKNOWN_NAME, PROFILE_MANAGER_INTERFACE_OBJECT_PATH),
m_connection(connection)
{
  Log("%s%s", TAG, __func__);
  registerProxy();
}

ProfileManagerProxy::~ProfileManagerProxy()
{
  Log("%s%s", TAG, __func__);
  unregisterProxy();
}

void ProfileManagerProxy::RegisterProfile(const sdbus::ObjectPath& profile, 
                       const std::string& UUID, 
                       const std::map<std::string, sdbus::Variant>& options)
{
  Log("%s%s Profile Path - %s, UUID - %s", TAG, __func__, LOG_STRING(profile), LOG_STRING(UUID));
  org::bluez::ProfileManager1_proxy::RegisterProfile(profile, UUID, options);
}

void ProfileManagerProxy::UnregisterProfile(const sdbus::ObjectPath& profile)
{
  Log("%s%s Profile Path - %s", TAG, __func__, LOG_STRING(profile));
  org::bluez::ProfileManager1_proxy::UnregisterProfile(profile);
}
