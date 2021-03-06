
#include "Game.hpp"

#define POISON -1

/*--------------------------------------------------------------------------------
								Thread inherited struct
--------------------------------------------------------------------------------*/

class GOL_thread : Thread {
private:
    Game* game;

public:
    GOL_thread(uint thread_id, Game* game) : Thread(thread_id), game(game) {};
    ~GOL_thread(){};

    void thread_workload() override {
        int num=INIT;
        while(num!=POISON) {
            num=game->game_get_queue()->pop();          //computing the next tasks
            if (num == POISON) break;                   //inserting POISON tasks to kill the threads
            Board* board=game->game_get_board();

            auto tile_start = std::chrono::system_clock::now();     //saving the time before the step
            board->tile_step(num);
            auto tile_end = std::chrono::system_clock::now();       //saving the time after the step

            struct tile_record record={(double)std::chrono::duration_cast<std::chrono::microseconds>
                    (tile_end - tile_start).count(), this->m_thread_id};

            *(game->tile_hist(game->get_curr_gen()*game->thread_num() + thread_id()))= record;

    //----------Start of critical code---------------

            pthread_mutex_lock(&game->thread_lock);
            board->task_done();                                         //updates counter of finished tasks
            pthread_cond_signal(&game->thread_cond);
            pthread_mutex_unlock(&game->thread_lock);

     //-----------End of critical code---------------
        }
    }
};

/*--------------------------------------------------------------------------------
								Game Struct
--------------------------------------------------------------------------------*/
Game::Game(game_params params){
//Initializing game fields according to given params
    this->m_gen_num=params.n_gen;
    this->m_thread_num=params.n_thread;
    this->interactive_on=params.interactive_on;
    this->print_on=params.print_on;

	this->game_board = new Board(params.filename, this->m_thread_num);
	this->tiles_q=new PCQueue<int>();

    pthread_mutexattr_t attribute;
    pthread_mutexattr_init(&attribute);
    pthread_mutexattr_settype(&attribute, PTHREAD_MUTEX_ERRORCHECK);
    pthread_mutex_init(&thread_lock, &attribute);
    pthread_cond_init(&thread_cond, NULL);

    m_tile_hist.resize(m_thread_num*m_gen_num);
}

Game::~Game(){}

void Game::run() {

	_init_game(); // Starts the threads and all other variables you need
	print_board("Initial Board");
	for (uint i = 0; i < m_gen_num; ++i) {
		auto gen_start = std::chrono::system_clock::now();          //saving the time before the generation
		_step(i); // Iterates a single generation 
		auto gen_end = std::chrono::system_clock::now();            //saving the time after the generation
		m_gen_hist.push_back((double)std::chrono::duration_cast<std::chrono::microseconds>(gen_end - gen_start).count());
		print_board(nullptr);
	} // generation loop
		print_board("Final Board");

	_destroy_game();
}

void Game::_init_game() {
    // Create game fields - Consider using utils:read_file, utils::split
	// Create & Start threads
	// Testing of your implementation will presume all threads are started here

	for (uint i = 0; i < this->m_thread_num; i++) {
		this->m_threadpool.push_back((Thread*)(new GOL_thread(i, this)));
		this->m_threadpool.back()->start();
	}
}

void Game::_step(uint curr_gen) {
	// Push jobs to queue
	// Wait for the workers to finish calculating 
	// Swap pointers between current and next field 
	// NOTE: Threads must not be started here - doing so will lead to a heavy penalty in your grade
    this->curr_gen=curr_gen;
    for (int i = 0; i < game_board->get_tiles_num() ; i++) {
		tiles_q->push(i);
    }

    //----------Start of critical code---------------
    pthread_mutex_lock(&thread_lock);
    while(game_board->get_tasks_done()!= game_board->get_tiles_num()){
        pthread_cond_wait(&thread_cond, &thread_lock);
    }
    game_board->reset_tasks_done();		//resets tasks finished counter for next generation
    pthread_mutex_unlock(&thread_lock);
    //----------End of critical code---------------

    game_board->swap_boards();
}

void Game::_destroy_game(){
	// Destroys board and frees all threads and resources 
	// Not implemented in the Game's destructor for testing purposes. 
	// All threads must be joined here
	for (int j = 0; j < m_thread_num; ++j) {
		tiles_q->push(POISON);          //Inserting POISON TASKS to kill the threads on the next generation
	}
	game_board->reset_tasks_done();		//resets tasks finished counter for next generation

	for (uint i = 0; i < m_thread_num; ++i)
        m_threadpool[i]->join();

    for (uint i = 0; i < m_thread_num; ++i)
        delete(m_threadpool[i]);

    delete game_board;
	delete tiles_q;
}

const vector<double> Game::gen_hist() const{
    return this->m_gen_hist;
}

const vector<tile_record> Game::tile_hist() const {
    return this->m_tile_hist;
}

tile_record* Game::tile_hist(int index) {
    return &this->m_tile_hist.at(index);
}

uint Game::thread_num() const{
    return this->m_thread_num;
}

uint Game::get_curr_gen(){
    return this->curr_gen;
}

Board* Game::game_get_board(){
    return this->game_board;
}

PCQueue<int>* Game::game_get_queue(){
    return this->tiles_q;
}

/*--------------------------------------------------------------------------------
                        Printing function
--------------------------------------------------------------------------------*/
inline void Game::print_board(const char* header) {

	if(print_on){

		// Clear the screen, to create a running animation
		if(interactive_on)
			system("clear");

		// Print small header if needed
		if (header != nullptr)
			cout << "<------------" << header << "------------>" << endl;

		this->game_board->printboard();

		// Display for GEN_SLEEP_USEC micro-seconds on screen
		if(interactive_on)
			usleep(GEN_SLEEP_USEC);
	}

}
