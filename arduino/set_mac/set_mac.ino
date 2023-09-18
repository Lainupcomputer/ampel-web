#include <ESP8266WiFi.h>
uint8_t newMacAddress[] = {0x94, 0x3C, 0xC6, 0x33, 0x68, 0x02}; // customize as you wish
void setup(){
      Serial.begin(115200);
      // delay(1000); // uncomment if your serial monitor is empty
      Serial.print("Default ESP Board MAC Address: ");
      Serial.println(WiFi.macAddress());
      
      WiFi.mode(WIFI_STA);
      wifi_set_macaddr(STATION_IF, newMacAddress);
      
      Serial.print("New ESP Board MAC Address:     ");
      Serial.println(WiFi.macAddress());     
}
 
void loop(){}