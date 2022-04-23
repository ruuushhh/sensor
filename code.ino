/*Temperature and Humidity monitoring system with Thingspeak
 * http://srituhobby.com
 */
 
#include <ESP8266WiFi.h>
#include "DHT.h"

String apiKey = "X25MXCVQ5ODHSQJH";
const char *ssid =  "Rushikesh";
const char *pass =  "12345678";
const char* server = "api.thingspeak.com";

DHT dht(D2, DHT11);
DHT dht1(D3p, DHT11);
WiFiClient client;

void setup() {
  Serial.begin(115200);
  delay(10);
  dht.begin();
  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
}

void loop() {
  float h1 = dht.readHumidity();
  float t1= dht.readTemperature();
  float h2 = dht1.readHumidity();
  float t2= dht1.readTemperature();

  if (isnan(h1) || isnan(t1)) {
    Serial.println("Failed to read from DHT 1 sensor!");
    return;
  }

  if (isnan(h2) || isnan(t2)) {
    Serial.println("Failed to read from DHT 2 sensor!");
    return;
  }

  float temp = ((t1+t2)/2);
  float hum = ((h1+h2)/2);
  if (client.connect(server, 80)) {
    String postStr = apiKey;
    postStr += "&field1=";
    postStr += String(temp);
    postStr += "&field2=";
    postStr += String(hum);
    postStr += "\r\n\r\n";

    client.print("POST /update HTTP/1.1\n");
    client.print("Host: api.thingspeak.com\n");
    client.print("Connection: close\n");
    client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
    client.print("Content-Type: application/x-www-form-urlencoded\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);

    Serial.print("Temperature: ");
    Serial.print(temp);
    Serial.print("\t");
    Serial.print("Humidity: ");
    Serial.println(hum);

  }
  client.stop();
  delay(1000);
}
