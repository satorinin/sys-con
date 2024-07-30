#pragma once

#include <cstdint>

enum ControllerResult : uint8_t
{
    CONTROLLER_STATUS_SUCCESS = 0,
    CONTROLLER_STATUS_INVALID_ENDPOINT = 100,
    CONTROLLER_STATUS_BUFFER_EMPTY = 101,
    CONTROLLER_STATUS_NOTHING_TODO = 102,
    CONTROLLER_STATUS_NOT_IMPLEMENTED = 103,
    CONTROLLER_STATUS_UNEXPECTED_DATA = 104,
    CONTROLLER_STATUS_INVALID_ARGUMENT = 105,
    CONTROLLER_STATUS_INVALID_REPORT_DESCRIPTOR = 106,
    CONTROLLER_STATUS_HID_IS_NOT_JOYSTICK = 107,
    CONTROLLER_STATUS_NO_INTERFACES = 108,
    CONTROLLER_STATUS_NO_DATA_AVAILABLE = 109,
    CONTROLLER_STATUS_OUT_OF_MEMORY = 110,
    CONTROLLER_STATUS_USB_INTERFACE_ACQUIRE = 111,
    CONTROLLER_STATUS_OPEN_FAILED = 112,
    CONTROLLER_STATUS_WRITE_FAILED = 113,
    CONTROLLER_STATUS_READ_FAILED = 114,
    CONTROLLER_STATUS_TIMEOUT = 115,
    CONTROLLER_STATUS_USB_ENDPOINT_OPEN = 116,
    CONTROLLER_STATUS_INVALID_INDEX = 117,
    CONTROLLER_STATUS_UNKNOWN_ERROR = 255,
};
