#pragma once

#include "BaseController.h"
#include <memory>

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
