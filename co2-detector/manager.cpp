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

#include "manager.h"
#include <string>

/*
template class std::basic_string<char>;
*/
bool shouldSaveConfig=false;

Manager::Manager(){ 
  
  network_ip = "192.168.1.206";
  network_mask = "255.255.255.0";
  network_gateway = "192.168.1.1";
  
  mqtt_server = "192.168.1.100";
  mqtt_port = "1883";
  mqtt_user = "your_username";
  mqtt_password = "YOUR_PASSWORD";

  //MQTT subscriptions
  dht_temperature_topic = "room/temperature";
  dht_humidity_topic = "room/humidity";
  dht_heatindex_topic = "room/heatindex";
  /*  bmp_pressure_topic = "room/pressure";
  bmp_temperature_topic = "room/device/temperature";
  */
  sgp_co2_topic = "room/co2";
  sgp_tvoc_topic = "room/tvoc";
  
  //flag for saving data
  shouldSaveConfig = false;
  configFileExists = false;
}

void Manager::setup_config_data(){
  //read configuration from FS json
  Serial.println("mounting FS...");

  if (SPIFFS.begin()) {
    Serial.println("mounted file system");
    //SPIFFS.remove("/config.json");
    if (SPIFFS.exists("/config.json")) {
      //file exists, reading and loading
      Serial.println("reading config file");
      File configFile = SPIFFS.open("/config.json", "r");
      if (configFile) {
        size_t size = configFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        configFile.readBytes(buf.get(), size);
        DynamicJsonDocument json(1024);
        deserializeJson(json,buf.get());
        serializeJson(json, Serial);
        if (!json.isNull()) {
          configFileExists=true;
          Serial.println("\nparsed json:");
          
          serializeJson(json, Serial);

          network_ip = (const char *)json["network_ip"];
          network_mask = (const char *)json["network_mask"];
          network_gateway = (const char *)json["network_gateway"];

          mqtt_server = (const char *)json["mqtt_server"];
          mqtt_port = (const char *)json["mqtt_port"];
          mqtt_user = (const char *)json["mqtt_user"];
          mqtt_password = (const char *)json["mqtt_password"];

          dht_temperature_topic = (const char *)json["dht_temperature_topic"];
          dht_humidity_topic = (const char *)json["dht_humidity_topic"];
          dht_heatindex_topic = (const char *)json["dht_heatindex_topic"];

          sgp_co2_topic = (const char *)json["sgp_co2_topic"];
          sgp_tvoc_topic=(const char *)json["sgp_tvoc_topic"];
          
          dht_temperature_discovery_topic = "homeassistant/sensor/"+ String(dht_temperature_topic) + "/config";
          dht_humidity_discovery_topic = "homeassistant/sensor/"+ String(dht_humidity_topic) + "/config";
          dht_heatindex_discovery_topic = "homeassistant/sensor/"+ String(dht_heatindex_topic) + "/config";
          sgp_co2_discovery_topic = "homeassistant/sensor/"+ String(sgp_co2_topic) + "/config";
          sgp_tvoc_discovery_topic = "homeassistant/sensor/"+ String(sgp_tvoc_topic) + "/config";          

        } else {
          Serial.println("failed to load json config");
        }
      }
    }else{
      shouldSaveConfig=true;
    }
  } else {
    Serial.println("failed to mount FS");
  }
}


void Manager::setup_wifi(){
  long wifiTimeStart;
  
  WiFiManagerParameter custom_network_group("<p>Network settings</p>");
  WiFiManagerParameter custom_network_ip("IP", "IP", network_ip.c_str(), 15);
  WiFiManagerParameter custom_network_mask("mask", "mask", network_mask.c_str(), 15);
  WiFiManagerParameter custom_network_gateway("gateway", "gateway", network_gateway.c_str(), 15);
  
  WiFiManagerParameter custom_server_group("<p>MQTT Sercer settings</p>");
  WiFiManagerParameter custom_mqtt_server("server", "mqtt server", mqtt_server.c_str(), 15);
  WiFiManagerParameter custom_mqtt_port("port", "mqtt port", mqtt_port.c_str(), 6);
  WiFiManagerParameter custom_mqtt_password("password", "mqtt password", mqtt_password.c_str(), 30);
  WiFiManagerParameter custom_mqtt_username("username", "user name", mqtt_user.c_str(), 30);
  
  WiFiManagerParameter custom_topics_group("<p>MQTT topics</p>");

  WiFiManagerParameter custom_dht_temperature_topic("temperature","temperature",dht_temperature_topic.c_str(),40);
  WiFiManagerParameter custom_dht_humidity_topic("humidity","humidity",dht_humidity_topic.c_str(),40);
  WiFiManagerParameter custom_dht_heatindex_topic("heatindex","heatindex",dht_heatindex_topic.c_str(),40);

  WiFiManagerParameter custom_sgp_co2_topic("co2","co2",sgp_co2_topic.c_str(),40);
  WiFiManagerParameter custom_sgp_tvoc_topic("tvoc","tvoc",sgp_tvoc_topic.c_str(),40);
  
  WiFiManager wifiManager;

  //set config save notify callback
  wifiManager.setSaveConfigCallback(&Manager::saveConfigCallback);
  wifiManager.setBreakAfterConfig(true); //https://github.com/tzapu/WiFiManager/issues/992 IF YOU NEED TO SAVE PARAMETERS EVEN ON WIFI FAIL OR EMPTY, you must set setBreakAfterConfig to true, or else saveConfigCallback will not be called.

  wifiManager.addParameter(&custom_network_group);
  wifiManager.addParameter(&custom_network_ip);
  wifiManager.addParameter(&custom_network_mask);
  wifiManager.addParameter(&custom_network_gateway);
 
  wifiManager.addParameter(&custom_server_group);
  wifiManager.addParameter(&custom_mqtt_server);
  wifiManager.addParameter(&custom_mqtt_port);
  wifiManager.addParameter(&custom_mqtt_username);
  wifiManager.addParameter(&custom_mqtt_password);
  
  wifiManager.addParameter(&custom_topics_group);
  wifiManager.addParameter(&custom_dht_temperature_topic);
  wifiManager.addParameter(&custom_dht_humidity_topic);
  wifiManager.addParameter(&custom_dht_heatindex_topic);
  
  wifiManager.addParameter(&custom_sgp_co2_topic);
  wifiManager.addParameter(&custom_sgp_tvoc_topic);

  if (configFileExists){
    //read updated parameters
    network_ip=custom_network_ip.getValue();
    network_mask= custom_network_mask.getValue();
    network_gateway= custom_network_gateway.getValue();
    
    mqtt_server=custom_mqtt_server.getValue();
    mqtt_port= custom_mqtt_port.getValue();
    mqtt_user= custom_mqtt_username.getValue();
    mqtt_password=custom_mqtt_password.getValue();
    
    dht_temperature_topic=custom_dht_temperature_topic.getValue();
    dht_humidity_topic=custom_dht_humidity_topic.getValue();
    dht_heatindex_topic=custom_dht_heatindex_topic.getValue();

    sgp_co2_topic=custom_sgp_co2_topic.getValue();
    sgp_tvoc_topic=custom_sgp_tvoc_topic.getValue();
    
    IPAddress ip,gateway,mask;
    Serial.println(network_ip);
    Serial.println(network_gateway.c_str());
    Serial.println(network_mask.c_str());
    Serial.println(WiFi.macAddress());
    ip.fromString(network_ip.c_str());
    gateway.fromString(network_gateway.c_str());
    mask.fromString(network_mask.c_str());
    
    WiFi.config(ip, gateway,mask);
    WiFi.hostname("CO2-Detector");
    WiFi.mode(WIFI_STA);
    while (WiFi.status() != WL_CONNECTED){
      wifiTimeStart = millis();
      if (strlen(WiFi.psk().c_str())==0){
        WiFi.begin(WiFi.SSID().c_str());
        Serial.printf("\nConnecting to an open network (%s).\n",WiFi.SSID().c_str());  
      }
      else {
        WiFi.begin(WiFi.SSID().c_str(), WiFi.psk().c_str());
        Serial.printf("\nConnecting to an encrypted network (%s).\n",WiFi.SSID().c_str());  
      }
      
      while (WiFi.status() != WL_CONNECTED && (millis() - wifiTimeStart < WIFI_CONNECTION_TIMEOUT)) {
        delay(500);
        Serial.print(".");
      }
      
      Serial.println("\nUnable to connect to the WiFi network. Trying again.");   
    }
    
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("Network configuration: ");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.gatewayIP());
    Serial.println(WiFi.subnetMask());
    Serial.println(WiFi.hostname());
  }else {
        wifiManager.setTimeout(300);
        wifiManager.startConfigPortal("CO2-detector");
  }
  
  //if you get here you have connected to the WiFi
  Serial.println("Connected!!)");
    
  //save the custom parameters to FS
  if (shouldSaveConfig) {
    Serial.println("saving config");
    DynamicJsonDocument json(1024);

    json["network_ip"] = custom_network_ip.getValue();
    json["network_mask"] = custom_network_mask.getValue();
    json["network_gateway"] = custom_network_gateway.getValue();
    
    json["mqtt_server"] = custom_mqtt_server.getValue();
    json["mqtt_port"] = custom_mqtt_port.getValue();
    json["mqtt_user"] = custom_mqtt_username.getValue();
    json["mqtt_password"] = custom_mqtt_password.getValue();
    
    json["dht_temperature_topic"] = custom_dht_temperature_topic.getValue();
    json["dht_humidity_topic"] = custom_dht_humidity_topic.getValue();
    json["dht_heatindex_topic"] = custom_dht_heatindex_topic.getValue();

    json["sgp_co2_topic"] = custom_sgp_co2_topic.getValue();
    json["sgp_tvoc_topic"] = custom_sgp_tvoc_topic.getValue();
    
    File configFile = SPIFFS.open("/config.json", "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }

    serializeJson(json, Serial);    
    serializeJson(json, configFile);
    configFile.close();
    //end save
  }
 
}

String Manager::temperatureDiscoveryMsg() {

  DynamicJsonDocument doc(1024);
  String buffer;

  doc["name"] = String(dht_temperature_topic) + " Temperature";
  doc["stat_t"]   = dht_temperature_topic;
  doc["unit_of_meas"] = "°C";
  doc["dev_cla"] = "temperature";
  doc["frc_upd"] = true;
  doc["uniq_id"] =  dht_temperature_topic;

  serializeJson(doc, buffer);

  return buffer;
}

String Manager::getDiscoveryMsg(String topic, String unit) {

  DynamicJsonDocument doc(1024);
  String buffer;

  doc["name"] = topic;
  doc["stat_t"]   = topic;
  doc["unit_of_meas"] = unit;
  doc["dev_cla"] = "temperature";
  doc["frc_upd"] = true;
  doc["uniq_id"] =  topic;

  serializeJson(doc, buffer);

  return buffer;
}

String Manager::humidityDiscoveryMsg() {

  DynamicJsonDocument doc(1024);
  String buffer;

  doc["name"] = String(dht_humidity_topic) + " Humidity";
  doc["stat_t"]   = dht_humidity_topic;
  doc["unit_of_meas"] = "%";
  doc["dev_cla"] = "humidity";
  doc["frc_upd"] = true;
  doc["uniq_id"] =  dht_humidity_topic;

  serializeJson(doc, buffer);

  return buffer;
}

void Manager::readBaseline(SGP30 *sensor){
  if (SPIFFS.begin()) {
    //SPIFFS.remove("/baseline.json"); // Uncomment to remove the file if the device sends extremly high values of CO2 / TVOC
    if (SPIFFS.exists("/baseline.json")) {
      //file exists, reading and loading
      File blFile = SPIFFS.open("/baseline.json", "r");
      if (blFile) {
        size_t size = blFile.size();
        // Allocate a buffer to store contents of the file.
        std::unique_ptr<char[]> buf(new char[size]);

        blFile.readBytes(buf.get(), size);
        DynamicJsonDocument json(1024);
        DeserializationError error =deserializeJson(json,buf.get());
        if (!error) {
          Serial.println("\nparsed json");          
          serializeJson(json, Serial);

          sensor->baselineCO2 = json["blCO2"].as<uint16_t>();;
          sensor->baselineTVOC = json["blTVOC"].as<uint16_t>();;          
          sensor->setBaseline(sensor->baselineCO2, sensor->baselineTVOC);
          Serial.println("\nBaseline read:");
          Serial.println(sensor->baselineCO2);
          Serial.println(sensor->baselineTVOC);
        }else{
          Serial.println("\nBaseline json is corrpupt.");      
        }
      }
      blFile.close();
    }else{
      Serial.println("\nBaseline file not found.");      
    }
  }
}

void Manager::saveBaseline(SGP30 *sensor){
  DynamicJsonDocument json(1024);

  sensor->getBaseline();
  
  json["blCO2"] = sensor->baselineCO2;
  json["blTVOC"] = sensor->baselineTVOC;
  Serial.println("\nBaseline:");
  Serial.println(sensor->baselineCO2);
  Serial.println(sensor->baselineTVOC);
    
  File blFile = SPIFFS.open("/baseline.json", "w");
  if (!blFile) {
    Serial.println("failed to open config file for writing");
  }
  
  serializeJson(json, blFile);
  blFile.close();

 }
  
