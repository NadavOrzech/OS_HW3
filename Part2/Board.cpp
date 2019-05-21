//
// Created by Dell on 5/21/2019.
//

#include "Board.hpp"

Board::Board(vector<string> input_board, int tile_num): tiles_num(tile_num) {
    this->current=new vector<string>(input_board);
    this->next=new vector<string>();

    this->tiles_start_delimiters=new int[tile_num];

    int size_of_tile=current.size()/tile_num;
    for (int i = 0; i < tile_num; i++)
        this->tiles_start_delimiters[i]=i*size_of_tile;
}

Board::~Board() {
    delete this->current;
    delete this->next;
    delete[] this->tiles_start_delimiters;
}

int Board::get_start_line(int tile_num){
    return this->tiles_start_delimiters[tile_num];
}

int Board::get_end_line(int tile_num) {
    if(tile_num==this->tiles_num-1)
        return this->current.size();

    return (this->tiles_start_delimiters[tile_num+1]-1)
}

int Board::live_neighbors(int row, int col){
    int count=0;

    for (int i = row-1; i <= row+1; ++i) {
        for (int j = col-1; j <= col+1 ; ++j) {
            if(i==row && j==col)
                continue;

            if(i>=0 && i<this->current[i].length() && j>=0 && j<this->current.size())
                if((this->current[i])[j]==LIVE_CELL)                //checks if the cell is alive
                    count++;
        }
    }

    return count;

}
void Board::tile_step(int tile_num) {
    int start_line=this->get_start_line(tile_num);
    int end_line=this->get_end_line(tile_num);
    int live_neighbors;
    string str;

    for (int i = start_line; i <=end_line ; ++i) {
        for (int j = 0; j < this->current[i].length(); ++j) {
            live_neighbors=this->live_neighbors(i,j);

            if((this->current[i])[j]==DEAD_CELL && live_neighbors==BIRTH)
                str+="1";
        }
    }
}