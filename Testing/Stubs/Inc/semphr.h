#ifndef SEMPHR_H
#define SEMPHR_H

typedef int SemaphoreHandle_t;
#define xSemaphoreCreateMutex() 0
long xSemaphoreTake(int, int);
#define xSemaphoreGive(x) ((void)x)

#endif  // SEMPHR_H