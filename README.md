# CO2 detector

After reading [this article](https://theconversation.com/how-to-use-ventilation-and-air-filtration-to-prevent-the-spread-of-coronavirus-indoors-143732) ([ES](https://www.bbc.com/mundo/noticias-53754274)) I started to think about creating my own sensor board. I searched which sensor to use and the SGP30 seemed a good option to me. After a few tests, I modified the code of another project and then I built two prototypes which are currently working in my house. The device uses a mqtt queue to send data to an IoT gateway (a minipc running Home Assistant in my case). In addition to that, there are three LEDs to show what is the current state of the air based on CO2 PPMs:

- Green light is shown if there are less than 600 PPM of CO2.
- Yellow light if there are between 600 and 800 PPM.
- Red when the CO2 is greater than 800.

## Features
- Configuration of WiFi network and MQTT through a web portal.
- Temperature, humidity and air qualiity (CO2 and TVOC) sensors.
- Possibility of being used with Home Assistant or any software capable of reading a mqtt queue.

## Hardware

You will find the hardware schematics and design files in the board directory. The hardware is distributed under the Cern Open Source Hardware License 1.2.

![Prototype](doc/images/board-red-light.png) 

### Hardware components
- Wemos D1 mini
- DHT22 sensor
- SGP30 sensor
- Wires, tools...
- Green, yellow and red LEDs
- ... (See bill of materials below)

## Software

The software is available unde Apache 2.0 license and can be found in the co2-detector directory.

### Requirements
- This source uses Arduino IDE and its standard libraries
- Additional libraries: WiFiManager, Arduino Json, PubSubClient, DHT and Adafruit SGP30
- A computer running MQTT
- [Optional] A computer running Home Assistant

## Configuration

During the first execution, the device will launch a WiFi access point. Using your PC or mobile device, check the available networks and connect to the network whose name starts with "CO2-detector". After that, point your browser to http://192.168.4.1 and the configuration page will be displayed. 

![Home](doc/images/home.png) 

Choose the first option ("Configure WiFi") and select your WiFi network from the list of detected access points then, write your WiFi password.

![Available networks](doc/images/wifi-scan.png) 

Now configure you network settings.

![Network](doc/images/network.png) 

The device will publish all sensor data through mqtt so, the information is sent to another machine such as a minipc used as an IoT gatway. To do that, you have to introduce the data of the mqtt server. 

![Network](doc/images/mqtt-server.png) 

Then, fill all the fields related to your MQTT topic names and save your configuration. If you have several devices in different places you can change the prefix (room1/Temperature, room2/Temperature and so on).

![Network](doc/images/mqtt-topics.png) 

After saving the settings the device will be reset and then it will run in its normal mode, gathering data from all sensors.

## Usage

All LEDs will be turned on when the device starts. After configuring the device, it will try to connect to the configured network. After that, only one LED will be lighted.

The CO2 sensor takes a measurement every second and sends an average of the last 60 CO2 and TVOC values every minute. The temperature and humidity sensor is used to correct the obtained data as explained in the SGP30 data sheet.

## Home Assistant

I use Home Assistant to collect all the data and display the information. The software runs in a miniPC (a SolidRun CuBox-i) which is a small home IoT server and also has the Mosquitto mqtt broker. The device sends sensors data to the mqtt broker and Home Assistant reads it and stores everything in a data base. 
In addition to that Home Assistant has a frontend application that shows the current status of all your sensors and historical graphs of a selected day.
You can use any other similar software with the capability of subscribing to read from mqtt.

![Home Assistant](doc/images/hass.png) 

# Bill of materials

| Reference |  Value                     |  Footprint                                                     |  Datasheet |
| --------- | -------------------------- | -------------------------------------------------------------- | ---------- |
| D1        | Red                        | LEDs:LED_D5.0mm                                                |            |
| R3        | 90                         | Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal |            |
| R4        | 10K                        | Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal |            |
| TH1       | DHT22_Temperature_Humidity | Sensors:DHT22_Temperature_Humidity                             |            |
| U2        | SGP30                      | Pin_Headers:Pin_Header_Straight_1x04_Pitch2.54mm               |            |
| D2        | Yellow                     | LEDs:LED_D5.0mm                                                |            |
| D3        | Green                      | LEDs:LED_D5.0mm                                                |            |
| R1        | 160                        | Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal |            |
| R2        | 160                        | Resistors_THT:R_Axial_DIN0207_L6.3mm_D2.5mm_P7.62mm_Horizontal |            |
| U1        | WeMos_D1_mini              | wemos-d1-mini:wemos-d1-mini-with-pin-header                    |            |

# Example of a configuration for Home Assistant
The following Home Assistant configuration file is an example of two CO2 detectors placed at different rooms within a house. One is in a dinner room while the second is monitoring a sleeping room. Each device sends the data provided by the SGP20 (CO2 and TVOC) and DHT22 (temperature, humidity and heat index) sensors to a mqtt queue which is installed in the computer hosting Home Assistant.

```
homeassistant:
  # Name of the location where Home Assistant is running
  name: Home
  # Impacts weather/sunrise data
  elevation: 50
  # C for Celsius, F for Fahrenheit
  temperature_unit: C
  # 'metric' for the metric system, 'imperial' for the imperial system
  unit_system: metric
  # Pick yours from here: http://en.wikipedia.org/wiki/List_of_tz_database_time_zones
  time_zone: Europe/Madrid

# Show links to resources in log and frontend
#introduction:

system_health:


group:
  kids:
    name: Kids room
    entities:
      - sensor.kids_temperature
      - sensor.kids_humidity
      - sensor.kids_heatindex
      - sensor.kids_co2
      - sensor.kids_tvoc
  dinner_room:
    name: Dinner room
    entities:
      - sensor.comedor_temperature
      - sensor.comedor_humidity
      - sensor.comedor_heatindex
      - sensor.comedor_co2
      - sensor.comedor_tvoc

# Enables the frontend
frontend:

# Checks for available updates
updater:

# Discover some devices automatically
discovery:

# Allows you to issue voice commands from the frontend in enabled browsers
conversation:

# Enables support for tracking state changes over time.
history:

# View all events in a logbook
logbook:


#MQTT configuration
mqtt:
  broker: 127.0.0.1
  port: 1883
  client_id: home-assistant-1
  keepalive: 60
  #username: USERNAME
  #password: PASSWORD
  #certificate: /home/username/dev/addtrustexternalcaroot.crt
  protocol: 3.1


##### Kids room  #####
sensor kids_temperature:
   platform: mqtt
   name: "Kids temperature"
   state_topic: "kids/temperature"
   qos: 0
   unit_of_measurement: "C"

sensor kids_humidity:
   platform: mqtt
   name: "Kids humidity"
   state_topic: "kids/humidity"
   qos: 0
   unit_of_measurement: "%"

sensor kids_heatindex:
   platform: mqtt
   name: "Kids heatindex"
   state_topic: "kids/heatindex"
   qos: 0
   unit_of_measurement: "C"


sensor kids_co2:
   platform: mqtt
   name: "Kids CO2"
   state_topic: "kids/co2"
   qos: 0
   unit_of_measurement: "ppm"
                      
sensor kids_tvoc:
    platform: mqtt
    name: "Kids TVOC"
    state_topic: "kids/tvoc"
    qos: 0
    unit_of_measurement: "ppb"

##### Dinner room #####
sensor comedor_temperature:
    platform: mqtt
    name: "Dinner room temperature"
    state_topic: "comedor/temperature"
    qos: 0
    unit_of_measurement: "C"

sensor comedor_humidity:
    platform: mqtt
    name: "Dinner room humidity"
    state_topic: "comedor/humidity"
    qos: 0
    unit_of_measurement: "%"

sensor comedor_heatindex:
    platform: mqtt
    name: "Dinner room heatindex"
    state_topic: "comedor/heatindex"
    qos: 0
    unit_of_measurement: "C"

sensor comedor_co2:
    platform: mqtt
    name: "Dinner room CO2"
    state_topic: "comedor/co2"
    qos: 0
    unit_of_measurement: "ppm"
                         
sensor comedor_tvoc:
    platform: mqtt
    name: "Dinner room TVOC"
    state_topic: "comedor/tvoc"
    qos: 0
    unit_of_measurement: "ppb"
  

#config:

#mobile_app:

recorder:
    db_url: postgresql://user:password@localhost/hass
```
