#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "PID.hpp" // Add the missing include directive for "PID.hpp"
#include <iostream>


class PIDControllerTest: public ::testing::Test {
    protected:
        PIDController pidController;

        void SetUp() override {
            // Code here will be called immediately after the constructor (right before each test).
            
            PIDController:: PIDValues pidValues = {0.5, 0.5, 0.5, 25, 0, 100, 37, 15, 2};
            pidController = PIDController(pidValues);

        }
}

//Test Cases
TEST(PIDControllerTest, ProportionalCalculationTest) {

    ControlData _controlData = {10.0, 5.0, 2.0};

    float tolerance = 0.0001;
    float result = pidController.execute_p(_controlData);

    expected_res = (_controlData.desired - _controlData.actual) * pidController.kp;

    ASSERT_NEAR(result, expected_res, tolerance);
}

TEST(PIDControllerTest, IntegralCalculationTest) {
    
    ControlData _controlData = {10.0, 5.0, 2.0};

    float result = pidController.execute_i(_controlData);
    ControlData _controlData2 = {15.0, 7.0, 3.0};

    //_error1 becomes prev_error
    float _error1 = _controlData.desired - _controlData.actual; // 5.0
    float _error2 = _controlData2.desired - _controlData2.actual; //8.0
    //Integral and prev_error is initialized at zero so after first calculation integral is 5
    float _integral = _error1;
    float expectedValue = _integral + (_error2 - _error1);
    float tolerance = 0.0001
    ASSERT_NEAR(result, expectedValue, tolerance);
}

// TEST(PIDControllerTest, DerivativeCalculationTest2) {
//     PIDController pidController = PIDController(0.1, 0.1, 0.1, 0, 1, 100, 100.0, 5.0, 2.0);
//     desired = 10.0;
//     actual = 5.0;
//     actualRate = 0;

//     pidController.setDesired(desired);
//     pidController.setActual(actual);
//     pidController.setActualRate(actualRate);

//     float tolerance = 0.0001;

//     float result = pidController.execute_d();

//     pid.Controller.setActual(2.0);
//     result = pidController.execute_d();

//     pidController.setActual(1.0);
//     result = pidController.execute_d();

//     float expected_result = ((3*1) - (4*2) + (5)) * pidController.kd;

//     ASSERT_NEAR(result, expected_result, tolerance);

// }

// TEST(PIDControllerTest, OutputCalculationTest) {
//     PIDController pidController = PIDController(0.1, 0.1, 0.1, 0, 1, 100, 100.0, 5.0, 2.0);
//     desired = 10.0;
//     actual = 5.0;
//     actualRate = 0;

//     pidController.setDesired(desired);
//     pidController.setActual(actual);
//     pidController.setActualRate(actualRate);

//     float result = pidController.execute();

//     float expected_res = constrain<float>((pid.kp * (desired - actual)) + (pid.ki * integral) - (pid.kd * ((3*5) - (4*5) + (5))), pid.max_output, pid.min_output);

//     float tolerance = 0.0001;


//     ASSERT_NEAR(result, expected_res, tolerance);
// }

TEST(PIDControllerTest, SetpointChangesTest) {

    ControlData values = {10.0, 5.0, 2.0};

    pidController.setDesired(values.desired);
    pidController.setActual(values.actual);
    pidController.setActualRate(values.actualRate);

    ASSERT_TRUE(pidController.desired == values.desired);
    ASSERT_TRUE(pidController.actual == values.actual);
    ASSERT_TRUE(pidController.actualRate == values.actualRate);
}

TEST(PIDControllerTest, TuningTest) {

    //Change the PID gains
    PIDGains newGains = {0.34, 0.76, 0.89};
    pidController.updatePIDGains(newGains);

    ASSERT_TRUE(pidController.kp == newGains.kp);
    ASSERT_TRUE(pidController.ki == newGains.ki);
    ASSERT_TRUE(pidController.kd == newGains.kd);
}


int main(int argc, char** argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
