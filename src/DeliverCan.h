motorStruct DeliverCan(sensorBoolStruct inputSensors, int state, char  speed, char comp){
    motorStruct returnMovement;
	
	bool onTrack = 	  !inputSensors.middle && !inputSensors.left; //On track
	bool compRight =  !inputSensors.middle &&  inputSensors.left; //Compensate from right
	bool compLeft =    inputSensors.middle && !inputSensors.left; //Compensate from left
	bool placed = 	   inputSensors.right;						  //Can placed
	bool cross = 	   inputSensors.middle &&  inputSensors.left; //Cross
            

    if (state == 0){//On cross when start
       if (!inputSensors.middle && !inputSensors.left){      //Drive until out of cross
          returnMovement.state = 10;
       }
		else{
			returnMovement.state = 0;
		}
	
       returnMovement.speed =  speed;
       returnMovement.turn = 0;
    }
    else if (state == 10){ //On track
       if (compLeft)          //Compensate from left
            returnMovement.state = 20;
       else if (compRight)
            returnMovement.state = 30;
       else if (placed)      //Stop to deliver
            returnMovement.state = 60;
		else{
			returnMovement.state = 10;
		}
               
       returnMovement.speed =  speed;
       returnMovement.turn = 0;
    }
    else if (state == 20){ //Compensate from left
       if (!inputSensors.middle && !inputSensors.left)	//On track
               returnMovement.state = 10;
       else if (compRight)
               returnMovement.state = 30;
       else if (placed)      //Stop to deliver
               returnMovement.state = 60;
		else{
		returnMovement.state = 20;
		}
               
       returnMovement.speed =  speed;
       returnMovement.turn = -comp;
    }
    else if (state == 30){ //Compensate from right
       if (!inputSensors.middle && !inputSensors.left)	//On track
               returnMovement.state = 10;
       else if (compLeft)
               returnMovement.state = 20;
       else if (placed)      //Stop to deliver
               returnMovement.state = 60;
		else{
		returnMovement.state = 30;
		}
               
       returnMovement.speed =  speed;
       returnMovement.turn = comp;
    }
	
	// backwards
	
    else if (state == 60){ //On track
       if (cross)
               returnMovement.state = 99;
		else{
			returnMovement.state = 60;
		}
               
       returnMovement.speed = speed;
       returnMovement.turn = 0;
    }
    else if (state == 99){ //Stop at cross
    
       returnMovement.speed = 0;
       returnMovement.turn = 0;
	   returnMovement.state = 99;
    }
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
	
    else if (state == 99){ //Stop at cross
    
       returnMovement.speed = 0;
       returnMovement.turn = 0;
    }

    return returnMovement;
};
