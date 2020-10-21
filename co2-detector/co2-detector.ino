/*
Copyright 2020 co2-detector

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#include <ESP8266WiFi.h>
#include <SparkFun_SGP30_Arduino_Library.h>
#include <Wire.h>
#include <PubSubClient.h>
#include <DHT.h>
#include "manager.h"

//Temperature sensor settings
#define DHTPIN 12 // what digital pin we're connected to
#define DHTTYPE DHT22 // DHT 22  (AM2302), AM2321

// LEDs pins
#define GREEN_PIN 15
#define YELLOW_PIN 13
#define RED_PIN 14

// Sensor objects
DHT dht(DHTPIN, DHTTYPE); // Initializes the DHT sensor.
SGP30 mySensor; //create an object of the SGP30 class

//Timeout connection for wifi or mqtt server
#define CONNECTION_TIMEOUT 20000 //Timeout for connections. The idea is to prevent for continuous conection tries. 

//Portal and wific connection manager
Manager manager; 
 
//MQTT client
WiFiClient espClient;
PubSubClient client(espClient);

//Global variables for adding sensor data
float temperature = 0;
float humidity = 0;
float heatindex = 0;
int co2 = 0;
int tvoc = 0;

long t_elapsed;
int counter = 0;

void setup() {
  t_elapsed = millis();
  Serial.begin(9600);

  // Configuration of LED pins
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(RED_PIN, OUTPUT);

  // WiFi setup
  manager.setup_config_data();
  manager.setup_wifi();
  
  //DHT sensor for humidity and temperature
  dht.begin();
  
  Wire.begin();
  //Initialize sensor
  if (mySensor.begin() == false) {
    Serial.println("No SGP30 Detected. Check connections.");
    while (1);
  }
  //Initializes sensor for air quality readings
  mySensor.initAirQuality();
  manager.readBaseline(&mySensor); // Read the previously stored baseline

  //Setup mqtt
  IPAddress addr;
  addr.fromString(manager.mqttServer());
  client.setServer(addr, atoi(manager.mqttPort().c_str())); 
  Serial.println("Configured!!");
}


void reconnect() {
  // Loop until we're reconnected
  long t1 = millis();
  while (!client.connected() && (millis() - t1 < CONNECTION_TIMEOUT)) {
    // Attempt to connect
    String clientName("ESP8266Client-");
    clientName.concat(ESP.getChipId());   
    Serial.print("Attempting MQTT connection... ");
    Serial.println(clientName);
    if (client.connect(clientName.c_str())) {
      Serial.println("Connected to mqtt");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(manager.mqttServer());
      Serial.println(manager.mqttPort().c_str()); 
      Serial.println(" trying again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

float readDHT22(){  
  //read dht22 value
  temperature = dht.readTemperature();    
  humidity = dht.readHumidity();
  heatindex = dht.computeHeatIndex(temperature, humidity, false);
     
  Serial.print(temperature);
  Serial.print(" ");
  Serial.print(humidity);
  Serial.print(" ");
  Serial.println(heatindex);
  client.publish(manager.dhtTemperatureTopic().c_str(), String(temperature).c_str(), true);
  Serial.print(String(manager.dhtTemperatureTopic().c_str()));
  Serial.println(String(temperature).c_str());
  delay(50);
  client.publish(manager.dhtHumidityTopic().c_str(), String(humidity).c_str(), true);
  Serial.print(String(manager.dhtHumidityTopic().c_str()));
  Serial.println(String(humidity).c_str());
  delay(50);
  client.publish(manager.dhtHeatindexTopic().c_str(), String(heatindex).c_str(), true);
  Serial.print(String(manager.dhtHeatindexTopic().c_str()));
  Serial.println(String(heatindex).c_str());
  delay(50);
}

double RHtoAbsolute (float relHumidity, float tempC) {
  double eSat = 6.11 * pow(10.0, (7.5 * tempC / (237.7 + tempC)));
  double vaporPressure = (relHumidity * eSat) / 100; //millibars
  double absHumidity = 1000 * vaporPressure * 100 / ((tempC + 273) * 461.5); //Ideal gas law with unit conversions
  return absHumidity;
}

uint16_t doubleToFixedPoint( double number) {
  int power = 1 << 8;
  double number2 = number * power;
  uint16_t value = floor(number2 + 0.5);
  return value;
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }

  client.loop();
  
  //First fifteen readings will be
  //CO2: 400 ppm  TVOC: 0 ppb
  delay(1000); //Wait 1 second
  //measure CO2 and TVOC levels
  mySensor.measureAirQuality();          
  /*mySensor.getBaseline();
  Serial.println("\nBaseline read:");
  Serial.println(mySensor.baselineCO2);
  Serial.println(mySensor.baselineTVOC);
  */

  // Publish each 60 secs. Maybe we can send an average of this interval?
  counter++;
  if (counter == 60){
    readDHT22();

    int avgCO2=co2/60;

    if (avgCO2<600){
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(RED_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
    }else if (avgCO2<800){
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
    }else{
      digitalWrite(GREEN_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(RED_PIN, HIGH);
    }
    
         
    client.publish(manager.sgpCO2Topic().c_str(), String(avgCO2).c_str(), true);
    client.publish(manager.sgpTVOCTopic().c_str(), String(tvoc/60).c_str(), true);
    manager.saveBaseline(&mySensor);
    counter = 0;
    co2 = 0;
    tvoc = 0;

    //Convert relative humidity to absolute humidity
    double absHumidity = RHtoAbsolute(humidity, temperature);
  
    //Convert the double type humidity to a fixed point 8.8bit number
    uint16_t sensHumidity = doubleToFixedPoint(absHumidity);
  
    //Set the humidity compensation on the SGP30 to the measured value
    //If no humidity sensor attached, sensHumidity should be 0 and sensor will use default
    mySensor.setHumidity(sensHumidity);
    Serial.print("Absolute Humidity Compensation set to: ");
    Serial.print(absHumidity);
    Serial.println("g/m^3 ");

    

    
  }
  co2 += mySensor.CO2;
  tvoc += mySensor.TVOC;
  
  Serial.print("CO2: ");
  Serial.print(mySensor.CO2);
  Serial.print(" ppm\tTVOC: ");
  Serial.print(mySensor.TVOC);
  Serial.println(" ppb");



}