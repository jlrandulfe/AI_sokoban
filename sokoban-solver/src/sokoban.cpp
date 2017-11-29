// Standard libraries
#include <algorithm>
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
    this->max_depth = 120;
    this->max_iterations = 80000;
    // Resize the walkable and goal arrays to the corresponding rows and cols.
    this->walkable_squares.resize(this->width, vector<bool>(this->height));
    this->goal_squares.resize(this->width, vector<bool>(this->height));
    this->deadlock_squares.resize(this->width, vector<bool>(this->height));
    this->free_squares.resize(this->width, vector<bool>(this->height));
    this->labeled_squares.resize(this->width, vector<int>(this->height));
    for (auto col = 0; col < this->width; ++col) {
        fill(this->labeled_squares[col].begin(),
             this->labeled_squares[col].end(), 0);
    }
    // Resize the current state vector, accordingly to the input constraints,
    // to a (M+3) vector, being:
    // - index=0: state depth
    // - index=1: state parent
    // - index=2: player/man position
    // - index=3: state index (Deprecated)
    // - index=4 to M+4: diamonds/boxes positions
    this->current_state.resize(this->num_of_diamonds+4);
    this->current_state[0] = 0;
    this->current_state[1] = -1;
    this->current_state[3] = 0;
    this->states_hist.resize(1, vector <int> (this->num_of_diamonds+4));
    this->current_state_index = 0;
    // Vector containing the possible actions for the current state.
    // 1=North, 2=East, 3=South, 4=West
    // Written in reverse order for code optimization (use of pop_back function)
    this->actions_hist = {0};
    // this->valid_actions = {{4, 3, 2, 1}};
    // this->current_action = 0;
    // this->actions_counter = 0;
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
    // Store the Cartesian coordinates, in the format XY = YxWIDTH + X
    this->current_state[2] = y_coord*this->width + x_coord;
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
    // Store the Cartesian coordinates, in the format XY = YxWIDTH + X
    this->current_state[box_index+4] = y_coord*this->width + x_coord;
    this->states_hist[0] = this->current_state;
    return;
}

void sokoban::SokobanPuzzle::get_deadlock_positions() {
    // Look for the corner squares, and set them as deadlock squares
    for (auto y_coord = 0; y_coord < this->height; ++y_coord) {
        for (auto x_coord = 0; x_coord < this->width; ++x_coord) {
            if (this->walkable_squares[x_coord][y_coord] == true) {
                bool walkable_N = this->walkable_squares[x_coord][y_coord-1];
                bool walkable_E = this->walkable_squares[x_coord+1][y_coord];
                bool walkable_S = this->walkable_squares[x_coord][y_coord+1];
                bool walkable_W = this->walkable_squares[x_coord-1][y_coord];
                if (this->goal_squares[x_coord][y_coord] == true) {
                    this->deadlock_squares[x_coord][y_coord] = false;
                }
                else if (!walkable_N && (!walkable_E || !walkable_W)
                    || !walkable_S && (!walkable_E || !walkable_W)) {
                    this->deadlock_squares[x_coord][y_coord] = true;
                }
                else {
                    this->deadlock_squares[x_coord][y_coord] = false;
                }
            } 
        }       
    }
    // Look for connected corner squares, and set them as deadlock squares.
    for (auto y_coord = 0; y_coord < this->height; ++y_coord) {
        for (auto x_coord = 0; x_coord < this->width; ++x_coord) {
            if (this->deadlock_squares[x_coord][y_coord] == true) {
                // Iterate in the same row, in the positive direction
                for (auto x2 = x_coord+1; x2 < this->width; ++x2) {
                    // If a non-deadlock wall is found, break the iteration
                    if (this->walkable_squares[x2][y_coord] == false) {
                        break;
                    }
                    // If a goal is found, break the iteration
                    if (this->goal_squares[x2][y_coord] == true) {
                        break;
                    }
                    // If a deadlock is found, all the intermeadiate squares
                    // are also a deadlock.
                    else if (this->deadlock_squares[x2][y_coord] == true) {
                        for (auto x = x_coord; x < x2; ++x) {
                            this->deadlock_squares[x][y_coord] = true;
                        }
                    }
                    // If a scape route is found, break the iteration
                    else if (this->walkable_squares[x2][y_coord+1] == true
                             && this->walkable_squares[x2][y_coord-1] == true) {
                        break;
                    }
                }
                // Iterate in the same row, in the negative direction
                for (auto x2 = x_coord-1; x2 > 0; --x2) {
                    // If a non-deadlock wall is found, break the iteration
                    if (this->walkable_squares[x2][y_coord] == false) {
                        break;
                    }
                    // If a goal is found, break the iteration
                    if (this->goal_squares[x2][y_coord] == true) {
                        break;
                    }
                    // If a deadlock is found, all the intermeadiate squares
                    // are also a deadlock.
                    else if (this->deadlock_squares[x2][y_coord] == true) {
                        for (auto x = x_coord; x < x2; ++x) {
                            this->deadlock_squares[x][y_coord] = true;
                        }
                    }
                    // If a scape route is found, break the iteration
                    else if (this->walkable_squares[x2][y_coord+1] == true
                             && this->walkable_squares[x2][y_coord-1] == true) {
                        break;
                    }
                }
                // Iterate in the same column, in the positive direction
                for (auto y2 = y_coord+1; y2 < this->height; ++y2) {
                    // If a non-deadlock wall is found, break the iteration
                    if (this->walkable_squares[x_coord][y2] == false) {
                        break;
                    }
                    // If a goal is found, break the iteration
                    if (this->goal_squares[x_coord][y2] == true) {
                        break;
                    }
                    // If a deadlock is found, all the intermeadiate squares
                    // are also a deadlock.
                    else if (this->deadlock_squares[x_coord][y2] == true) {
                        for (auto y = y_coord; y < y2; ++y) {
                            this->deadlock_squares[x_coord][y] = true;
                        }
                    }
                    // If a scape route is found, break the iteration
                    else if (this->walkable_squares[x_coord+1][y2] == true
                             && this->walkable_squares[x_coord-1][y2] == true) {
                        break;
                    }
                }
                // Iterate in the same column, in the negative direction
                for (auto y2 = y_coord-1; y2 > 0; --y2) {
                    // If a non-deadlock wall is found, break the iteration
                    if (this->walkable_squares[x_coord][y2] == false) {
                        break;
                    }
                    // If a goal is found, break the iteration
                    if (this->goal_squares[x_coord][y2] == true) {
                        break;
                    }
                    // If a deadlock is found, all the intermeadiate squares
                    // are also a deadlock.
                    else if (this->deadlock_squares[x_coord][y2] == true) {
                        for (auto y = y2; y < y_coord; ++y) {
                            this->deadlock_squares[x_coord][y] = true;
                        }
                    }
                    // If a scape route is found, break the iteration
                    else if (this->walkable_squares[x_coord+1][y2] == true
                             && this->walkable_squares[x_coord-1][y2] == true) {
                        break;
                    }
                }
            }
        }
    }
    return;
}

void sokoban::SokobanPuzzle::get_free_squares() {
    for (auto y_coord = 0; y_coord < this->height; ++y_coord) {
        for (auto x_coord = 0; x_coord < this->width; ++x_coord) {
            if (this->walkable_squares[x_coord][y_coord] == true) {
                for (auto box_index = 4; box_index < this->num_of_diamonds+4;
                     ++box_index) {
                    int box_x = this->current_state[box_index]%this->width;
                    int box_y = this->current_state[box_index]/this->width;
                    if (box_x == x_coord && box_y == y_coord) {
                        this->free_squares[x_coord][y_coord] = false;
                        break;
                    }
                    else {
                        this->free_squares[x_coord][y_coord] = true;
                    }
                }
            }
            else {
                this->free_squares[x_coord][y_coord] = false;
            }
        }
    }
    return;
}

void sokoban::SokobanPuzzle::label_squares() {
    int label = 1;
    vector<int> relations {1};
    // 1st pass: assign to each free square a label
    for (auto y_coord = 1; y_coord < this->height; ++y_coord) {
        for (auto x_coord = 1; x_coord < this->width; ++x_coord) {
            if (this->free_squares[x_coord][y_coord] != false) {
                int west_label = this->labeled_squares[x_coord-1][y_coord];
                int north_label = this->labeled_squares[x_coord][y_coord-1];
                if (west_label != 0) {
                    this->labeled_squares[x_coord][y_coord] = west_label;
                    // Case when 2 regions are connected.
                    if (north_label != 0 && north_label != west_label) {
                        int minimum = min(west_label, north_label);
                        int maximum = max(west_label, north_label);
                        relations[maximum-1] = minimum;
                    }
                }
                else if (north_label != 0) {
                    this->labeled_squares[x_coord][y_coord] = north_label;
                }
                else {
                    this->labeled_squares[x_coord][y_coord] = label;
                    label += 1;
                    relations.push_back(label);
                }
            }
            else {
                this->labeled_squares[x_coord][y_coord] = 0;
            }
        }
    }
    // Remove intermediate relationships between regions
    for(auto index = 0; index< relations.size(); ++index) {
        cout << relations[index] << "\n";
        int label = relations[index];
        while(true) {
            if (label != relations[label-1]) {
                label = relations[label-1];
            }
            else {
                relations[index] = label;
                break;
            }
        }
    }

    // 2nd pass: Put connected regions together
    for (auto y_coord = 1; y_coord < this->height; ++y_coord) {
        for (auto x_coord = 1; x_coord < this->width; ++x_coord) {
            int current_label = this->labeled_squares[x_coord][y_coord];
            if (current_label != 0) {
                this->labeled_squares[x_coord][y_coord] =
                        relations[current_label-1];
            }
        }
    }
    return;
}

void sokoban::SokobanPuzzle::get_children() {
    // Analyze the current state
    // Get all the walkable squares connected to the player

    // Get all the pushable boxes from the connected region

    // For every pushable box, get the children state
    // Check if it leads to a deadlock
    // If the child is in the open list, pass
    // If the child is in the explored list, pass only if depth is bigger

    // Add current state to the explored list
    // Order the open list
    return;
}

void sokoban::SokobanPuzzle::update_state() {
    // Select the child with the shortest heuristic weight (optional)

    // Set the current state to be the selected child.
}


/**
    Find a new valid action to perform, following the algorithm rules.

    The priority of the actions are 1>2>3>4 for the current state. If
    none of them can be accomplished, a roll back to the previous
    state has to be done, and a new action has to be found there.
*/
// int sokoban::SokobanPuzzle::new_action() {
//     int action = 0;
//     while (true){
//         // Read the last element, and remove it from the valid actions vector.
//         if (!this->valid_actions[this->current_state_index].empty()
//                 && this->current_state[0] < this->max_depth) {
//             action = this->valid_actions[this->current_state_index].back();
//             this->valid_actions[this->current_state_index].pop_back();
//             // In a standard transition, the parent is the next-to-last state.
//             // this->current_state_index = this->states_hist.size() - 1;
//             break;
//         } else {
//             // Roll back to parent state, as there are no more valid actions.
//             cout << "Rollback!!\n";
//             if (this->current_state_index == 0) {
//                 cout << "Rollback from state 0. Exiting...\n";
//                 exit(0);
//             }
//             cout << "Current state index " << this->current_state_index << "\n";
//             cout << "Stored parent is " << this->current_state[1] << "\n";
//             this->current_state_index = this->current_state[1];
//             this->current_state = this->states_hist[this->current_state[1]];
//             this->actions_hist.pop_back();
//             // this->valid_actions = {4, 3, 2, 1};
//             cout << "Rolling back to state " << this->current_state_index
//                     << "\n";
//         }
//     }
//     this->current_action = action;
//     return this->current_action;
// }

// void sokoban::SokobanPuzzle::move_player() {
//     int man_x = this->current_state[2] % this->width;
//     int man_y = this->current_state[2] / this->width;
//     int new_pos;
//     int further_pos;
//     vector <int> next_state = this->current_state;
//     // Calculate the new man position and the further position, depending on the
//     // chosen direction. The further position is where a box would be pushed, in
//     // there was one in the new position.
//     // Move north action.
//     if (this->current_action == 1) {
//         new_pos = (man_y-1)*this->width + man_x;
//         further_pos = (man_y-2)*this->width + man_x;
//     }
//     // Move east action.
//     else if (this->current_action == 2) {
//         new_pos = man_y*this->width + (man_x+1);
//         further_pos = man_y*this->width + (man_x+2);
//     }
//     // Move south action.
//     else if (this->current_action == 3) {
//         new_pos = (man_y+1)*this->width + man_x;
//         further_pos = (man_y+2)*this->width + man_x;   
//     }
//     // Move west action.
//     else if (this->current_action == 4) {
//         new_pos = man_y*this->width + (man_x-1);
//         further_pos = man_y*this->width + (man_x-2);
//     }
//     // Check if the action leads to a walkable square.
//     bool valid_action = true;
//     if (this->walkable_squares[new_pos%this->width][new_pos/this->width]
//             == true) {
//         for (auto box_n = 4; box_n < this->num_of_diamonds+4; ++box_n) {
//             int box_pos = this->current_state[box_n];
//             // Check is the box is in the new position of the player.
//             if (box_pos == new_pos) {
//                 // Check if there is a wall blocking the push.
//                 if (this->walkable_squares[further_pos%this->width]
//                         [further_pos/this->width] == true) {
//                     // Check if there is a second box blocking the push.
//                     for (auto box2_n = 4; box2_n < this->num_of_diamonds+4;
//                             ++box2_n) {
//                         int box2_pos = this->current_state[box2_n];
//                         if (box2_pos == further_pos){
//                             valid_action = false;
//                             break;
//                         }
//                     }
//                     // If there is a box, and can be pushed.
//                     if (valid_action == true) {
//                         next_state[box_n] = further_pos;
//                         cout << "Moving box to " << further_pos%this->width
//                                 << ", " << further_pos/this->width << "\n";
//                     }
                
//                 }
//                 // If the box is adjacent to a wall.
//                 else {
//                     valid_action = false;
//                 }
//                 break;
//             }
//         }
//     }
//     // If the movement is directed towards a wall.
//     else {
//         valid_action = false;
//     }
//     // Update the state of the puzzle, after checking it is not a repeated one. 
//     if (valid_action == true) {
//         next_state[2] = new_pos;
//         if (this->deadlock_state(next_state) == false) {
//             if (this->is_repeated_state(next_state) == false) {
//                 // Increase the state depth.
//                 next_state[0] += 1;
//                 // Set the parent index.
//                 next_state[1] = this->current_state_index;
//                 this->actions_counter += 1;
//                 this->valid_actions.push_back({4, 3, 2, 1});
//                 this->current_state = next_state;
//                 this->actions_hist.push_back(this->current_action);
//                 this->states_hist.push_back(next_state);
//                 this->current_state_index = this->states_hist.size()-1;
//                 cout << this->states_hist.size() << " States.\n";
//                 cout << "Moving to: " << new_pos%this->width << ", " <<
//                         this->width/new_pos << "\n\n";
//             }
//         } else {
//             cout << "Deadlock!\n";
//         }
//     }
//     return;
// }

/**
    Check if the input state has already been passed through.

    For 2 states to be equals, both the man/player and all of the
    boxes have to be in the same position.
*/
bool sokoban::SokobanPuzzle::is_repeated_state(vector <int> state) {
    bool repeated_state = false;
    for (auto state_h = 0; state_h < this->states_hist.size(); ++state_h)
    {
        bool all_boxes_match = true;
        // Check for states with the man/player at the same position.
        if (this->states_hist[state_h][2] == state[2]
            && this->states_hist[state_h][0] < state[0]) {
            // For every box in the current state, compare to the historical
            // states boxes.
            for (auto box_h = 4; box_h < this->num_of_diamonds+4; ++box_h) {
                bool box_match = false;
                for (auto box_c = 4; box_c < this->num_of_diamonds+4; ++box_c) {
                    if (this->states_hist[state_h][box_h] == state[box_c]) {
                        box_match = true;
                        break;
                    }
                }
                // If one of the boxes is not in the historical state, there is
                // no match, and the loop is exited.
                if (box_match == false) {
                    all_boxes_match = false;
                    break;
                }
            }
        } 
        else {
            all_boxes_match = false;
        }
        if (all_boxes_match == true) {
            repeated_state = true;
            cout << "Repeated state!!\n";
            break;
        }
    } 
    return repeated_state;
}

/**
    Check if the input state lead to a puzzle deadlock.
*/
bool sokoban::SokobanPuzzle::deadlock_state(vector <int> state) {
    bool deadlock = false;
    // Check the deadlock for every box in the current state.
    for (auto box_index = 4; box_index < this->num_of_diamonds+4; ++box_index) {
        int box_x = state[box_index]%this->width;
        int box_y = state[box_index]/this->width;
        bool walkable_N = this->walkable_squares[box_x][box_y-1];
        bool walkable_E = this->walkable_squares[box_x+1][box_y];
        bool walkable_S = this->walkable_squares[box_x][box_y+1];
        bool walkable_W = this->walkable_squares[box_x-1][box_y];
        // If box over a goal, skip the check.
        if (this->goal_squares[box_x][box_y] == true) {
            continue;
        }

        // 1st deadlock: Box in a corner, or between corners.
        if (this->deadlock_squares[box_x][box_y] == true) {
            cout << "Box in a corner deadlock!\n";
            deadlock = true;
            break;
        }

        // 2nd deadlock: Box in a simple West-to-East non-scape tunnel.
        if (!walkable_N && !walkable_S){
            int check_x = box_x;
            int check_y = box_y;
            bool keep_searching = true;
            bool no_scape = true;
            while (keep_searching) {
                check_x += 1;
                // Check if there is a box at the new checking position
                for (auto box_2 = 4; box_2 < this->num_of_diamonds+4; ++box_2) {
                    int box_2_x = state[box_2]%this->width;
                    int box_2_y = state[box_2]/this->width; 
                    if (check_x == box_2_x && check_y == box_2_y) {
                        keep_searching = false;
                        break;
                    }
                }
                if (keep_searching == false) {
                    break;
                }
                // If the checking square is a wall.
                if (this->walkable_squares[check_x][check_y] == false) {
                    keep_searching = false;
                }
                // If a goal is found
                else if (this->goal_squares[check_x][check_y] == true) {
                    no_scape = false;
                    keep_searching = false;
                }
                // If a scape route is found, towards north or south.
                else if (this->walkable_squares[check_x][check_y+1] == true
                    || this->walkable_squares[check_x][check_y-1] == true) {
                    no_scape = false;
                    keep_searching = false;
                }
            }
            if (no_scape == true) {
                cout << "West-to-East deadlock!\n";
                deadlock = true;
                break;
            }
            check_x = box_x;
            check_y = box_y;
            keep_searching = true;
            no_scape = true;
            while (keep_searching) {
                check_x -= 1;
                // Check if there is a box at the new checking position
                for (auto box_2 = 4; box_2 < this->num_of_diamonds+4; ++box_2) {
                    int box_2_x = state[box_2]%this->width;
                    int box_2_y = state[box_2]/this->width; 
                    if (check_x == box_2_x && check_y == box_2_y) {
                        keep_searching = false;
                        break;
                    }
                }
                if (keep_searching == false) {
                    break;
                }
                // If the checking square is a wall.
                if (this->walkable_squares[check_x][check_y] == false) {
                    keep_searching = false;
                }
                // If a goal is found
                else if (this->goal_squares[check_x][check_y] == true) {
                    no_scape = false;
                    keep_searching = false;
                }
                // If a scape route is found, towards north or south.
                else if (this->walkable_squares[check_x][check_y+1] == true
                    || this->walkable_squares[check_x][check_y-1] == true) {
                    no_scape = false;
                    keep_searching = false;
                }
            }
            if (no_scape == true) {
                cout << "East-to-West deadlock!\n";
                deadlock = true;
                break;
            }
        }

        // 2nd deadlock: Box in a simple North-to-South non-scape tunnel.
        if (!walkable_W && !walkable_E){
            int check_x = box_x;
            int check_y = box_y;
            bool keep_searching = true;
            bool no_scape = true;
            while (keep_searching) {
                check_y += 1;
                // Check if there is a box at the new checking position
                for (auto box_2 = 4; box_2 < this->num_of_diamonds+4; ++box_2) {
                    int box_2_x = state[box_2]%this->width;
                    int box_2_y = state[box_2]/this->width; 
                    if (check_x == box_2_x && check_y == box_2_y) {
                        keep_searching = false;
                        break;
                    }
                }
                if (keep_searching == false) {
                    break;
                }
                // If the checking square is a wall.
                if (this->walkable_squares[check_x][check_y] == false) {
                    keep_searching = false;
                }
                // If a goal is found
                else if (this->goal_squares[check_x][check_y] == true) {
                    no_scape = false;
                    keep_searching = false;
                }
                // If a scape route is found, towards north or south.
                else if (this->walkable_squares[check_x+1][check_y] == true
                    || this->walkable_squares[check_x-1][check_y] == true) {
                    no_scape = false;
                    keep_searching = false;
                }
            }
            if (no_scape == true) {
                cout << "North-to-South deadlock!\n";
                deadlock = true;
                break;
            }
            check_x = box_x;
            check_y = box_y;
            keep_searching = true;
            no_scape = true;
            while (keep_searching) {
                check_y -= 1;
                // Check if there is a box at the new checking position
                for (auto box_2 = 4; box_2 < this->num_of_diamonds+4; ++box_2) {
                    int box_2_x = state[box_2]%this->width;
                    int box_2_y = state[box_2]/this->width; 
                    if (check_x == box_2_x && check_y == box_2_y) {
                        keep_searching = false;
                        break;
                    }
                }
                if (keep_searching == false) {
                    break;
                }
                // If the checking square is a wall.
                if (this->walkable_squares[check_x][check_y] == false) {
                    keep_searching = false;
                }
                // If a goal is found
                else if (this->goal_squares[check_x][check_y] == true) {
                    no_scape = false;
                    keep_searching = false;
                }
                // If a scape route is found, towards north or south.
                else if (this->walkable_squares[check_x+1][check_y] == true
                    || this->walkable_squares[check_x-1][check_y] == true) {
                    no_scape = false;
                    keep_searching = false;
                }
            }
            if (no_scape == true) {
                cout << "South-to-North deadlock!\n";
                deadlock = true;
                break;
            }
        }

        // Check deadlocks consisting in at least 2 boxes.
        for (auto box_2 = 4; box_2 < this->num_of_diamonds+4; ++box_2) {
            int box_2_x = state[box_2]%this->width;
            int box_2_y = state[box_2]/this->width;
            // 2nd dedlock: 2 boxes in a row, blocked by 2 pieces of wall.
            if (abs(box_2_x-box_x)==1 && box_y==box_2_y
                || abs(box_2_y-box_y)==1 && box_x==box_2_x) {
                bool walkable_2_N = this->walkable_squares[box_2_x][box_2_y-1];
                bool walkable_2_E = this->walkable_squares[box_2_x+1][box_2_y];
                bool walkable_2_S = this->walkable_squares[box_2_x][box_2_y+1];
                bool walkable_2_W = this->walkable_squares[box_2_x-1][box_2_y];
                if (!walkable_N && !walkable_2_N || !walkable_E && !walkable_2_E
                    || !walkable_S && !walkable_2_S
                    || !walkable_W && !walkable_2_W) {
                    deadlock = true;
                    cout << "2 attached boxes deadlock!\n";
                    break;
                }
            }
        }
        if (deadlock == true) {
            break;
        }
    }
    return deadlock;
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
    for (auto box_index = 4; box_index < this->num_of_diamonds+4; ++box_index) {
        x_coord = this->current_state[box_index]%this->width;
        y_coord = this->current_state[box_index]/this->width;
        if (this->goal_squares[x_coord][y_coord] == false) {
            success = false;
            break;
        }
    }
    if (success == true) {
        cout << "SUCCESS!!!!!\n" << this->current_state[0] << " iterations\n";
        for(auto i = 0; i< this->actions_hist.size(); ++i) {
            cout << this->actions_hist[i] << " ";
        }
        cout << "\n";
    }
    if (this->actions_counter >= this->max_iterations) {
        success = true;
        // DEBUG
        cout << "End of iterations. Boxes at ... \n";
        for (auto box_index = 4; box_index < this->num_of_diamonds+4;
                ++box_index) {
            x_coord = this->current_state[box_index]%this->width;
            y_coord = this->current_state[box_index]/this->width;
            cout << x_coord << ", " << y_coord << "\n";
        }
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

/**
    Return true if the selected square is a deadlock.
*/
int sokoban::SokobanPuzzle::get_deadlock_square(int x_coord, int y_coord) {
    int value  = this->deadlock_squares[x_coord][y_coord];
    return value;
}

void sokoban::SokobanPuzzle::test() {
    this->get_free_squares();
    this->label_squares();
    cout << "\nFree squares:\n";
    for (auto y_coord = 0; y_coord < this->height; ++y_coord) {
        for (auto x_coord = 0; x_coord < this->width; ++x_coord) {
            cout << this->free_squares[x_coord][y_coord];
        }
        cout << "\n";
    }
    cout << "\nLabeled squares:\n";
    for (auto y_coord = 0; y_coord < this->height; ++y_coord) {
        for (auto x_coord = 0; x_coord < this->width; ++x_coord) {
            cout << this->labeled_squares[x_coord][y_coord];
        }
        cout << "\n";
    }
    cout << "Player position " << current_state[0]%this->width << ", " <<
            current_state[0]/this->width << "\n";
    cout << "Boxes positions:\n";
    for(auto index = 4; index < this->num_of_diamonds+4; ++index){
        cout << this->current_state[index]%this->width << ", " <<
                this->current_state[index]/this->width << "\n";
    }
}
