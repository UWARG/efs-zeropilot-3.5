#include "FilteringInterface.hpp"
#include "FilteringGeneral.hpp"
#include "MovingAverageFilter.hpp"

FilteringError_t FilteringInterfaceInit(void) {
  FilteringError_t ret;
  
  ret = FilteringGeneral::Init();
  if (ret.errorCode != 0) return ret;

  MovingAverageFiltering::Init();
  if (ret.errorCode != 0) return ret;

  return ret;
}

FilteringError_t FilteringInterfaceExecute(void) {
  FilteringError_t ret;

  MovingAverageOutput_t output;
  ret = MovingAverageFiltering::Update(&output);
  if (ret.errorCode != 0) return ret;

  return ret;
}
