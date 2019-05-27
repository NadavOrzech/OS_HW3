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
	~PCQueue();

	// Blocks while queue is empty. When queue holds items, allows for a single
	// thread to enter and remove an item from the front of the queue and return it. 
	// Assumes multiple consumers.
	T pop(); 

	// Allows for producer to enter with *minimal delay* and push items to back of the queue.
	// Hint for *minimal delay* - Allow the consumers to delay the producer as little as possible.  
	// Assumes single producer 
	void push(const T& item); 



};
// Recommendation: Use the implementation of the std::queue for this exercise
#endif