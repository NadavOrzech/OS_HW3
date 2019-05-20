
#include "Semaphore.hpp"

#define LOCKED 0

Semaphore::Semaphore() : semaphorMax(0), counter(0){
    pthread_cond_init(&this->cond, nullptr);
    pthread_mutex_init(&this->mutex, nullptr);          // NEED TO CHECK what attribute to add
}

Semaphore::Semaphore(unsigned val) : semaphorMax(val), counter(val){}

Semaphore::~Semaphore() {
    pthread_cond_destroy(&this->cond);
    pthread_mutex_destroy(&this->mutex);
}

void Semaphore::up(){
    mutex_lock(&this->mutex);
    if(this->counter < this->semaphorMax)
        counter++;

    cond_signal(&this->cond);
    mutex_unlock(&this->mutex);
}

void Semaphore::down() {
    mutex_lock(&this->mutex);
    while (this->counter <= 0)
        cond_wait(&this->cond, &this->mutex);

    counter--;
    mutex_unlock(&this->mutex);
}