/**
 * @file SPPHandler.cpp
 * @brief Implementation of Serial Port Profile (SPP) connection handler
 * @author Gokul
 * @date 2025
 */

#include <chrono>
#include <limits>
#include <cstring>
#include <errno.h>
#include <sys/epoll.h>
#include <fcntl.h>

#include "SPPHandler.h"

#include "Logger.h"

#define TAG "SPPHandler::"                              ///< Tag for logging messages
#define BUFFER_SIZE 1024                                ///< Size of read/write buffers
#define SLEEP_DURATION std::chrono::seconds(1)          ///< Sleep duration for thread loops

const int ERROR = -1; ///< Error return value constant

/**
 * @brief Construct a new SPP Handler object
 * 
 * Initializes the SPP handler with a Unix file descriptor and creates
 * a control pipe for thread synchronization.
 * 
 * @param fd Unix file descriptor for the SPP connection
 */
SPPHandler::SPPHandler(sdbus::UnixFd fd) : m_fd(fd),
                                           m_readRunning(true),
                                           m_writeRunning(true)
{
  Log("%s%s", TAG, __func__);

  // Create a pipe
  if (pipe(m_pipeCtrl) < 0)
  {
    Log("%s%s Error: Creating pipe, Error - %s", TAG, __func__, strerror(errno));
  }
}

SPPHandler::~SPPHandler()
{
  Log("%s%s", TAG, __func__);

  try {
    {
      std::lock_guard<std::mutex> lock(m_sppMutex);
      m_readRunning = false;
      m_writeRunning = false;
    }

    // Write to the pipe to wake up epoll_wait
    if (write(m_pipeCtrl[1], "x", 1) == -1) {
      Log("%s%s Error: Writing to pipe, Error - %s", TAG, __func__, strerror(errno));
    }

    CloseThread(m_read_thread);
    CloseThread(m_write_thread);
    ClosePipe();
    CloseFD();
  } catch (std::system_error &e) {
    Log("%s%s Error: %s", TAG, __func__, e.what());
  }
}

void SPPHandler::StartOperations()
{
  m_read_thread = std::thread(&SPPHandler::ReadBuffer, this);
  m_write_thread = std::thread(&SPPHandler::WriteBuffer, this);
}

void SPPHandler::ReadBuffer()
{
  Log("%s%s", TAG, __func__);
  int fd = m_fd.get();
  MakeSocketNonBlocking(fd);

  int nfds = 0;
  int epoll_fd = -1;
  const uint32_t MAXEVENTS = 4;
  struct epoll_event events[MAXEVENTS] = {};
  struct epoll_event fileDesPollEvents = {};
  struct epoll_event pipePollEvents = {};

  pipePollEvents.events = EPOLLIN | EPOLLET;
  pipePollEvents.data.fd = m_pipeCtrl[0]; // monitoring the Pipe read end

  fileDesPollEvents.events = EPOLLIN;
  fileDesPollEvents.data.fd = fd;

  epoll_fd = epoll_create1(0);
  if (epoll_fd < 0)
  {
    Log("%s%s Error: Creating epoll FD, Error - %s", TAG, __func__, strerror(errno));
    return;
  }

  // Add the pipe read end to epoll
  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_pipeCtrl[0], &pipePollEvents) == -1)
  {
    Log("%s%s Error: Adding pipe to epoll, Error - %s", TAG, __func__, strerror(errno));
    close(epoll_fd);
    return;
  }

  if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, m_fd.get(), &fileDesPollEvents) == -1)
  {
    Log("%s%s Error: Adding FD to epoll, Error - %s", TAG, __func__, strerror(errno));
    close(epoll_fd);
    return;
  }

  while (m_readRunning)
  {
    nfds = epoll_wait(epoll_fd, events, 1, -1); // Wait indefinitely for data
    if (nfds < 0)
    {
      Log("%s%s Error: epoll_wait, Error - %s", TAG, __func__, strerror(errno));
      continue;
    }
    else if (nfds > 0)
    {
      for (auto n = 0; n < nfds; ++n)
      {
        if (events[n].data.fd == m_pipeCtrl[0] && (events[n].events & EPOLLIN))
        {
          Log("%s%s Pipe event", TAG, __func__);
          m_readRunning = false;
        }
        else if (events[n].data.fd == fd)
        {
          if (events[n].events & EPOLLIN)
          {
            char buffer[BUFFER_SIZE] = {0};
            ssize_t bytes_read = read(events[n].data.fd, buffer, sizeof(buffer));
            if (bytes_read < 0)
            {
              Log("%s%s Error: Reading from FD - %d, Error - %s", TAG, __func__, events[n].data.fd, strerror(errno));
              m_readRunning = false;
            }
            else if (bytes_read == 0)
            {
              Log("%s%s Error: No data read from FD - %d", TAG, __func__, events[n].data.fd);
              m_readRunning = false;
            }
            Log("%s%s Data - %s", TAG, __func__, buffer);
          }
        }
      }
    }
    if (!m_readRunning)
    {
      break;
    }
  }

  close(epoll_fd);
}

void SPPHandler::WriteBuffer()
{
  Log("%s%s", TAG, __func__);
  uint64_t count = 0;
  uint64_t max_value = std::numeric_limits<uint64_t>::max();
  while (m_writeRunning)
  {
    int fd = m_fd.get();
    if (fd < 0)
    {
      Log("%s%s Error: Invalid FD - %d", TAG, __func__, fd);
      return;
    }
    if (count == max_value)
    {
      count = 0;
    }
    std::string data = "Ping " + std::to_string(count++);
    ssize_t bytes_written = write(fd, data.c_str(), data.size());
    if (bytes_written < 0)
    {
      Log("%s%s Error: Writing to FD - %d, Error - %d", TAG, __func__, fd, errno);
      m_writeRunning = false;
    }
    else if (bytes_written == 0)
    {
      Log("%s%s Error: No data written to FD - %d", TAG, __func__, fd);
      m_writeRunning = false;
    }
    Log("%s%s Data - %s", TAG, __func__, data.c_str());
    if(!m_writeRunning) {
      break;
    }
    std::this_thread::sleep_for(SLEEP_DURATION);
  }
}

void SPPHandler::MakeSocketNonBlocking(int fd)
{
  // Set the file descriptor to non-blocking mode
  int flags = fcntl(fd, F_GETFL, 0);
  if (flags == -1)
  {
    Log("%s%s Error: Getting FD flags - %d, Error - %s", TAG, __func__, fd, strerror(errno));
    return;
  }
  flags |= O_NONBLOCK;
  if (fcntl(fd, F_SETFL, flags) == -1)
  {
    Log("%s%s Error: Setting FD to non-blocking - %d, Error - %s", TAG, __func__, fd, strerror(errno));
    return;
  }
}

void SPPHandler::ClosePipe()
{
  Log("%s%s", TAG, __func__);
  for (auto pipe : m_pipeCtrl) {
    if (ERROR != pipe) {
      close(pipe);
      pipe = ERROR;
    }
  }

}

void SPPHandler::CloseThread(std::thread &thread)
{
  Log("%s%s", TAG, __func__);
  if (thread.joinable())
  {
    thread.join();
  }
}

void SPPHandler::CloseFD()
{
  Log("%s%s", TAG, __func__);
  int fd = m_fd.get();
  if (fd >= 0)
  {
    close(fd);
    Log("%s%s Closed FD - %d", TAG, __func__, fd);
  }
}
