#include <iostream>
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
}

void sokoban::SokobanPuzzle::set_walkable_square(int row, int col, bool value) {
    this->walkable_squares[row][col] = value;
    return;
}

void sokoban::SokobanPuzzle::set_goal_square(int row, int col, bool value) {
    this->goal_squares[row][col] = value;
    return;
}

int sokoban::SokobanPuzzle::get_diamonds() {
    return num_of_diamonds;
}

void sokoban::SokobanPuzzle::test() {
    cout << walkable_squares[0].size() << "\n";
}
