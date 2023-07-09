#include "AM.hpp"
#include "FreeRTOS_mock.hpp"

#include "gmock/gmock.h"
#include <gtest/gtest_prod.h>

extern freertos::FreeRTOSMock FreeRTOSMockObj;

class AttitudeManagerTest : public ::testing::Test {
    public:
        void SetUp() override {
            AM::AttitudeManager::setControlInputs(new_control_inputs);
            this->stored_control_inputs = AM::AttitudeManager::getControlInputs();
        };
    
        struct AM::AttitudeManagerInput new_control_inputs = { 
            .roll = 10.0f,
            .pitch = 20.0f,
            .yaw = 30.0f,
            .throttle = 40.0f
        };

        struct AM::AttitudeManagerInput stored_control_inputs;
};

TEST_F(AttitudeManagerTest, SetGetControlInputs) {
    EXPECT_CALL(FreeRTOSMockObj, xQueueSemaphoreTake(testing::_, testing::_))
        .Times(2);
    EXPECT_CALL(FreeRTOSMockObj, xQueueGenericSend(testing::_, testing::_, testing::_, testing::_))
        .Times(2);
    EXPECT_EQ(new_control_inputs.roll, stored_control_inputs.roll);
    EXPECT_EQ(new_control_inputs.pitch, stored_control_inputs.pitch);
    EXPECT_EQ(new_control_inputs.yaw, stored_control_inputs.yaw);
    EXPECT_EQ(new_control_inputs.throttle, stored_control_inputs.throttle);
}
