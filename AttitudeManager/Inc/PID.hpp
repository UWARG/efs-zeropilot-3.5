/*
 * PID.hpp
 *
 *  Created on: May 27, 2022
 *      Author: Anthony (anni) Luo
 */

#ifndef PID_HPP
#define PID_HPP

#include <cmath>
#include <cstdint>
#include "FreeRTOS.h"
#include "task.h"

/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

struct PIDValues
{ 
   /* data */
   float desired;
   float actual;
   float actualRate;
};


class PIDController {
   public:
      class PID {
         public:
         float kp, kd, ki, i_max, min_output, max_output, desired, actual, actualRate;
      };

      /**
     * Initialises the Pid object.
     * @param[in]	_kp 	The proportional gain.
     * @param[in]	_ki 	The integral gain.
     * @param[in]	_kd  	The derivative gain.
     * @param[in]	_i_max	Max value the integral should ever be allowed to
     * take on.
     * @param[in]	_min_output		The minimum value that can be
     * output, if computations return smaller, the output will be set to this
     * value.
     * @param[in]	_max_output		The maximum value that can be
     * output, if computations return larger, the output will be set to this
     * value.
     */
      //Constructor taking individual parameters
      PIDController(float _kp, float _ki, float _kd, float _i_max, float _min_output,
                  float _max_output, float _desired, float _actual, float _actualRate)
         : pid{.kp = _kp,
               .kd = _kd,
               .ki = _ki,
               .i_max = _i_max,
               .min_output = _min_output,
               .max_output = _max_output,
               .desired = _desired,
               .actual = _actual,
               .actualRate = _actualRate},
               integral(0.0f),
               taskUpdateFrequency(0),
               stopTaskFlag(false) {}

   //Constructor taking pre-constructed PID structure
   PIDController(PIDController::PID _pid) : pid(_pid), integral(0.0f), taskUpdateFrequency(0), stopTaskFlag(false) {}

   ~PIDController() {
      stopUpdateTask();
   }
   /**
     * Executes a PID computation.
     * The PID algorithm uses the derivative of the actual values, rather than
     * the derivatives of the error. That makes it immune to sudden changes in
     * commanded set points. THIS METHOD MUST BE CALLED AT A REGULAR INTERVAL!!!
     * UNDEFINED BEHAVIOUR WILL OCCUR OTHERWISE. Will use the measured
     * derivative if provided with one. Otherwise, will compute the derivative
     * from the given measurements.
     * @param[in]	desired 	The point we wish to reach.
     * @param[in]	actual 		The current point.
     * @param[in]	actualRate  The current measured derivative (This
     * parameter is optional).
     * @return					The result of the PID
     * computation.
     */
   float execute();
   float execute_p();
   float execute_i();
   float execute_d();
   
   void setKi(float _ki) { pid.ki = _ki; };
   void setKp(float _kp) { pid.kp = _kp; };
   void setKd(float _kd) { pid.kd = _kd; };
   void setDesired(float _desired) { pid.desired = _desired; };
   void setActual(float _actual) { pid.actual = _actual; };
   void setActualRate(float _actualRate) { pid.actualRate = _actualRate; };

   bool getStopFlag() { return stopTaskFlag; }
   uint32_t getTaskUpdateFrequency() { return taskUpdateFrequency; }

   void setStopFlag(bool _stopFlag) { stopTaskFlag = _stopFlag; }
   void setTaskUpdateFrequency(uint32_t _taskUpdateFrequency) { taskUpdateFrequency = _taskUpdateFrequency; }

   float map(float x, float in_min, float in_max, float out_min, float out_max);
   void updatePid(PIDValues values, TickType_t updateFreq, volatile bool& stopFlag);

   void setNewPid(PID _pid) { pid = _pid; }
   void setNewPid(float _kp, float _ki, float _kd, float _i_max, float _min_output,
                  float _max_output) {
      pid = PID{.kp = _kp,
                  .kd = _kd,
                  .ki = _ki,
                  .i_max = _i_max,
                  .min_output = _min_output,
                  .max_output = _max_output};
   }

   static void periodicTask(void* pvParameters);
   void createPeriodicTask(uint32_t updateFrequency);
   void stopUpdateTask();

   private:
      PID pid;

      float integral = 0.0f;
      float historicalValue[3] = {0.0f};  // Allows us to compute our derivative
      uint32_t taskUpdateFrequency;
      bool stopTaskFlag = false;
      TaskHandle_t periodicTaskHandle;                                  // if necessary
};

#endif /* PID_HPP_ */
