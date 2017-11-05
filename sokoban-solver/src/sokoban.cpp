// Standard libraries
#include <iostream>
// Local libraries
#include "sokoban.hpp"

using namespace std;

// Class constructor
sokoban::SokobanPuzzle::SokobanPuzzle(int diamonds, int width, int height) {
    this->num_of_diamonds = diamonds;
    this->width = width;
    this->height = height;
    // Resize the walkable and goal arrays to the corresponding rows and cols.
    this->walkable_squares.resize(this->height, vector<bool>(this->width));
    this->goal_squares.resize(this->height, vector<bool>(this->width));
    // Resize the current state vector, accordingly to the input constraints,
    // to a (M+1)x2 matrix, being M the number of boxes or diamonds.
    this->current_state.resize(this->num_of_diamonds+1, vector <int> (2));
}

void sokoban::SokobanPuzzle::set_walkable_square(int row, int col, bool value) {
    this->walkable_squares[row][col] = value;
    return;
}

void sokoban::SokobanPuzzle::set_goal_square(int row, int col, bool value) {
    this->goal_squares[row][col] = value;
    return;
}

void sokoban::SokobanPuzzle::update_player_position(int x_coord, int y_coord) {
    // Store the Cartesian coordinates, in the format (X, Y)
    this->current_state[0][0] = x_coord;
    this->current_state[0][1] = y_coord;
    return;
}

void sokoban::SokobanPuzzle::update_box_position(int box_index, int x_coord,
                                                 int y_coord) {
    // Store the Cartesian coordinates, in the format (X, Y)
    this->current_state[box_index+1][0] = x_coord;
    this->current_state[box_index+1][1] = y_coord;
    return;
}

bool sokoban::SokobanPuzzle::goal_reached() {
    // Returned boolean, indicating if the goal has been reached.
    bool success = true;
    int x_coord;
    int y_coord;
    for (auto box_index = 0; box_index < this->num_of_diamonds; ++box_index) {
        x_coord = this->current_state[box_index+1][0];
        y_coord = this->current_state[box_index+1][1];
        if (this->goal_squares[y_coord][x_coord] == false) {
            success = false;
            break;
        }
    }
    return success;
}

int sokoban::SokobanPuzzle::get_diamonds() {
    return num_of_diamonds;
}

void sokoban::SokobanPuzzle::test() {
    cout << "Test " << current_state[0].size() << "\n";
    cout << "Player position " << current_state[0][0] << ", " <<
            current_state[0][1] << "\n";
    cout << "Boxes positions:\n";
    for(auto index = 0; index < this->num_of_diamonds; ++index){
        cout << this->current_state[index+1][0] << ", " <<
                this->current_state[index+1][1] << "\n";
    }
}
