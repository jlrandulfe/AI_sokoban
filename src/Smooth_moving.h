


void sensorSmooth(int sensor_raw_right,int sensor_raw_left, &float turn){
	int SensorMin_R=16;
	int SensorMax_R=40;
	int SensorMin_L=22;
	int SensorMax_L=48;
	int SensorRange_L=SensorMax_L-SensorMin_L;
	int SensorRange_R=SensorMax_R-SensorMin_R;
	float pct_speed_right=0;
	float pct_speed_left=0;
	float pos_pct_turn=100/100;
	float neg_pct_turn=100/100;
	float sensorpct_right=(100/SensorRange);
	float sensorpct_left=(100/SensorRange);
	float sensor_raw_right_pct=((sensor_raw_right-SensorMin_R)*sensorpct_right;
	float sensor_raw_left_pct=((sensor_raw_left-SensorMin_L)*sensorpct_left;
	if(sensor_raw_right_pct>100){
	  pct_speed_right=100;
	}
	else if(sensor_raw_right_pct<0){
	  pct_speed_right=0;
	}
	else{
	  pct_speed_right=sensor_raw_right_pct;
	}
	if(sensor_raw_left_pct>100){
	  pct_speed_left=100;
	}
	else if(sensor_raw_left_pct<0){
	  pct_speed_left=0;
	}
	else{
	  pct_speed_left=sensor_raw_left_pct;
	}
	
	float diff_pct=pct_speed_right-pct_speed_right;
	float comb_pct=pct_speed_right+pct_speed_right;
	if (diff_pct<0){
		turn=diff_pct*neg_pct_turn;
	}
	if (diff_pct>0){
		turn=diff_pct*pos_pct_turn;
	}
	if (diff_pct==0){
		turn=0;
	}
		
		
}
  	  

 
