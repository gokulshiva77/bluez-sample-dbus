cmake_minimum_required(VERSION 3.10)
project(BluezEg VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(gen_dir                 ${CMAKE_CURRENT_BINARY_DIR}/Generated)
set(xml_dir                 ${CMAKE_CURRENT_SOURCE_DIR}/xml)
set(xml_files               Adapter1.xml
                            Agent1.xml
                            AgentManager1.xml
                            Device1.xml
                            Media1.xml
                            ProfileManager1.xml
                            Profile1.xml
                            )

file(MAKE_DIRECTORY ${gen_dir})

# generating sdbusc++ components
foreach(xml ${xml_files})
    string(REPLACE ".xml" "" PREFIXNAMES "${xml}")
    string(REPLACE "." "_" FILENAMES "${PREFIXNAMES}")
    execute_process(COMMAND sdbus-c++-xml2cpp 
                            --proxy=${gen_dir}/${FILENAMES}-proxy-generated.hpp 
                            --adaptor=${gen_dir}/${FILENAMES}-adapter-generated.hpp 
                            ${xml_dir}/${xml} 
                            RESULT_VARIABLE ret)
    if(NOT ret EQUAL 0)
        message(FATAL_ERROR "sdbus-c++-xml2cpp failed")
    endif()
endforeach()

add_library(SDBUSGenLib INTERFACE)
target_include_directories(SDBUSGenLib INTERFACE ${gen_dir})

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDBUS REQUIRED sdbus-c++)

find_package(Boost REQUIRED)



target_include_directories(SDBUSGenLib INTERFACE ${SDBUS_INCLUDE_DIRS})
target_link_libraries(SDBUSGenLib INTERFACE ${SDBUS_LIBRARIES})

file (GLOB SOURCES main.cpp
                   Src/Application.cpp
                   Src/Menu/Menu.cpp
                   Src/AgentManager/AgentManager.cpp
                   Src/AgentManager/AgentManagerProxy.cpp
                   Src/Agent/Agent.cpp
                   Src/Agent/AgentProxy.cpp
                   Src/Adapter/Adapter.cpp
                   Src/Adapter/AdapterProxy.cpp
                   Src/DeviceManager/DeviceManager.cpp
                   Src/Device/Device.cpp
                   Src/Device/DeviceProxy.cpp
                   Src/ObjectManager/ObjectManagerProxy.cpp
                   Src/ProfileManager/ProfileManager.cpp
                   Src/ProfileManager/ProfileManagerProxy.cpp
                   Src/Profile/Profile.cpp
                   Src/Profile/ProfileProxy.cpp
                   Src/SPPHandler/SPPHandler.cpp
                   Src/Utilities/Utilities.cpp
                   Src/Logger/Logger.cpp)

add_executable(BluezEg ${SOURCES})

target_include_directories(BluezEg PRIVATE Src/Adapter
                                           Src/AgentManager
                                           Src/Agent
                                           Src/DeviceManager/
                                           Src/Device
                                           Src/ObjectManager/
                                           Src/ProfileManager
                                           Src/Profile
                                           Src/SPPHandler
                                           Src/Logger
                                           Src/Utilities/
                                           Src/Menu
                                           Src/
                                           Inc
                                           Int
                                           ${CMAKE_SOURCE_DIR}
                                           ${Boost_INCLUDE_DIRS}
                                           )

target_link_libraries(BluezEg PRIVATE SDBUSGenLib ${Boost_LIBRARIES} pthread)

# Copy deleteDevices.sh to the build directory
add_custom_command(TARGET BluezEg POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
    ${CMAKE_SOURCE_DIR}/DeleteDevices.sh
    ${CMAKE_BINARY_DIR}/DeleteDevices.sh
)

# Install the executable
install(TARGETS BluezEg DESTINATION ${CMAKE_INSTALL_BINDIR} )