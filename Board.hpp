//#include "Headers.hpp"
#include "utils.hpp"
#include "Semaphore.hpp"
#define LIVE_CELL 1
#define DEAD_CELL 0



#ifndef PART1_BOARD_H
#define PART1_BOARD_H

class Board{
private:
    bool_mat* current;
    bool_mat* next;

    int tiles_num;                      //number of tiles in generation
    int* tiles_start_delimiters;        //index i holds the start line of tile i
    int width;
    int height;
    int tasks_done;                     //counter of tasks that finished in current generation
    Semaphore gen_done;                 //semaphore for producer to wait for the tasks in current generation to finish

public:
    Board(const string& s, int tile_num);
    ~Board();

    int get_start_line(int tile_num);
    int get_end_line(int tile_num);

    int live_neighbors(int row, int col);
    void tile_step(int tile_num);
    void printboard();
    void swap_boards();

    int get_tiles_num();                //returns number of tiles in game (equals number of threads
    void task_done();                   //updates counter of finished tasks
    void reset_tasks_done();            //resets counter of finished tasks for new generation
    int get_tasks_done();               //returns number of finished tasks in current generation
    void sem_up();
    void sem_down();

};


#endif //PART1_BOARD_H
