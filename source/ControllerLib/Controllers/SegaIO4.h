#pragma once

#include "BaseController.h"
#include <memory>

_PACKED(struct SegaIO4ButtonData {
    // byte0-28
    uint8_t pad[29];

    // byte29
    bool button2 : 1;  // button 2
    bool button1 : 1; // button 1
    bool dpad_right : 1;
    bool dpad_left : 1;

    bool dpad_down : 1;
    bool dpad_up : 1;
    bool binpad0;
    bool START : 1; // start

    // byte30
    bool binpad1;
    bool binpad2;
    bool button8 : 1;
    bool button7 : 1; 

    bool button6 : 1; // triangle
    bool button5 : 1; // circle
    bool button4 : 1; // cross
    bool button3 : 1; // square

    // byte31
    uint8_t pad2;

    // byte32
    uint8_t HOME : 1;

    // byte33-63
    uint8_t pad3[27];
});


class SegaIO4 : public BaseController
{
private:
    uint8_t m_joystick_count = 0;

public:
    SegaIO4(std::unique_ptr<IUSBDevice> &&device, const ControllerConfig &config, std::unique_ptr<ILogger> &&logger);
    virtual ~SegaIO4() override;

    virtual ControllerResult Initialize() override;

    virtual uint16_t GetInputCount() override;

    virtual ControllerResult ParseData(uint8_t *buffer, size_t size, RawInputData *rawData, uint16_t *input_idx) override;
};

