/*
  Thanks much to @corbanmailloux for providing a great framework for implementing flash/fade with HomeAssistant https://github.com/corbanmailloux/esp-mqtt-rgb-led

  To use this code you will need the following dependancies: 
  
  - Support for the ESP8266 boards. 
        - You can add it to the board manager by going to File -> Preference and pasting http://arduino.esp8266.com/stable/package_esp8266com_index.json into the Additional Board Managers URL field.
        - Next, download the ESP8266 dependancies by going to Tools -> Board -> Board Manager and searching for ESP8266 and installing it.
  
  - You will also need to download the follow libraries by going to Sketch -> Include Libraries -> Manage Libraries
      - DHT sensor library 
      - Adafruit unified sensor
      - PubSubClient
      - ArduinoJSON
    
  UPDATE 16 MAY 2017 by Knutella - Fixed MQTT disconnects when wifi drops by moving around Reconnect and adding a software reset of MCU
             
  UPDATE 23 MAY 2017 - The MQTT_MAX_PACKET_SIZE parameter may not be setting appropriately do to a bug in the PubSub library. If the MQTT messages are not being transmitted as expected please you may need to change the MQTT_MAX_PACKET_SIZE parameter in "PubSubClient.h" directly.

*/



#include <ESP8266WiFi.h>
#include <DHT.h>
#include <PubSubClient.h>
#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <ArduinoJson.h>


/************ WIFI and MQTT INFORMATION (CHANGE THESE FOR YOUR SETUP) ******************/
#define wifi_ssid "Bogert-2G" //type your WIFI information inside the quotes
#define wifi_password "gotoronto"
#define mqtt_server "m10.cloudmqtt.com"
#define mqtt_user "itay" 
#define mqtt_password "itaym777"
#define mqtt_port 17307


/************* MQTT TOPICS (change these topics as you wish)  **************************/
#define light_state_topic "homie/multisensor1"
#define light_set_topic "bruh/multisensor1/set"

const char* on_cmd = "ON";
const char* off_cmd = "OFF";

/**************************** FOR OTA **************************************************/
#define SENSORNAME "sensornode1"
#define OTApassword "YouPassword" // change this to whatever password you want to use when you upload OTA
int OTAport = 8266;

/**************************** PIN DEFINITIONS ********************************************/
#define PIRPIN    D5
#define DHTPIN    D7
#define DHTTYPE   DHT22
#define LDRPIN    A0
#define SOUNDPIN  D1
#define SMOKEPIN  D2
#define UNUSED    D3

/**************************** SENSOR DEFINITIONS *******************************************/
float ldrValue;
int LDR;
float calcLDR;
float diffLDR = 25;

float diffTEMP = 0.2;
float tempValue;

float diffHUM = 1;
float humValue;

int pirValue;
int pirStatus;
String motionStatus;

int smokeValue;
int smokeStatus;
String smokeStatusMsg;

int soundValue;
int soundStatus;
String soundStatusMsg;

char message_buff[100];
int calibrationTime = 0;
const int BUFFER_SIZE = 300;
#define MQTT_MAX_PACKET_SIZE 512


/******************************** GLOBALS for fade/flash *******************************/

bool stateOn = false;
bool startFade = false;
unsigned long lastLoop = 0;
int transitionTime = 0;
int loopCount = 0;
int redVal, grnVal, bluVal;




WiFiClient espClient;
PubSubClient client(espClient);
DHT dht(DHTPIN, DHTTYPE);



/********************************** START SETUP*****************************************/
void setup() {

  Serial.begin(115200);

  pinMode(PIRPIN, INPUT);
  pinMode(DHTPIN, INPUT);
  pinMode(LDRPIN, INPUT);

  pinMode(SMOKEPIN, INPUT);
  pinMode(SOUNDPIN, INPUT);
  

  Serial.begin(115200);
  delay(10);

  ArduinoOTA.setPort(OTAport);

  ArduinoOTA.setHostname(SENSORNAME);

  ArduinoOTA.setPassword((const char *)OTApassword);

  Serial.print("calibrating sensor ");
  for (int i = 0; i < calibrationTime; i++) {
    Serial.print(".");
    delay(1000);
  }

  Serial.println("Starting Node named " + String(SENSORNAME));


  setup_wifi();

  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);


  ArduinoOTA.onStart([]() {
    Serial.println("Starting");
  });
  ArduinoOTA.onEnd([]() {
    Serial.println("\nEnd");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total) {
    Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
  });
  ArduinoOTA.begin();
  Serial.println("Ready");
  Serial.print("IPess: ");
  Serial.println(WiFi.localIP());
  reconnect();
}




/********************************** START SETUP WIFI*****************************************/
void setup_wifi() {

  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}



/********************************** START CALLBACK*****************************************/
void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");

  char message[length + 1];
  for (int i = 0; i < length; i++) {
    message[i] = (char)payload[i];
  }
  message[length] = '\0';
  Serial.println(message);

  if (!processJson(message)) {
    return;
  }

  sendState();
}



/********************************** START PROCESS JSON*****************************************/
bool processJson(char* message) {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;

  JsonObject& root = jsonBuffer.parseObject(message);

  if (!root.success()) {
    Serial.println("parseObject() failed");
    return false;
  }
  return true;
}



/********************************** START SEND STATE*****************************************/
void sendState() {
  StaticJsonBuffer<BUFFER_SIZE> jsonBuffer;
  JsonObject& root = jsonBuffer.createObject();

  // root["brightness"] = brightness;
  // root["humidity"] = (String)humValue;
  root["motion"] = (String)motionStatus;
  root["ldr"] = (String)LDR;
  root["smoke"] = (String)smokeStatusMsg;
  root["sound"] = (String)soundStatusMsg;
  // root["temperature"] = (String)tempValue;
  // root["heatIndex"] = (String)calculateHeatIndex(humValue, tempValue);


  char buffer[root.measureLength() + 1];
  root.printTo(buffer, sizeof(buffer));

  Serial.println(buffer);
  client.publish(light_state_topic, buffer, true);
}


/*
 * Calculate Heat Index value AKA "Real Feel"
 * NOAA heat index calculations taken from
 * http://www.wpc.ncep.noaa.gov/html/heatindex_equation.shtml
 */
float calculateHeatIndex(float humidity, float temp) {
  float heatIndex= 0;
  if (temp >= 80) {
    heatIndex = -42.379 + 2.04901523*temp + 10.14333127*humidity;
    heatIndex = heatIndex - .22475541*temp*humidity - .00683783*temp*temp;
    heatIndex = heatIndex - .05481717*humidity*humidity + .00122874*temp*temp*humidity;
    heatIndex = heatIndex + .00085282*temp*humidity*humidity - .00000199*temp*temp*humidity*humidity;
  } else {
     heatIndex = 0.5 * (temp + 61.0 + ((temp - 68.0)*1.2) + (humidity * 0.094));
  }

  if (humidity < 13 && 80 <= temp <= 112) {
     float adjustment = ((13-humidity)/4) * sqrt((17-abs(temp-95.))/17);
     heatIndex = heatIndex - adjustment;
  }

  return heatIndex;
}


/********************************** START RECONNECT*****************************************/
void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect(SENSORNAME, mqtt_user, mqtt_password)) {
      Serial.println("connected");
      client.subscribe(light_set_topic);
      sendState();
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}



/********************************** START CHECK SENSOR **********************************/
bool checkBoundSensor(float newValue, float prevValue, float maxDiff) {
  return newValue < prevValue - maxDiff || newValue > prevValue + maxDiff;
}


/********************************** START MAIN LOOP***************************************/
void loop() {

  ArduinoOTA.handle();
  
  if (!client.connected()) {
    // reconnect();
    software_Reset();
  }
  client.loop();

//  if (!inFade) {

    float newTempValue = dht.readTemperature(true); //to use celsius remove the true text inside the parentheses  
    float newHumValue = dht.readHumidity();

    //PIR CODE
    pirValue = digitalRead(PIRPIN); //read state of the
    smokeValue = digitalRead(SMOKEPIN); //read state of the
    soundValue = digitalRead(SOUNDPIN); //read state of the

    if (pirValue == LOW && pirStatus != 1) {
      motionStatus = "standby";
      sendState();
      pirStatus = 1;
    }

    else if (pirValue == HIGH && pirStatus != 2) {
      motionStatus = "motion detected";
      sendState();
      pirStatus = 2;
    }

    if (smokeValue == LOW && smokeStatus != 1) {
      smokeStatusMsg = "All Clear";
      sendState();
      smokeStatus = 1;
    }
    
    else if (smokeValue == HIGH && smokeStatus != 2) {
      smokeStatusMsg = "SMOKE!";
      sendState();
      smokeValue = 2;
    }

    if (soundValue == LOW && soundStatus != 1) {
      soundStatusMsg = "No Noise";
      sendState();
      soundStatus = 1;
    }
    
    else if (soundValue == HIGH && soundStatus != 2) {
      soundStatusMsg = "Noise!!!";
      sendState();
      soundValue = 2;
    }

    delay(100);

    if (checkBoundSensor(newTempValue, tempValue, diffTEMP)) {
      tempValue = newTempValue;
      sendState();
    }

    if (checkBoundSensor(newHumValue, humValue, diffHUM)) {
      humValue = newHumValue;
      sendState();
    }


    int newLDR = analogRead(LDRPIN);

    if (checkBoundSensor(newLDR, LDR, diffLDR)) {
      LDR = newLDR;
      sendState();
    }

//  }
}




/**************************** START TRANSITION FADER *****************************************/

int calculateStep(int prevValue, int endValue) {
  int step = endValue - prevValue; // What's the overall gap?
  if (step) {                      // If its non-zero,
    step = 1020 / step;          //   divide by 1020
  }

  return step;
}

/* The next function is calculateVal. When the loop value, i,
   reaches the step size appropriate for one of the
   colors, it increases or decreases the value of that color by 1.
   (R, G, and B are each calculated separately.)
*/
int calculateVal(int step, int val, int i) {
  if ((step) && i % step == 0) { // If step is non-zero and its time to change a value,
    if (step > 0) {              //   increment the value if step is positive...
      val += 1;
    }
    else if (step < 0) {         //   ...or decrement it if step is negative
      val -= 1;
    }
  }

  // Defensive driving: make sure val stays in the range 0-255
  if (val > 255) {
    val = 255;
  }
  else if (val < 0) {
    val = 0;
  }

  return val;
}

/****reset***/
void software_Reset() // Restarts program from beginning but does not reset the peripherals and registers
{
Serial.print("resetting");
ESP.reset(); 
}
