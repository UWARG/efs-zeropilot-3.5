#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "AM.hpp"
#include "config.hpp"

namespace AM {

TEST(AttitudeManager, MotorInitialization) {

    {
        config::NUM_MOTORS = 5;
        config::motors = new ::config::Motor_t[config::NUM_MOTORS];

        config::motors[0] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ 0>
            };
        config::motors[1] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ 0>
            };
        config::motors[2] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ 0>
            };
        config::motors[3] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ 0>
            };
        config::motors[4] =
            config::Motor_t{   //Yaw servo motor
                .axis = config::yaw,
                .isInverted = false,
                .driverConstructor = config::constructObject<MotorChannel, PWMChannel,
                                                    /*timer*/ &htim1, /*timer_channel*/ 0>
            };


        AM::Flightmode *control_algorithm  = config::flightmodes[0].flightmodeConstructor();
        AM::AttitudeManager am{control_algorithm};

        // Non null references
        EXPECT_THAT(am.motorReferences_[0], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[1], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[2], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[3], ::testing::NotNull());
        EXPECT_THAT(am.motorReferences_[4], ::testing::NotNull());

        am.outputToMotor(config::yaw, 50);

        EXPECT_EQ(50, am.motorReferences_[0]->motorInstance->percent_);
        EXPECT_EQ(50, am.motorReferences_[1]->motorInstance->percent_);
        EXPECT_EQ(50, am.motorReferences_[2]->motorInstance->percent_);
        EXPECT_EQ(50, am.motorReferences_[3]->motorInstance->percent_);
        EXPECT_EQ(20, am.motorReferences_[4]->motorInstance->percent_); // just to see it fail for now

        delete control_algorithm;

    }

    {
        EXPECT_EQ(1, 1);
    }

}

}


// namespace AM {

// using config::motors;
// config::Motor_t config::motors[32];
// uint8_t NUM_MOTORS = sizeof(motors)/sizeof(Motor_t);


// TEST(AttitudeManager, MotorInitialization) {

//     EXPECT_EQ(config::NUM_MOTORS, 2);

    
//         // using namespace config;
//         // motors[0] = 
//         //     {   //Yaw servo motor
//         //         .axis = yaw,
//         //         .isInverted = false,
//         //         .driverConstructor = constructObject<MotorChannel, PWMChannel,
//         //                                             /*timer*/ &htim1, /*timer_channel*/ 0>
//         //     };
//         // motors[1] = 
//         //     {   //Yaw servo motor
//         //         .axis = yaw,
//         //         .isInverted = false,
//         //         .driverConstructor = constructObject<MotorChannel, PWMChannel,
//         //                                             /*timer*/ &htim1, /*timer_channel*/ 0>
//         //     };
//         // motors[2] =
//         //     {   //Yaw servo motor
//         //         .axis = yaw,
//         //         .isInverted = false,
//         //         .driverConstructor = constructObject<MotorChannel, PWMChannel,
//         //                                             /*timer*/ &htim1, /*timer_channel*/ 0>
//         //     };

        
        
//         // Motor_t motors[] = {
//         // {   //Yaw servo motor
//         //     .axis = yaw,
//         //     .isInverted = false,
//         //     .driverConstructor = constructObject<MotorChannel, PWMChannel,
//         //                                          /*timer*/ &htim1, /*timer_channel*/ 0>
//         // },
//         // {   //Roll BLDC motor
//         //     .axis = roll,
//         //     .isInverted = true,
//         //     .driverConstructor = constructObject<MotorChannel, TempDSHOTDriver>
//         // }
        
    

        

        

//     }

//     EXPECT_EQ(config::NUM_MOTORS, 3);    
//     EXPECT_EQ(sizeof(config::motors)/sizeof(config::Motor_t), config::NUM_MOTORS);
    

//     // using ::testing::NotNull;

//     // AM::Flightmode *control_algorithm  = config::flightmodes[0].flightmodeConstructor();
//     // AM::AttitudeManager am{control_algorithm};

//     // // Non null references
//     // EXPECT_THAT(am.motorReferences_[0], NotNull());
//     // EXPECT_THAT(am.motorReferences_[1], NotNull());
//     // EXPECT_THAT(am.motorReferences_[2], NotNull());
//     // EXPECT_THAT(am.motorReferences_[3], NotNull());
//     // EXPECT_THAT(am.motorReferences_[4], NotNull());

//     // // Orderly setting by each type
//     // am.outputToMotor(config::yaw)
//     // pitch
//     // roll
//     // throttle

//     // // Setting by each type in a random order

//     // // 

//     // am.outputToMotor(config::roll, 25);

//     // // for (int j; j < )
//     // // am.motorReferences_

//     // // Expect two strings not to be equal.
//     // EXPECT_STRNE("hello", "world");
//     // // Expect equality.
//     // EXPECT_EQ(7 * 6, 42);

//     // delete control_algorithm;
// }

// }
