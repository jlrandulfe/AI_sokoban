// Standard libraries
#include <iostream>
// Local libraries
#include "sokoban.hpp"

using namespace std;

// Class constructor
sokoban::SokobanPuzzle::SokobanPuzzle(int diamonds, int width, int height) {
    // Puzzle initial constraints.
    this->num_of_diamonds = diamonds;
    this->width = width;
    this->height = height;
    // Resize the walkable and goal arrays to the corresponding rows and cols.
    this->walkable_squares.resize(this->width, vector<bool>(this->height));
    this->goal_squares.resize(this->width, vector<bool>(this->height));
    // Resize the current state vector, accordingly to the input constraints,
    // to a (M+1)x2 matrix, being M the number of boxes or diamonds.
    this->current_state.resize(this->num_of_diamonds+1, vector <int> (2));
    this->states_hist.resize(1, vector < vector <int> > (
                             this->num_of_diamonds+1, vector <int> (2)));
    // Vector containing the possible actions for the current state.
    // 1=North, 2=East, 3=South, 4=West
    // Written in reverse order for code optimization (use of pop_back function)
    this->valid_actions = {4, 3, 2, 1};
    this->current_action = 0;
    this->actions_counter = 0;
}

/**
    Set an element of the walkable_squares matrix to a given value.

    A value of '1' or true represents a walkable square, while a '0'
    or false represents a non-walkable square.
*/
void sokoban::SokobanPuzzle::set_walkable_square(int x_coord, int y_coord,
                                                 bool value) {
    this->walkable_squares[x_coord][y_coord] = value;
    return;
}

/**
    Set an element of the goal_squares matrix to a given value.

    A value of '1' or true represents a goal square, while a '0'
    or false represents a non-goal square.
*/
void sokoban::SokobanPuzzle::set_goal_square(int x_coord, int y_coord,
                                             bool value) {
    this->goal_squares[x_coord][y_coord] = value;
    return;
}

/**
    Update the position of the man/player in the puzzle.

    The position is represented by the 2-D cartesian coordinates, and
    is stored in the first sub-vector of the current_state vector.
*/
void sokoban::SokobanPuzzle::update_player_position(int x_coord, int y_coord) {
    // Store the Cartesian coordinates, in the format (X, Y)
    this->current_state[0][0] = x_coord;
    this->current_state[0][1] = y_coord;
    this->states_hist[0] = this->current_state;
    return;
}

/**
    Update the position of a box/diamond in the puzzle.

    The position is represented by the 2-D cartesian coordinates, and
    is stored in one of the sub-vectors of the current_state vector,
    excepting the first one, that corresponds to the man/player.
*/
void sokoban::SokobanPuzzle::update_box_position(int box_index, int x_coord,
                                                 int y_coord) {
    // Store the Cartesian coordinates, in the format (X, Y)
    this->current_state[box_index+1][0] = x_coord;
    this->current_state[box_index+1][1] = y_coord;
    this->states_hist[0] = this->current_state;
    return;
}

/**
    Find a new valid action to perform, following the algorithm rules.

    The priority of the actions are 1>2>3>4 for the current state. If
    none of them can be accomplished, a roll back to the previous
    state has to be done, and a new action has to be found there.
*/
int sokoban::SokobanPuzzle::new_action() {
    int action = 0;
    while (true){
        // Read the last element, and remove it from the valid actions vector.
        if (!this->valid_actions.empty()){
            action = this->valid_actions.back();
            this->valid_actions.pop_back();
            break;
        } else {
            // Roll back to previous step, as there are no more valid actions.
        }
    }
    this->current_action = action;
    return this->current_action;
}

void sokoban::SokobanPuzzle::move_player() {
    int man_x = this->current_state[0][0];
    int man_y = this->current_state[0][1];
    vector<int> new_pos;
    vector<int> further_pos;
    vector < vector<int> > next_state = this->current_state;
    // Calculate the new man position and the further position, depending on the
    // chosen direction. The further position is where a box would be pushed, in
    // there was one in the new position.
    // Move north action.
    if (this->current_action == 1) {
        new_pos = {man_x, man_y-1};
        further_pos = {man_x, man_y-2};
    }
    // Move east action.
    else if (this->current_action == 2) {
        new_pos = {man_x+1, man_y};
        further_pos = {man_x+2, man_y};
    }
    // Move south action.
    else if (this->current_action == 3) {
        new_pos = {man_x, man_y+1};
        further_pos = {man_x, man_y+2};   
    }
    // Move west action.
    else if (this->current_action == 4) {
        new_pos = {man_x-1, man_y};
        further_pos = {man_x-2, man_y};
    }
    // Check if the action leads to a walkable square.
    bool valid_action = true;
    if (this->walkable_squares[new_pos[0]][new_pos[1]] == true) {
        for (auto box_n = 1; box_n < this->num_of_diamonds+1; ++box_n) {
            vector<int> box_pos = this->current_state[box_n];
            // Check is the box is in the new position of the player.
            if (box_pos == new_pos) {
                // Check if there is a wall blocking the push.
                if (this->walkable_squares
                        [further_pos[0]][further_pos[1]] == true) {
                    // Check if there is a second box blocking the push.
                    for (auto box2_n = 1; box2_n < this->num_of_diamonds+1; 
                            ++box2_n) {
                        vector<int> box2_pos = this->current_state[box2_n];
                        if (box2_pos == further_pos){
                            valid_action = false;
                            break;
                        }
                    }
                    // If there is a box, and can be pushed.
                    if (valid_action == true) {
                        next_state[box_n] = further_pos;
                        cout << "Moving box to " << further_pos[0] << ", " 
                                << further_pos[1] << "\n";
                    }
                
                }
                // If the box is adjacent to a wall.
                else {
                    valid_action = false;
                }
                break;
            }
        }
    }
    // If the movement is directed towards a wall.
    else {
        valid_action = false;
    }
    if (valid_action == true) {
        next_state[0] = new_pos;
        if (this->is_repeated_state(next_state) == false) {
            this->actions_counter += 1;
            this->valid_actions = {4, 3, 2, 1};
            cout << "Moving to: " << new_pos[0] << ", " << new_pos[1] << "\n";
        } else {
            cout << "Repeated state.\n";
        }
    } else {
        cout << "Impossible move: " << new_pos[0] << ", " << new_pos[1] << "\n";
    }
    return;
}

bool sokoban::SokobanPuzzle::is_repeated_state(vector < vector<int> > state) {
    return false;
}

/**
    Analyze if the current state of the puzzle is the desired goal.

    For that purpose, it is checked if every box/diamond coordinates
    correspond to a puzzle goal position.
*/
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
    if (this->actions_counter > 10) {
        success = true;
    }
    return success;
}

/**
    Return the number of boxes/diamonds in the puzzle.
*/
int sokoban::SokobanPuzzle::get_diamonds() {
    return this->num_of_diamonds;
}

/**
    Return true if the selected square is walkable.
*/
int sokoban::SokobanPuzzle::get_walkable_square(int x_coord, int y_coord) {
    int value  = this->walkable_squares[x_coord][y_coord];
    return value;
}

void sokoban::SokobanPuzzle::test() {
    cout << "Player position " << current_state[0][0] << ", " <<
            current_state[0][1] << "\n";
    cout << "Boxes positions:\n";
    for(auto index = 0; index < this->num_of_diamonds; ++index){
        cout << this->current_state[index+1][0] << ", " <<
                this->current_state[index+1][1] << "\n";
    }
}
