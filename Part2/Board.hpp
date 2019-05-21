#include "Headers.hpp"

#define LIVE_CELL 1
#define DEAD_CELL 0
#define BIRTH 3


#ifndef PART1_BOARD_H
#define PART1_BOARD_H

class Board{
private:
    vector<string> current;
    vector<string> next;

    int tiles_num;
    int [] tiles_start_delimiters;                          //index i holds the start line of tile i

public:
    Board(vector<string> input_board, int tile_num);
    ~Board();

    int get_start_line(int tile_num);
    int get_end_line(int tile_num);

    int live_neighbors(int row, int col);

//    void get_tile_delimeter(int tile_num, int* delimiter);      //send an empty array of 2 cells to update


    void tile_step(int tile_num);
    void make_step();
};


#endif //PART1_BOARD_H
