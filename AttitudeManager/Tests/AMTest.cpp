#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "AM.hpp"
#include "config.hpp"

namespace AM {

TEST(AttitudeManager, MotorInitializationAndOutput) {

    // No motors
    {
        config::NUM_MOTORS = 0;
        config::motors = new config::Motor_t[config::NUM_MOTORS];

        AM::Flightmode *control_algorithm  = config::flightmodes[0].flightmodeConstructor();
        AM::AttitudeManager am{control_algorithm};

        // Non null references
        EXPECT_THAT(am.motorReferences_[0], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[1], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[2], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[3], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[4], ::testing::NotNull());

        // Shouldn't do anything
        am.outputToMotor(config::throttle, 40);
        am.outputToMotor(config::pitch, 30);
        am.outputToMotor(config::roll, 20);
        am.outputToMotor(config::yaw, 10);

        delete control_algorithm;
        delete[] config::motors;
        config::motors = nullptr;
    }

    // 5 motors of yaw type, some are inverted, *DO MIXED DRIVERS WHEN THERE ARE IMPLEMENATIONS OF MORE THAN ONE DRIVER*
    {
        config::NUM_MOTORS = 5;
        config::motors = new config::Motor_t[config::NUM_MOTORS];

        config::motors[0] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::yaw>
            };
        config::motors[1] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = true,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::yaw>
            };
        config::motors[2] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = true,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::yaw>
            };
        config::motors[3] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::yaw>
            };
        config::motors[4] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = true,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::yaw>
            };

        AM::Flightmode *control_algorithm  = config::flightmodes[0].flightmodeConstructor();
        AM::AttitudeManager am{control_algorithm};

        // Non null references
        EXPECT_THAT(am.motorReferences_[0], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[1], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[2], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[3], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[4], ::testing::NotNull());

        // Setting non-existent motors shouldn't affect yaw
        am.outputToMotor(config::yaw, 0);
        am.outputToMotor(config::throttle, 40);
        am.outputToMotor(config::pitch, 30);
        am.outputToMotor(config::roll, 20);

        EXPECT_EQ(0, am.motorInstances_[0].motorInstance->percent_);
        EXPECT_EQ(100, am.motorInstances_[1].motorInstance->percent_);
        EXPECT_EQ(100, am.motorInstances_[2].motorInstance->percent_);
        EXPECT_EQ(0, am.motorInstances_[3].motorInstance->percent_);
        EXPECT_EQ(100, am.motorInstances_[4].motorInstance->percent_);

        // Should set all of them
        am.outputToMotor(config::yaw, 50);

        EXPECT_EQ(50, am.motorInstances_[0].motorInstance->percent_);
        EXPECT_EQ(50, am.motorInstances_[1].motorInstance->percent_);
        EXPECT_EQ(50, am.motorInstances_[2].motorInstance->percent_);
        EXPECT_EQ(50, am.motorInstances_[3].motorInstance->percent_);
        EXPECT_EQ(50, am.motorInstances_[4].motorInstance->percent_);

        // Another setting test
        am.outputToMotor(config::yaw, 70);

        EXPECT_EQ(70, am.motorInstances_[0].motorInstance->percent_);
        EXPECT_EQ(30, am.motorInstances_[1].motorInstance->percent_);
        EXPECT_EQ(30, am.motorInstances_[2].motorInstance->percent_);
        EXPECT_EQ(70, am.motorInstances_[3].motorInstance->percent_);
        EXPECT_EQ(30, am.motorInstances_[4].motorInstance->percent_);

        delete control_algorithm;
        delete[] config::motors;
        config::motors = nullptr;
    }

    // 1 motor of roll type
    {
        config::NUM_MOTORS = 1;
        config::motors = new ::config::Motor_t[config::NUM_MOTORS];

        config::motors[0] =
            config::Motor_t{   //Roll servo motor
                .axis = config::roll,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::roll>
            };

        AM::Flightmode *control_algorithm  = config::flightmodes[0].flightmodeConstructor();
        AM::AttitudeManager am{control_algorithm};

        // Non null references
        EXPECT_THAT(am.motorReferences_[0], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[1], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[2], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[3], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[4], ::testing::NotNull());

        // Setting non-existent motors first shouldn't affect roll
        am.outputToMotor(config::throttle, 40);
        am.outputToMotor(config::pitch, 30);
        am.outputToMotor(config::yaw, 20);
        am.outputToMotor(config::roll, 10);

        EXPECT_EQ(10, am.motorInstances_[0].motorInstance->percent_);

        // Setting non-existent motors last shouldn't affect roll
        am.outputToMotor(config::roll, 50);
        am.outputToMotor(config::throttle, 60);
        am.outputToMotor(config::pitch, 70);
        am.outputToMotor(config::yaw, 80);
        
        EXPECT_EQ(50, am.motorInstances_[0].motorInstance->percent_);

        // Another setting test
        am.outputToMotor(config::roll, 100);

        EXPECT_EQ(100, am.motorInstances_[0].motorInstance->percent_);

        delete control_algorithm;
        delete[] config::motors;
        config::motors = nullptr;
    }

    // 7 motors of all motor types, given in a non-uniform order, some are inverted
    {
        config::NUM_MOTORS = 7;
        config::motors = new config::Motor_t[config::NUM_MOTORS];

        config::motors[0] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::roll,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::roll>
            };
        config::motors[1] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::throttle,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::throttle>
            };
        config::motors[2] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::yaw>
            };
        config::motors[3] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::pitch,
                .isInverted = true,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::pitch>
            };
        config::motors[4] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::pitch,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::pitch>
            };
        config::motors[5] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::throttle,
                .isInverted = true,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::throttle>
            };
        config::motors[6] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ config::yaw>
            };

        AM::Flightmode *control_algorithm  = config::flightmodes[0].flightmodeConstructor();
        AM::AttitudeManager am{control_algorithm};

        // Non null references
        EXPECT_THAT(am.motorReferences_[0], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[1], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[2], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[3], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[4], ::testing::NotNull());

        // Check motor order
        EXPECT_EQ(config::yaw, am.motorInstances_[0].motorInstance->timerChannel_);
        EXPECT_EQ(config::yaw, am.motorInstances_[1].motorInstance->timerChannel_);
        EXPECT_EQ(config::pitch, am.motorInstances_[2].motorInstance->timerChannel_);
        EXPECT_EQ(config::pitch, am.motorInstances_[3].motorInstance->timerChannel_);
        EXPECT_EQ(config::roll, am.motorInstances_[4].motorInstance->timerChannel_);
        EXPECT_EQ(config::throttle, am.motorInstances_[5].motorInstance->timerChannel_);
        EXPECT_EQ(config::throttle, am.motorInstances_[6].motorInstance->timerChannel_);

        // Setting motors
        am.outputToMotor(config::yaw, 0);
        am.outputToMotor(config::throttle, 5);
        am.outputToMotor(config::pitch, 12);
        am.outputToMotor(config::roll, 33);

        EXPECT_EQ(0, am.motorInstances_[0].motorInstance->percent_); // yaw
        EXPECT_EQ(0, am.motorInstances_[1].motorInstance->percent_); // yaw
        EXPECT_EQ(88, am.motorInstances_[2].motorInstance->percent_); // pitch inverted
        EXPECT_EQ(12, am.motorInstances_[3].motorInstance->percent_); // pitch
        EXPECT_EQ(33, am.motorInstances_[4].motorInstance->percent_); // roll
        EXPECT_EQ(5, am.motorInstances_[5].motorInstance->percent_); // throttle
        EXPECT_EQ(95, am.motorInstances_[6].motorInstance->percent_); // throttle inverted

        // Should set only pitch
        am.outputToMotor(config::pitch, 52);

        EXPECT_EQ(0, am.motorInstances_[0].motorInstance->percent_); // yaw
        EXPECT_EQ(0, am.motorInstances_[1].motorInstance->percent_); // yaw
        EXPECT_EQ(48, am.motorInstances_[2].motorInstance->percent_); // pitch inverted
        EXPECT_EQ(52, am.motorInstances_[3].motorInstance->percent_); // pitch
        EXPECT_EQ(33, am.motorInstances_[4].motorInstance->percent_); // roll
        EXPECT_EQ(5, am.motorInstances_[5].motorInstance->percent_); // throttle
        EXPECT_EQ(95, am.motorInstances_[6].motorInstance->percent_); // throttle inverted

        // Another setting test
        am.outputToMotor(config::roll, 7);
        am.outputToMotor(config::pitch, 67);
        am.outputToMotor(config::throttle, 92);
        am.outputToMotor(config::yaw, 44);
       
        EXPECT_EQ(44, am.motorInstances_[0].motorInstance->percent_); // yaw
        EXPECT_EQ(44, am.motorInstances_[1].motorInstance->percent_); // yaw
        EXPECT_EQ(33, am.motorInstances_[2].motorInstance->percent_); // pitch inverted
        EXPECT_EQ(67, am.motorInstances_[3].motorInstance->percent_); // pitch
        EXPECT_EQ(7, am.motorInstances_[4].motorInstance->percent_); // roll
        EXPECT_EQ(92, am.motorInstances_[5].motorInstance->percent_); // throttle
        EXPECT_EQ(8, am.motorInstances_[6].motorInstance->percent_); // throttle inverted

        delete control_algorithm;
        delete[] config::motors;
        config::motors = nullptr;
    }

}

}