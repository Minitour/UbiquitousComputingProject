#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


#include "dht.h"

#define dht_apin A0 // Analog Pin sensor is connected to
#define light_analog A1
#define water_level_1 A2
int backlight_pin10 = 10;
int brightness = 255;

int limit_watering = 3;
int current_watering = 0;

// demo mode -> 1, real mode -> 0
int demo = 1;
 
dht DHT;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);
 
void setup(){

  // setup for light
  pinMode(7, OUTPUT);
  pinMode(8, OUTPUT);
  pinMode(9, OUTPUT);
  
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
    
    delay(500);//Wait 5 seconds before accessing sensor again.

    // checking if to turn the light of or on
    check_if_turn_light(light_level, temperture);

    // checking if to start the pump
    check_if_start_pump(temperture, humiditiy, water_level);

    // activate fan
    check_if_start_fan(temperture);
 
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

void shut_light() {
  digitalWrite(7, HIGH);
}

void turn_light() {
  digitalWrite(7, LOW);
}

void start_pump() {
  if (current_watering < limit_watering) {
     digitalWrite(8, LOW);
     delay(3000);
     stop_pump();
     current_watering += 1;
  }
}

void stop_pump() {
  digitalWrite(8, HIGH);
}

void start_fan() {
  digitalWrite(9, LOW);
}

void stop_fan() {
  digitalWrite(9, HIGH);
}

void check_if_turn_light(int light, double temperture) {
  
  // darkness
  if (light > 700) {
    turn_light();
  }

  // too much light -> turn off lamp
  if (light <= 200) {
    shut_light();
  }

  // temperture low -> turn on lamp
  if (temperture < 15.0) {
    turn_light();
  }
}

void check_if_start_pump(double temperture, double humiditiy, int water_level) {

  if (demo == 1) {
    if (temperture > 23.0) {
      start_pump();
    }

    if (temperture <= 23.0) {
      stop_pump();
    }
    
  } else {
      // check when to start pump
    if (temperture > 23.0 && humiditiy < 50.0) {
      start_pump();
    }

    // check when to stop pump
    if (temperture <= 23.0 && humiditiy >= 30.0 && humiditiy <= 50.0) {
      stop_pump();
    } 
  }

}

void check_if_start_fan(double temperture) {
  
  if (demo == 1) {
      if (temperture >= 27.0) {
        start_fan();
      }
      
      if (temperture < 27.0) {
        stop_fan();
      }
      
  } else {
      if (temperture >= 32.0) {
        start_fan();
      }

      if (temperture < 32.0) {
        stop_fan();
      }
  }
  

}
