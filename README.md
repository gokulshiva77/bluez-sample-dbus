# BlueZ D-Bus C++ Sample Application

A comprehensive C++ application demonstrating Bluetooth communication using the BlueZ stack via D-Bus interface. This project showcases device discovery, pairing, SPP (Serial Port Profile) connections, and Bluetooth device management using modern C++17 and the sdbus-c++ library.

## Overview

This application provides a complete Bluetooth stack implementation using BlueZ's D-Bus interface, featuring:

- Bluetooth adapter management and configuration
- Device discovery and scanning
- Bluetooth pairing and authentication
- SPP (Serial Port Profile) connections for data communication
- Interactive menu-driven interface
- Thread-safe device management
- Real-time D-Bus event handling

## Prerequisites

### System Requirements

- Linux system with BlueZ installed and running
- D-Bus system bus
- Bluetooth hardware adapter (USB, PCIe, or built-in)
- Root privileges or appropriate permissions for Bluetooth operations

### Required Libraries

- **sdbus-c++**: Modern C++ D-Bus library for BlueZ communication
- **Boost**: C++ libraries for additional functionality
- **BlueZ**: Linux Bluetooth protocol stack (system dependency)
- **CMake 3.10+**: Build system
- **C++17 compatible compiler**: GCC/Clang with C++17 support

### Installation Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install libsdbus-c++-dev libsdbus-c++-bin libboost-dev bluez cmake build-essential

# Fedora/CentOS
sudo dnf install sdbus-c++-devel boost-devel bluez cmake gcc-c++
```

### Bluetooth Adapter Configuration

Before running the application, ensure your Bluetooth adapter is properly configured:

#### **Hardware Adapter Setup**

- **USB Bluetooth Adapter**: Should be automatically detected when plugged in
- **Built-in Bluetooth**: Usually pre-configured on laptops and desktop systems
- **Serial/UART Adapters**: May require manual attachment using `hciattach`

#### **HCI Interface Configuration**

The adapter must be configured as an HCI (Host Controller Interface) device:

```bash
# Check available Bluetooth adapters
hciconfig

# If no adapters are shown, you may need to attach a serial adapter
# For UART/serial adapters (replace /dev/ttyUSB0 with your device):
sudo hciattach /dev/ttyUSB0 <protocol> <speed>
# Example: sudo hciattach /dev/ttyUSB0 bcsp 115200

# Bring up the HCI interface (replace hci0 with your adapter)
sudo hciconfig hci0 up

# Verify the adapter is active
hciconfig -a
```

#### **Common HCI Configuration Commands**

```bash
# Reset the adapter
sudo hciconfig hci0 reset

# Enable/disable the adapter
sudo hciconfig hci0 up
sudo hciconfig hci0 down

# Make adapter discoverable
sudo hciconfig hci0 piscan

# Check adapter status
sudo hciconfig hci0
```

### Service Dependencies

Ensure the following services are running:

```bash
# Check BlueZ service status
sudo systemctl status bluetooth

# Start BlueZ service if not running
sudo systemctl start bluetooth

# Enable BlueZ service to start at boot
sudo systemctl enable bluetooth

# Check D-Bus service
sudo systemctl status dbus
```

## Architecture

The project follows a modular, object-oriented design with clear separation of concerns:

### Core Components

#### **Application Layer** (`Src/Application.*`)

- **Purpose**: Main application orchestrator that coordinates all Bluetooth operations
- **Functionality**:
  - Initializes and manages all subsystems
  - Coordinates D-Bus event loop
  - Configures device class (SMARTPHONE or HELMET)
  - Manages application lifecycle

#### **Menu System** (`Src/Menu/`)

- **Purpose**: Interactive command-line interface for user operations
- **Features**:
  - Start/stop discovery and scanning
  - List and manage discovered devices
  - Connect/disconnect operations
  - SPP profile connections
  - Device pairing operations

### Bluetooth Stack Components

#### **Adapter Management** (`Src/Adapter/`)

- **Adapter** (`Adapter.cpp/h`): High-level adapter interface implementing IAdapter
- **AdapterProxy** (`AdapterProxy.cpp/h`): D-Bus proxy for org.bluez.Adapter1 interface
- **Functionality**:
  - Power management (on/off)
  - Discovery control
  - Discoverability and pairing settings
  - Device class configuration
  - Property change notifications

#### **Device Management** (`Src/Device/`, `Src/DeviceManager/`)

- **DeviceManager** (`DeviceManager.*`): Central device registry and lifecycle manager
- **Device** (`Device.*`): Individual Bluetooth device representation
- **DeviceProxy** (`DeviceProxy.*`): D-Bus proxy for org.bluez.Device1 interface
- **Features**:
  - Device discovery and enumeration
  - Connection state management
  - Pairing operations
  - Profile connections (SPP, etc.)
  - Property monitoring and updates

#### **Agent System** (`Src/Agent/`, `Src/AgentManager/`)

- **Agent** (`Agent.*`): Handles authentication and pairing requests
- **AgentManager** (`AgentManager.*`): Manages agent registration with BlueZ
- **AgentProxy** (`AgentProxy.*`): D-Bus adaptor for org.bluez.Agent1 interface
- **Capabilities**:
  - PIN code handling
  - Passkey authentication
  - Pairing confirmation
  - Authorization requests

#### **Profile Management** (`Src/Profile/`, `Src/ProfileManager/`)

- **ProfileManager** (`ProfileManager.*`): Registers and manages Bluetooth profiles
- **ProfileProxy** (`ProfileProxy.*`): D-Bus adaptor for org.bluez.Profile1 interface
- **SPP Support**: Serial Port Profile implementation for data communication

#### **Object Management** (`Src/ObjectManager/`)

- **ObjectManagerProxy**: Monitors D-Bus object additions/removals
- **Functionality**:
  - Real-time device discovery notifications
  - Device filtering by class
  - Event-driven device management

### Utility Components

#### **SPP Handler** (`Src/SPPHandler/`)

- **Purpose**: Manages Serial Port Profile connections and data transfer
- **Features**:
  - Unix socket file descriptor handling
  - Asynchronous read/write operations
  - Thread-safe data communication
  - Connection lifecycle management

#### **Logger** (`Src/Logger/`)

- **Purpose**: Centralized logging system with timestamps and tagging
- **Features**: Formatted logging with component identification

#### **Utilities** (`Src/Utilities/`)

- **Purpose**: Common utility functions and D-Bus variant helpers

## Project Structure

```text
├── CMakeLists.txt              # Build configuration with sdbus-c++ integration
├── main.cpp                    # Application entry point and signal handling
├── main.h                      # Main application headers
├── DeleteDevices.sh            # Utility script to clean paired devices
├── conf/
│   └── org.gokul.service       # D-Bus service configuration
├── Inc/                        # Public interface headers
│   ├── AdapterHelper.h         # Adapter property constants
│   └── DeviceHelper.h          # Device property constants
├── Int/                        # Interface definitions
│   ├── IAdapter.h              # Adapter interface
│   ├── IAgent.h                # Agent interface
│   ├── IDevice.h               # Device interface
│   └── IDeviceManager.h        # Device manager interface
├── Src/                        # Implementation source files
│   ├── Application.*           # Main application orchestrator
│   ├── Adapter/               # Bluetooth adapter management
│   ├── Agent/                 # Authentication and pairing agent
│   ├── AgentManager/          # Agent registration and management
│   ├── Device/                # Individual device handling
│   ├── DeviceManager/         # Device lifecycle management
│   ├── Logger/                # Logging subsystem
│   ├── Menu/                  # User interface
│   ├── ObjectManager/         # D-Bus object monitoring
│   ├── Profile/               # Bluetooth profile handling
│   ├── ProfileManager/        # Profile registration
│   ├── SPPHandler/            # Serial Port Profile implementation
│   └── Utilities/             # Common utility functions
└── xml/                       # D-Bus interface definitions
    ├── *.xml                  # BlueZ D-Bus interface specifications
    └── generate.sh            # Code generation script
```

## Dependencies

The build process automatically:

1. Generates D-Bus proxy/adaptor classes from XML specifications
2. Compiles all source components
3. Links against required libraries
4. Copies utility scripts to build directory

## Building



### Build Steps

```bash
mkdir build && cd build
cmake ..
make
```

The build process automatically:

1. Generates D-Bus proxy/adaptor classes from XML specifications
2. Compiles all source components
3. Links against required libraries
4. Copies utility scripts to build directory

## Usage

### Command Line Options

```bash
./BluezEg --hci <hci_device> --name <device_name> [--class <device_class>]
```

**Parameters:**

- `--hci`: Bluetooth adapter identifier (e.g., "hci0")
- `--name`: Device name for advertising
- `--class`: Device class - "SMARTPHONE" (0x3C0408) or "HELMET" (0x240408, default)

### Example Usage

```bash
# Run as smartphone device
./BluezEg --hci hci0 --name "MyPhone" --class SMARTPHONE

# Run as helmet device (default)
./BluezEg --hci hci0 --name "MyHelmet" --class HELMET
```

### Interactive Menu Operations

Once running, the application provides an interactive menu:

1. **Start Discovery** - Begin scanning for nearby devices
2. **Start Scan** - Start advertising this device
3. **List Devices** - Show discovered devices
4. **Get Device** - Select a specific device by MAC address
5. **Print Properties** - Display selected device properties
6. **Auto Connect SPP** - Automatically connect to SPP-capable devices
7. **Connect/Disconnect** - Manual connection control
8. **Profile Operations** - Connect/disconnect specific profiles
9. **Pairing** - Initiate or cancel device pairing

## Key Features

### Device Discovery and Management

- Real-time device discovery with filtering by device class
- Automatic device enumeration and property tracking
- Thread-safe device registry with MAC address indexing

### SPP (Serial Port Profile) Support

- Complete SPP implementation for data communication
- Asynchronous socket handling with epoll
- Bidirectional data transfer capabilities

### Authentication and Security

- Comprehensive pairing agent with multiple authentication methods
- PIN code, passkey, and confirmation support
- Automatic agent registration and management

### Event-Driven Architecture

- D-Bus signal handling for real-time updates
- Property change notifications
- Device addition/removal events

## Documentation Generation

### API Documentation with Doxygen

This project includes comprehensive Doxygen documentation for all classes and functions. To generate HTML documentation:

#### Doxygen Prerequisites

```bash
# Ubuntu/Debian
sudo apt-get install doxygen graphviz

# Fedora/CentOS
sudo dnf install doxygen graphviz
```

#### Generate Documentation

A pre-configured `Doxyfile` is included in the project root. To generate documentation:

```bash
# Generate HTML documentation (uses existing Doxyfile)
doxygen

# Or create a new configuration file and customize it
doxygen -g MyDoxyfile
doxygen MyDoxyfile
```

#### Doxygen Configuration

The included `Doxyfile` is optimized for this project with the following settings:

```bash
# Project information
PROJECT_NAME           = "BlueZ D-Bus C++ Sample"
PROJECT_BRIEF          = "Bluetooth communication using BlueZ D-Bus interface"
PROJECT_VERSION        = "1.0"

# Input settings
INPUT                  = . Src/ Inc/ Int/ README.md
RECURSIVE              = YES
FILE_PATTERNS          = *.h *.cpp *.md
USE_MDFILE_AS_MAINPAGE = README.md

# Output settings
GENERATE_HTML          = YES
HTML_OUTPUT            = docs/html
GENERATE_LATEX         = NO

# Documentation extraction
EXTRACT_ALL            = YES
EXTRACT_PRIVATE        = YES
EXTRACT_STATIC         = YES

# Graph generation (requires graphviz)
HAVE_DOT               = YES
CALL_GRAPH             = YES
CALLER_GRAPH           = YES
CLASS_DIAGRAMS         = YES
SOURCE_BROWSER         = YES
```

#### Access Documentation

After generation, open the documentation in your browser:

```bash
# Open main documentation page
firefox docs/html/index.html
# or
google-chrome docs/html/index.html
```

The documentation includes:

- **Class hierarchies** and inheritance diagrams
- **Function documentation** with parameters and return values
- **Call graphs** showing function relationships
- **File documentation** with dependencies
- **Code examples** and usage patterns

## Development Notes

### D-Bus Code Generation

The project uses `sdbus-c++-xml2cpp` to generate C++ bindings from BlueZ D-Bus XML specifications. The generated files are created in the build directory and provide type-safe interfaces to BlueZ services.

### Thread Safety

- All device operations are thread-safe using mutex protection
- Event loops run in dedicated threads
- Condition variables coordinate between discovery and processing

### Error Handling

- Comprehensive exception handling for D-Bus operations
- Graceful degradation on connection failures
- Detailed logging for debugging

## License

This project is distributed under the terms specified in the LICENSE file.

## Contributing

When contributing to this project:

1. Follow the existing code structure and naming conventions
2. Ensure thread safety for concurrent operations
3. Add appropriate logging for debugging
4. Update this README for any architectural changes

## Troubleshooting

### Common Issues

- **Permission denied**: Ensure user has Bluetooth permissions or run with appropriate privileges
- **D-Bus connection failed**: Verify BlueZ service is running (`systemctl status bluetooth`)
- **Device not found**: Check if Bluetooth adapter is enabled and visible
- **Pairing failures**: Verify agent is properly registered and device is in pairing mode

## References
* [SDBUS C++](https://github.com/Kistler-Group/sdbus-cpp/blob/master/docs/using-sdbus-c%2B%2B.md)
* [Bluez](https://github.com/bluez/bluez)