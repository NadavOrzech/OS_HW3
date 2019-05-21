#include "PCQueue.hpp"


template <class T> // Add definition in PCQueue.hpp if you need this constructor
PCQueue<T>::PCQueue():number_of_producers(0) {
    queue = new std::queue<T>();
    pthread_mutex_init(&mutex, nullptr);        // NEED TO CHECK what attribute to add
    this->sem_tasks = new Semaphore();
    pthread_cond_init(&producer_lock, nullptr);
}

template <class T>
PCQueue<T>::~PCQueue() {
    delete this->sem_tasks;
    delete this->queue;
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&producer_lock);

}

template <class T>
T PCQueue<T>::pop(){
    T retVal;
    sem_tasks.down();

    while(number_of_producers>0){}

    pthread_mutex_lock(&mutex);             //Start of the critical section
    retVal=this->queue.front();
    this->queue.pop();
    pthread_mutex_unlock(&mutex);           //End of the critical section

    return retVal;
}

template <class T>
void PCQueue<T>::push(const T& item){

    this->number_of_producers++;
    pthread_mutex_lock(&mutex);             //Start of the critical section
    this->queue.push(item);
    pthread_mutex_unlock(&mutex);           //End of the critical section

    this->number_of_producers--;
    sem_tasks.up();
}
