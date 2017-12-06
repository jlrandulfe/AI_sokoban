motorStruct MoveStraight(sensorBoolStruct inputSensors, int state, char speed, char speedSlow, char comp, int nextPath){
    motorStruct returnMovement;

    if (state == 0){//On cross when start
       if (!inputSensors.middle      //Drive until out of cross
       || !inputSensors.left){
          returnMovement.state = 10;
       }
       else{
		   returnMovement.state = 0;
       }
       
       returnMovement.speed = speed;
       returnMovement.turn = 0;
    }
    else if (state == 10){ //On track
       if (inputSensors.middle           //Compensate from left
            && !inputSensors.left)
               returnMovement.state = 20;
       else if (!inputSensors.middle     //Compensate from right
            && inputSensors.left)
               returnMovement.state = 30;
       else if (inputSensors.right)		// Near
               returnMovement.state = 60;
		else
			returnMovement.state = 10;
               
       returnMovement.speed = speed;
       returnMovement.turn = 0;
    }
    else if (state == 20){ //Compensate from left
       if (!inputSensors.middle          //On track
            && !inputSensors.left)
               returnMovement.state = 10;
       else if (!inputSensors.middle     //Compensate from right
            && inputSensors.left)
               returnMovement.state = 30;
       else if (inputSensors.right)		//near
               returnMovement.state = 60;
		else
			returnMovement.state = 20;
			   
               
       returnMovement.speed = speed;
       returnMovement.turn = -comp;
    }
    else if (state == 30){ //Compensate from right
       if (!inputSensors.middle          //On track
            && !inputSensors.left)
               returnMovement.state = 10;
       else if (inputSensors.middle      //Compensate from left
            && !inputSensors.left)
               returnMovement.state = 20;
       else if (inputSensors.right)      //Near
               returnMovement.state = 60;
		else
			returnMovement.state = 30;
               
       returnMovement.speed = speed;
       returnMovement.turn = comp;
    }
	
	// ------------ Near to the cross ---------------
    else if (state == 60){ //On track
       if (inputSensors.middle           //Compensate from left
            && !inputSensors.left)
               returnMovement.state = 70;
       else if (!inputSensors.middle     //Compensate from right
            && inputSensors.left)
               returnMovement.state = 80;
       else if (inputSensors.middle      //Stop at cross
            && inputSensors.left)
               returnMovement.state = 99;
		else
			returnMovement.state = 60;
        if (nextPath == 1){
			returnMovement.speed = speed;
		}
		else{
			returnMovement.speed = speedSlow;
		}
       returnMovement.turn = 0;
    }
    else if (state == 70){ //Compensate from left
       if (!inputSensors.middle          //On track
            && !inputSensors.left)
               returnMovement.state = 60;
       else if (!inputSensors.middle     //Compensate from right
            && inputSensors.left)
               returnMovement.state = 80;
       else if (inputSensors.middle      //Stop at cross
            && inputSensors.left)
               returnMovement.state = 99;
		else
			returnMovement.state = 70;
			   
               
		if (nextPath == 1){
			returnMovement.speed = speed;
		}
		else{
			returnMovement.speed = speedSlow;
		}
       returnMovement.turn = -comp;
    }
    else if (state == 80){ //Compensate from right
       if (!inputSensors.middle          //On track
            && !inputSensors.left)
               returnMovement.state = 60;
       else if (inputSensors.middle      //Compensate from left
            && !inputSensors.left)
               returnMovement.state = 70;
       else if (inputSensors.middle      //Stop at cross
            && inputSensors.left)
               returnMovement.state = 99;
		else
			returnMovement.state = 80;
       
	   
	   
	   if (nextPath == 1){
			returnMovement.speed = speed;
		}
		else{
			returnMovement.speed = speedSlow;
		}
       returnMovement.turn = comp;
    }
	
	
	
	
    else if (state == 99){ //Stop at cross
    
       returnMovement.speed = 0;
       returnMovement.turn = 0;
	   returnMovement.state = 99;
    }

    return returnMovement;
};
