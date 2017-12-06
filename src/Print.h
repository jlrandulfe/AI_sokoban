void print(int i, int path[], sensorBoolStruct sensors, motorStruct motor){
        if (path[i] == 99){                             // Done
           OnRev(OUT_A, 0);
           OnRev(OUT_C, 0);
            TextOut(10, LCD_LINE1, StrCat("PATH DONE"));
            TextOut(10, LCD_LINE2, StrCat("                 "));
            TextOut(10, LCD_LINE3, StrCat("                 "));
            TextOut(10, LCD_LINE4, StrCat("                 "));
            TextOut(10, LCD_LINE5, StrCat("                 "));
           }
        else{
        TextOut(10, LCD_LINE1, StrCat(" L ", NumToStr(sensors.left), "         "));
        TextOut(10, LCD_LINE2, StrCat(" M ", NumToStr(sensors.middle), "         "));
        TextOut(10, LCD_LINE3, StrCat(" R ", NumToStr(sensors.right), "         "));
        TextOut(10, LCD_LINE4, StrCat("state: ", NumToStr(motor.state), "              "));
        TextOut(10, LCD_LINE5, StrCat("speed: ", NumToStr(motor.speed), "              "));
        }
        

        TextOut(10, LCD_LINE6, StrCat("                 "));
        TextOut(10, LCD_LINE7, StrCat("i: ", NumToStr(i), "              "));
        TextOut(10, LCD_LINE8, StrCat("path: ", NumToStr(path[i]), "              "));
};
