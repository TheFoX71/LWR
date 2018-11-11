/*
    This sketch sends a message to a TCP server

*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

ESP8266WiFiMulti WiFiMulti;
// Use WiFiClient class to create TCP connections
WiFiClient client;
const uint16_t port = 80;
const char * host = "192.168.54.1"; // ip or dns
  
void setup() {
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network
  WiFi.mode(WIFI_STA);
  WiFiMulti.addAP("GM1-8F985B", "a1c994c8f985b");

  Serial.println();
  Serial.println();
  Serial.print("Wait for WiFi... ");

  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
 
  
  String cmd1 = "/cam.cgi?mode=camcmd&value=recmode";
  String cmd2 = "/cam.cgi?mode=getinfo&type=allmenu";
  String cmd3 = "/cam.cgi?mode=getinfo&type=curmenu";
  Serial.print("--------------------------------------------cmd1----------------------------------");
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(15000);
    return;
  }
  Serial.print("Requesting URL: ");
  Serial.println(cmd1);
  // This will send the request to the server
  client.print(String("GET ") + cmd1 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout0 = millis();
  while (client.available() == 0) {
    if (millis() - timeout0 >100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      break;
    }
  }
  delay(1000);
  Serial.print("--------------------------------------------cmd2----------------------------------");
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(15000);
    return;
  }
  Serial.print("Requesting URL: ");
  Serial.println(cmd2);
  // This will send the request to the server
  client.print(String("GET ") + cmd2 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout1 = millis();
  while (client.available() == 0) {
    if (millis() - timeout1 >100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      break;
    }
  }
  delay(1000);
  Serial.print("--------------------------------------------cmd3----------------------------------");
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(15000);
    return;
  }
  Serial.print("Requesting URL: ");
  Serial.println(cmd3);
  // This will send the request to the server
  client.print(String("GET ") + cmd3 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout2 = millis();
  while (client.available() == 0) {
    if (millis() - timeout2 >100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      break;
    }
  }
  delay(5000);
}


void loop() {
  Serial.print("connecting to ");
  Serial.println(host);

  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(15000);
    return;
  }
 // We now create a URI for the request
  
  String url1 = "/cam.cgi?mode=camcmd&value=capture";
  String url2 = "/cam.cgi?mode=camcmd&value=capture_cancel";
  
  Serial.print("--------------------------------------------url1----------------------------------");
  Serial.print("Requesting URL: ");
  Serial.println(url1);
  // This will send the request to the server
  client.print(String("GET ") + url1 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout4 = millis();
  while (client.available() == 0) {
    if (millis() - timeout4 >100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      break;
    }
  }
  delay(2000);
  Serial.print("--------------------------------------------url2----------------------------------");

  Serial.println(url2);
  client.print(String("GET ") + url2 + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout5 = millis();
  while (client.available() == 0) {
    if (millis() - timeout5 > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // This will send the request to the server
  client.println("Send this data to server");

  //read back one line from server
  String line = client.readStringUntil('\r');
  Serial.println(line);

  Serial.println("closing connection");
  client.stop();

  Serial.println("wait 5 sec...");
  delay(2000);
}
