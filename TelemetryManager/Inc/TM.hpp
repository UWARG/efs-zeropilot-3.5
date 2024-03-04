//
// Created by Yarema Dzulynsky on 2023-08-24.
//

#ifndef TM_H
#define TM_H

#include "FreeRTOS.h"
#include "GroundStationComms.hpp"
#include "MavlinkTranslator.hpp"
#include "TimerInterrupt.hpp"
#include "task.h"

class TelemetryManager {
    TelemetryManager();
    ~TelemetryManager() {};

   public:
    GroundStationComms GSC;
    MavlinkTranslator MT;
    TimerInterrupt TI;

/**
 * @brief Allows access to the TelemetryManager instance statically. This can be done because
 * we will only ever have one instance of the TelemetryManager. This is a singleton pattern.
 * 
 * @return TelemetryManager& Reference to the static TelemetryManager instance.
 */
    static TelemetryManager &getInstance() {
        static TelemetryManager instance;
        return instance;
    }

    TelemetryManager(const TelemetryManager &) = delete;             // Prevent copying
    TelemetryManager &operator=(const TelemetryManager &) = delete;  // Prevent assignment
    

    /**
     * @brief Initialize TM threads and timer interrupts.
     *
     */
    static void init();

    /**
     * @brief This thread is responsible for taking the bytes from the GSC.DMAReceiveBuffer and
     * converting them to Mavlink messages/triggering the callbacks associated with each Mavlink
     * message.
     */
    static void translateToMavlinkThread(void *pvParameters);

    /**
     * @brief This thread is responsible for taking data from other managers and converting
     * them to Mavlink bytes, then putting them into GSC.lowPriorityTransmitBuffer.
     */
    static void mavlinkToBytesThread(void *pvParameters);

    /**
     * @brief This interrupt service routine is called every 1000ms. It is responsible for
     * sending the highest priority drone "state" data to the ground station. Data such as
     * heartbeat message, altitude, attitude, latitude, longitude... And anything else deemed
     * important enough to be transmitted at a regular interval. This is the highest priority
     * data in the GSC.highPriorityTransmitBuffer.
     *
     */
    static void TimerISR1000ms();

    /**
     * @brief This method is responsible for
     * sending non routine data to the ground station. Such as arm disarmed message status,
     * fulfilling data requests from the ground station etc. This is the lowest priority data
     * in the GSC.lowPriorityTransmitBuffer.
     */
    static void sendLowPriorityData();
};

#endif
