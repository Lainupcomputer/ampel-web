#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <EEPROM.h>
#include <ESP8266WebServer.h>
int step = 0;
int last = 0;
const int ledPin = D5;                    // Der Pin, an dem die LED für Wlan angeschlossen ist (interne LED)
char ssid[] = "Ampel_Hotspot";             // Standard-SSID
char password[] = "Mht:=0E,g15v9,G9W%6Y$4vjn!bs";     // Standard-Passwort
char target_ip[] = "192.168.4.1";
                                          // Ampel Steuerung
const int red = D4;                       // Pin Für die Steuerung des Lichtes: Rot
const int yellow = D5;                    // Pin Für die Steuerung des Lichtes: Gelb
const int green = D6;                     // Pin Für die Steuerung des Lichtes: Grün
const int phase = 100;                  // Die länge der Phase bevor automatisch gewechselt wird

ESP8266WebServer server(80);              // Webserver Für Einrichtung

void switch_to_green() {
   // repräsentiert eine schaltsequenz zu Grün
  Serial.print("Switched to step Green");
  digitalWrite(yellow, HIGH);
  delay(100);
  digitalWrite(red, LOW);
  delay(1000);
  digitalWrite(yellow, LOW);
  delay(10);
  digitalWrite(green, HIGH);
  last = 1;
}

void switch_to_red() {
  // repräsentiert eine schaltsequenz zu Rot
  Serial.print("Switched to step Red");
  digitalWrite(yellow, HIGH);
  delay(100);
  digitalWrite(green, LOW);
  delay(100);
  digitalWrite(yellow, LOW);
  digitalWrite(red, HIGH);
  last = 2;
}

void createHotspot() {
  Serial.println("Hotspot-Modus wird aktiviert...");
  
  WiFi.softAP(ssid, password); // SSID und Passwort für den Hotspot
  delay(100); // Warte kurz, bis der Hotspot erstellt wird

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("Hotspot-IP-Adresse: ");
  Serial.println(myIP);

  // Routen für die Konfigurationsseite
  server.on("/", HTTP_GET, [](){
    String html = "<html><body>";
    html += "<h1>WiFi-Konfiguration</h1>";
    html += "<form method='POST' action='/save'>";
    html += "SSID: <input type='text' name='ssid'><br>";
    html += "Passwort: <input type='password' name='password'><br>";
    html += "Link-IP: <input type='text' name='IP'><br>";
    html += "<input type='submit' value='Speichern'>";
    html += "</form>";
    html += "</body></html>";
    server.send(200, "text/html", html);
  });

  server.on("/save", HTTP_POST, [](){
    
  });

  server.begin();
  Serial.println("Server started");

}

void connectToWiFi() {
  Serial.print("Verbindung zum WLAN-Netzwerk: ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);

  int timeout = 20; // Maximale Zeit für die Verbindung in Sekunden
  while (WiFi.status() != WL_CONNECTED && timeout > 0) {
    digitalWrite(ledPin, HIGH); // LED einschalten
    delay(1000);
    Serial.print(".");
    digitalWrite(ledPin, LOW); // LED ausschalten
    delay(1000);
    timeout--;
  }

  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\nVerbunden mit WiFi.");
  } else {
    Serial.println("\nVerbindung fehlgeschlagen.");
    createHotspot();
    Serial.println("\nHotspot Erstellt.");
  }
}

void send_Switch_call() {

}
void setup() {
  pinMode(D4, OUTPUT);
  pinMode(D5, OUTPUT);
  pinMode(D6, OUTPUT);
  
  // Serielle Verbindung
  Serial.begin(115200);
  delay(10);
  Serial.println();
  connectToWiFi();
  


  // Weitere Initialisierung...
}

void loop() {
  Serial.println(step);
  server.handleClient(); // Stelle sicher, dass dieser Aufruf regelmäßig erfolgt
  if (step >= phase) {
    Serial.print("Switched to step");
    Serial.println(step);
    step = 0;
    send_Switch_call();
    if (last == 1) {
      switch_to_red();
    }
    else if (last == 2)
    {
      switch_to_green();
    }
    else{
      digitalWrite(yellow, HIGH);
      delay(100);
      digitalWrite(yellow, LOW);
      last = 1;
    }
  } 
  delay(100);
  step ++;

  // Hier kommt der Hauptcode deines Programms
  // Du kannst die Verbindung zum WLAN überwachen und andere Aufgaben erledigen
}

