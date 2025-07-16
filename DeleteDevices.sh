#!/bin/bash

# List all paired devices and remove them
echo "Listing all paired devices..."
bluetoothctl power on
# Get the list of paired devices and extract their MAC addresses
device_list=$(bluetoothctl devices | awk '{print $2}')

# Check if there are any paired devices
if [ -z "$device_list" ]; then
  echo "No paired Bluetooth devices found."
else
  # Iterate over each device MAC address and remove it
  for device in $device_list; do
    echo "Removing device with MAC address: $device"
    bluetoothctl remove $device
  done
fi

# Optionally, you can also power off the Bluetooth adapter after removal
bluetoothctl power off

bluetoothctl devices

echo "All paired devices have been removed."
