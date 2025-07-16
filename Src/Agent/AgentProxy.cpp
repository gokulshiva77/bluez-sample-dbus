#include "AgentProxy.h"

#include "Logger.h"

#define TAG "AgentProxy::"

AgentProxy::AgentProxy(sdbus::IConnection &connection, std::string path, IAgent &agent):
m_connection(connection),
m_agent(agent),
AdaptorInterfaces(connection, path)
{
  Log("%s%s", TAG,__func__);
  try
  {
    registerAdaptor();
  }
  catch(const sdbus::Error& e)
  {
    Log("%s%s Error - %s ", TAG,__func__, e.what());
    throw;
  }
}
AgentProxy::~AgentProxy()
{
  Log("%s%s", TAG,__func__);
  unregisterAdaptor();
}

void AgentProxy::Release()
{
  Log("%s%s", TAG,__func__);
}

std::string AgentProxy::RequestPinCode(const sdbus::ObjectPath& arg0)
{
  Log("%s%s Path - %s", TAG,__func__, LOG_STRING(std::string(arg0)));
  return "1";
}

void AgentProxy::DisplayPinCode(const sdbus::ObjectPath& arg0, const std::string& arg1)
{
  Log("%s%s Path - %s, PIN - %d", TAG,__func__, LOG_STRING(std::string(arg0)), arg1);
}

uint32_t AgentProxy::RequestPasskey(const sdbus::ObjectPath& arg0)
{
  Log("%s%s Path - %s", TAG,__func__, LOG_STRING(std::string(arg0)));
  return 1;
}

void AgentProxy::DisplayPasskey(const sdbus::ObjectPath& arg0, const uint32_t& arg1, const uint16_t& arg2)
{
  Log("%s%s Path - %s, Pass - %d", TAG,__func__, LOG_STRING(std::string(arg0)), arg1);
}

void AgentProxy::RequestConfirmation(const sdbus::ObjectPath& arg0, const uint32_t& arg1)
{
  Log("%s%s Path - %s, Confirm - %d", TAG,__func__, LOG_STRING(std::string(arg0)), arg1);
  m_agent.RequestConfirmation(std::string(arg0));
}

void AgentProxy::RequestAuthorization(const sdbus::ObjectPath& arg0)
{
  Log("%s%s Path - %s", TAG,__func__, LOG_STRING(std::string(arg0)));
}

void AgentProxy::AuthorizeService(const sdbus::ObjectPath& arg0, const std::string& arg1)
{
  Log("%s%s Path - %s, Service - %s", TAG,__func__, LOG_STRING(std::string(arg0)), LOG_STRING(arg1));
}

void AgentProxy::Cancel()
{
  Log("%s%s", TAG,__func__);
}
