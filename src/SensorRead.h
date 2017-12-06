sensorBoolStruct sensor_read(sensorIntStruct thresholds){
    sensorIntStruct sensor_raw;             // Variables where the raw value of the sensors will be stored.
    sensorBoolStruct sensor_bool;      // Values defining the thresholds of the 3 sensors limits

    // Get the value of the 3 infrared sensors.
        sensor_raw.left = Sensor(IN_1);
        sensor_raw.middle = Sensor(IN_2);
        sensor_raw.right = Sensor(IN_3);

        if (sensor_raw.middle > thresholds.middle)
            sensor_bool.middle = 0;
        else
            sensor_bool.middle = 1;

        if (sensor_raw.right > thresholds.right)
            sensor_bool.right = 0; //black
        else
            sensor_bool.right = 1; // white

        if (sensor_raw.left > thresholds.left)
            sensor_bool.left = 0;
        else
            sensor_bool.left = 1;


        return sensor_bool;
};
