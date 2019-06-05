
#include "utils.hpp"
#include "Semaphore.hpp"

#define LIVE_CELL 1
#define DEAD_CELL 0

#ifndef PART1_BOARD_H
#define PART1_BOARD_H

class Board{
private:
    bool_mat* current;                  //The current board to calculate
    bool_mat* next;                     //The output board for calculations, when generation finished we switch those two

    int tiles_num;                      //Number of tiles to divide the given board
    int* tiles_start_delimiters;        //Cell of index i holds the start line of tile i
    int width;                          //The width of the given board
    int height;                         //The height of the given board
    int tasks_done;                     //Counter of tasks that finished in current generation

public:
    Board(const string& s, int tile_num);
    ~Board();

    int get_start_line(int tile_num);   //Returning from delimiters array the start line index of the given tile num
    int get_end_line(int tile_num);     //Returning from delimiters array the end line index of the given tile num

    int live_neighbors(int row, int col);   //Returning number of live neighbors of current cell according to the GameOfLife rules
    void tile_step(int tile_num);           //Calculating generation of a single thread
    void printboard();                  //Printing the Board
    void swap_boards();                 //Swapping the current and the next boards

    int get_tiles_num();                //returns number of tiles in game (equals number to the number threads)
    void task_done();                   //updates counter of finished tasks
    void reset_tasks_done();            //resets counter of finished tasks for new generation
    int get_tasks_done();               //returns number of finished tasks in current generation
};
#endif //PART1_BOARD_H
