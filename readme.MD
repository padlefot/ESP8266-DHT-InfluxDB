Code for the ESP8266 (NodeMCU) with a DHT11 or DHT22/21 sensor to push data to InfluxDB over WiFi.

Can be flashed simply using the Arduino IDE.

Caveat:

Only works with older versions of <Influx.h> due to NTP support in newer versions - tested with v3.0.
