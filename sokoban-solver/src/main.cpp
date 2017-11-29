// Standard libraries
#include <chrono>
#include <fstream>
#include <iostream>
#include <stdio.h> 
#include <stdlib.h>
#include <string>
// Local libraries
#include "sokoban.hpp"

using namespace std;


/**
    Create and construct an object of SokobanPuzzle with the text file

    the text file must contain a header with the number of rows, cols,
    and number of diamonds of the puzzle. It must also contain the
    representation of the map.

    With the given information, an instance of SokobanPuzzle is
    created and constructed.
**/
sokoban::SokobanPuzzle read_puzzle_file(fstream &map_file){
    // Read the map parameters, at its header.
    int map_width;
    int map_height;
    int num_of_diamonds;
    map_file >> map_width;
    map_file >> map_height;
    map_file >> num_of_diamonds;
    cout << "Sokoban puzzle: \n" << map_width << " " << map_height 
            << " " << num_of_diamonds << '\n';

    // Instance SokobanPuzzle
    sokoban::SokobanPuzzle puzzle(num_of_diamonds, map_width, map_height);

    bool diamond_squares[map_width][map_height];
    // Go over the whole map file and build the map structure.
    // Get the characters including whitespaces (the >> operator ignores them).
    int box_index = 0;
    char map_item;
    map_file.get();
    for (auto y_coord = 0; y_coord < map_height; ++y_coord){
        for (auto x_coord = 0; x_coord < map_width; ++x_coord){
            map_item = map_file.get();
            if (map_item == '.'){
                puzzle.set_walkable_square(x_coord, y_coord, true);
                puzzle.set_goal_square(x_coord, y_coord, false);
            }
            else if (map_item == 'J'){
                puzzle.set_walkable_square(x_coord, y_coord, true);
                puzzle.set_goal_square(x_coord, y_coord, false);
                puzzle.update_box_position(box_index, x_coord, y_coord);
                box_index += 1;
            }
            else if (map_item == 'G'){
                puzzle.set_walkable_square(x_coord, y_coord, true);
                puzzle.set_goal_square(x_coord, y_coord, true);
            }
            else if (map_item == 'M'){
                puzzle.set_walkable_square(x_coord, y_coord, true);
                puzzle.set_goal_square(x_coord, y_coord, false);
                puzzle.update_player_position(x_coord, y_coord);
            }
            else{
                puzzle.set_walkable_square(x_coord, y_coord, false);
                puzzle.set_goal_square(x_coord, y_coord, false);
            }
            cout << puzzle.get_walkable_square(x_coord, y_coord);
        }
        // Instruction for ignoring the 'end of line' character.
        map_file.get();
        cout << "\n";
    }
    cout << "\nDeadlocks:\n";
    puzzle.get_deadlock_positions();
    for (auto y_coord = 0; y_coord < map_height; ++y_coord) {
        for (auto x_coord = 0; x_coord < map_width; ++x_coord) {
            cout << puzzle.get_deadlock_square(x_coord, y_coord);
        }
        cout << "\n";
    }
    return puzzle;
}

int main(int argc, char** argv)
{
    // Get the file name from the first argument.
    string filename = argv[1];
    // Open the file in read mode and check no errors
    fstream map_file(filename.c_str(), ios_base::in);
    if (!map_file.is_open()) {
        cout << "failed to open " << filename << '\n';
        return EXIT_FAILURE;
    }

    // Start measuring time, and initialize the puzzle.
    chrono::high_resolution_clock::time_point t_i =
            chrono::high_resolution_clock::now();
    sokoban::SokobanPuzzle puzzle = read_puzzle_file(map_file);

    puzzle.test();

    // Main solver loop
    bool success = false;
    // while (success == false){
    //     puzzle.new_action();
    //     puzzle.move_player();
    //     success = puzzle.goal_reached();
    //     // success = true;     // TEST instruction.
    // }

    // Stop measuring time.
    chrono::high_resolution_clock::time_point t_f =
            chrono::high_resolution_clock::now();
    auto t_tot = chrono::duration_cast<chrono::milliseconds>(t_f - t_i).count();
    
    cout << "Success: " << success << "\n";
    cout << "Elapsed time: " << t_tot << " milliseconds.\n";
    return 0;
}
