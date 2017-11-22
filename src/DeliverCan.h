motorStruct DeliverCan(sensorBoolStruct inputSensors, int state, char  speed, char comp){
    motorStruct returnMovement;

    if (state == 0){//On cross when start
       if (!inputSensors.middle      //Drive until out of cross
       && !inputSensors.left){
          returnMovement.state = 10;
       }
       returnMovement.speed =  speed;
       returnMovement.turn = 0;
    }
    else if (state == 10){ //On track
       if (inputSensors.middle           //Compensate from left
            && !inputSensors.left)
               returnMovement.state = 20;
       else if (!inputSensors.middle     //Compensate from right
            && inputSensors.left)
               returnMovement.state = 30;
       else if (inputSensors.right)      //Stop to deliver
               returnMovement.state = 99;
               
       returnMovement.speed =  speed;
       returnMovement.turn = 0;
    }
    else if (state == 20){ //Compensate from left
       if (!inputSensors.middle          //On track
            && !inputSensors.left)
               returnMovement.state = 10;
       else if (!inputSensors.middle     //Compensate from right
            && inputSensors.left)
               returnMovement.state = 30;
       else if (inputSensors.right)      //Stop to deliver
               returnMovement.state = 99;
               
       returnMovement.speed =  speed;
       returnMovement.turn = -comp;
    }
    else if (state == 30){ //Compensate from right
       if (!inputSensors.middle          //On track
            && !inputSensors.left)
               returnMovement.state = 10;
       else if (inputSensors.middle      //Compensate from left
            && !inputSensors.left)
               returnMovement.state = 20;
       else if (inputSensors.right)      //Stop to deliver
               returnMovement.state = 99;
               
       returnMovement.speed =  speed;
       returnMovement.turn = comp;
    }
    else if (state == 99){ //Stop at cross
    
       returnMovement.speed = 0;
       returnMovement.turn = 0;
    }

    return returnMovement;
};
