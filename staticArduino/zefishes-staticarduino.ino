// This #include statement was automatically added by the Particle IDE.
#include <RCSwitch.h>

#define rfReceivePin A0  //RF Receiver pin = Analog pin 0
#define ledPin 7        //Onboard LED = digital pin 13

unsigned int data = 0;   // variable used to store received data
const unsigned int upperThreshold = 70;  //upper threshold value
const unsigned int lowerThreshold = 50;  //lower threshold value

RCSwitch mySwitch = RCSwitch();

void setup(){
    
    pinMode(ledPin, OUTPUT);
    Serial.begin(9600);

    mySwitch.enableReceive(A0);  // Receiver on interrupt 0 => that is pin #2
    
}

unsigned int loops = 0;

void loop() {
    
    if (mySwitch.available()) {
        // Serial.println(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
        Serial.print(loops);
        Serial.print(") ------------------| RECEIVING: ");
        Serial.println(mySwitch.getReceivedValue());
        mySwitch.resetAvailable();
    } else {
        Serial.print(loops);
        Serial.println(") ...................");
    }
    delay(250);
    loops++;

    // data = analogRead(rfReceivePin);    //listen for data on Analog pin 0
   
    // if (data > upperThreshold){
    //     digitalWrite(ledPin, LOW);   //If a LOW signal is received, turn LED OFF
    //     Serial.print("............ ");
    //     Serial.println(data);
    // }

    // if (data < lowerThreshold){
    //     digitalWrite(ledPin, HIGH);   //If a HIGH signal is received, turn LED ON
    //     Serial.print("**** RECEIVING: ");
    //     Serial.println(data);
    // }

    // delay(100);
}