#include <vector>

using namespace std;

namespace sokoban {
    class SokobanPuzzle {
    private:
        // Map constraints.
        int num_of_diamonds;
        int width;
        int height;
        vector< vector<bool> > walkable_squares;
        vector< vector<bool> > goal_squares;
    public:
        // Constructor
        SokobanPuzzle(int diamonds, int width, int height);
        // Destructor
        ~SokobanPuzzle(){};

        void set_walkable_square(int row, int col, bool value);
        void set_goal_square(int row, int col, bool value);
        int get_diamonds();
        void test();
    };
}
