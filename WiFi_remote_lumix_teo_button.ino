/*
    This sketch sends a message to a TCP server

*/

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#define BOTTONE 7             // pin di input dove è collegato il pulsante  
int  val = 0;                 // variabile usata per conservare lo stato del pin BOTTONE  
int  status_old = 0;          // variabile usata per conservare lo stato precedente del pin BOTTONE  


ESP8266WiFiMulti WiFiMulti;
// Use WiFiClient class to create TCP connections
WiFiClient client;
const uint16_t port = 80;
const char * host = "192.168.54.1"; // ip or dns

// Send HTTP Command to LUMIX camera
void SendCommand(String command){
  Serial.print("debug: " + command);
  if (!client.connect(host, port)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(15000);
    return;
  }
  Serial.print("Requesting URL: ");
  Serial.println(command);
  // This will send the request to the server
  client.print(String("GET ") + command + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout0 = millis();
  while (client.available() == 0) {
    String line = client.readStringUntil('\n');
    if (line == "\r") {
      Serial.println(line);
    }
  
    if (millis() - timeout0 >100) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      break;
    }
  }
  delay(1000);
  
}



void setup() {
  Serial.begin(115200);
  delay(20);
  pinMode(BOTTONE, INPUT);    // imposta il pin digitale come input  

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
  delay(9000);

  // Initialize camera
  SendCommand("/cam.cgi?mode=camcmd&value=recmode");
  //SendCommand("/cam.cgi?mode=getinfo&type=allmenu");
  //SendCommand("/cam.cgi?mode=getinfo&type=curmenu");

  // take a picture
  // SendCommand("/cam.cgi?mode=camcmd&value=capture");
  // SendCommand("/cam.cgi?mode=camcmd&value=capture_cancel");
}


void loop() {

  val = digitalRead(BOTTONE);  // legge il valore dell'input e lo conserva  
  
  // controlla che l'input sia HIGH (pulsante premuto)  
  if (val == HIGH)
       if (status_old == 1)
          return;
       else
          status_old = 1;
          SendCommand("/cam.cgi?mode=camcmd&value=capture");
          
    
  else  
       if (status_old == 0)
          return;
       else
          status_old = 0;
          SendCommand("/cam.cgi?mode=camcmd&value=capture_cancel");
          
    
  // SendCommand("/cam.cgi?mode=camcmd&value=capture");
  // SendCommand("/cam.cgi?mode=camcmd&value=capture_cancel");
  
}