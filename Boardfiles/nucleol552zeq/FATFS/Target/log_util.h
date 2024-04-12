#ifndef LOG_UTIL_H
#define LOG_UTIL_H

#include "stddef.h"
#include "ff.h"

FRESULT logWrite(char*, void*, UINT, UINT*); 
FRESULT logRead(char*, void*, UINT, UINT*);

#endif /* LOG_UTIL_H */