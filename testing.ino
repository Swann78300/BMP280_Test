

/***************************************************************************
  This is a library for the BMP280 humidity, temperature & pressure sensor

  Designed specifically to work with the Adafruit BMEP280 Breakout 
  ----> http://www.adafruit.com/products/2651

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required 
  to interface.

  Adafruit invests time and resources providing this open source code,
  please support Adafruit andopen-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.  
  BSD license, all text above must be included in any redistribution
  
  ---------------------------------------------------------
  BMP280 ADDRESS IS 0X76, CHANGED ADDRESS IN BMP280 LIBRARY!
  ---------------------------------------------------------
  added features such as recording highest altitude, simplyfying input of forecast variable and more
  latest edit: 20.3.2017 u/wooghee
 ***************************************************************************/

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BMP280.h>

float alti; // saved altitude 
float ten; //added up first ten height measurements
float forecast; // this should be adjusted to your local forcase
float approx;
float highest;
float store;
float start;


Adafruit_BMP280 bme; // I2C

void setup() {
  delay(2000);
  ten = 0;
  forecast = 1013.1;
  highest = 0;
  approx = 0;
  highest = 0;
  alti = 0;
  store = 0;
  start = 0;
  
        bme.readTemperature();
        bme.readPressure();
        
  Serial.begin(9600);
  Serial.println(F("REBOOT"));
  Serial.println(F("\nBMP280 test\n"));

        bme.readTemperature();
        bme.readPressure();
        bme.readAltitude(forecast);
  
  if (!bme.begin()) {  
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }
  ten = 0;
  store = 0;
  start = 0;
  for(int s=1; s<=10; s++){
        store = bme.readAltitude(forecast);
        ten += store;
        if(s=1){
          ten = ten-store;
          s++;
        }
        Serial.println(ten);
        Serial.println(store);
        Serial.println(start);
        Serial.print(F("Temperature = "));
        Serial.print(bme.readTemperature());
        Serial.println(" *C");
        Serial.print(F("Pressure = "));
        Serial.print(bme.readPressure());
        Serial.println(" Pa");

        Serial.print(F("Approx altitude = "));
        Serial.print(store); 
        Serial.println(" m");
        Serial.println("calibrating sensor...\n");
        store = 0;
        delay(1000);
  }
  Serial.println("\n\nCALIBRATION COMPLETE\n");
  Serial.print("ALTITUDE AT START:  ");
  start = ten/9;
  //start = ;
  Serial.print(start);
  Serial.println(" m \n\n\n\n");
  delay(1000);
}

void loop() {
      //approx = 0;   //reset change
      for(int i=1; i<=5; i++){
        alti = bme.readAltitude(forecast)-(start);  //measure height difference 5 times
        approx += alti;
        delay(200);
      }
      Serial.print(F("Approx altitude:            "));
      Serial.print(bme.readAltitude(forecast)); 
      Serial.println(" m");
      
      Serial.print("Approx height difference:   ");
      Serial.print(approx/5);                         //serial out average height difference
      Serial.println(" m");

      if(approx/5 > highest){                           //compare heights and store highest value
        highest = approx/5;
        Serial.print("rekord altitude:            ");
        Serial.print(highest+start);
        Serial.println(" m");
        Serial.print("\nNEW REKORD ALTITUDE:        ");
        Serial.print(highest);
        Serial.println(" m    NEW REKORD!\n\n");
      }else{
        Serial.print("rekord difference:          ");
        Serial.print(highest);
        Serial.println(" m\n");
        
      }
}
