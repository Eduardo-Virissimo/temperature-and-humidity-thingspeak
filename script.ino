#include "WiFi.h"
#include "HTTPClient.h"
#include "DHTesp.h"

const char* ssid = "Wokwi-GUEST";
const char* password = "";
const int DHT_PIN = 15;

DHTesp dhtSensor;

void setup() {

  Serial.begin(115200);
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  pinMode(12, OUTPUT);
  pinMode(13, OUTPUT);
  Serial.println("Hello, ESP32!");

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("Conectando...");

}

void loop() {
  TempAndHumidity data = dhtSensor.getTempAndHumidity();

  HTTPClient http;

  //String de conexão
  String url = "https://api.thingspeak.com/update?api_key=91QWY2BZHZR0AAQH";
  url = url + "&field1=" + float(data.temperature) + "&field2=" + float(data.humidity);

  http.begin(url);

  int httpCode = http.GET();

  Serial.println(httpCode);
  if (httpCode >= 200 && httpCode <= 299) {
    String payload = http.getString();
    Serial.println(payload);
  } else {
    Serial.println("Problemas ao conectar API.");
  }

  //Caso a temperatura ultrapasse 35°C
  if (data.temperature >= 35) {
    digitalWrite(13, HIGH);
  } else {
    digitalWrite(13, LOW);
  }

  //Caso a umidade ultrapasse 70%
  if (data.humidity >=	70) {
    digitalWrite(12, HIGH);
  } else {
    digitalWrite(12, LOW);
  }

  Serial.println("|  Temperatura atual: " + String(data.temperature, 1) + "°C    |");
  Serial.println("|     Umidade atual: " + String(data.humidity, 1) + "%      |");
  Serial.println("<------------------------------->");


  delay(5000); // delay de 5 segundos
}