//
// Created by Rahul on 2024-01-17.
//

#include "gtest/gtest.h"
#include "../src/Transmit.cpp"

IncomingMessage message_IN;
TransmitMessage message_OUT;

// Example usage:
float latitudeValue = 40.7;
float longitudeValue = 4.2;
float vxValue = 1.0;
int altitudeValue = 1000;




TEST(TransmitTestSuite, Latitude_Equivalence){
message_OUT.populateIncomingMessage(message_IN,latitudeValue,longitudeValue, vxValue,altitudeValue);
float expectedLatitude = 40.7;
float expectedLongitude = 4.2;
    EXPECT_EQ(message_OUT.getLatitude(message_IN), expectedLatitude);
    EXPECT_EQ(message_OUT.getLongitude(message_IN), expectedLongitude);
    EXPECT_EQ(message_IN.yaw.has_value(), false);
};

