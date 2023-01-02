/*
DS3231_set.pde
Eric Ayars
4/11

Test of set-time routines for a DS3231 RTC

*/
#include <Arduino.h>
/*
DS3231_set.pde
Eric Ayars
4/11

Test of set-time routines for a DS3231 RTC

*/

#include <Arduino.h>
#include <DS3231.h>
#include <Wire.h>

DS3231 clock1;

byte year;
byte month;
byte date;
byte dOW;
byte hour;
byte minute;
byte second;

// YYMMDDwHHMMSS
// 221122w171600
// 221123w160800x
// 221212w163100

void getDateStuff(byte& year, byte& month, byte& date, byte& dOW,
                  byte& hour, byte& minute, byte& second) {
    // Call this if you notice something coming in on
    // the serial port. The stuff coming in should be in
    // the order YYMMDDwHHMMSS, with an 'x' at the end.
    boolean gotString = false;
    char inChar;
    byte temp1, temp2;
    char inString[20];
    
    byte j=0;
    while (!gotString) {
        if (Serial.available()) {
            inChar = Serial.read();
            inString[j] = inChar;
            j += 1;
            if (inChar == 'x') {
                gotString = true;
            }
        }
    }
    Serial.println(inString);
    // Read year first
    temp1 = (byte)inString[0] -48;
    temp2 = (byte)inString[1] -48;
    year = temp1*10 + temp2;
    // now month
    temp1 = (byte)inString[2] -48;
    temp2 = (byte)inString[3] -48;
    month = temp1*10 + temp2;
    // now date
    temp1 = (byte)inString[4] -48;
    temp2 = (byte)inString[5] -48;
    date = temp1*10 + temp2;
    // now Day of Week
    dOW = (byte)inString[6] - 48;
    // now hour
    temp1 = (byte)inString[7] -48;
    temp2 = (byte)inString[8] -48;
    hour = temp1*10 + temp2;
    // now minute
    temp1 = (byte)inString[9] -48;
    temp2 = (byte)inString[10] -48;
    minute = temp1*10 + temp2;
    // now second
    temp1 = (byte)inString[11] -48;
    temp2 = (byte)inString[12] -48;
    second = temp1*10 + temp2;
}

void setup() {
    // Start the serial port
    Serial.begin(9600);
    
    // Start the I2C interface
    Wire.begin();
}

void loop() {
    
    // If something is coming in on the serial line, it's
    // a time correction so set the clock1 accordingly.
    if (Serial.available()) {
        getDateStuff(year, month, date, dOW, hour, minute, second);
        
        clock1.setClockMode(false);  // set to 24h
        //setclock1Mode(true); // set to 12h
        
        clock1.setYear(year);
        clock1.setMonth(month);
        clock1.setDate(date);
        clock1.setDoW(dOW);
        clock1.setHour(hour);
        clock1.setMinute(minute);
        clock1.setSecond(second);
        
        // Test of alarm functions
        // set A1 to one minute past the time we just set the clock1
        // on current day of week.
        clock1.setA1Time(dOW, hour, minute+1, second, 0x0, true,
                        false, false);
        // set A2 to two minutes past, on current day of month.
        clock1.setA2Time(date, hour, minute+2, 0x0, false, false,
                        false);
        // Turn on both alarms, with external interrupt
        clock1.turnOnAlarm(1);
        clock1.turnOnAlarm(2);
        
    }
    delay(1000);
}
