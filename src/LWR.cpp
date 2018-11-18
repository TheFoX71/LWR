#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>


ESP8266WiFiMulti WiFiMulti;
// Use WiFiClient class to create TCP connections
WiFiClient client;
const uint16_t port = 80;
const char * host = "192.168.54.1"; // ip or dns

int inputPin = D3;  // pushbutton connected to digital pin D3
int val = 0;        // variable to store the read value

// Send HTTP Command to LIMIX shit camera
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
  pinMode(LED_BUILTIN, OUTPUT);   // set onboard LED as output
  pinMode(inputPin, INPUT);       // set pin as input
  
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
  SendCommand("/cam.cgi?mode=camcmd&value=capture");
  SendCommand("/cam.cgi?mode=camcmd&value=capture_cancel");
}


void loop() {
  val = digitalRead(inputPin); // read the input pin
  if (val == HIGH){
    SendCommand("/cam.cgi?mode=camcmd&value=capture");
    SendCommand("/cam.cgi?mode=camcmd&value=capture_cancel");
    for (int i = 0; i < 3; i++){
      digitalWrite(LED_BUILTIN, LOW);
      delay(100);
      digitalWrite(LED_BUILTIN, HIGH);
      delay(100);
    }
  }  
}
