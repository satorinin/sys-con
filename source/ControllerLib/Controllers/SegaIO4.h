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
    bool binpad0 : 1;
    bool START : 1; // start

    // byte30
    bool binpad1 : 1;
    bool binpad2 : 1;
    bool button8 : 1;
    bool button7 : 1; 

    bool button6 : 1;
    bool button5 : 1;
    bool button4 : 1;
    bool button3 : 1;

    // byte31
    uint8_t pad2 : 1;

    // byte32
    bool dummy0 : 1;
    bool dummy1: 1;
    bool dummy2: 1;
    bool dummy3: 1; 

    bool HOME : 1;
    bool dummy4: 1;
    bool dummy5: 1;
    bool dummy6: 1;



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

