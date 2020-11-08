#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <InfluxDb.h> // https://github.com/tobiasschuerg/InfluxDB-Client-for-Arduino <-manually import
#include <DHT.h>
//Define Variables
#define ESP_DEVICENAME "dht-11-devel" //Name to display for device in Influx Database
#define INFLUXDB_HOST "192.168.1.8"   //Enter IP of device running Influx Database
#define INFLUXDB_PORT "8086"          //Enter the port # of InfluxDB (This does nothing atm)
#define INFLUXDB_DATABASE "sensors"   //Enter the Influx Database name
#define INFLUXDB_USER "test"          //Enter the Influx Database Username
#define INFLUXDB_PASS "test"          //Enter the Influx Database Password
#define WIFI_SSID "some"              //Enter SSID of your WIFI Access Point
#define WIFI_PASS "wifi"              //Enter Password of your WIFI Access Point
#define DHTTYPE DHT11                 //Define the sensor model type: DHT 11, DHT22 or DHT21
#define DHT_DPIN 2                    //Define GPIO pin for data (for nodeMCU ESP8266 PIN2 is GPIO D4)
//Start Setup
ESP8266WiFiMulti WiFiMulti;
Influxdb influx(INFLUXDB_HOST);
DHT dht(DHT_DPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  WiFiMulti.addAP(WIFI_SSID, WIFI_PASS);
  Serial.print("Connecting to WIFI");
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.print(".");
    delay(1000);
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  influx.setDbAuth (INFLUXDB_DATABASE, INFLUXDB_USER, INFLUXDB_PASS);
 // influx.setDb (INFLUXDB_DATABASE); // This can be used instead of the line above if auth is disabled on the database
  Serial.println("Setup/Boot Complete.");
  dht.begin();
}

void loop() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
    if (isnan(t)) {
      Serial.println("Failed to read from DHT sensor!");
      delay(5000);
      return;
 
  }

 // create a measurement object for temperature
InfluxData measurement ("temperature");
//measurement.addTag("device", d2);
measurement.addTag("sensor", ESP_DEVICENAME);
measurement.addValue("value", t);

// write temperature measurement into db
influx.write(measurement);

 // create a measurement object for humidity
InfluxData measurement2 ("humidity");
//measurement2.addTag("device", d2);
measurement2.addTag("sensor", ESP_DEVICENAME);
measurement2.addValue("value", h);

// write humidity measurement into db
influx.write(measurement2);
  
// print measurements to serial for visibility
    Serial.print("Writing to InfluxDB = ");
    Serial.print(INFLUXDB_DATABASE);
    Serial.print(" @ ");
    Serial.println(INFLUXDB_HOST);
 delay(5000);
    Serial.print("temperature = ");
    Serial.print(t); 
    Serial.println("C  ");
    Serial.print("humidity = ");
    Serial.print(h);
    Serial.println("%   ");
 delay(2000);
}

