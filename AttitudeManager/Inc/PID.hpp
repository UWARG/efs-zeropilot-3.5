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

class PIDController;  // Forward declaration to avoid errors in structs

struct PIDGains {
    float kp;
    float ki;
    float kd;
};

enum GainTerm { Proportional, Integral, Derivative };

enum PidAxis { Pitch, Roll, Yaw, Throttle };

typedef struct PidConfig_t {
    float kp;
    float kd;
    float ki;
    float i_max;
    float min_output;
    float max_output;
    float min_input;
    float max_input;
    float desired;
    float actual;
    float actualRate;
} PidConfig_t;

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

class PIDController {
   public:
    PidConfig_t pid_conf;

    // Default Constructor
    PIDController()
        : pid_conf{.kp = 0.0f,
                   .kd = 0.0f,
                   .ki = 0.0f,
                   .i_max = 0.0f,
                   .min_output = 0.0f,
                   .max_output = 100.0f,
                   .min_input = 0.0f,
                   .max_input = 100.0f,
                   .desired = 0.0f,
                   .actual = 0.0f,
                   .actualRate = 0.0f},
          integral(0.0f),
          prevError(0.0f) {}

    PIDController(float _kp, float _kd, float _ki, float _i_max, float _min_output,
                  float _min_input, float _max_input, float _max_output, float _desired,
                  float _actual, float _actualRate)
        : pid_conf{.kp = _kp,
                   .kd = _kd,
                   .ki = _ki,
                   .i_max = _i_max,
                   .min_output = _min_output,
                   .max_output = _max_output,
                   .min_input = _min_input,
                   .max_input = _max_input,
                   .desired = _desired,
                   .actual = _actual,
                   .actualRate = _actualRate},
          integral(0.0f),
          prevError(0.0f) {}

    // Constructor taking pre-constructed PID strucutre
    PIDController(const PidConfig_t& _pid_conf)
        : pid_conf(_pid_conf), integral(0.0f), prevError(0.0f) {}

    // Destructor using C++ default
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
    float execute(float _desired, float _actual, float _actualRate);
    float execute_p(float _desired, float _actual, float _actualRate);
    float execute_i(float _desired, float _actual, float _actualRate);

    // 2 derivative calculations to compare historical calculation vs back calculation
    // Derivative calculation using historical data
    float execute_d_hist(float _desired, float _actual, float _actualRate);

    // Derivative calculation using back calculation
    float execute_d_back(float _desired, float _actual, float _actualRate);

    float getIntegral() { return integral; }
    void setKi(float _ki) { pid_conf.ki = _ki; };
    void setKp(float _kp) { pid_conf.kp = _kp; };
    void setKd(float _kd) { pid_conf.kd = _kd; };
    void setGainTerm(GainTerm _gainTerm, float _desired_gain);
    void setDesired(float _desired) { pid_conf.desired = _desired; };
    void setActual(float _actual) { pid_conf.actual = _actual; };
    void setActualRate(float _actualRate) { pid_conf.actualRate = _actualRate; };

    float map(float x, float out_min, float out_max);

    void setNewPid(const PidConfig_t& new_pid_conf) { pid_conf = new_pid_conf; }

    void setNewPid(float _kp, float _ki, float _kd, float _i_max, float _min_output,
                   float _max_output, float _desired, float _actual, float _actualRate) {
        pid_conf = PidConfig_t{.kp = _kp,
                               .kd = _kd,
                               .ki = _ki,
                               .i_max = _i_max,
                               .min_output = _min_output,
                               .max_output = _max_output,
                               .desired = _desired,
                               .actual = _actual,
                               .actualRate = _actualRate};
    }

   private:
    float integral = 0.0f;
    float prevError = 0.0f;             // Compute derivative using back calculation if needed
    float historicalValue[3] = {0.0f};  // Compute derivative using historical data if needed
};

#endif /* PID_HPP_ */
