motorStruct turn(sensorBoolStruct inputSensors, bool move){
    motorStruct returnMovement;
    char speed = 50;
    int turning_state;
    int x;
    returnMovement.done = 0;


    if (turning_state == 0){ // First turning state
         x = 10;
         if (!inputSensors.left){
            turning_state = 1;
         }
    }
    else if (turning_state == 1){ //Second turning state
         x = 10;
         if (inputSensors.left){
            turning_state = 2;
         }
    }
    else if (turning_state == 2){ //Third turning state
         x = 10;
         if (!inputSensors.left){
            turning_state = 3;
         }
    }
    else if (turning_state == 3){ //Fourth turning state
         x = 10;
         if (inputSensors.left){
            turning_state = 4;
         }
    }
    else if (turning_state == 4){ //Done turning state
         x = 20;
    }


    if (x==10){ //Turning left
         returnMovement.speed = speed;
         returnMovement.done = 0;
    }
    else if (x==20){ //Turning right
         returnMovement.done = 1;
         returnMovement.speed = 0;
    }

    // Print sensors values to screen.
         TextOut(10, LCD_LINE1, StrCat(" L ", NumToStr(inputSensors.left)));
         TextOut(10, LCD_LINE2, StrCat(" M ", NumToStr(inputSensors.middle)));
         TextOut(10, LCD_LINE3, StrCat(" R ", NumToStr(inputSensors.right)));
         TextOut(10, LCD_LINE4, StrCat("turning_state: ", NumToStr(turning_state)));
    return returnMovement;

};
