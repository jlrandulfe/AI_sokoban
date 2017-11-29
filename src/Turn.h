motorStruct turn(sensorBoolStruct inputSensors, int state, speedStruct inputSpeed, int kind){
    motorStruct returnMovement;
	bool sensor;
	int speed;
	//kind: 2 - right, 3 - left, 4 - 180
	
	
	if (kind == 2){
		sensor = inputSensors.middle;
		speed = inputSpeed.turn90;
	}
	else if (kind == 3){
		sensor = inputSensors.left;
		speed = inputSpeed.turn90;
	}
	else{
		sensor = inputSensors.middle;
		speed = inputSpeed.turn180;
	}
	

    if (state == 0){ // First turning state
         if (!sensor){
            returnMovement.state = 10;
            }
         else{
              returnMovement.state = 0;
         }

         returnMovement.speed = speed;
    }
    else if (state == 10){ //Second turning state
         if (sensor){
            returnMovement.state = 20;
         }
         else{
              returnMovement.state = 10;
         }

         returnMovement.speed = speed;
    }
	
	else if (state == 20){ //Third turning state
         if (!sensor){
            returnMovement.state = 99;
         }
         else{
              returnMovement.state = 20;
         }

         returnMovement.speed = speed;
    }


    else if (state == 99){ //Done turning state
         returnMovement.speed = 0;
         returnMovement.state = 99;
    }

    return returnMovement;

};

