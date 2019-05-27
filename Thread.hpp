#ifndef __THREAD_H
#define __THREAD_H
#include "Headers.hpp"
#include "Board.hpp"
#include "PCQueue.hpp"
class Thread
{
public:
	Thread(uint thread_id)  : m_thread_id(thread_id)
	{
        // Only places thread_id
	} 
	virtual ~Thread() {} // Does nothing 

	// Returns true if the thread was successfully started, false if there was an error starting the thread 
	// Creates the internal thread via pthread_create 
	bool start()
	{
        pthread_t thread;
        int retval=pthread_create(&thread, NULL, entry_func, NULL);
    }

	// Will not return until the internal thread has exited. 
	void join()
	{
		pthread_join()
	}

	// Returns the user identifier
	uint thread_id()
	{
		return m_thread_id; 
	}
protected:
	// Implement this method in your subclass with the code you want your thread to run. 
	virtual void thread_workload() = 0;
	uint m_thread_id; // A number from 0 -> Number of threads initialized, providing a simple numbering for you to use

private:
	static void * entry_func(void * thread) { ((Thread *)thread)->thread_workload(); return NULL; }
	pthread_t m_thread;
};

class GOL_thread : Thread {
private:
	Board* board;
    PCQueue<int>* queue;

public:
	GOL_thread(uint thread_id, Board* board, PCQueue<int>* queue) :
            Thread(thread_id), board(board), queue(queue){};
	~GOL_thread();

	void thread_workload() override {
		int num = queue->pop();
		board->tile_step(num);

		board->task_done();
		if(board->get_tasks_done()==board->get_tiles_num()){
			board->sem_up();
		}

	}
};

#endif
