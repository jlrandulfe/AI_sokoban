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
        // Possible actions that can be taken at the current state.
        // 1=North, 2=East, 3=South, 4=West
        vector <int> valid_actions;
        int current_action;

    public:
        // Constructor
        SokobanPuzzle(int diamonds, int width, int height);
        // Destructor
        ~SokobanPuzzle(){};

        void set_walkable_square(int x_coord, int y_coord, bool value);
        void set_goal_square(int x_coord, int y_coord, bool value);
        void update_player_position(int x_coord, int y_coord);
        void update_box_position(int box_index, int x_coord, int y_coord);
        int new_action();
        bool goal_reached();
        int get_diamonds();
        int get_walkable_square(int x_coord, int y_coord);
        void test();
    };
}
