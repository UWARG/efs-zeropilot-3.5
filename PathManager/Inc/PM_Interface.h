#ifndef ZPSW3_PM_INTERFACE_HPP
#define ZPSW3_PM_INTERFACE_HPP
/***This interface exists so that freeRTOS can call C-Style functions while not messing up the fact that most of our development is in C++***/

#ifdef __cplusplus
extern "C" {
#endif

void PathManagerInterfaceInit(void);

bool PathManagerInterfaceExecute(void);


#ifdef __cplusplus
}
#endif

#endif 
