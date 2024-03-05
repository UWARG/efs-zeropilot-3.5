// FilteringInterface is the only source that will be used directly by the client.
// We define the exact filters we want to use in FilteringInterface.cpp.

#ifndef FILTERING_INTERFACE_HPP
#define FILTERING_INTERFACE_HPP

#include "FilteringGeneral.hpp"

FilteringError_t FilteringInterfaceInit(void);
FilteringError_t FilteringInterfaceExecute(void);

#endif