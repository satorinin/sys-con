#pragma once

#include "BaseController.h"

// References used:
// https://github.com/quantus/xbox-one-controller-protocol
// https://cs.chromium.org/chromium/src/device/gamepad/xbox_controller_mac.mm

struct XboxOneButtonData
{
    uint8_t type;
    uint8_t const_0;
    uint16_t id;

    bool sync : 1;
    bool dummy1 : 1; // Always 0.
    bool start : 1;
    bool back : 1;

    bool a : 1;
    bool b : 1;
    bool x : 1;
    bool y : 1;

    bool dpad_up : 1;
    bool dpad_down : 1;
    bool dpad_left : 1;
    bool dpad_right : 1;

    bool bumper_left : 1;
    bool bumper_right : 1;
    bool stick_left_click : 1;
    bool stick_right_click : 1;

    uint16_t trigger_left;
    uint16_t trigger_right;

    int16_t stick_left_x;
    int16_t stick_left_y;
    int16_t stick_right_x;
    int16_t stick_right_y;
};

enum XboxOneInputPacketType : uint8_t
{
    XBONEINPUT_BUTTON = 0x20,
    XBONEINPUT_HEARTBEAT = 0x03,
    XBONEINPUT_GUIDEBUTTON = 0x07,
    XBONEINPUT_WAITCONNECT = 0x02,
};

class XboxOneController : public BaseController
{
private:
    bool m_GuidePressed{false};
    ams::Result SendInitBytes(uint16_t input_idx);
    ams::Result WriteAckGuideReport(uint16_t input_idx, uint8_t sequence);

public:
    XboxOneController(std::unique_ptr<IUSBDevice> &&device, const ControllerConfig &config, std::unique_ptr<ILogger> &&logger);
    virtual ~XboxOneController() override;

    virtual ams::Result Initialize() override;

    virtual ams::Result ReadInput(NormalizedButtonData *normalData, uint16_t *input_idx) override;

    bool Support(ControllerFeature feature) override;

    ams::Result SetRumble(uint16_t input_idx, float amp_high, float amp_low) override;
};