
#include "Semaphore.hpp"
//#include <windows.h>

#define LOCKED 0

Semaphore::Semaphore() : semaphorMax(0), counter(0){
    pthread_cond_init(&cond, nullptr);
    pthread_mutex_init(&mutex, nullptr);          // NEED TO CHECK what attribute to add
}

Semaphore::Semaphore(unsigned val) : semaphorMax(val), counter(val){
    pthread_cond_init(&cond, nullptr);
    pthread_mutex_init(&mutex, nullptr);          // NEED TO CHECK what attribute to add
}

Semaphore::~Semaphore() {
    pthread_cond_destroy(&cond);
    pthread_mutex_destroy(&mutex);
}

void Semaphore::up(){
    pthread_mutex_lock(&mutex);
    counter++;

    pthread_cond_signal(&cond);
    pthread_mutex_unlock(&mutex);
}

void Semaphore::down() {
    pthread_mutex_lock(&mutex);
    while (counter <= LOCKED) {
        pthread_cond_wait(&cond, &mutex);
    }
    counter--;
    pthread_mutex_unlock(&mutex);
}