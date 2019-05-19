
#include "Semaphore.hpp"

#define LOCKED 0

Semaphore::Semaphore() : semaphorMax(0), counter(0){}

Semaphore::Semaphore(unsigned val) : semaphorMax(val), counter(val){}

void
