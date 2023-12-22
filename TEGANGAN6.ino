#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "ZMPT101B.h"



const char* ssid = "Cilok";
const char* password = "sodaGembirah";
const char* host ="192.168.0.111";

ZMPT101B voltageSensor(A0);

#define PIN_LED 5
#define ledDrop 15
#define ledOver 13

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(ledDrop, OUTPUT);
  pinMode(ledOver, OUTPUT);

  WiFi.hostname("NodeMCU");
  WiFi.begin(ssid,password);

  voltageSensor.calibrate();
  Serial.println("Done!");

  while(WiFi.status () != WL_CONNECTED)
  {
    digitalWrite(PIN_LED, LOW);
    Serial.print (".");
    delay(500);
  
}
  digitalWrite(PIN_LED, HIGH);
  Serial.println("wifi tersambung");
}

void loop() {
float U = voltageSensor.getVoltageAC() * 2;
Serial.println(String("U = ") + U + " V");

WiFiClient client ;
if (!client.connect (host, 80))
{
  Serial.print("connection failed");
  return ;
}

String Link ;
HTTPClient http;
Link ="http://192.168.0.111/monitoringtegangan/kirimdata.php?sensor=" + String( U );
http.begin(client,Link);
http.GET();
http.end();
delay(1000);

  if (U < 190) {
    digitalWrite(ledOver, LOW);
    digitalWrite(ledDrop, HIGH);
  } else if (U > 239 ){
    digitalWrite(ledOver, HIGH);
    digitalWrite(ledDrop, LOW);
  } else {
    digitalWrite(ledOver, LOW);
    digitalWrite(ledDrop, LOW);
  }
}
