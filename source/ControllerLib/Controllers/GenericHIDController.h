#pragma once

#include "IController.h"

class HIDJoystick;

class GenericHIDController : public IController
{
private:
    IUSBEndpoint *m_inPipe = nullptr;
    IUSBEndpoint *m_outPipe = nullptr;
    bool m_features[SUPPORTS_COUNT];
    std::shared_ptr<HIDJoystick> m_joystick;

public:
    GenericHIDController(std::unique_ptr<IUSBDevice> &&device, const ControllerConfig &config, std::unique_ptr<ILogger> &&logger);
    virtual ~GenericHIDController() override;

    virtual ams::Result Initialize() override;
    virtual void Exit() override;

    ams::Result OpenInterfaces();
    void CloseInterfaces();

    virtual uint16_t GetInputCount() override;

    virtual ams::Result ReadInput(NormalizedButtonData *normalData, uint16_t *input_idx) override;

    virtual bool Support(ControllerFeature feature) override;

    float NormalizeTrigger(uint8_t deadzonePercent, uint8_t value);
    void NormalizeAxis(uint8_t x, uint8_t y, uint8_t deadzonePercent, float *x_out, float *y_out);

    ams::Result SendInitBytes();
    ams::Result SetRumble(uint8_t strong_magnitude, uint8_t weak_magnitude);
};