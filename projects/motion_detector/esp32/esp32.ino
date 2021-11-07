/*
 *  This sketch demonstrates how to scan WiFi networks.
 *  The API is almost the same as with the WiFi Shield library,
 *  the most obvious difference being the different file you need to include:
 */
#include "WiFi.h"
#include <stdio.h>
#include <HTTPClient.h>

#define timeSeconds 10
#define MY_SSID "NETGEAR27_EXT"
#define PASSWORD "greenorchestra821"
#include "soc/rtc_wdt.h"


#define MY_URL String("https://webhook.site/c9c462fd-4069-47e1-a524-fa8dbde80e34")
const int motionSensor = 27;

/* We use a flag to indicate when the sensor movement was detected because 
 * we have to send the HTTP request outside of the interrupt handler
*/
int sensor = 0;

// Performs a GET request to the endpoint appending the specified sensor
void do_request(String sensor) {
  String request_string =  MY_URL + String("?") +  sensor;
  HTTPClient http;
  http.begin(request_string);
  Serial.println(request_string);
  int httpCode = http.GET();  
    if (httpCode > 0) { //Check for the returning code
        String payload = http.getString();
      }
    Serial.println(httpCode);
    http.end(); //Free the resources
}

// Checks if motion was detected, sets LED HIGH and starts a timer
void IRAM_ATTR detectsMovement() {
  Serial.println("MOTION DETECTED!!!");
  digitalWrite(LED_BUILTIN, HIGH);
  
  // Interupt handler only raises the flag to send the sensor event
  sensor = 1;
}

void setup()
{  
    Serial.begin(115200);
    // Set WiFi to station mode and disconnect from an AP if it was previously connected
    WiFi.mode(WIFI_STA);
    WiFi.begin(MY_SSID, PASSWORD);
    if(WiFi.waitForConnectResult() != WL_CONNECTED) {
         Serial.println("Wifi Connection failed"); 
    }
    WiFi.printDiag(Serial);
    
    Serial.println("Setup done");
    pinMode (LED_BUILTIN, OUTPUT);

    // PIR Motion Sensor mode INPUT_PULLUP
    pinMode(motionSensor, INPUT_PULLUP);
    // Set motionSensor pin as interrupt, assign interrupt function and set RISING mode
    attachInterrupt(digitalPinToInterrupt(motionSensor), detectsMovement, RISING);

    // Set LED to LOW
    digitalWrite(LED_BUILTIN, LOW);

  
}

void loop()
{
    // If the sensor flag is up then we send the request and reset the flag
    if (sensor > 0) {
      do_request( sensor == 1 ? "sensor_1" : "sensor_2");    
      sensor = 0;
    }
    // Wait a bit before looping
    delay(3000);

}
