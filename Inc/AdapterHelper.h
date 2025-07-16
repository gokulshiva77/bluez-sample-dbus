/**
 * @file AdapterHelper.h
 * @brief Constants and definitions for BlueZ Adapter1 interface properties
 * @author Gokul
 * @date 2025
 */

#pragma once

// BlueZ Adapter1 interface property names
#define ADAPTER_PROPERTY_Address "Address"                       ///< MAC address of the adapter
#define ADAPTER_PROPERTY_AddressType "AddressType"               ///< Address type (public/random)
#define ADAPTER_PROPERTY_Name "Name"                             ///< System name of the adapter
#define ADAPTER_PROPERTY_Alias "Alias"                           ///< User-friendly alias for the adapter
#define ADAPTER_PROPERTY_Class "Class"                           ///< Device class of the adapter
#define ADAPTER_PROPERTY_Powered "Powered"                       ///< Power state of the adapter
#define ADAPTER_PROPERTY_Discoverable "Discoverable"             ///< Discoverability state
#define ADAPTER_PROPERTY_DiscoverableTimeout "DiscoverableTimeout" ///< Discoverable timeout in seconds
#define ADAPTER_PROPERTY_Pairable "Pairable"                     ///< Pairable state
#define ADAPTER_PROPERTY_PairableTimeout "PairableTimeout"       ///< Pairable timeout in seconds
#define ADAPTER_PROPERTY_Discovering "Discovering"               ///< Discovery state
#define ADAPTER_PROPERTY_UUIDs "UUIDs"                           ///< List of supported service UUIDs
