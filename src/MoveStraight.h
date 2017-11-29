motorStruct MoveStraight(sensorBoolStruct inputSensors, int state, speedStruct speed, compStruct comp, int nextMovement){
    motorStruct returnMovement;
	
	bool outOfCross = !inputSensors.middle || !inputSensors.left; //Out of cross
	bool onTrack = 	  !inputSensors.middle && !inputSensors.left; //On track
	bool compRight =  !inputSensors.middle &&  inputSensors.left; //Compensate from right
	bool compLeft =    inputSensors.middle && !inputSensors.left; //Compensate from left
	bool cross = 	   inputSensors.middle &&  inputSensors.left; //Cross
	bool slowDown =    inputSensors.right &&  (nextMovement != 1); //Cross
	

	if (slowDown){
		state = 60; 
	}	
	else{
		state = state;
	}
			
    if (state == 0){//On cross when start
       if (outOfCross){
          returnMovement.state = 10;
       }
       else{
		   returnMovement.state = 0;
       }
       
       returnMovement.speed = speed.slow ;
       returnMovement.turn = 0;
    }
    else if (state == 10){ //On track
       if (compLeft) 
               returnMovement.state = 20;
       else if (compRight)
               returnMovement.state = 30;
       else if (cross)
               returnMovement.state = 99;
		else
			returnMovement.state = 10;
               
       returnMovement.speed = speed.forwards;
       returnMovement.turn = 0;
    }
    else if (state == 20){ //Compensate from left
       if (onTrack)
               returnMovement.state = 10;
       else if (compRight)
               returnMovement.state = 30;
       else if (cross)
               returnMovement.state = 99;
		else
			returnMovement.state = 20;
			   
               
       returnMovement.speed = comp.speedForwards;
       returnMovement.turn = -comp.compForwards;
    }
    else if (state == 30){ //Compensate from right
       if (onTrack)
               returnMovement.state = 10;
       else if (compLeft)
               returnMovement.state = 20;
       else if (cross)
               returnMovement.state = 99;
		else
			returnMovement.state = 30;
               
       returnMovement.speed = comp.speedForwards;
       returnMovement.turn = comp.compForwards;
    }
    else if (state == 60){ //On track
       if (compLeft) 
               returnMovement.state = 70;
       else if (compRight)
               returnMovement.state = 80;
       else if (cross)
               returnMovement.state = 99;
		else
			returnMovement.state = 60;
               
       returnMovement.speed = speed.slow;
       returnMovement.turn = 0;
    }
    else if (state == 70){ //Compensate from left
       if (onTrack)
               returnMovement.state = 60;
       else if (compRight)
               returnMovement.state = 80;
       else if (cross)
               returnMovement.state = 99;
		else
			returnMovement.state = 70;
			   
       returnMovement.speed = comp.speedSlow;
       returnMovement.turn = -comp.compForwards;
    }
    else if (state == 80){ //Compensate from right
       if (onTrack)
               returnMovement.state = 60;
       else if (compLeft)
               returnMovement.state = 70;
       else if (cross)
               returnMovement.state = 99;
		else
			returnMovement.state = 80;
               
       returnMovement.speed = comp.speedSlow;
       returnMovement.turn = comp.compForwards;
    }
    else if (nextMovement != 1){ //Stop at cross
    	if (slowDown){
			returnMovement.speed = 0;
			returnMovement.turn = 0;
		}	
		else{
			returnMovement.speed = speed.slow;
			returnMovement.turn = 0;
		}
	
	   returnMovement.state = 99;
    }

	

    return returnMovement;
};
