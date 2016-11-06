/*
 *  This sketch blink built-in LED by sending HTTP requests to HTTP WebServer.
 *
 *  You should connect to `blink-nodemcu` WiFi network first. Then touch/click
 *  the whole gray page. The LED will start blinking as you touched.
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char *ssid = "********";
const char *pass = "********";

const char *softSSID = "blink-nodemcu";

ESP8266WebServer server(80);

void setup()
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  
  Serial.begin(115200);
  
  // Connect to WiFi
  Serial.print("Connecting to WiFi...");

  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(3000);
    Serial.print(".");
  }
  Serial.println();

  Serial.printf("Connected to [%s] at %s", ssid, WiFi.localIP().toString().c_str());
  Serial.println();

  // Configure softAP
  Serial.print("Configuring soft access point...");
  WiFi.mode(WIFI_AP_STA);
  Serial.println(WiFi.softAP(softSSID) ? "Ready" : "Failed!");

  // Setup HTTP Server

  server.on("/", handleRoot);
  //server.on("/", HTTP_POST, handleRootPOST);
  server.begin();

  Serial.println("HTTP server started");
}


void loop() 
{
  server.handleClient();
}

void printWiFiStatus()
{
  Serial.printf("SSID : %s\n", WiFi.SSID().c_str());
  Serial.printf("IP   : %s\n", WiFi.localIP().toString().c_str());
}

void handleRoot() 
{
  char plainHtml[600];

  int isOn = server.arg("light") == "on";
  String checked = isOn ? "checked" : "";
  digitalWrite(LED_BUILTIN, isOn ? LOW : HIGH);
  Serial.println(isOn ? LOW : HIGH);
  
  sprintf(plainHtml, 
  "<head>"
  "<title>Blink with NodeMCU</title>"
  "<style>"
  "  input {"
  "    opacity: 0;"
  "    width: 100%;"
  "    height: 100%;"
  "    position: absolute;"
  "    top: 0;"
  "    left: 0;"
  "  }"
  "</style>"
  "</head>"
  "<body>"
  "<script>"
  "  function myClick(e) {"
  "  e.form.submit();"
  "  document.body.style.backgroundColor = e.checked ? '#02BDAD' : 'gray'; }"
  "</script>"
  "<form method='post'>"
  "<input type='checkbox' name='light' value='on' onchange='myClick(this);' %s></input>"
  "</form>"
  "<script> document.body.style.backgroundColor = %d ? '#02BDAD' : 'gray'; </script>"
  "</body>",
  checked.c_str(), isOn);
  
  server.send(200, "text/html", plainHtml);
}

