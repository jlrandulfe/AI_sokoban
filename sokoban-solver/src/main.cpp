#include <iostream>
#include <fstream>
#include <stdio.h> 
#include <stdlib.h>
#include <string>

using namespace std;


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

    // Read the map parameters, at its header.
    int map_width;
    int map_height;
    int num_of_diamonds;
    map_file >> map_width;
    map_file >> map_height;
    map_file >> num_of_diamonds;
    cout << "read back from file: \n" << map_width << " " << map_height 
            << " " << num_of_diamonds << '\n';

    bool walkable_squares[map_width][map_height];
    bool diamond_squares[map_width][map_height];
    bool goal_squares[map_width][map_height];
    // Go over the whole map file and build the map structure.
    // Get the characters including whitespaces (the >> operator ignores them).
    char map_item;
    map_file.get();
    for (auto row = 0; row < map_height; ++row){
        for (auto col = 0; col < map_width; ++col){
            map_item = map_file.get();
            if (map_item == '.'){
                walkable_squares[row][col] = true;
                diamond_squares[row][col] = false;
                goal_squares[row][col] = false;
            }
            else if (map_item == 'J'){
                walkable_squares[row][col] = true;
                diamond_squares[row][col] = true;
                goal_squares[row][col] = false;
            }
            else if (map_item == 'G'){
                walkable_squares[row][col] = true;
                diamond_squares[row][col] = false;
                goal_squares[row][col] = true;
            }
            else if (map_item == 'M'){
                walkable_squares[row][col] = true;
                diamond_squares[row][col] = false;
                goal_squares[row][col] = false;
            }
            else{
                walkable_squares[row][col] = false;
                diamond_squares[row][col] = false;
                goal_squares[row][col] = false;
            }
            cout << diamond_squares[row][col];
        }
        // Ignore the 'end of line' character.
        map_file.get();
        cout << "\n";
    }
    cout << "\n";
    return 0;
}
