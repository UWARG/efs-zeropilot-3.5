/**
 * @file TM.hpp
 * @brief What does this file do?
 *
 * @note Anything future maintainers should know about this file?
 *
 * @version 1.0
 * @date 2023-08-24
 * @author Yarema Dzulynsky: initial structure & Implementation
 *
 * @warning Any issues you think are important/foresee in the future?
 */

#ifndef TM_H
#define TM_H

/**
 * @brief This is essentially a compatibility/wrapper function that allows us to turn
 * a member function into a static function. This is necessary because FreeRTOS tasks
 * can only take static functions as arguments.
 * @param memberFunction - The member function to be turned into a static function.
 * Note that the Macro simply takes the name in ANSI text of the member function and
 * appends "Static" to it. All you need to do is pass the name of the member function.
 */
#define TRANSLATE_TO_STATIC(memberFunction)                              \
    static void memberFunction##Static(void* pvParameters) {             \
        auto* tmInstance = static_cast<TelemetryManager*>(pvParameters); \
        tmInstance->memberFunction();                                    \
    }

#include "GroundStationComms.hpp"
#include "MavlinkTranslator.hpp"
#include "TimerInterrupt.hpp"

class TelemetryManager {
    /**
     * @brief Construct a new Telemetry Manager object. Does not initialize the threads or timer
     * interrupts. To do so call the init() method.
     */
    TelemetryManager();
    ~TelemetryManager();

   public:
    GroundStationComms GSC;
    MavlinkTranslator MT;

    /**
     * @brief Initialize TM threads and timer interrupts.
     *
     */
    void init();

    /**
     * @brief This thread is responsible for taking the bytes from the GSC.DMAReceiveBuffer and
     * converting them to Mavlink messages/triggering the callbacks associated with each Mavlink
     * message.
     */
    void translateToMavlinkThread();

    /*
    Create a static version of the translateToMavlinkThread function to be used as a callback for
    the FreeRTOS task.
    */
    TRANSLATE_TO_STATIC(translateToMavlinkThread);

    /**
     * @brief This thread is responsible for taking data from other managers and converting
     * them to Mavlink bytes, then putting them into GSC.lowPriorityTransmitBuffer.
     */
    void mavlinkToBytesThread();

    /*
    Create a static version of the mavlinkToBytesThread function to be used as a callback for the
    FreeRTOS task.
    */
    TRANSLATE_TO_STATIC(mavlinkToBytesThread);

    /**
     * @brief This method is responsible for
     * sending non routine data to the ground station. Such as arm disarmed message status,
     * fulfilling data requests from the ground station etc. This is the lowest priority data
     * in the GSC.lowPriorityTransmitBuffer.
     */
    void sendLowPriorityData();

    /**
     * @brief Create the timer interrupts for the TelemetryManager.
     *
     */
    void initTimerInterrupts();

    /**
     * @brief Create and configure the threads for the TelemetryManager.
     *
     */
    void initTasks();
};

#endif
