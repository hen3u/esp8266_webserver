#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include "secrets.h"

#define ssid      SECRET_SSID       // WiFi SSID
#define password  SECRET_PASS       // WiFi password

#define LED       1
String  state = "OFF";

ESP8266WebServer server(80);

String getPage(){
  String page = "<html lang='fr'><head><meta http-equiv='refresh' content='60' name='viewport' content='width=device-width, initial-scale=1'/>";
  page += "<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css'><script src='https://ajax.googleapis.com/ajax/libs/jquery/3.1.1/jquery.min.js'></script><script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js'></script>";
  page += "<title>ESP8266 - bootstrap</title></head><body>";
  page += "<div class='container-fluid'>";
  page +=   "<div class='row'>";
  page +=     "<div class='col-md-12'>";
  page +=       "<h1>ESP8266 bootstrap interface</h1>";
  page +=       "<h3>GPIO</h3>";
  page +=       "<div class='row'>";
  page +=         "<div class='col-md-4'><h4 class ='text-left'>GPIO1 ";
  page +=           "<span class='badge'>";
  page +=           state;
  page +=         "</span></h4></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='GPIO1' value='1' class='btn btn-success btn-lg'>ON</button></form></div>";
  page +=         "<div class='col-md-4'><form action='/' method='POST'><button type='button submit' name='GPIO1' value='0' class='btn btn-danger btn-lg'>OFF</button></form></div>";
  page +=       "</div>";
  page += "</div></div></div>";
  page += "</body></html>";
  return page;
}
void handleRoot(){
  if (server.hasArg("GPIO1")) {
    updateGPIO(1,server.arg("GPIO1"));
  } else {
    server.send(200, "text/html", getPage());
  }
}

void updateGPIO(int gpio, String gpioName) {
  Serial.println("");
  Serial.println("Update GPIO ");
  Serial.print(gpio);
  Serial.print(" -> ");
  Serial.println(gpioName);

  if (gpioName == "1") {
    digitalWrite(gpio, HIGH);
    state = "On";
    server.send(200, "text/html", getPage());
  } else if (gpioName == "0") {
    digitalWrite(gpio, LOW);
    state = "Off";
    server.send(200, "text/html", getPage());
  } else {
    Serial.println("Err Led Value");
  }
}

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid,password);

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  server.on("/", handleRoot);

  server.begin();
  Serial.println ( "HTTP server started" );
}

void loop() {
  server.handleClient();
}
