#ifndef _QUEUEL_H
#define _QUEUEL_H
#include "Headers.hpp"
#include "Semaphore.hpp"



// Single Producer - Multiple Consumer queue
template <typename T>
class PCQueue
{
private:
	pthread_mutex_t	mutex, cond_mutex;
	Semaphore size;
	bool writer_lock, reader_lock;
	pthread_cond_t cond;
	std::queue<T> queue;

public:
	PCQueue(){
        queue=new std::queue<T>();
        pthread_mutex_init(&this->cond_mutex, nullptr);        // NEED TO CHECK what attribute to add
        pthread_mutex_init(&this->mutex, nullptr);
        this->size=new Semaphore();

        this->writer_lock= false;
        this->reader_lock=false;

	}
	~PCQueue(){
        delete this->size;
        delete this->consumer;
        delete this->resource;
        delete this->queue;
        pthread_mutex_destroy(&cond_mutex);
        pthread_mutex_destroy(&mutex);
    }

	// Blocks while queue is empty. When queue holds items, allows for a single
	// thread to enter and remove an item from the front of the queue and return it. 
	// Assumes multiple consumers.
	T pop(){
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

	// Allows for producer to enter with *minimal delay* and push items to back of the queue.
	// Hint for *minimal delay* - Allow the consumers to delay the producer as little as possible.  
	// Assumes single producer 
	void push(const T& item){
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



};
// Recommendation: Use the implementation of the std::queue for this exercise
#endif