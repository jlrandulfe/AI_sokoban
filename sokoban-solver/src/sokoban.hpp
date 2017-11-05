// Standard libraries
#include <vector>

using namespace std;

namespace sokoban {
    class SokobanPuzzle {
    private:
        // Map constraints.
        int num_of_diamonds;
        int width;
        int height;
        // 2D (width x height) bool maps for walkable and goal squares.
        vector< vector<bool> > walkable_squares;
        vector< vector<bool> > goal_squares;
        // Variable for storing the actual state of the puzzle.
        vector < vector<int> > current_state;

    public:
        // Constructor
        SokobanPuzzle(int diamonds, int width, int height);
        // Destructor
        ~SokobanPuzzle(){};

        void set_walkable_square(int row, int col, bool value);
        void set_goal_square(int row, int col, bool value);
        void update_player_position(int x_coord, int y_coord);
        void update_box_position(int box_index, int x_coord, int y_coord);
        int get_diamonds();
        void test();
    };
}
