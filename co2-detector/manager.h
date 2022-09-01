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

#ifndef _MANAGER_
#define _MANAGER_
#include <DNSServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <SparkFun_SGP30_Arduino_Library.h>
#include "WiFiManager.h"      

#include <ArduinoJson.h> 
#include <FS.h> 

//Deep sleep
#define DEEP_SLEEP_TIME 60 //time in seconds
#define WIFI_CONNECTION_TIMEOUT 20000 //Timeout for WIFI connections. The idea is to prevent for continuous conection tries. This would cause battery drain

#define ARDUINOJSON_ENABLE_STD_STREAM

using namespace std;

extern bool shouldSaveConfig;//flag for saving data

class Manager{

public:  
  explicit Manager();
  void setup_config_data();
  void setup_wifi();

  bool configFileExists;

  //callback notifying us of the need to save config
  static void saveConfigCallback () { Serial.println("Should save config"); shouldSaveConfig = true;}

  //Make the device discoverable
  String temperatureDiscoveryMsg();
  String humidityDiscoveryMsg();
  String getDiscoveryMsg(String topic, String unit);

  void readBaseline(SGP30 *sensor);
  void saveBaseline(SGP30 *sensor);

  String networkIp(){return network_ip;}
  String networkMask(){return network_mask;}
  String networkGateway(){return network_gateway;}

  String mqttServer(){return mqtt_server;}
  String mqttPort(){return mqtt_port;}
  String mqttUser(){return mqtt_user;}
  String mqttPassword(){return mqtt_password;}

  String dhtTemperatureTopic(){return dht_temperature_topic;}
  String dhtHumidityTopic(){return dht_humidity_topic;}
  String dhtHeatindexTopic(){return dht_heatindex_topic;}
  
  String dhtTemperatureDiscoveryTopic(){return dht_temperature_discovery_topic;}
  String dhtHumidityDiscoveryTopic(){return dht_humidity_discovery_topic;}
  String dhtHeatindexDiscoveryTopic(){return dht_heatindex_discovery_topic;}
  String sgpCO2DiscoveryTopic(){return sgp_co2_discovery_topic;}
  String sgpTVOCDiscoveryTopic(){return sgp_tvoc_discovery_topic;}

  String sgpCO2Topic(){return sgp_co2_topic;}
  String sgpTVOCTopic(){return sgp_tvoc_topic;}

private:
  //MQTT  server
  String network_ip;
  String network_mask ;
  String network_gateway;
  
  //MQTT  server
  String mqtt_server;
  String mqtt_port ;
  String mqtt_user;
  String mqtt_password;

  //MQTT subscriptions
  String dht_temperature_topic;
  String dht_humidity_topic;
  String dht_heatindex_topic;
  
  //MQTT discovery topics
  String dht_temperature_discovery_topic;
  String dht_humidity_discovery_topic;
  String dht_heatindex_discovery_topic;
  String sgp_co2_discovery_topic;
  String sgp_tvoc_discovery_topic;

  String sgp_co2_topic;
  String sgp_tvoc_topic;  
};

#endif
