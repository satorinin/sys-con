#include "Controllers/SegaIO4.h"
#include "HIDReportDescriptor.h"
#include <string.h>

// https://www.usb.org/sites/default/files/documents/hid1_11.pdf  p55

SegaIO4::SegaIO4(std::unique_ptr<IUSBDevice> &&device, const ControllerConfig &config, std::unique_ptr<ILogger> &&logger)
    : BaseController(std::move(device), config, std::move(logger)),
      m_joystick_count(0)
{
    Log(LogLevelDebug, "SegaIO4[%04x-%04x] Created !", m_device->GetVendor(), m_device->GetProduct());
}

SegaIO4::~SegaIO4()
{
}

ControllerResult SegaIO4::Initialize()
{
    ControllerResult result = BaseController::Initialize();
    if (result != CONTROLLER_STATUS_SUCCESS)
        return result;

    uint8_t buffer[CONTROLLER_HID_REPORT_BUFFER_SIZE];
    uint16_t size = sizeof(buffer);
    // https://www.usb.org/sites/default/files/hid1_11.pdf

    /// SET_IDLE
    result = m_interfaces[0]->ControlTransferOutput((uint8_t)IUSBEndpoint::USB_ENDPOINT_OUT | 0x20 | (uint8_t)USB_RECIPIENT_INTERFACE, USB_REQUEST_SET_IDLE, 0, m_interfaces[0]->GetDescriptor()->bInterfaceNumber, nullptr, 0);
    if (result != CONTROLLER_STATUS_SUCCESS)
        Log(LogLevelError, "SegaIO4[%04x-%04x] SET_IDLE failed, continue anyway ...", m_device->GetVendor(), m_device->GetProduct());

    // Get HID report descriptor
    result = m_interfaces[0]->ControlTransferInput((uint8_t)IUSBEndpoint::USB_ENDPOINT_IN | (uint8_t)USB_RECIPIENT_INTERFACE, USB_REQUEST_GET_DESCRIPTOR, (USB_DT_REPORT << 8), m_interfaces[0]->GetDescriptor()->bInterfaceNumber, buffer, &size);
    if (result != CONTROLLER_STATUS_SUCCESS)
    {
        Log(LogLevelError, "SegaIO4[%04x-%04x] Failed to get HID report descriptor", m_device->GetVendor(), m_device->GetProduct());
        return result;
    }

    Log(LogLevelTrace, "SegaIO4[%04x-%04x] Got descriptor for interface %d", m_device->GetVendor(), m_device->GetProduct(), m_interfaces[0]->GetDescriptor()->bInterfaceNumber);
    LogBuffer(LogLevelTrace, buffer, size);

    Log(LogLevelDebug, "SegaIO4[%04x-%04x] Parsing descriptor ...", m_device->GetVendor(), m_device->GetProduct());
    std::shared_ptr<HIDReportDescriptor> descriptor = std::make_shared<HIDReportDescriptor>(buffer, size);

    Log(LogLevelDebug, "SegaIO4[%04x-%04x] Looking for joystick/gamepad profile ...", m_device->GetVendor(), m_device->GetProduct());

    Log(LogLevelInfo, "SegaIO4[%04x-%04x] USB joystick successfully opened (%d inputs detected) !", m_device->GetVendor(), m_device->GetProduct(), GetInputCount());

    return CONTROLLER_STATUS_SUCCESS;
}

uint16_t SegaIO4::GetInputCount()
{
    return std::min((int)m_joystick_count, CONTROLLER_MAX_INPUTS);
}

ControllerResult SegaIO4::ParseData(uint8_t *buffer, size_t size, RawInputData *rawData, uint16_t *input_idx)
{
    /*
         Special case for generic HID, input_idx might be bigger than 0 in case of multiple interfaces.
         If this is the case we expect to have 1 input per interface, thus we don't want to overwrite the input index.
    */
   
    return CONTROLLER_STATUS_SUCCESS;
}
