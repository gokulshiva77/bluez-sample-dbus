/**
 * @file main.cpp
 * @brief Main entry point for BlueZ D-Bus sample application
 * @author Gokul
 * @date 2025
 */

#include <iostream>
#include <thread>
#include <chrono>
#include <atomic>
#include <memory>
#include <csignal>
#include <exception>
#include <execinfo.h> // For backtrace
#include <functional>

#include <cstdlib>

#include "Menu.h"

#define BACKTRACE_SIZE 32  ///< Maximum number of stack frames to capture in backtrace

std::atomic<bool> keepRunning(true);      ///< Global flag to control application lifecycle
std::shared_ptr<Application> app = nullptr; ///< Global application instance
std::unique_ptr<Menu> menu = nullptr;     ///< Global menu interface instance

/**
 * @brief Gracefully stop the application and cleanup resources
 * 
 * This function safely shuts down the application by setting the running flag
 * to false and properly destroying the application and menu objects.
 */
void StopApp()
{
    if(keepRunning) {
        keepRunning = false;
    }
    if(app) {
        Log("Deleting Application");
        app.reset();
    }
    if(menu) {
        Log("Deleting Menu");
        menu.reset();
    }
}

/**
 * @brief Print stack backtrace for debugging crashes
 * 
 * Captures and prints the current call stack using execinfo functions.
 * This is useful for debugging segmentation faults and other crashes.
 * Automatically calls StopApp() after printing the backtrace.
 */
void printBacktrace()
{
    void *array[BACKTRACE_SIZE];
    size_t size = backtrace(array, BACKTRACE_SIZE);
    char **messages = backtrace_symbols(array, size);

    std::cerr << "Backtrace:\n";
    for (size_t i = 0; i < size; i++)
    {
        std::cerr << messages[i] << std::endl;
    }
    free(messages);
    StopApp();
}

/**
 * @brief Signal handler for graceful application shutdown
 * @param signum Signal number that was received (SIGINT, SIGSEGV, SIGABRT)
 * 
 * Handles various signals by printing a backtrace and gracefully shutting down
 * the application. This helps with debugging crashes and ensures clean shutdown.
 */
void signalHandler(int signum)
{
    std::cerr << "Interrupt signal (" << signum << ") received.\n";

    // Print backtrace
    printBacktrace();
    keepRunning=false;
    exit(signum);
}

/**
 * @brief Delete all previously paired Bluetooth devices
 * 
 * Executes the DeleteDevices.sh script to remove all paired devices from the system.
 * This is useful for starting with a clean slate for testing purposes.
 */
void DeleteDevices() {
    int ret = system("./DeleteDevices.sh");
    if (ret != 0) {
        std::cerr << "Failed to run deleteDevices.sh" << std::endl;
    }else {
        Log("Delete Deivce Success");
    }
}

/**
 * @brief Main entry point for the BlueZ D-Bus sample application
 * @param argc Number of command line arguments
 * @param argv Array of command line argument strings
 * @return Exit status code (0 for success, 1 for error)
 * 
 * Parses command line arguments, initializes the Bluetooth subsystem,
 * creates the application instance, and runs the interactive menu.
 * 
 * Required arguments:
 * - --hci: HCI device identifier (e.g., "hci0")
 * - --name: Device name for advertising
 * 
 * Optional arguments:
 * - --class: Device class ("SMARTPHONE" or "HELMET", defaults to "HELMET")
 */
int main(int argc, char **argv)
{
    // Register signal handler
    signal(SIGINT, signalHandler);
    signal(SIGSEGV, signalHandler);
    signal(SIGABRT, signalHandler);

    std::string hciDevice;
    std::string deviceName;
    std::string deviceClass = "HELMET";
    std::vector<std::string> args(argv, argv + argc);

    for(size_t i = 0; i < args.size(); i++) {
        if(args[i] == "--hci" && i + 1 < args.size()) {
            hciDevice = args[++i];
        } else if(args[i] == "--name" && i + 1 < args.size()) {
            deviceName = args[++i];
        } else if(args[i] == "--class" && i + 1 < args.size()) {
            deviceClass = args[++i];
            std::transform(deviceClass.begin(), deviceClass.end(), deviceClass.begin(), ::toupper);
        }
    }

    if (hciDevice.empty() || deviceName.empty()) {
        std::cerr << "Usage: " << args[0] << " --hci <hci_device> --name <device_name> --class <SMARTPHONE/HELMET>" << std::endl;
        return 1;
    }

    Log("HCI Device: %s", hciDevice.c_str());
    Log("Device Name: %s", deviceName.c_str());
    try
    {
        Log("%s Starting Application", __func__);
        DeleteDevices();
        // Create system bus connection
        auto connection = sdbus::createSystemBusConnection();
        if(!connection) {
            Log("Failed to create system bus connection");
            return 1;
        }
        // Create and start the application
        app = std::make_shared<Application>(*connection, hciDevice, deviceName, deviceClass);
        if(app) {
            app->StartApplication();
        }
        // std::this_thread::sleep_for(std::chrono::seconds(1));
        menu = std::make_unique<Menu>(app);
        if(!menu) {
            StopApp();
        }
        std::string option;
        while(keepRunning)
        {
            menu->PrintMenu();
            std::cin >> option;
            menu->ProcessMenu(option);
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;

        // Print backtrace
        printBacktrace();
    }
    return 0;
}