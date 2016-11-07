/*
 *  This sketch blink built-in LED by sending HTTP requests to HTTP WebServer.
 *
 *  You should connect to `blink-nodemcu` WiFi network first. Then touch/click
 *  the whole gray page. The LED will start blinking as you touched.
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *softSSID = "blink-nodemcu";

ESP8266WebServer server(80);
  
void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.begin(115200);
  
  // Configure softAP
  Serial.print("Configuring soft access point...");
  WiFi.mode(WIFI_AP);
  Serial.println(WiFi.softAP(softSSID) ? "Ready" : "Failed!");

  // Setup HTTP Server
  server.on("/", handleOn);
  server.on("/on", handleOn);
  server.on("/off", handleOff);

  server.begin();

  Serial.println("HTTP server started");
}


void loop() 
{
  server.handleClient();
}

void handleOn() 
{
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("LED on");
  
  const char *plainHtml = 
  "<head>"
  "<style> html, body { height: 100vh } </style>"
  "<style> body { background-color: #02BDAD </style>"
  "<script> function action() { window.location.href = '/off'; } </script>"
  "</head>"
  "<body onclick='action()' ontouchstart='action()'></body>";
  
  server.send(200, "text/html", plainHtml);
}

void handleOff()
{
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("LED off");
  
  const char *plainHtml = 
  "<head>"
  "<style> html, body { height: 100vh } </style>"
  "<style> body { background-color: gray </style>"
  "<script> function action() { window.location.href = '/on'; } </script>"
  "</head>"
  "<body onclick='action()' ontouchstart='action()'></body>";
  
  server.send(200, "text/html", plainHtml);
}
