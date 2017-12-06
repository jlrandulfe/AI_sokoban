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
