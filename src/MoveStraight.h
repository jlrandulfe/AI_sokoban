motorStruct MoveStraight(sensorBoolStruct inputSensors, bool move){
    char Compensation = 20;
    char Speed = 20;
    int x;
    motorStruct returnMovement;
    returnMovement.done = 0;

    if (!inputSensors.middle          //On track
       && !inputSensors.left)
       x = 10;
    else if (inputSensors.middle      //Compensate from left
       && !inputSensors.left)
       x = 20;
    else if (!inputSensors.middle     //Compensate from right
       && inputSensors.left)
       x = 30;
    else if (inputSensors.middle      //Stop at cross
       && inputSensors.left)
       x = 50;
    else                              //Error
       x = 99;

        if (x==10){ //Go straight
             returnMovement.speed = Speed;
             returnMovement.turn = 0;
             returnMovement.direction = 1;
             returnMovement.done = 0;
        }

        else if (x==20){ //Compensate from left
             returnMovement.speed = Speed;
             returnMovement.turn = -Compensation;
             returnMovement.direction = 1;
             returnMovement.done = 0;
        }

        else if (x==30){ //Compensate from right
             returnMovement.speed = Speed;
             returnMovement.turn = Compensation;
             returnMovement.direction = 1;
             returnMovement.done = 0;
        }

        else if (x==50){ //Cross
             returnMovement.speed = 0;
             returnMovement.turn = 0;
             returnMovement.direction = 1;
             returnMovement.done = 1;
        }

        else { //Error -> don't move
             returnMovement.speed = 0;
             returnMovement.turn = 0;
             returnMovement.direction = 1;
             returnMovement.done = 0;
             x=99;
         }


         // Print sensors values to screen.
         TextOut(10, LCD_LINE1, StrCat(" L ", NumToStr(inputSensors.left)));
         TextOut(10, LCD_LINE2, StrCat(" M ", NumToStr(inputSensors.middle)));
         TextOut(10, LCD_LINE3, StrCat(" R ", NumToStr(inputSensors.right)));
         TextOut(10, LCD_LINE4, StrCat(" Move state: ", NumToStr(x)));
    return returnMovement;
};
