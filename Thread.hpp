#ifndef __THREAD_H
#define __THREAD_H
//#include "Headers.hpp"
#include "Board.hpp"
#include "PCQueue.hpp"

#define INIT -2;
#define GAME_DONE -1

struct tile_record {
	double tile_compute_time; // Compute time for the tile
	uint thread_id; // The thread responsible for the compute
};

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

        int retval=pthread_create(&m_thread, NULL, entry_func, this);
        return (bool)retval;        //success==0
    }

	// Will not return until the internal thread has exited. 
	void join(){
		pthread_join(m_thread, NULL);
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
	static void * entry_func(void * thread) {
	    ((Thread *)thread)->thread_workload();
	    return NULL;
	}

	pthread_t m_thread;
};

class GOL_thread : Thread {
private:
	Board** board;
    PCQueue<int>** queue;
	vector<tile_record>* tile_hist;
    pthread_mutex_t mutex;

public:
	GOL_thread(uint thread_id, Board** board, PCQueue<int>** queue, vector<tile_record>* tile_hist) :
            Thread(thread_id), board(board), queue(queue), tile_hist(tile_hist){
        pthread_mutex_init(&mutex, nullptr);          // NEED TO CHECK what attribute to add
    };
	~GOL_thread(){
	    pthread_mutex_destroy(&mutex);
	};

	void thread_workload() override {
		int num=INIT;
        while(num!=GAME_DONE) {
            num = (*queue)->pop();         //num=tile number to do step

			auto tile_start = std::chrono::system_clock::now();
			(*board)->tile_step(num);
			auto tile_end = std::chrono::system_clock::now();
			
			tile_record record;
			record.thread_id=this->m_thread_id;
			record.tile_compute_time=(double)std::chrono::duration_cast<std::chrono::microseconds>(tile_end - tile_start).count();

            //critical code
            pthread_mutex_lock(&mutex);
            tile_hist->push_back(record);
			(*board)->task_done();											//does ++ to task finished counter
            if (num != GAME_DONE && (*board)->get_tasks_done() == (*board)->get_tiles_num())  	//gen finished
				(*board)->sem_up();
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
            else if ((*board)->get_tasks_done() == )  	//gen finished
					(*board)->sem_up();
            //!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!11
			(*this->queue)->signal_cond_thread();
            pthread_mutex_unlock(&mutex);
            //end of critical code
        }
        pthread_exit(nullptr);
	}
};

#endif
