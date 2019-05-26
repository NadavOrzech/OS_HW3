//
// Created by Dell on 5/21/2019.
//

#include "Board.hpp"

Board::Board(const string& s, int tile_num): tiles_num(tile_num) {
    this->current= new bool_mat;
    this->next= new bool_mat;
    vector<string> lines=utils::read_lines(s);          //maybe need copy constructor
    this->height=lines.size();
    vector<string> tmp;

    for (int i = 0; i < this->height ; i++) {
        tmp=utils::split(lines.at(i), ' ');
        vector<bool> bool_row;
        vector<bool> bool_row_next;
        for (int j = 0; j <(int)tmp.size() ; j++) {
            bool_row.push_back(std::stoi(tmp.at(j)));
            bool_row_next.push_back(std::stoi(tmp.at(j)));
        }
        current->push_back(bool_row);
        next->push_back(bool_row_next);

    }

    this->width=tmp.size();
    this->tiles_start_delimiters=new int[tile_num];

    int size_of_tile=this->height/tile_num;
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
        return this->height-1;

    return (this->tiles_start_delimiters[tile_num+1]-1);
}

int Board::live_neighbors(int row, int col){
    int count=0;

    for (int i = row-1; i <= row+1; ++i) {
        for (int j = col-1; j <= col+1 ; ++j) {
            if(i==row && j==col)
                continue;

            if(i>=0 && i<this->height && j>=0 && j<this->width)
                if(current->at(i).at(j)==LIVE_CELL)                //checks if the cell is alive
                    count++;
        }
    }

    return count;

}

void Board::tile_step(int tile_num) {
    int start_line=this->get_start_line(tile_num);
    int end_line=this->get_end_line(tile_num);
    int live_neighbors;


    for (int i = start_line; i <=end_line ; ++i) {
        for (int j = 0; j < this->width; ++j) {
            live_neighbors=this->live_neighbors(i,j);

            if(current->at(i).at(j)==DEAD_CELL && live_neighbors==3)
                next->at(i).at(j)=LIVE_CELL;

            else if(current->at(i).at(j)==LIVE_CELL && live_neighbors>3 && live_neighbors<2)
                next->at(i).at(j)=DEAD_CELL;

            else
                next->at(i).at(j)=current->at(i).at(j);
        }
    }
}

void Board::make_step(){
    for (int i = 0; i < this->tiles_num; i++) {
        tile_step(i);
    }
    swap_boards();
}

void Board::swap_boards(){
    bool_mat* tmp;
    tmp=current;
    current=next;
    next=tmp;
}

void Board::printboard(){

    for (int i = 0; i <this->height ; i++) {
        for (int j = 0; j <this->width ; j++) {
            cout <<current->at(i).at(j);
        }
        cout << "\n";
    }
}