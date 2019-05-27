#include "PCQueue.hpp"


//template <class T> // Add definition in PCQueue.hpp if you need this constructor
//PCQueue<T>::PCQueue(){
//    queue=new std::queue<T>();
//    pthread_mutex_init(&cond_mutex, nullptr);        // NEED TO CHECK what attribute to add
//    pthread_mutex_init(&mutex, nullptr);
//
//    this->size=new Semaphore();
//    this->consumer=new Semaphore();
//    this->resource=new Semaphore();
//
//    this->c_count=0;
//    this->p_count=0;
//}

template <class T>
PCQueue<T>::~PCQueue() {
    delete this->size;
    delete this->consumer;
    delete this->resource;
    delete this->queue;
    pthread_mutex_destroy(&cond_mutex);
    pthread_mutex_destroy(&mutex);
}

template <class T>
T PCQueue<T>::pop(){
    T retVal;
    size.down();                            //preventing pop from empty queue

    pthread_mutex_lock(&cond_mutex);
    while(this->writer_lock==1 || this->reader_lock==1)
        pthread_cond_wait(&cond,&cond_mutex);

    this->reader_lock = true;               //if there is no writer by this line, unable reading
    pthread_mutex_unlock(&cond_mutex);

//---------Start of the critical section--------

    pthread_mutex_lock(&mutex);
    retVal=this->queue.front();
    this->queue.pop();
    pthread_mutex_unlock(&mutex);

//---------End of the critical section----------

    pthread_mutex_lock(&cond_mutex);
    this->reader_lock = false;
    pthread_cond_broadcast(&cond_mutex);    //enabling the next tread to enter
    pthread_mutex_unlock(&cond_mutex);

    return retVal;
}

template <class T>
void PCQueue<T>::push(const T& item){
    pthread_mutex_lock(&cond_mutex);
    this->writer_lock = true;
    pthread_mutex_unlock(&cond_mutex);

//---------Start of the critical section--------

    pthread_mutex_lock(&mutex);
    this->queue.push(item);
    size.up();                              //allowing the next reader to pop
    pthread_mutex_unlock(&mutex);

//---------End of the critical section----------

    pthread_mutex_lock(&cond_mutex);
    this->writer_lock = false;
    pthread_cond_broadcast(&cond_mutex);
    pthread_mutex_unlock(&cond_mutex);
}
