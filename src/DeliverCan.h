motorStruct DeliverCan(sensorBoolStruct inputSensors, int state, speedStruct speed, compStruct comp){
//char  speed, char  speedComp, char  speedBack, char speedCompBack, char comp, char compBack){
    motorStruct returnMovement;
	
	bool onTrack = 	  !inputSensors.middle && !inputSensors.left; //On track
	bool compRight =  !inputSensors.middle &&  inputSensors.left; //Compensate from right
	bool compLeft =    inputSensors.middle && !inputSensors.left; //Compensate from left
	bool placed = 	   inputSensors.right;						  //Can placed
	bool cross = 	   inputSensors.middle &&  inputSensors.left; //Cross

    if (state == 0){//On cross when start
       if (onTrack){
          returnMovement.state = 10;
       }
       returnMovement.speed =  speed.slow;
       returnMovement.turn = 0;
    }
    else if (state == 10){ //On track
       if (compLeft)
               returnMovement.state = 20;
       else if (compRight)
               returnMovement.state = 30;
       else if (placed)      //Stop to deliver
               returnMovement.state = 60;
               
       returnMovement.speed =  speed.deliver;
       returnMovement.turn = 0;
    }
    else if (state == 20){ //Compensate from left
       if (onTrack)
               returnMovement.state = 10;
       else if (compRight)
               returnMovement.state = 30;
       else if (placed)      //Stop to deliver
               returnMovement.state = 60;
               
       returnMovement.speed =  comp.speedDeliver;
       returnMovement.turn = -comp.compDeliver;
    }
    else if (state == 30){ //Compensate from right
       if (onTrack)
               returnMovement.state = 10;
       else if (compLeft)
               returnMovement.state = 20;
       else if (placed)      //Stop to deliver
               returnMovement.state = 60;
               
       returnMovement.speed =  -comp.speedDeliver;
       returnMovement.turn = comp.compDeliver;
    }	
	
	// backwards
	
    else if (state == 60){ //On track
       if (compLeft)
               returnMovement.state = 70;
       else if (compRight)
               returnMovement.state = 80;
       else if (cross)
               returnMovement.state = 99;
		else
			returnMovement.state = 60;
               
       returnMovement.speed = speed.backwards;
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
			   
               
       returnMovement.speed = comp.speedBackwards;
       returnMovement.turn = -comp.compBackwards;
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
               
       returnMovement.speed = comp.speedBackwards;
       returnMovement.turn = comp.compBackwards;
    }
    else if (state == 99){ //Stop at cross
    
       returnMovement.speed = 0;
       returnMovement.turn = 0;
	   returnMovement.state = 99;
    }

   

    return returnMovement;
};
