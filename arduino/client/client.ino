#include <espnow.h>
#include "ESP8266WiFi.h"

uint8_t receiverAddress[] = {0x94, 0x3C, 0xC6, 0x33, 0x68, 0x02};

typedef struct messageToBeSent {
    char text[64];
    int intVal;
    float floatVal;
} messageToBeSent;

typedef struct receivedMessage {
    char text[64];
    long runTime;
} receivedMessage;

messageToBeSent myMessageToBeSent; 
receivedMessage myReceivedMessage; 

void messageSent(uint8_t *macAddr, uint8_t status) {
    Serial.print("Send status: ");
    if(status == 0){
        Serial.println("Success");
    }
    else{
        Serial.println("Error");
    }
}

void messageReceived(uint8_t* macAddr, uint8_t* incomingData, uint8_t len){
    memcpy(&myReceivedMessage, incomingData, sizeof(myReceivedMessage));
    Serial.printf("Incoming Message from: %02X:%02X:%02X:%02X:%02X:%02X \n\r", 
            macAddr[0], macAddr[1], macAddr[2], macAddr[3], macAddr[4], macAddr[5]);    
    Serial.print("Message: ");
    Serial.println(myReceivedMessage.text);
    Serial.print("RunTime [s]: ");
    Serial.println(myReceivedMessage.runTime);
    Serial.println();
}

void setup(){
    Serial.begin(115200);
    // delay(1000); // uncomment if your serial monitor is empty
    WiFi.mode(WIFI_STA);
    
    if (esp_now_init() == 0) {
        Serial.println("ESPNow Init success");
    }
    else {
        Serial.println("ESPNow Init fail");
        return;
    }

    esp_now_set_self_role(ESP_NOW_ROLE_COMBO);
    uint8_t result = esp_now_add_peer(receiverAddress, ESP_NOW_ROLE_COMBO, 0, NULL, 0);
    if(result != 0){
        Serial.println("Failed to add peer");
    }
    
    esp_now_register_send_cb(messageSent);  
    esp_now_register_recv_cb(messageReceived); 

   
}
 
void loop(){
    char textMsg[] = "Hi, here's my data for you: ";
    memcpy(&myMessageToBeSent.text, textMsg, sizeof(textMsg));
    myMessageToBeSent.intVal = 4242;
    myMessageToBeSent.floatVal = 42.42;
    esp_now_send(receiverAddress, (uint8_t *) &myMessageToBeSent, sizeof(myMessageToBeSent));
    delay(5000);
}