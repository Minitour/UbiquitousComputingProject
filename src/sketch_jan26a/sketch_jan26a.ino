#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#include "dht.h"

#define dht_apin A0 // Analog Pin sensor is connected to
#define light_analog A1
#define water_level_1 A2
int backlight_pin10 = 10;
int brightness = 255;
 
dht DHT;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
 
void setup(){

  
  Serial.begin(9600);
  delay(500);//Delay to let system boot
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  pinMode(backlight_pin10, OUTPUT);
  analogWrite(backlight_pin10,brightness);  
  lcd.begin(20,4); 
  
  delay(1000);//Wait before accessing Sensor

  
 
}//end "setup()"
 
void loop(){
  //Start of Program 
    
    DHT.read11(dht_apin);

    double humiditiy = get_humidity();
    double temperture = get_temperature();
    int light_level = get_light_level();
    int water_level = get_water_level_1();
    lcd.setCursor(0,0);
    lcd.print("TEMP: " + String(temperture));
    lcd.setCursor(0,1);
    lcd.print("HUMI: " + String(humiditiy));
    lcd.setCursor(0,2);
    lcd.print("LIGH: " + String(light_level));
    lcd.setCursor(0,3);
    lcd.print("WATR: " + String(water_level));
    delay(1000);//Wait 5 seconds before accessing sensor again.
 
  //Fastest should be once every two seconds.
 
}// end loop() 

int get_water_level_1(){
  return analogRead(water_level_1);
}

double get_temperature(){
  return DHT.temperature;
}

double get_humidity(){
  return DHT.humidity;
}

int get_light_level(){
  return analogRead(light_analog);
}
