#ifndef SENSOR_FUSION_INTERFACE_H
#define SENSOR_FUSION_INTERFACE_H

#include "sensor_fusion.hpp"

#ifdef __cplusplus
extern "C" {
#endif

/* Note on Implementation */
/* Give the thread with init function higher priority than */
/* other threads to make sure everything works properly */

bool SensorFusionInterfaceInit(void);
SFError_t SensorFusionInterfaceExecute(void);
SFError_t SensorFusionInterfaceOutput(SFOutput_t *SFoutput);

#ifdef __cplusplus
}
#endif 

#endif
