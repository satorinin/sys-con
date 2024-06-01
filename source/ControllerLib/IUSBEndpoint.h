#pragma once
#include <stratosphere.hpp>
#include "ControllerErrors.h"
#include <cstddef>

class IUSBEndpoint
{
public:
    enum Direction : uint8_t
    {
        USB_ENDPOINT_IN = 0x80,
        USB_ENDPOINT_OUT = 0x00,
    };

    enum Mode : uint8_t
    {
        USB_MODE_NON_BLOCKING = 0x01,
        USB_MODE_BLOCKING = 0x02,
    };

    struct EndpointDescriptor
    {
        uint8_t bLength;
        uint8_t bDescriptorType;
        uint8_t bEndpointAddress;
        uint8_t bmAttributes;
        uint16_t wMaxPacketSize;
        uint8_t bInterval;
    };

    virtual ~IUSBEndpoint() = default;

    // Open and close the endpoint. if maxPacketSize is not set, it uses wMaxPacketSize from the descriptor.
    virtual ams::Result Open(int maxPacketSize = 0) = 0;
    virtual void Close() = 0;

    // This will read from the inBuffer pointer for the specified size and write it to the endpoint.
    virtual ams::Result Write(const void *inBuffer, size_t bufferSize) = 0;

    // This will read from the endpoint and put the data in the outBuffer pointer for the specified size.
    virtual ams::Result Read(void *outBuffer, size_t *bufferSizeInOut, Mode mode) = 0;

    // Get endpoint's direction. (IN or OUT)
    virtual IUSBEndpoint::Direction GetDirection() = 0;
    // Get the endpoint descriptor
    virtual EndpointDescriptor *GetDescriptor() = 0;
};