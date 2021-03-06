/*
 *  This sketch is for detection of PM 2.5 along with Project SensorWeb.
 *
 *  Hardware Requirments:
 *  1. NodeMCU
 *  2. Plantower PMS5003 Dust Sensor
 *
 *  **Project SensorWeb** is a crowdsourcing sensor network by Mozilla. 
 *  For more information, visit https://sensorweb.io/pm25/.
 */

#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

const char *SSID = "SSID";
const char *PASS = "password";
const String HOST = "http://example.com";
const String PATH = "/path/to/api";

const String API = HOST + PATH; 

HTTPClient http;

unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

void setup() 
{
    Serial.begin(9600);

    WiFi.begin(SSID, PASS);
    while (WiFi.status() != WL_CONNECTED) {
        delay(2000);
        Serial.print(".");
    }

    Serial.println();
    Serial.println("Wi-Findex connected.");
}

void loop() {

    int index = 0;
    char high;
    char low;

    while (Serial.available()) {

        low = Serial.read();

        // Check Start byte
        if ((index == 0 && low != 0x42) || (index == 1 && low != 0x4d)) {
            Serial.println("Start byte not found.");
            break;
        }

        // Shift high-order byte
        if (index == 10 || index == 12 || index == 14)
            high = low << 8;

        if (index == 11) 
            pm1 = high + low;
        
        if (index == 13)
            pm2_5 = high + low;

        if (index == 15)
            pm10 = high + low;

        if (index > 15)
            break;

        index++;
    }

    // Clear ongoing serial buffer
    while (Serial.available()) 
        Serial.read();

    Serial.println();
    Serial.printf("{ pm1: %u, pm2.5: %u, pm10: %u }", pm1, pm2_5, pm10);
    Serial.println();

    // Post to SensorWeb API
    String payload = "pm2=test1&test2=ArduinoIsFun";
    http.begin(API);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    http.POST(payload);

    // Write response to Serial
    http.writeToStream(&Serial);
    http.end();

    delay(30000);
}
