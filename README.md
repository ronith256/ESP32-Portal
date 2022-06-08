# ReadmeESP

## ESP32 Emergency Portal

ESP32 LoRa based emergency portal for quick and easy access to first respondents without internet and compatible with any device which has WiFi and basic browser with Javascript support.

### Why?

There are are few reasons why a system like this should exist.

It can't replace any existing technologies, but instead can fix a lot of issues.

* It is very common to get floods in the last few years and it usually wrecks the existing communication technologies which makes it extremely difficult to even get help.
* In my village it's very common to have no ways to communicate, including mobile and wired networks when there's some natural disaster. This is extremely dangerous as there are means to even contact an ambulance in case of a medical emergency.
* We have to wait for somebody to notice or to the local body to report it to higher authorities.
* Even then many cases will go unnoticed for a while

### Solution

* It's not practical to have direct satellite or something like that as even that will break in heavy rain.
* Wires \~ Won't work as they'll break
* P2P Wifi \~ Maybe will work, but it can't cover a whole village.
  * It will cost an absurd amount to cover an entire village
  * As even a single setup will cost close to 10K INR

### Solution (Practical)

Devices used:

* ESP32 with LoRa
* ESP8266 with LoRa (This can be used to cut cost, by almost half)

#### How it works?

* There will be one (or more) master ESP32s which is connected to internet and also have a LoRa receiver/transmitter.
* It will be with the first respondents.
* The slave ESP32s (or nodes) are distributed among a region connected to the master through LoRa.
* These nodes will have a webserver which can gather information from the user.
* The collected information is then sent to the Master ESP32 which will output the information and also update a Map in server with the difficulty faced by the person and also their location.

#### Why this?

* It is very cost efficient, easy to deploy and scale, and just works!
* It costs about 500rs combined for the main components that are used in this project.
  * LoRa Module \~ 300 RS
  * ESP8266 \~ 200 RS
* It should also be considered that these are the retail prices of these hardware and many of the functions these general purpose devboards are not required by this project. As they're opensource hardware it can be watered down thus reducing the cost even further.
* The WiFi range that's advertised by the company is about 1KM in Line Of Sight
* If we take 100m as the omni-directional range, with about 100 of these devices it can cover by village which is about 14.sqKM.
* Even though 100 sounds like a lot, it will only cost about 50K (in retail price) which is equal to setting up 5 \* P2P WiFi networks. Which will not cover the entire village and requires extensive setup process.
* The setup process of ESP32 nodes is really straightforward.
  * Upload the firmware
  * Put it in an enclosure
  * Connect battery
  * Yeet it somewhere (probably would want to use a drone or something else to launch it)
* Energy Efficiency of ESP32/ESP8266 is the major reason why this just works.
  * It will last about 3-4 days on single charge with just a 5700Mah battery. (Tested personally by driving a WiFi controlled RGB LED using ESP32)
* While a P2P WiFi or any other solutions mentioned might use more than a Kw per day.
* This is a huge deal as electricity availability will be a major issue during disasters.
* This device may also be leveraged as a security system when not in use for distress management.

### Current Progress

* The ESP Node webserver is ready.
* Even though it's not very pretty, it able to cater to the basic functionality of emergency reporting.
* It transfers data the collected from user through LoRa to master device.
* The Master device is able to display the information to Serial Monitor.
* OTA updates are supported. (More work required).

### Path Ahead

* A pretty website for ESP Nodes.
* Webserver/App for representing the data from Master.
* Complete OTA updates through LoRa and WiFi.
* Some server to log the collected information like IBM Watson or Azure IoT.
* Add Temperature/Humidity sensor.

### Credits

* LoRa, AsyncWebServer creators
* Floods

### Individual Contributions

* Prathyush: Python Script to plot the data.
* Lakshmi: Logging and data management.&#x20;
* Mridhula: Project Modeling and management.&#x20;
* Ronith: ESP codes.

### License

* Codes in this repository may not be used commercially.
