#include <Arduino.h>
#include <LoRa.h>
#include <SPI.h>
#include <DNSServer.h>
#ifdef ESP32
  #include <WiFi.h>
  #include <AsyncTCP.h>
#else
  #include <ESP8266WiFi.h>
  #include <ESPAsyncTCP.h>
#endif
#include <ESPAsyncWebServer.h>

//define the pins used by the transceiver module
#define ss 18
#define rst 14
#define dio0 26

//define Temperature sensor pins
#define tempPin 12
#define beta 4090 
#define resistance 10 

// Start WebServer and DNS Server
DNSServer dnsServer;
AsyncWebServer server(80);


// REPLACE WITH YOUR NETWORK CREDENTIALS
// To be used in WiFi client mode
//const char* ssid = "OPPO 2.4";
//const char* password = "mynameissajith";


const char* singleString = "singleString";

// Emergency Website in HTML

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Emergency Portal</title>
    <style>
        /* Global Styles */
body {
  background-color: #f2f2f2;
  font-family: Arial, sans-serif;
  font-size: 16px;
  line-height: 1.5;
  margin: 0;
  padding: 0;
}

h1 {
  margin: 0;
  font-size: 2.5em;
  text-align: center;
  padding: 20px 0;
  color: #333;
}

p {
  margin: 0;
  padding: 0;
}

label {
  font-weight: bold;
}

input[type="text"], input[type="checkbox"], input[type="submit"] {
  margin-bottom: 10px;
  padding: 10px;
  width: 100%;
  border: none;
  border-radius: 5px;
  font-size: 1.2em;
}

input[type="checkbox"] {
  margin-left: 20px;
}

input[type="submit"] {
  background-color: #333;
  color: #fff;
  cursor: pointer;
  font-weight: bold;
}

input[type="submit"]:hover {
  background-color: #666;
}

/* Container */
.container {
  max-width: 800px;
  margin: 0 auto;
}

/* Header */
.header {
  background-color: #fff;
  box-shadow: 0 2px 5px rgba(0,0,0,0.1);
}

/* Location */
.location {
  background-color: #fff;
  box-shadow: 0 2px 5px rgba(0,0,0,0.1);
  margin-bottom: 20px;
  padding: 20px;
}

.location h2 {
  margin-top: 0;
  font-size: 1.5em;
  color: #333;
}

/* Form */
.form {
  background-color: #fff;
  box-shadow: 0 2px 5px rgba(0,0,0,0.1);
  padding: 20px;
}

.form label {
  display: block;
  margin-bottom: 10px;
}

.form p {
  font-weight: bold;
  margin-bottom: 10px;
}

input[type="text"], input[type="checkbox"] {
    border: 1px solid #000;
  }

/* Media Queries */
@media only screen and (max-width: 767px) {
  h1 {
    font-size: 2em;
  }
  
  input[type="text"], input[type="checkbox"], input[type="submit"] {
    font-size: 1em;
  }
}
</style>
  </head>
  <body onload="getLocation()">
    <div class="container">
      <div class="header">
        <h1>Emergency Portal</h1>
      </div>
      <div class="content">
        <div class="form">
          <form action="/get">
            <div class="form-group">
              <label for="fname">Name:</label>
              <input type="text" onfocus="getLocation()" id="fname" name="fname" value="">
            </div>
            <div class="form-group">
              <label for="location">Nearby Location:</label>
              <input type="text" id="location" name="location" value="">
            </div>
            <div class="form-group">
              <p><b>What is your Emergency?</b></p>
              <label for="Medical">Medical</label>
              <input type="checkbox" id="medical" name="Medical">
              <label for="Fire">Fire/Explosion</label>
              <input type="checkbox" id="fire" name="fire">
              <label for="Collapse">Building Collapse</label>
              <input type="checkbox" id="Collapse" name="Collapse">
            </div>
            <div class="form-group">
              <label for="message">Enter a Message (120 Characters) </label>
              <input type="text" id="message" name="message">
            </div>
            <div class="form-group">
              <input type="hidden" id="singleString" name="singleString" value="3487">
              <input onclick="toSingleString()" type="submit" value="Submit">
            </div>
          </form>
        </div>
        <div class="location">
          <p>Your location is:</p>
          <p id="gps"></p>
        </div>
      </div>
    </div>
    <script>
        // Get the GPS coordinates of the user's location
var x = document.getElementById("gps");

function getLocation() {
  if (navigator.geolocation) {
    navigator.geolocation.getCurrentPosition(showPosition);
  } else { 
    x.innerHTML = "Geolocation is not supported by this browser.";
  }
}

function showPosition(position) {
  x.innerHTML = "Latitude: " + position.coords.latitude + "<br>Longitude: " + position.coords.longitude;
}

// Convert form input to a single string for submission
function toSingleString(){
  let singleString;
  let ss = document.getElementById("singleString");
  
  singleString = document.getElementById("fname").value;
  singleString += "<!>";
  singleString += document.getElementById("location").value;
  singleString += "<@>";
  singleString += document.getElementById("medical").checked;
  singleString += "<#>";
  singleString += document.getElementById("fire").checked;
  singleString += "<$>";
  singleString += document.getElementById("Collapse").checked;
  singleString += "<%>";
  singleString += document.getElementById("message").value;
  singleString += "<&>";
  singleString += "11.499797|75.688635";
  
  ss.value = singleString;
}
</script>
  </body>
</html>
)rawliteral";
  
void notFound(AsyncWebServerRequest *request) {
  request->send(404, "text/plain", "Not found");
}


void setup() {
  Serial.begin(115200);
  // To be used in Wifi Client mode
//  WiFi.mode(WIFI_STA);
//  WiFi.begin(ssid, password);
//  if (WiFi.waitForConnectResult() != WL_CONNECTED) {
//    Serial.println("WiFi Failed!");
//    return;
//  }

// Enter AP Name here
  WiFi.softAP("EMERGENCY-NETWORK");
// Default IP is 192.168.4.1
  IPAddress IP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(IP);

 // Set LoRa pins. Definition of these pins are mentioned above
  LoRa.setPins(ss, rst, dio0);
  Serial.println();

// To be used in WiFi client mode
//  Serial.print("IP Address: ");
//  Serial.println(WiFi.localIP());

  // Setup LoRa Frequency and also set sync word
  // Currently using 868MHz
   while (!LoRa.begin(866E6)) {
    Serial.println(".");
    delay(500);
  }
  LoRa.setSyncWord(0xF3);
  Serial.println("LoRa Initializing OK!");

// Code for Captive Portal  
class CaptiveRequestHandler : public AsyncWebHandler {
public:
  CaptiveRequestHandler() {}
  virtual ~CaptiveRequestHandler() {}

  bool canHandle(AsyncWebServerRequest *request){
    //request->addInterestingHeader("ANY");
    return true;
  }
  };
  
// Send web page to client.
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    request->send_P(200, "text/html", index_html);
  });

  // Send a GET request to get value in singleString 
  server.on("/get", HTTP_GET, [] (AsyncWebServerRequest *request) {
    String inputMessage;
    String inputParam;
    // If a value is returned, store that value in variables.
    if (request->hasParam(singleString)) {
      inputMessage = request->getParam(singleString)->value();
      inputParam = singleString;
    }
    else {
      inputMessage = "No message sent";
      inputParam = "none";
    }

// Print the input of the client    
    Serial.println(inputMessage);

// Client input to organised text.    
    int pos1 = inputMessage.indexOf("<!>");
    int pos2 = inputMessage.indexOf("<@>");
    int pos3 = inputMessage.indexOf("<#>");
    int pos4 = inputMessage.indexOf("<$>");
    int pos5 = inputMessage.indexOf("<%>");
    int pos6 = inputMessage.indexOf("<&>");
    
    String cName = inputMessage.substring(0,inputMessage.indexOf("<"))+ " ";
    String location = inputMessage.substring(pos1+3,pos2)+ " ";
    String medical = inputMessage.substring(pos2+3,pos3) + " ";
    String fire = inputMessage.substring(pos3+3, pos4) + " ";
    String collapse = inputMessage.substring(pos4+3, pos5) + " ";
    String message = inputMessage.substring(pos5+3, pos6) + " ";
    String gps = inputMessage.substring(pos6+3);
    String singleString = gps + "|" + cName + "|" + location + "|" + medical + "|" + fire + "|" + collapse + "|" + message;
    sender(cName,location,medical,fire,collapse,message,gps, singleString);

// Show a success page    
    request->send(200, "text/html", "Your input has been registered. Please wait while we try to contact you (" 
                                     + inputParam + ") with value: " + inputMessage +
                                     "<br><a href=\"/\">Return to Home Page</a>");
                                  
  });
  server.onNotFound(notFound);
  server.begin();
}

void loop() {
  
}

// Send the fromatted data received from client to LoRa master
void sender(String cName, String location, String medical, String fire, String collapse, String message, String gps, String singleString){
  // Send macID so the device can be recognised.
  byte mac [6];
  String macID;
  WiFi.macAddress(mac);
  for(int looper = 0; looper < 6; looper++){
    if(looper==0){
      macID = String(mac[looper], HEX);
    } else {
      macID = macID + String(mac[looper], HEX);
    }
  }
  singleString = macID + "|" + singleString;
  float tempC = tempReturn();
  singleString = singleString + "|" + String(tempC,2);
  LoRa.beginPacket();
//  LoRa.println(macID);
  LoRa.println(singleString);
//  LoRa.println(cName);
//  LoRa.println(location);
//  LoRa.println(medical);
//  LoRa.println(fire);
//  LoRa.println(collapse);
//  LoRa.println(message);
//  LoRa.println(gps);
  LoRa.endPacket();
}

float tempReturn(){
  long a = 1023 - analogRead(tempPin);
  float tempC = beta/(log((1025.0*10/a-10)/10)+beta/298.0) - 273.0;
  return tempC;
}
