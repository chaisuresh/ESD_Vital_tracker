#include "nRF24L01-STM.h"
#include "RF24-STM.h"

const int pinCE = 8;
const int pinCSN = 10;
bool display = true;

RF24 wirelessSPI(pinCE, pinCSN);

const uint64_t pAddress = 0x68;

void setup(){
  Serial.begin(115200);
  wirelessSPI.begin();
  wirelessSPI.setPALevel(RF24_PA_LOW);
  wirelessSPI.setDataRate(RF24_2MBPS);
  wirelessSPI.openReadingPipe(1, pAddress);
  wirelessSPI.startListening();
}

void loop(){
char data[24] = "";


  if(display) {
    
    display = false;
    Serial.println("/------------------------------------------------------------------\\");
    Serial.println("|                          VITAL TRACKER                           |");
    Serial.println("\\------------------------------------------------------------------/");
//    Serial.println("                  ---------------MENU---------------                ");
    Serial.println(" PRESS ANY NUMBER TO KNOW YOU'RE ALIVE");
  }
char rx_byte = NULL;
  if (Serial.available() > 0) {    // is a character available?
        rx_byte = Serial.read();       // get the character
    // check if a number was received
    if ((rx_byte >= '0') && (rx_byte <= '9')) {
  if(wirelessSPI.available()){
    int count  = 0;
    Serial.println("Initializing...");
    delay(5000);
    while(count < 2) {
      count++;
    wirelessSPI.read(&data, sizeof(data));
    Serial.println(String(data));
    }
  }
    display = true;
    }
  }
}
