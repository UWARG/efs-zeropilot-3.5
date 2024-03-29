/*
 * PID.hpp
 *
 *  Created on: May 27, 2022
 *      Author: Anthony (anni) Luo
 *  Restructured on: Mar 2024
 *       Author: Camron Sabahi
 */

#ifndef PID_HPP
#define PID_HPP

#include <cmath>
#include <cstdint>

/***********************************************************************************************************************
 * Prototypes
 **********************************************************************************************************************/

struct ControlData{ 
   /* data */
   float desired;
   float actual;
   float actualRate;
};

struct PIDGains{
   float kp;
   float ki;
   float kd;
};

struct PIDValues{
   float kp;
   float ki;
   float kd;
   float desired;
   float actual;
   float actualRate;
   float max_output;
   float min_output;
   float i_max;
};

typedef enum {
   proportional,
   integral,
   derivative
} GainTerm;

struct AxisPIDs{
   PIDController pitchPID;
   PIDController rollPID;
   PIDController yawPID;
   PIDController thrustPID;
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
      PIDController(PIDValues _data)
         : pid{.kp = _data.kp,
               .kd = _data.kd,
               .ki = _data.ki,
               .i_max = _data.i_max,
               .min_output = _data.min_output,
               .max_output = _data.max_output,
               .desired = _data.desired,
               .actual = _data.actual,
               .actualRate = _data.actualRate},
               integral(0.0f),
               prevError(0.0f){}

      //Constructor taking pre-constructed PID strucutre
      PIDController(PIDController::PID _pid) : pid(_pid), integral(0.0f) {}

      //Destructor using C++ default
      ~PIDController() = default;

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
   float execute(ControlData _data);
   float execute_p(ControlData _data);
   float execute_i(ControlData _data);

   // 2 derivative calculations to compare historical calculation vs back calculation
   //Derivative calculation using historical data
   float execute_d_hist(ControlData _data);

   //Derivative calculation using back calculation
   float execute_d_back(ControlData _data);

   void updatePIDGains(PIDGains _gains);

   float getIntegral() { return integral; }
   void setKi(float _ki) {pid.ki = _ki;};
   void setKp(float _kp) {pid.kp = _kp;};
   void setKd(float _kd) {pid.kd = _kd;};
   void setDesired(float _desired) {pid.desired = _desired;};
   void setActual(float _actual) {pid.actual = _actual;};
   void setActualRate(float _actualRate) {pid.actualRate = _actualRate;};

   float map(float x, float in_min, float in_max, float out_min, float out_max);


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

   private:
      PID pid;

      float integral = 0.0f;
      float prevError = 0.0f;
      float historicalValue[3] = {0.0f};  // Allows us to compute our derivative
                                        // if necessary
};

#endif /* PID_HPP_ */
