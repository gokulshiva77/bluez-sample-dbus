/**
 * @file ProfileManager.cpp
 * @brief Implementation of manager for Bluetooth profile registration and lifecycle
 * @author Gokul
 * @date 2025
 */

#include "ProfileManager.h"

#include "Logger.h"

#define TAG "ProfileManager::" ///< Tag for logging messages

/**
 * @brief Construct a new Profile Manager object
 * 
 * Initializes the profile manager with a D-Bus connection and sets up
 * the profile manager proxy for communication with BlueZ.
 * 
 * @param connection Reference to D-Bus system bus connection
 */
ProfileManager::ProfileManager(sdbus::IConnection &connection):
m_profileManagerProxy(connection),
m_connection(connection),
m_profileProxy(nullptr)
{
  Log("%s%s", TAG, __func__);
}

ProfileManager::~ProfileManager()
{
  Log("%s%s", TAG, __func__);
}

void ProfileManager::RegisterProfile(const sdbus::ObjectPath& profile, 
                       const std::string& UUID, 
                       const std::map<std::string, sdbus::Variant>& options)
{
  Log("%s%s Profile Path - %s, UUID - %s", TAG, __func__, LOG_STRING(profile), LOG_STRING(UUID));
  try
  {
    m_profileManagerProxy.RegisterProfile(profile, UUID, options);
    m_profileProxy = std::make_unique<ProfileProxy>(m_connection, profile);
  }
  catch(const sdbus::Error& e)
  {
    Log("%s%s Profile Path - %s, UUID - %s, Error - %s", TAG, __func__, LOG_STRING(profile), LOG_STRING(UUID), e.what());
  }
}

void ProfileManager::UnregisterProfile(const sdbus::ObjectPath& profile)
{
  Log("%s%s Profile Path - %s", TAG, __func__, LOG_STRING(profile));
  try
  {
    m_profileManagerProxy.UnregisterProfile(profile);
  }
  catch(const sdbus::Error& e)
  {
    Log("%s%s Profile Path - %s, Error - %s", TAG, __func__, LOG_STRING(profile), e.what());
  }
}
