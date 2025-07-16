#include "Application.h"

#define TAG "Application::"

Application::Application(sdbus::IConnection &connection, std::string hcidevice, std::string deviceName, std::string deviceClass):
m_running(true),
m_connection(connection),
m_hcidevice(hcidevice),
m_deviceName(deviceName),
m_deviceClassStr(deviceClass)
{
  Log("%s%s", TAG, __func__);
  if(m_deviceClassStr == "SMARTPHONE") {
    m_deviceClass = 0x3C0408;
  } else {
    m_deviceClass = 0x240408;
  }
  m_deviceManager = std::make_unique<DeviceManager>(m_connection);
  m_agent = std::make_unique<Agent>(m_connection, AGENT_MANAGER_PATH, *m_deviceManager);
  m_agentManager = std::make_unique<AgentManager>(m_connection, AGENT_MANAGER_PATH);
  m_adapter = std::make_unique<Adapter>(m_connection, m_hcidevice, m_deviceName, m_deviceClass);
  m_profileManager = std::make_unique<ProfileManager>(m_connection);
  m_objProxy = std::make_unique<ObjectManagerProxy>(m_connection, *m_deviceManager);
}

Application::~Application()
{
  Log("%s%s", TAG, __func__);
  m_running = false;
  if(m_eventLoopThread.joinable()) {
    m_connection.leaveEventLoop();
    m_eventLoopThread.join();
  }
}

void Application::runEventLoopAsync(sdbus::IConnection &m_connection)
{
  // Main thread simulates other work but will keep running forever
  while (m_running)
  {
    m_connection.enterEventLoopAsync();
    std::this_thread::sleep_for(std::chrono::seconds(1)); // Simulate doing some work
  }
}

void Application::StartApplication()
{
  Log("%s%s", TAG, __func__);
  m_agentManager->StartLooping();
  m_deviceManager->StartLooping();
  m_objProxy->StartLooping();
  std::map<std::string, sdbus::Variant> options = {
    { "Name", sdbus::Variant("Test SPP Profile") },
    { "Role", sdbus::Variant("client") },
    { "PSM", sdbus::Variant(uint16_t(0x0003)) } };  

  m_profileManager->RegisterProfile(SPP_PATH, SPP_UUID, options);

  // Start the event loop asynchronously in a separate thread
  m_eventLoopThread = std::thread(&Application::runEventLoopAsync, this, std::ref(m_connection));
    
}

IDeviceManager& Application::GetDeviceManager()
{
  return *m_deviceManager;
}

void Application::StartDiscovery()
{
  m_adapter->StartDiscovery();
}

void Application::StartScan()
{
  m_adapter->StartScan();
}