#ifndef __SEMAPHORE_H
#define __SEMAPHORE_H
#include "Headers.hpp"



// Synchronization Warm up 
class Semaphore {
private:
    // TODO
    int semaphorMax;
    int counter;
	pthread_cond_t cond;
	pthread_mutex_t mutex;
public:
	Semaphore(); // Constructs a new semaphore with a counter of 0
	Semaphore(unsigned val); // Constructs a new semaphore with a counter of val
	~Semaphore();

	void up(); // Mark: 1 Thread has left the critical section
	void down(); // Block until counter >0, and mark - One thread has entered the critical section.


};

#endif
