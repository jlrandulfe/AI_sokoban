

// Function to see if the sensors detecting blak or white
void sensorsWB(int sensor_raw, int sensor_thresholds,  bool &WhiteBlack){
/* sensor_raw= sensor_input
   sensor_thresholds= threshold of the sensor
   Whitblack = bolean outcome to se if sensor detects black or white  */
  if(sensor_raw> sensor_thresholds)
{
 WhiteBlack=true;
}
if (sensor_raw <= sensor_thresholds)
{
 WhiteBlack=false;
}
}
