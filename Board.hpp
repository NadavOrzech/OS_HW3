#include "Headers.hpp"
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

    int tiles_num;
    int* tiles_start_delimiters;   //index i holds the start line of tile i
    int width;
    int height;
    int tasks_done;
    Semaphore gen_done;

public:
    Board(const string& s, int tile_num);
    ~Board();

    int get_start_line(int tile_num);
    int get_end_line(int tile_num);

    int live_neighbors(int row, int col);

//    void get_tile_delimeter(int tile_num, int* delimiter);      //send an empty array of 2 cells to update

    void tile_step(int tile_num);
//    void make_step();
    void printboard();
    void swap_boards();

    int get_tiles_num();
    void task_done();
    void reset_tasks_done();
    int get_tasks_done();
    void sem_up();
    void sem_down();
};


#endif //PART1_BOARD_H
