#include "ProfileProxy.h"

#include "Logger.h"

#define TAG "ProfileProxy::"


ProfileProxy::ProfileProxy(sdbus::IConnection &connection, std::string profilePath):
AdaptorInterfaces(connection, profilePath),
m_connection(connection),
m_profilePath(profilePath),
m_spp(nullptr)
{
  Log("%s%s", TAG, __func__);
  registerAdaptor();
}

ProfileProxy::~ProfileProxy()
{
  Log("%s%s", TAG, __func__);
  unregisterAdaptor();
}

void ProfileProxy::Release()
{
  Log("%s%s", TAG, __func__);

}

void ProfileProxy::NewConnection(const sdbus::ObjectPath& device, 
                     const sdbus::UnixFd& fd, 
                     const std::map<std::string, sdbus::Variant>& fd_properties)
{
  Log("%s%s Path - %s FD - %d", TAG, __func__, LOG_STRING(std::string(device)), fd.get());
  for (auto properties : fd_properties) {
    Log("%s%s Properties - %s", TAG, __func__, LOG_STRING(properties.first));
  }
  m_spp = std::make_unique<SPPHandler>(fd);
  if(m_spp) {
    m_spp->StartOperations();
  }
}

void ProfileProxy::RequestDisconnection(const sdbus::ObjectPath& device)
{
  Log("%s%s Path - %s", TAG, __func__, LOG_STRING(std::string(device)));
}
