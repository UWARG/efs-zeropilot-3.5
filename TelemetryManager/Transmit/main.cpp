#include <iostream>
#include "src/Transmit.cpp"


int main() {
    IncomingMessage message_Incoming;
    TransmitMessage message_Outgoing;

    // Example usage:
    float latitudeValue = 40.7128;
    float longitudeValue = -74.0060;
    float vxValue = 1.0;
    int altitudeValue = 1000;

    std::cout << message_Outgoing.getLatitude(message_Incoming) << std::endl;
    message_Outgoing.populateIncomingMessage(message_Incoming,latitudeValue,longitudeValue,vxValue,altitudeValue);

    std::cout << message_Outgoing.getLatitude(message_Incoming);
    return 0;
}