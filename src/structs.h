struct motorStruct {
char speed;             // 0%-100% Speed
char turn;              // -100% - 100% Turn
bool direction;         // 1: Fwd 0: Rev
int state;              // Store pre state
};

struct sensorBoolStruct {
bool middle;
bool right;
bool left;
};

struct sensorIntStruct {
int middle;
int right;
int left;
};

struct speedStruct {
char forwards;
char slow;
char turn90;
char turn180;
char deliver;
char backwards;
};

struct compStruct {
char speedForwards;
char speedSlow;
char speedDeliver;
char speedBackwards;
char compForwards;
char compSlow;
char compDeliver;
char compBackwards;
};