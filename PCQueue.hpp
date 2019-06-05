#ifndef _QUEUEL_H
#define _QUEUEL_H

#include "Semaphore.hpp"

// Single Producer - Multiple Consumer queue
template <typename T>
class PCQueue
{
private:
	pthread_mutex_t	mutex, cond_mutex;  //Mutex for the critical section and for the conditional lock (
	Semaphore *size;                    //Semaphore to prevent access to an empty Queue
	bool writer_lock, reader_lock;      //Variables that indicates if there is writer or reader inside lock
	pthread_cond_t cond;                //Cond variable for readers waiting Queue
	std::queue<T> *queue;               //The tasks Queue

public:
	PCQueue(){
        queue=new std::queue<T>();

        pthread_mutexattr_t attribute;
        pthread_mutexattr_init(&attribute);
        pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_ERRORCHECK);

        pthread_mutex_init(&cond_mutex, &attribute);
        pthread_mutex_init(&mutex, &attribute);
        pthread_cond_init(&cond, NULL);
        this->size=new Semaphore();

        this->writer_lock= false;
        this->reader_lock=false;
	}

	~PCQueue(){
        delete this->size;
        delete this->queue;
        pthread_mutex_destroy(&cond_mutex);
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&cond);
    }

	// Blocks while queue is empty. When queue holds items, allows for a single
	// thread to enter and remove an item from the front of the queue and return it. 
	// Assumes multiple consumers.
	T pop(){
        T retVal;
        size->down();                           //preventing pop from empty queue

        pthread_mutex_lock(&cond_mutex);
        while(this->writer_lock== true || this->reader_lock==true)
            pthread_cond_wait(&cond,&cond_mutex);

        this->reader_lock = true;               //if there is no writer by this line, unable reading
        pthread_mutex_unlock(&cond_mutex);

     //---------Start of the critical section----------

        pthread_mutex_lock(&mutex);
        retVal=this->queue->front();
        this->queue->pop();
        pthread_mutex_unlock(&mutex);

     //---------End of the critical section----------

        pthread_mutex_lock(&cond_mutex);
        this->reader_lock = false;
        pthread_cond_broadcast(&cond);          //enabling the next thread to enter
        pthread_mutex_unlock(&cond_mutex);

        return retVal;
    }

	// Allows for producer to enter with *minimal delay* and push items to back of the queue.
	// Hint for *minimal delay* - Allow the consumers to delay the producer as little as possible.  
	// Assumes single producer 
	void push(const T& item){
        pthread_mutex_lock(&cond_mutex);
        this->writer_lock = true;               //announcing writer pushing tasks, giving it priority
        pthread_mutex_unlock(&cond_mutex);

    //---------Start of the critical section--------

        pthread_mutex_lock(&mutex);
        this->queue->push(item);
        pthread_mutex_unlock(&mutex);
        size->up();                             //allowing the next reader to pop

    //---------End of the critical section----------

        pthread_mutex_lock(&cond_mutex);
        this->writer_lock = false;
        pthread_cond_broadcast(&cond);
        pthread_mutex_unlock(&cond_mutex);
    }
};
// Recommendation: Use the implementation of the std::queue for this exercise
#endif