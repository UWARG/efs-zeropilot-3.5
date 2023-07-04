#include "sensor_fusion_interface.hpp"
#include "sensor_fusion.hpp"

SFError_t SensorFusionInterfaceInit(void) {
    SFError_t err = SF_Init();
    return err;
}

SFError_t SensorFusionInterfaceExecute(void) {
    SFError_t err = SF_GenerateNewResult();
    return err;
}

SFError_t SensorFusionInterfaceOutput(SFOutput_t *SFoutput){
    SFError_t err = SF_GetResult(SFoutput);
    return err;
}
