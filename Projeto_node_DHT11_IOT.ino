#include <DHT11.h>
#include <WiFi.h>
#include <PubSubClient.h>

DHT11 dht11(28);

const char* ssid = "Faill";
const char* password = "Thamblers666";
const char* mqtt_server = "192.168.0.24";
float temperatura;
float umidade;

WiFiClient ESP32_DHT11;
PubSubClient client(ESP32_DHT11);

void setup_wifi() {

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }


  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");

    if (client.connect("ESP32_DHT11")) {
      Serial.println("connected");

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(115200);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  pinMode(2, OUTPUT);
}

void LED_CONEXAO() {
  if (WiFi.status() == WL_CONNECTED) {
    digitalWrite(2, HIGH);
  } else {
    digitalWrite(2, LOW);
  }
}

void READ_DHT11(){
  temperatura = dht11.readTemperature();
  client.publish("DHT_TEMP", String(temperatura).c_str());
  delay(1000);

  umidade = dht11.readHumidity();
  client.publish("DHT_UMI", String(umidade).c_str());


}

void loop() {
  reconnect();
  READ_DHT11();
  LED_CONEXAO();
}
