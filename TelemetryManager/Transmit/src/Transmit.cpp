#include <iostream>
#include <optional>

struct IncomingMessage { // telemetry message struct
    std::optional<float> latitude;
    std::optional<float> longitude;
    std::optional<float> vx;
    std::optional<float> vy;
    std::optional<float> vz;
    std::optional<float> altitude;
    std::optional<float> pitch;
    std::optional<float> roll;
    std::optional<float> yaw;
};

class TransmitMessage{

private:
    IncomingMessage message;

public:
    void populateIncomingMessage(IncomingMessage& msg, // Populates struct with received data
                                 std::optional<float> incomingLatitude = std::nullopt,
                                 std::optional<float> incomingLongitude = std::nullopt,
                                 std::optional<float> incomingVx = std::nullopt,
                                 std::optional<float> incomingVy = std::nullopt,
                                 std::optional<float> incomingVz = std::nullopt,
                                 std::optional<float> incomingAltitude = std::nullopt,
                                 std::optional<float> incomingPitch = std::nullopt,
                                 std::optional<float> incomingRoll = std::nullopt,
                                 std::optional<float> incomingYaw = std::nullopt) {
        if (incomingLatitude ) {
            msg.latitude = incomingLatitude;
        }

        if (incomingLongitude) {
            msg.longitude = incomingLongitude;
        }

        if (incomingVx) {
            msg.vx = incomingVx;
        }

        if (incomingVy) {
            msg.vy = incomingVy;
        }

        if (incomingVz) {
            msg.vz = incomingVz;
        }

        if (incomingAltitude) {
            msg.altitude = incomingAltitude;
        }

        if (incomingPitch) {
            msg.pitch = incomingPitch;
        }

        if (incomingRoll) {
            msg.roll = incomingRoll;
        }

        if (incomingYaw) {
            msg.yaw = incomingYaw;
        }
    }
    float getLatitude(IncomingMessage& msg){
        if(msg.latitude.has_value()){
            return msg.latitude.value();
        }
    }
    float getLongitude(IncomingMessage& msg){
        if(msg.longitude.has_value()){
            return msg.longitude.value();
        }
    }
    float getVx(IncomingMessage& msg){
        if(msg.vx.has_value()){
            return msg.vx.value();
        }
    }
    float getVy(IncomingMessage& msg){
        if(msg.vy.has_value()){
            return msg.vy.value();
        }
    }
    float getVz(IncomingMessage& msg){
        if(msg.vz.has_value()){
            return msg.vz.value();
        }
    }
    float getAltitude(IncomingMessage& msg){
        if(msg.altitude.has_value()){
            return msg.altitude.value();
        }
    }
    float getPitch(IncomingMessage& msg){
        if(msg.pitch.has_value()){
            return msg.pitch.value();
        }
    }
    float getRoll(IncomingMessage& msg){
        if(msg.roll.has_value()){
            return msg.roll.value();
        }
    }
    float getYaw(IncomingMessage& msg){
        if(msg.yaw.has_value()){
            return msg.yaw.value();
        }
    }
};