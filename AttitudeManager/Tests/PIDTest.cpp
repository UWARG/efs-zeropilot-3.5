// #include "FreeRTOS.h"
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "PID.hpp" // Add the missing include directive for "PID.hpp"
#include <iostream>

//Test Cases

TEST(PIDControllerTest, ProportionalCalculationTest) {
    PIDController pidController = PIDController(0.1, 0.1, 0.1, 0, 1, 100, 100.0, 5.0, 2.0);
    desired = 10.0;
    actual = 5.0;
    actualRate = 2.0;    

    pidController.setDesired(desired);
    pidController.setActual(actual);
    pidController.setActualRate(actualRate);

    float tolerance = 0.0001;

    float result = pidController.execute_p();

    expected_res = (desired - actual) * pidController.kp;

    ASSERT_NEAR(result, expected_res, tolerance);
}

TEST(PIDControllerTest, IntegralCalculationTest) {
    PIDController pidController = PIDController(0.1, 0.1, 0.1, 0, 1, 100, 100.0, 5.0, 2.0);
    desired = 10.0;
    actual = 5.0;
    actualRate = 2.0;

    pidController.setDesired(desired);
    pidController.setActual(actual);
    pidController.setActualRate(actualRate);

    float tolerance = 0.0001;
    float result = pidController.execute_i();
    float error = pid.desired - pid.actual;
    float expectedValue = constrain<float>(integral + error, pid.i_max, -pid.i_max);

    ASSERT_NEAR(result, expectedValue, tolerance);
}

TEST(PIDControllerTest, DerivativeCalculationTest2) {
    PIDController pidController = PIDController(0.1, 0.1, 0.1, 0, 1, 100, 100.0, 5.0, 2.0);
    desired = 10.0;
    actual = 5.0;
    actualRate = 0;

    pidController.setDesired(desired);
    pidController.setActual(actual);
    pidController.setActualRate(actualRate);

    float tolerance = 0.0001;

    float result = pidController.execute_d();

    pid.Controller.setActual(2.0);
    result = pidController.execute_d();

    pidController.setActual(1.0);
    result = pidController.execute_d();

    float expected_result = ((3*1) - (4*2) + (5)) * pidController.kd;

    ASSERT_NEAR(result, expected_result, tolerance);

}

TEST(PIDControllerTest, OutputCalculationTest) {
    PIDController pidController = PIDController(0.1, 0.1, 0.1, 0, 1, 100, 100.0, 5.0, 2.0);
    desired = 10.0;
    actual = 5.0;
    actualRate = 0;

    pidController.setDesired(desired);
    pidController.setActual(actual);
    pidController.setActualRate(actualRate);

    float result = pidController.execute();

    float expected_res = constrain<float>((pid.kp * (desired - actual)) + (pid.ki * integral) - (pid.kd * ((3*5) - (4*5) + (5))), pid.max_output, pid.min_output);

    float tolerance = 0.0001;


    ASSERT_NEAR(result, expected_res, tolerance);


}

TEST(PIDControllerTest, FrequencyUpdatingTest) {
    PIDController pidController;
    PIDValues values;
    values.desired = 10.0;
    values.actual = 5.0;
    values.actualRate = 2.0;

    // Add your test logic here to verify the frequency updating
    // For example:
    // pidController.updatePid(values, updateFreq, stopFlag);
    // ASSERT_TRUE(...);
}

TEST(PIDControllerTest, SetpointChangesTest) {
    PIDController pidController = PIDController(0.1, 0.1, 0.1, 0, 1, 100, 100.0, 5.0, 2.0);
    PIDValues values;
    values.desired = 10.0;
    values.actual = 5.0;
    values.actualRate = 2.0;

    pidController.setDesired(values.desired);
    pidController.setActual(values.actual);
    pidController.setActualRate(values.actualRate);

    ASSERT_TRUE(pidController.desired == values.desired);
    ASSERT_TRUE(pidController.actual == values.actual);
    ASSERT_TRUE(pidController.actualRate == values.actualRate);
}

TEST(PIDControllerTest, TuningTest) {
    PIDController pidController = PIDController(0.1, 0.1, 0.1, 0, 1, 100, 100.0, 5.0, 2.0);

    //Change the PID gains
    float newKp = 0.0;
    float newKi = 0.0;
    float newKd = 0.0;

    pidController.setKp(newKp);
    pidController.setKi(newKi);
    pidController.setKd(newKd);

    ASSERT_TRUE(pidController.pid.kp == newKp);
    ASSERT_TRUE(pidController.pid.ki == newKi);
    ASSERT_TRUE(pidController.pid.kd == newKd);
}

TEST(PIDControllerTest, EdgeCasesTest) {
    // Check Integral Windup handling
    PIDController pidController = PIDController(0.1, 0.1, 0.1, 0, 1, 100, 100.0, 5.0, 2.0);

    pidController.i_max = 25.0;
    pidController.desired = 100.0;
    pidController.actual = 5.0;

    pidController.execute_i();

    ASSERT_TRUE(pidController.integral == pidController.i_max);

}


int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
