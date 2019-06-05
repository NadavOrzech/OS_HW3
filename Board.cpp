
#include "Board.hpp"

Board::Board(const string& s, int tile_num): tiles_num(tile_num) {
    this->current= new bool_mat();
    this->next= new bool_mat();
    vector<string> lines=utils::read_lines(s);
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
    this->tasks_done=0;
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
    //returns number of live neighbors of given cell
    //the neighbors consist of the eight cells surrounding it, including diagonal

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
    //applies one step of the rules for the given tile
    //updates the "next" board according to the games rules

    int start_line=this->get_start_line(tile_num);
    int end_line=this->get_end_line(tile_num);
    int live_neighbors;

    for (int i = start_line; i <=end_line ; ++i) {
        for (int j = 0; j < this->width; ++j) {
            live_neighbors=this->live_neighbors(i,j);

            if(current->at(i).at(j)==DEAD_CELL && live_neighbors==3)
                next->at(i).at(j)=LIVE_CELL;
            else if(current->at(i).at(j)==LIVE_CELL && (live_neighbors>3 || live_neighbors<2))
                next->at(i).at(j)=DEAD_CELL;
            else
                next->at(i).at(j)=current->at(i).at(j);
        }
    }
}

void Board::swap_boards(){
    bool_mat* tmp;
    tmp=current;
    current=next;
    next=tmp;
}

void Board::printboard(){
    cout << u8"╔" << string(u8"═") * this->width << u8"╗" << endl;
    for (uint i = 0; i < this->height; ++i) {
        cout << u8"║";
        for (uint j = 0; j < this->width; ++j) {
            cout << (this->current->at(i).at(j) ? u8"█" : u8"░");
        }
        cout << u8"║" << endl;
    }
    cout << u8"╚" << string(u8"═") * this->width << u8"╝" << endl;
}

int Board::get_tiles_num(){
    return this->tiles_num;
}

void Board::task_done(){
    this->tasks_done++;
}
void Board::reset_tasks_done(){
    this->tasks_done=0;
}

int Board::get_tasks_done(){
    return this->tasks_done;
}


