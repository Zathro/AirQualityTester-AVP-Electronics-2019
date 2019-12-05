/*Aran O'Keeffe N00180895 IADT CMT Year 2 2019 Version 1
  Air Quality and humidity tester
  CMT Electronics/Audio Visiual Practice Project 2019
*/

/* *********************Pin Outs*********************
    SDS011 Tester - TX Pin 6
    SDS011 Tester - RX Pin 10  ***Please Be advised on the amazon Sensor TX and RX are labled wrong and reversed.
                                  If the sensor is reporting an error try swapping the TX And RX Pins the sensor will work with any of the pwm pins
                                  The SDS011 Sensor will work with any of the PWM Pins. ***

    DHT - Pin A2               ***TEMP and Humidity Sensor Pin Out Looking at the sensor from the front
                                  Pin 1 - Signal Pin= Pin A2, Pin 2- +5V VCC, Pin 3 - GND ***

    LCD Setup
    1- VSS - GND
    2- VDD - +5V
    3- V0 - Use a 10K pot between 5v and GND with the Wiper going to Pin V0 on the LCD This will adjust the contrast
    4- RS - Digital Pin 12
    5- RW - GND
    6- E - Digital Pin 11
    7- D0 - Not in Use
    8- D1 - Not in Use
    9- D2 - Not in Use
    10- D3 - Not in Use
    11- D4 - Digital Pin 5
    12- D5 - Digital Pin 4
    13- D6 - Digital Pin 3
    14- D7 - Digital Pin 2
    15- A - Resistor to +5V i use a 100Ohm use what ever resistor gives the best output for the Backlight
    16- K - GND
*/



#include <SdsDustSensor.h>
#include <DHT.h>
#include <DS3231.h>
#include <LiquidCrystal.h>


//DS3231 RTC Setup

DS3231  rtc(SDA, SCL);


//DHT Setup

#define DHTPIN A2     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11 Humidity tester version 

DHT dht(DHTPIN, DHTTYPE); // Sets the DHT DataPin and DHT Sensor Type (DHT11)

float temp, humid;


//SDS011 Setup

const byte rxPin = 6;
const byte txPin = 10;       //Be aware TX and RX are switched on the Sensor From Amazon
const byte sampleTime = 0.5; //This value sets the frequency that the system takes a temp air and humidity sample, it measures in 1min intervals and will report to the lcd and the serial monitor.
                             //If this value is set to below 1 the system will contoniusly run and take samples while reporting to the screen and Serial Monitor.

byte sampleCount;            //This is used by the serial monitor to count the amount of samples taken
float result25, result10;    //The SDS sensor stores its particulate valuses here parts per million 2.5 nano meters and parts per million 10 nano meters

SdsDustSensor sds(rxPin, txPin);

//LCD Setup
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

/************************************SETUP LOOP*******************************************/
void setup() {
  Serial.begin(9600);
  Serial.println("Setup Start");
  lcd.begin(16, 2);
  lcd.print("System Loading");
  rtc.begin();
  sds.begin();
  dht.begin();

  Serial.println(sds.setActiveReportingMode().toString());             //checks to see if sensor is reporting and active
  Serial.println (sds.setCustomWorkingPeriod(sampleTime).toString());  //Displays the working period of the system to the serial monitor
  delay(50);
  temp = dht.readTemperature();
  humid = dht.readHumidity();
  Serial.println("Setup End");
}

/*-----------------------------------VOID LOOP------------------------------------*/

void loop() {
  delay(50);
  PmResult pm = sds.readPm();
  result25 = pm.pm25;
  result10 = pm.pm10;
  temp = dht.readTemperature();
  humid = dht.readHumidity();

  if (pm.isOk()) {
    printResult();
    lcdPrintDat();
  }

  else {
    Serial.println(F("SDS001 Failed to Take Sample"));
  }
  delay(sampleTime * 60000);
}



void printResult () {
  delay(50);
  Serial.print("PM2.5 = ");
  Serial.print(result25);
  Serial.print(", PM10 = ");
  Serial.println(result10);
  Serial.print("Tempature °C= ");
  Serial.print(temp);
  Serial.print(" Humidity = ");
  Serial.print(humid);
  Serial.println("%");
  Serial.print("Sample Num:");
  Serial.println(sampleCount);
  sampleCount++;
}

void lcdPrintDat() {
  delay(50);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("2.5=");
  lcd.print(result25);
  lcd.print(" 10=");
  lcd.print(result10);
  lcd.setCursor(0, 1);
  lcd.print("T=");
  lcd.print(temp);
  lcd.print(" H=");
  lcd.print(humid);
  delay(50);
}
