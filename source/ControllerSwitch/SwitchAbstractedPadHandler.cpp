#include "SwitchAbstractedPadHandler.h"
#include "SwitchLogger.h"
#include <cmath>
#include <array>

SwitchAbstractedPadHandler::SwitchAbstractedPadHandler(std::unique_ptr<IController> &&controller, int polling_frequency_ms)
    : SwitchVirtualGamepadHandler(std::move(controller), polling_frequency_ms)
{
}

SwitchAbstractedPadHandler::~SwitchAbstractedPadHandler()
{
    Exit();
}

ams::Result SwitchAbstractedPadHandler::Initialize()
{
    R_TRY(m_controller->Initialize());

    if (GetController()->Support(SUPPORTS_NOTHING))
        return 0;

    R_TRY(InitAbstractedPadState());

    if (GetController()->Support(SUPPORTS_PAIRING))
    {
        R_TRY(InitOutputThread());
    }

    R_TRY(InitInputThread());

    return 0;
}

void SwitchAbstractedPadHandler::Exit()
{
    if (GetController()->Support(SUPPORTS_NOTHING))
    {
        m_controller->Exit();
        return;
    }

    ExitInputThread();
    ExitOutputThread();
    m_controller->Exit();
    ExitAbstractedPadState();
}

// Used to give out unique ids to abstracted pads
static std::array<bool, 8> uniqueIDs{};

static s8 getUniqueId()
{
    for (s8 i = 0; i != 8; ++i)
    {
        if (uniqueIDs[i] == false)
        {
            uniqueIDs[i] = true;
            return i;
        }
    }
    return 0;
}

static void freeUniqueId(s8 id)
{
    uniqueIDs[id] = false;
}

Result SwitchAbstractedPadHandler::InitAbstractedPadState()
{
    Result rc;
    m_state = {0};
    m_abstractedPadID = getUniqueId();
    m_state.type = BIT(0);
    m_state.npadInterfaceType = HidNpadInterfaceType_USB;
    m_state.flags = 0xff;
    m_state.state.battery_level = 4;
    ControllerConfig *config = GetController()->GetConfig();
    m_state.singleColorBody = config->bodyColor.rgbaValue;
    m_state.singleColorButtons = config->buttonsColor.rgbaValue;

    rc = hiddbgSetAutoPilotVirtualPadState(m_abstractedPadID, &m_state);
    if (R_FAILED(rc))
        return rc;

    return rc;
}

Result SwitchAbstractedPadHandler::ExitAbstractedPadState()
{
    freeUniqueId(m_abstractedPadID);
    return hiddbgUnsetAutoPilotVirtualPadState(m_abstractedPadID);
}

Result SwitchAbstractedPadHandler::UpdateAbstractedState(const NormalizedButtonData &data, uint16_t input_idx)
{
    (void)input_idx;
    m_state.state.buttons = 0;
    if (data.buttons[0])
        m_state.state.buttons |= HidNpadButton_X;
    if (data.buttons[1])
        m_state.state.buttons |= HidNpadButton_A;
    if (data.buttons[2])
        m_state.state.buttons |= HidNpadButton_B;
    if (data.buttons[3])
        m_state.state.buttons |= HidNpadButton_Y;
    if (data.buttons[4])
        m_state.state.buttons |= HidNpadButton_StickL;
    if (data.buttons[5])
        m_state.state.buttons |= HidNpadButton_StickR;
    if (data.buttons[6])
        m_state.state.buttons |= HidNpadButton_L;
    if (data.buttons[7])
        m_state.state.buttons |= HidNpadButton_R;
    if (data.buttons[8])
        m_state.state.buttons |= HidNpadButton_ZL;
    if (data.buttons[9])
        m_state.state.buttons |= HidNpadButton_ZR;
    if (data.buttons[10])
        m_state.state.buttons |= HidNpadButton_Minus;
    if (data.buttons[11])
        m_state.state.buttons |= HidNpadButton_Plus;

    ControllerConfig *config = GetController()->GetConfig();

    if (config && config->swapDPADandLSTICK)
    {
        if (data.sticks[0].axis_y > 0.5f)
            m_state.state.buttons |= HidNpadButton_Up;
        if (data.sticks[0].axis_x > 0.5f)
            m_state.state.buttons |= HidNpadButton_Right;
        if (data.sticks[0].axis_y < -0.5f)
            m_state.state.buttons |= HidNpadButton_Down;
        if (data.sticks[0].axis_x < -0.5f)
            m_state.state.buttons |= HidNpadButton_Left;

        float daxis_x{}, daxis_y{};

        daxis_y += data.buttons[12] ? 1.0f : 0.0f;  // DUP
        daxis_x += data.buttons[13] ? 1.0f : 0.0f;  // DRIGHT
        daxis_y += data.buttons[14] ? -1.0f : 0.0f; // DDOWN
        daxis_x += data.buttons[15] ? -1.0f : 0.0f; // DLEFT

        ConvertAxisToSwitchAxis(daxis_x, daxis_y, 0, &m_state.state.analog_stick_l.x, &m_state.state.analog_stick_l.y);
    }
    else
    {
        if (data.buttons[12])
            m_state.state.buttons |= HidNpadButton_Up;
        if (data.buttons[13])
            m_state.state.buttons |= HidNpadButton_Right;
        if (data.buttons[14])
            m_state.state.buttons |= HidNpadButton_Down;
        if (data.buttons[15])
            m_state.state.buttons |= HidNpadButton_Left;

        ConvertAxisToSwitchAxis(data.sticks[0].axis_x, data.sticks[0].axis_y, 0, &m_state.state.analog_stick_l.x, &m_state.state.analog_stick_l.y);
    }

    ConvertAxisToSwitchAxis(data.sticks[1].axis_x, data.sticks[1].axis_y, 0, &m_state.state.analog_stick_r.x, &m_state.state.analog_stick_r.y);

    m_state.state.buttons |= (data.buttons[16] ? HiddbgNpadButton_Capture : 0);
    m_state.state.buttons |= (data.buttons[17] ? HiddbgNpadButton_Home : 0);

    return hiddbgSetAutoPilotVirtualPadState(m_abstractedPadID, &m_state);
}

void SwitchAbstractedPadHandler::UpdateInput()
{
    NormalizedButtonData data;
    uint16_t input_idx;

    // We process any input packets here. If it fails, return and try again
    Result rc = m_controller->ReadInput(&data, &input_idx);
    if (R_FAILED(rc))
        return;

    rc = UpdateAbstractedState(data, input_idx);
    if (R_FAILED(rc))
        return;
}

void SwitchAbstractedPadHandler::UpdateOutput()
{
    if (R_SUCCEEDED(m_controller->OutputBuffer()))
        return;

    if (GetController()->Support(SUPPORTS_RUMBLE))
    {
        Result rc;
        HidVibrationValue value;
        rc = hidGetActualVibrationValue(m_vibrationDeviceHandle, &value);
        if (R_SUCCEEDED(rc))
            GetController()->SetRumble(static_cast<uint8_t>(value.amp_high * 255.0f), static_cast<uint8_t>(value.amp_low * 255.0f));
    }
}