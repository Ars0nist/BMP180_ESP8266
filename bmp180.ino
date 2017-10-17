#include <Adafruit_BMP085.h>
#include <Wire.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>

#define wifi_ssid "NachtDesWissens"
#define wifi_password "androidapp"

#define mqtt_server "192.168.11.8"

#define temp_topic "sensors/station2/temperature"
#define pressure_topic "sensors/station2/airpressure"
#define sealevel_topic "sensors/station2/seaairpressure"

Adafruit_BMP085 bmp;
 
#define ALTITUDE 12.0 //alt of hamburg in m

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  
  if (bmp.begin())
    Serial.println("BMP180 init success");
  else
  {

    Serial.println("BMP180 init fail\n\n");
    while(1); // Pause
  }
  setup_wifi();
  client.setServer(mqtt_server, 1883);
}


void setup_wifi() {
  delay(10);
  // wifi connection
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // optional
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());;
}

void reconnect() {
  // reconnect loop
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 5s delay
      delay(5000);
    }
  }
}

void loop() {

  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  float t = bmp.readTemperature();
  float p = bmp.readPressure();
  float a = bmp.readAltitude(103000);// insert pressure at sea level
  
  client.publish(temp_topic, String(t).c_str(), true);
  client.publish(pressure_topic, String(p).c_str(), true);
  client.publish(sealevel_topic, String(a).c_str(), true);
  
  Serial.println("Temp:" );
  Serial.print(String(t).c_str());
  Serial.println("");
  Serial.println("Press:");
  Serial.println(String(p).c_str());
  Serial.println("");
  Serial.println("Alt:");
  Serial.println(String(a).c_str());
  Serial.println("");
  delay(10000);  // Pause for 10 seconds.
}

