#include "PCQueue.hpp"


template <class T> // Add definition in PCQueue.hpp if you need this constructor
PCQueue<T>::PCQueue(){
    queue=new std::queue<T>();
    pthread_mutex_init(&mutex, nullptr);        // NEED TO CHECK what attribute to add
    this->sem=new Semaphore();
}

template <class T>
PCQueue<T>::~PCQueue() {
    delete this->sem;
    delete this->queue;
    pthread_mutex_destroy(&mutex);
}

template <class T>
T PCQueue<T>::pop(){
    T retVal;
    sem.down();

    pthread_mutex_lock(&mutex);             //Start of the critical section
    retVal=this->queue.front();
    this->queue.pop();
    pthread_mutex_unlock(&mutex);           //End of the critical section

    return retVal;
}

template <class T>
void PCQueue<T>::push(const T& item){
    pthread_mutex_lock(&mutex);             //Start of the critical section
    this->queue.push(item);
    pthread_mutex_unlock(&mutex);           //End of the critical section

    sem.up();
}
