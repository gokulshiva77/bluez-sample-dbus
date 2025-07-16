/**
 * @file SPPHandler.h
 * @brief Serial Port Profile (SPP) connection handler for Bluetooth
 * @author Gokul
 * @date 2025
 */

#pragma once

#include <iostream>
#include <thread>
#include <unistd.h>
#include <sys/types.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <map>
#include <string>
#include <stdexcept>
#include <mutex>

#include <sdbus-c++/sdbus-c++.h>

/**
 * @class SPPHandler
 * @brief Handles Serial Port Profile (SPP) connections over Bluetooth
 * 
 * This class manages bidirectional communication over an SPP connection.
 * It handles reading from and writing to the SPP socket using separate
 * threads for concurrent operation. The class provides thread-safe
 * operations and proper resource cleanup.
 */
class SPPHandler
{
public:
  /**
   * @brief Construct a new SPP Handler object
   * @param fd Unix file descriptor for the SPP connection
   */
  SPPHandler(sdbus::UnixFd fd);
  
  /**
   * @brief Destroy the SPP Handler object and cleanup resources
   * 
   * Stops all running threads, closes file descriptors, and cleans up
   * any allocated resources.
   */
  ~SPPHandler();

  /**
   * @brief Start SPP read/write operations
   * 
   * Launches separate threads for reading from and writing to the SPP
   * connection. This enables bidirectional communication with the
   * connected Bluetooth device.
   */
  void StartOperations();
  
private:
  /**
   * @brief Read data from SPP connection in a loop
   * 
   * Continuously reads data from the SPP socket and processes it.
   * Runs in a separate thread until the connection is closed.
   */
  void ReadBuffer();
  
  /**
   * @brief Write data to SPP connection in a loop
   * 
   * Continuously writes data to the SPP socket from a queue or buffer.
   * Runs in a separate thread until the connection is closed.
   */
  void WriteBuffer();

  /**
   * @brief Make a socket non-blocking
   * @param fd File descriptor to make non-blocking
   */
  void MakeSocketNonBlocking(int fd);
  
  /**
   * @brief Close the control pipe
   * 
   * Closes the pipe used for thread synchronization and cleanup.
   */
  void ClosePipe();
  
  /**
   * @brief Safely close a thread
   * @param thread Reference to the thread to close
   */
  void CloseThread(std::thread &thread);
  
  /**
   * @brief Close the SPP file descriptor
   */
  void CloseFD();
  
private:
  sdbus::UnixFd m_fd;              ///< SPP connection file descriptor
  int m_pipeCtrl[2] = {-1,-1};     ///< Control pipe for thread synchronization
  std::thread m_read_thread;       ///< Thread for reading SPP data
  std::atomic<bool> m_readRunning; ///< Flag to control read thread execution
  std::thread m_write_thread;      ///< Thread for writing SPP data
  std::atomic<bool> m_writeRunning;///< Flag to control write thread execution
  std::mutex m_sppMutex;           ///< Mutex for thread-safe operations
};
