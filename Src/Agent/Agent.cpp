#include "Agent.h"
#include "Logger.h"

#define TAG "Agent::"

Agent::Agent(sdbus::IConnection &connection, std::string Path, IDeviceManager &deviceManager):
m_connection(connection),
m_agentProxy(m_connection, Path, *this),
m_deviceManager(deviceManager)
{
  Log("%s%s", TAG,__func__);
}

Agent::~Agent()
{
  Log("%s%s", TAG,__func__);
}

void Agent::RequestConfirmation(std::string path)
{
  m_deviceManager.DeviceAdded(path, true);
}
