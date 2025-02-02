#include "Controllers/SegaIO4.h"
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
    (void)input_idx;
    SegaIO4ButtonData *buttonData = reinterpret_cast<SegaIO4ButtonData *>(buffer);

    if (size < sizeof(SegaIO4ButtonData))
        return CONTROLLER_STATUS_UNEXPECTED_DATA;

    rawData->buttons[1] = buttonData->button1;
    rawData->buttons[2] = buttonData->button2;
    rawData->buttons[3] = buttonData->button3;
    rawData->buttons[4] = buttonData->button4;
    rawData->buttons[5] = buttonData->button5;
    rawData->buttons[6] = buttonData->button6;
    rawData->buttons[7] = buttonData->button7;
    rawData->buttons[8] = buttonData->button8;

    rawData->buttons[9] = buttonData->START;
    rawData->buttons[10] = buttonData->HOME;

    /*
    rawData->buttons[9] = buttonData->button9;
    rawData->buttons[10] = buttonData->button10;
    rawData->buttons[11] = buttonData->button11;
    rawData->buttons[13] = buttonData->button13;

    rawData->analog[ControllerAnalogType_Rx] = BaseController::Normalize(buttonData->Rx, 0, 255);
    rawData->analog[ControllerAnalogType_Ry] = BaseController::Normalize(buttonData->Ry, 0, 255);
    rawData->analog[ControllerAnalogType_X] = BaseController::Normalize(buttonData->X, 0, 255);
    rawData->analog[ControllerAnalogType_Y] = BaseController::Normalize(buttonData->Y, 0, 255);
    rawData->analog[ControllerAnalogType_Z] = BaseController::Normalize(buttonData->Z, 0, 255);
    rawData->analog[ControllerAnalogType_Rz] = BaseController::Normalize(buttonData->Rz, 0, 255);
    */

    rawData->buttons[11] = 0;
    rawData->buttons[13] = 0;

    rawData->analog[ControllerAnalogType_Rx] = 0;
    rawData->analog[ControllerAnalogType_Ry] = 0;
    rawData->analog[ControllerAnalogType_X] = 0;
    rawData->analog[ControllerAnalogType_Y] = 0;
    rawData->analog[ControllerAnalogType_Z] = 0;
    rawData->analog[ControllerAnalogType_Rz] = 0;

    rawData->buttons[DPAD_UP_BUTTON_ID] = buttonData->dpad_up;
    rawData->buttons[DPAD_RIGHT_BUTTON_ID] = buttonData->dpad_right;
    rawData->buttons[DPAD_DOWN_BUTTON_ID] = buttonData->dpad_down;
    rawData->buttons[DPAD_LEFT_BUTTON_ID] = buttonData->dpad_left;

    return CONTROLLER_STATUS_SUCCESS;
}
