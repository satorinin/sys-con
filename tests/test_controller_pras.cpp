#include <gtest/gtest.h>
#include "Controllers/SegaIO4.h"
#include "mocks/Device.h"
#include "mocks/Logger.h"

TEST(Controller, test_pras_dpad_right)
{
    ControllerConfig config;
    RawInputData rawData;
    uint16_t input_idx = 0;
    SegaIO4 controller(std::make_unique<MockDevice>(), config, std::make_unique<MockLogger>());

    {
    uint8_t buffer[64] = {0x01, 0x20, 0x80, 0x38, 0x8A, 0x34, 0x83, 0x10, 0x88, 0x88, 0x83, 0x40, 0x89, 0x10, 0x8A, 0x10,
                          0x7B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x04, 0x00, 0x00,
                          0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    EXPECT_EQ(controller.ParseData(buffer, sizeof(buffer), &rawData, &input_idx), CONTROLLER_STATUS_SUCCESS);

    EXPECT_TRUE(rawData.buttons[DPAD_RIGHT_BUTTON_ID]);
    }

    {
    uint8_t buffer[64] = {0x01, 0x20, 0x80, 0x38, 0x8A, 0x34, 0x83, 0x10, 0x88, 0x88, 0x83, 0x40, 0x89, 0x10, 0x8A, 0x10,
                          0x7B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x20, 0x00, 0x00,
                          0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    EXPECT_EQ(controller.ParseData(buffer, sizeof(buffer), &rawData, &input_idx), CONTROLLER_STATUS_SUCCESS);

    EXPECT_TRUE(rawData.buttons[DPAD_UP_BUTTON_ID]);
    }

    {
    uint8_t buffer[64] = {0x01, 0x20, 0x80, 0x38, 0x8A, 0x34, 0x83, 0x10, 0x88, 0x88, 0x83, 0x40, 0x89, 0x10, 0x8A, 0x10,
                          0x7B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0x02, 0x00, 0x00, 0x80, 0x00,
                          0x00, 0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
                          0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};

    EXPECT_EQ(controller.ParseData(buffer, sizeof(buffer), &rawData, &input_idx), CONTROLLER_STATUS_SUCCESS);

    EXPECT_TRUE(rawData.buttons[3]);
    }
}
