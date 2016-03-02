#include <ESP8266WiFi.h>
#include <restclient.h>

// Global variables
#define ESP8266_LED 5
#define ESP8266_BTN1 0
#define ESP8266_BTN2 4
#define ESP8266_BTN3 13
#define ESP8266_BTN4 12

// Wireless Network Configuration
const char WIFI_SSID[] = "YOUR_WIFI_SSID";
const char WIFI_PSK[] = "YOUR_WIFI_PASSWORD";

// Hue Configuration
const char HUE_IP[] = "YOUR.HUE.BRIDGE.IP";
const  int HUE_PORT = 80;
restclient hue(HUE_IP,HUE_PORT);

const char LIGHTS_ON[] = "{\"on\":true}";
const char LIGHTS_OFF[] = "{\"on\":false}";
const char EFFECT_COLORLOOP[] = "{\"effect\":\"colorloop\"}";
const char EFFECT_NONE[] = "{\"effect\":\"none\"}";

// App Configuration
bool btn1 = false;
bool btn2 = false;
bool btn3 = false;
bool btn4 = false;
bool party_on = false;
int duration = 0;
int button_press = 0;

void blinkLED(int dlay, int count = 1)
{
  for (int c=0; c < count; c++) {
    delay(dlay);
    digitalWrite(ESP8266_LED, HIGH);
    delay(dlay);
    digitalWrite(ESP8266_LED, LOW);
  }
}

void setup() 
{
  pinMode(ESP8266_LED, OUTPUT);
  pinMode(ESP8266_BTN1, INPUT_PULLUP);
  pinMode(ESP8266_BTN2, INPUT_PULLUP);
  pinMode(ESP8266_BTN3, INPUT_PULLUP);
  pinMode(ESP8266_BTN4, INPUT_PULLUP);

  // Set WiFi mode to station (client)
  WiFi.mode(WIFI_STA);

  // Initiate connection with SSID and PSK
  WiFi.begin(WIFI_SSID, WIFI_PSK);

  // Blink LED while we wait for WiFi connection
  while (WiFi.status() != WL_CONNECTED) blinkLED(100);
  if (WiFi.status() != WL_CONNECTED) while(1) blinkLED(1000);
}

void loop() 
{
  btn1 = (digitalRead(ESP8266_BTN1) == LOW);
  btn2 = (digitalRead(ESP8266_BTN2) == LOW);
  btn3 = (digitalRead(ESP8266_BTN3) == LOW);
  btn4 = (digitalRead(ESP8266_BTN4) == LOW);
  duration = 1;
  button_press = 0;
  analogWrite(ESP8266_LED, 0);

  while (btn1 || btn2 || btn3 || btn4) {
    analogWrite(ESP8266_LED, duration * 100);
    if (btn1) button_press = 1;
    else if (btn2) button_press = 2;
    else if (btn3) button_press = 3;
    else if (btn4) button_press = 4;
    duration = (duration > 9) ? 10 : ++duration;
    btn1 = (digitalRead(ESP8266_BTN1) == LOW);
    btn2 = (digitalRead(ESP8266_BTN2) == LOW);
    btn3 = (digitalRead(ESP8266_BTN3) == LOW);
    btn4 = (digitalRead(ESP8266_BTN4) == LOW);
    delay(75);
  }

  if (1 < duration && duration < 10) { // short press
    if (button_press == 1)
      hue.put("/api/4ebe05cb477824dd933911f54ebe878/lights/1/state",LIGHTS_ON);  // Turn on light 1
    else if (button_press == 2)
      hue.put("/api/4ebe05cb477824dd933911f54ebe878/groups/1/action",LIGHTS_ON); // Turn on light group 1
    else if (button_press == 3)
      hue.put("/api/4ebe05cb477824dd933911f54ebe878/lights/3/state",LIGHTS_ON);  // Turn on light 3
    else if (button_press == 4)
      hue.put("/api/4ebe05cb477824dd933911f54ebe878/lights/2/state",LIGHTS_ON);  // Turn on light 2
  }
  else if (duration >= 10) { // long press
    if (button_press == 1)
      hue.put("/api/4ebe05cb477824dd933911f54ebe878/groups/0/action",LIGHTS_OFF);  // Turn off all lights
    else if (button_press == 2)
      hue.put("/api/4ebe05cb477824dd933911f54ebe878/groups/1/action",LIGHTS_OFF); // Turn off light group 1
    else if (button_press == 3)
      hue.put("/api/4ebe05cb477824dd933911f54ebe878/lights/3/state",LIGHTS_OFF);  // Turn off light 3
    else if (button_press == 4) {
      party_on = !party_on;
      hue.put("/api/4ebe05cb477824dd933911f54ebe878/groups/2/action",(party_on)?LIGHTS_ON : LIGHTS_OFF);
      hue.put("/api/4ebe05cb477824dd933911f54ebe878/groups/2/action",(party_on)?EFFECT_COLORLOOP : EFFECT_NONE);
    }
  }
}
