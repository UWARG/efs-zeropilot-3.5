//
// Created by Gagan Deep Singh on 2023-07-08.
//

#ifndef ZEROPILOT_3_5_SYSTEMMANAGER_HPP
#define ZEROPILOT_3_5_SYSTEMMANAGER_HPP

#include "../../Models/global_config.hpp"
#include "../../Drivers/rc_receiver/Inc/sbus_receiver.hpp"
#include "../../Drivers/rc_receiver/Inc/rcreceiver_datatypes.h"


class SystemManager {
public:
    /* Constructors and Destructors */
    SystemManager();
    ~SystemManager();

    /* Class Functions */
    void flyManually();

    /* Getters and Setters */
    config::Flightmode* getCurrentFlightMode();
    void setCurrentFlightMode(config::Flightmode *flightmode);
private:
    /* Private helper functions */
    void updateRCInputs();
    void executeInputs();

    /* SBUS and AM variables to follow */
    
    /* State-keeping variables to follow */
    config::Flightmode *currentFlightMode_;
    SBUSReceiver rcController_;
    RCControl rcInputs_;

};


#endif //ZEROPILOT_3_5_SYSTEMMANAGER_HPP
