#pragma once

#include "SwitchVirtualGamepadHandler.h"
#include <stratosphere.hpp>

namespace syscon::controllers
{
    bool IsAtControllerLimit();

    ams::Result Insert(std::unique_ptr<IController> &&controllerPtr);
    std::vector<std::unique_ptr<SwitchVirtualGamepadHandler>> &Get();
    ams::os::Mutex &GetScopedLock();

    // void Remove(void Remove(bool (*func)(std::unique_ptr<SwitchVirtualGamepadHandler> a)));;
    void SetPollingFrequency(int polling_frequency_ms);

    void Initialize();
    void Reset();
    void Exit();
} // namespace syscon::controllers