#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>
#include "ZMPT101B.h"



const char* ssid = "Cilok";
const char* password = "sodaGembirah";
const char* host ="192.168.0.106";

ZMPT101B voltageSensor(A0);

#define PIN_LED 5
#define ledDrop 15
#define ledOver 13
#define ledNormal 12
#define relay2 14

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  pinMode(ledDrop, OUTPUT);
  pinMode(ledOver, OUTPUT);
  pinMode(ledNormal, OUTPUT);
  pinMode(relay2, OUTPUT);

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

// Serial.println(voltageSensor.getVoltageAC());
Serial.println("analog");
Serial.println(analogRead(A0));

WiFiClient client ;
if (!client.connect (host, 80))
{
  Serial.print("connection failed");
  return ;
}

String Link ;
HTTPClient http;
Link ="http://192.168.0.106/monitoringtegangan/kirimdata.php?sensor=" + String( U );
http.begin(client,Link);
http.GET();
http.end();
delay(1000);

  if (U < 190) {
    digitalWrite(ledNormal, LOW);
    digitalWrite(ledOver, LOW);
    digitalWrite(ledDrop, HIGH);
    digitalWrite(relay2, LOW);
    Serial.println("drop");
    //led menyala kuning
  } else if (U > 239 ){
    digitalWrite(ledNormal, LOW);
    digitalWrite(ledOver, HIGH);
    digitalWrite(ledDrop, LOW);
    digitalWrite(relay2, HIGH);
    Serial.println("over");
    //led menyala merah
  } else {
    digitalWrite(ledNormal, HIGH);
    digitalWrite(ledOver, LOW);
    digitalWrite(ledDrop, LOW);
    Serial.println("normal");
    //led menyala hijau
  }

}
