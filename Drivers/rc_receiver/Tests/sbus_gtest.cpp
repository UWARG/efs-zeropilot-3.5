// #ifndef SBUS_GTEST_CPP
// #define SBUS_GTEST_CPP

#include <gtest/gtest.h>
#include <fff.h>
#include "fake_func.h"

 DEFINE_FFF_GLOBALS;


FAKE_VALUE_FUNC(int, return_3);


int plus_9(){
    return 9 + return_3();
}


TEST (Runing_Test, ShouldReturnThree)
{
    RESET_FAKE(return_3);

    return_3_fake.return_val = 4;
    EXPECT_EQ(plus_9(), 13);
}

// #include "sbus_receiver.hpp"
// #include "drivers_config.hpp"

// struct SBUS_Receiver_Test: testing :: test{
//     SBUSReceiver* sbus_test;

//     SBUS_Receiver_Test(){
//         sbus_test = sbus_pointer;
//     }

//     ~SBUS_Receiver_Test(){
//         delete sbus_test;
//     }

// };

// TEST_F(SBUS_Receiver_Test, RawDataConvertsToPercentage)
// {
    
//     EXPECT_EQ(sbus_test->sbus_to_rccontrol(SBUS_RANGE_MAX) , 100 );
// }



// #endif