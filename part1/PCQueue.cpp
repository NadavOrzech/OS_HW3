#include "PCQueue.hpp"


template <class T> // Add definition in PCQueue.hpp if you need this constructor
PCQueue<T>::PCQueue(){
    queue=new std::queue<T>();
    pthread_mutex_init(&c, nullptr);        // NEED TO CHECK what attribute to add
    pthread_mutex_init(&p, nullptr);

    this->size=new Semaphore();
    this->consumer=new Semaphore();
    this->resource=new Semaphore();

    this->c_count=0;
    this->p_count=0;
}

template <class T>
PCQueue<T>::~PCQueue() {
    delete this->size;
    delete this->consumer;
    delete this->resource;
    delete this->queue;
    pthread_mutex_destroy(&c);
    pthread_mutex_destroy(&p);
}

template <class T>
T PCQueue<T>::pop(){
    T retVal;
    size.down();

    consumer.down();
    pthread_mutex_lock(&c);
    c_count++;
    if(c_count==1){
        resource.down();
    }
    pthread_mutex_unlock(&c);
    consumer.up();

    //Start of the critical section
    retVal=this->queue.front();
    this->queue.pop();
    //End of the critical section

    pthread_mutex_lock(&c);
    c_count--;
    if(c_count==0){
        resource.up();
    }
    pthread_mutex_lock(&c);

    return retVal;
}

template <class T>
void PCQueue<T>::push(const T& item){
    pthread_mutex_lock(&p);
    p_count++;
    if(p_count==1){
        consumer.down();
    }
    pthread_mutex_unlock(&p);

    //Start of the critical section
    resource.down();
    this->queue.push(item);
    size.up();
    resource.up();
    //End of the critical section

    pthread_mutex_lock(&p);
    p_count--;
    if(p_count==0){
        consumer.up();
    }
    pthread_mutex_unlock(&p);
}
