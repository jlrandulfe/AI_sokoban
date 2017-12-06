motorStruct turnLeft(sensorBoolStruct inputSensors, int state, char speed, char speedSlow){
    motorStruct returnMovement;

    if (state == 0){ // First turning state
         if (!inputSensors.left){
            returnMovement.state = 10;
            }
         else{
              returnMovement.state = 0;
         }

         returnMovement.speed = speed;
    }
    else if (state == 10){ //Second turning state
         if (inputSensors.left){
            returnMovement.state = 20;
         }
         else{
              returnMovement.state = 10;
         }

         returnMovement.speed = speed;
    }
    else if (state == 20){ //Third turning state
         if (!inputSensors.left){
            returnMovement.state = 99;
         }
         else{
              returnMovement.state = 20;
         }

         returnMovement.speed = speedSlow;
    }

    else if (state == 99){ //Done turning state
         returnMovement.speed = 0;
         returnMovement.state = 99;
    }

    return returnMovement;

};

