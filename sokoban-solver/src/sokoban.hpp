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
        int depth;
        int max_depth;
        int iterations;
        // 2D (width x height) bool maps for walkable and goal squares.
        vector< vector<bool> > walkable_squares;
        vector< vector<bool> > goal_squares;
        vector < vector<bool> > deadlock_squares;
        // Variable for storing the actual state of the puzzle.
        vector < vector<int> > current_state;
        vector < vector < vector<int> > > states_hist;
        vector <int> parents_hist;
        int current_state_index;
        // Possible actions that can be taken at the current state.
        // 1=North, 2=East, 3=South, 4=West
        vector <int> valid_actions;
        int current_action;
        int actions_counter;

    public:
        // Constructor
        SokobanPuzzle(int diamonds, int width, int height);
        // Destructor
        ~SokobanPuzzle(){};

        void set_walkable_square(int x_coord, int y_coord, bool value);
        void set_goal_square(int x_coord, int y_coord, bool value);
        void update_player_position(int x_coord, int y_coord);
        void update_box_position(int box_index, int x_coord, int y_coord);
        void get_deadlock_positions();
        int new_action();
        void move_player();
        bool is_repeated_state(vector < vector<int> > state);
        bool deadlock_state(vector < vector<int> > state);
        bool goal_reached();
        int get_diamonds();
        int get_walkable_square(int x_coord, int y_coord);
        int get_deadlock_square(int x_coord, int y_coord);
        void test();
    };
}
