#include "sensorFusionInterface.hpp"
#include "SensorFusion.hpp"

void SensorFusionInterfaceInit(void) {
    SF_Init();
}

SFError_t SensorFusionInterfaceExecute(void) {
    SFError_t err = SF_GenerateNewResult();
    return err;
}

SFError_t SensorFusionInterfaceOutput(SFOutput_t *SFoutput){
    SFError_t err = SF_GetResult(SFoutput);
    return err;
}
