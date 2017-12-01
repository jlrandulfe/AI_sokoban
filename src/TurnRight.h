motorStruct turn(sensorBoolStruct inputSensors, int state, char speed){
    motorStruct returnMovement;


    if (state == 0){ // First turning state
         if (!inputSensors.middle){
            returnMovement.state = 10;
            }
         else{
              returnMovement.state = 0;
         }

         returnMovement.speed = speed;
    }
    else if (state == 10){ //Second turning state
         if (inputSensors.middle){
            returnMovement.state = 20;
         }
         else{
              returnMovement.state = 10;
         }

         returnMovement.speed = speed;
    }
    else if (state == 20){ //Third turning state
         if (!inputSensors.middle){
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

