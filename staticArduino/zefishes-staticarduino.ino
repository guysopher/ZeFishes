//-----------------------------------------------------------------------------------//
//--------------------------------------| FANS |-------------------------------------//
//-----------------------------------------------------------------------------------//

class Fans {
    int pin;

    public:
        Fans(int fansPin) {
            pin = fansPin;
        }

        void setup() {
          pinMode(pin, OUTPUT);
        }
        
        void relax() {
            digitalWrite(pin, LOW);
        }
        
        void blow() {
            digitalWrite(pin, HIGH);
        }

};

//-----------------------------------------------------------------------------------//
//--------------------------------------| RF |-------------------------------------//
//-----------------------------------------------------------------------------------//
#include <RCSwitch.h>

class RF {
    int pin;

    unsigned int data = 0;   // variable used to store received data
    unsigned int loops = 0;
    const unsigned int upperThreshold = 70;  //upper threshold value
    const unsigned int lowerThreshold = 50;  //lower threshold value
    
    RCSwitch rfSwitch = RCSwitch();
    
    public:
        RF(int rfPin) {
            pin = rfPin;
        }

        void setup() {
            rfSwitch.enableReceive(pin);  // Receiver on interrupt 0 => that is pin #2
        }

                
        String receive() {
            if (rfSwitch.available()) {
                // Serial.println(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
                // String str = "";
                // str += "------------------| RECEIVING: " + rfSwitch.getReceivedValue();
                // say(str);
                rfSwitch.resetAvailable();
                return rfSwitch.getReceivedValue();
            } else {
                // String str = "";
                // str += "...................";
                // Serial.println(str);
                return "";
            }
        }
                

};

//-----------------------------------------------------------------------------------//
//--------------------------------------| MAIN |-------------------------------------//
//-----------------------------------------------------------------------------------//

class Music {
    public:
        Music() {
        }

        void setup() {

        }

                
        void play(int set) {
            // if (random(0, 1000) <= 1) {
        
                String str = "";
                str += "![PLAY 2]";
                Serial.println(str);
        
            // } else {
            
            //     if (rfSwitch.available()) {
            //         // Serial.println(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
            //         String str = "";
            //         str += "------------------| RECEIVING: " + rfSwitch.getReceivedValue();
            //         say(str);
            //         rfSwitch.resetAvailable();
            //     } else {
            //         String str = "";
            //         str += "...................";
            //         Serial.println(str);
            //     }
            // }
            
        }
                

};

//-----------------------------------------------------------------------------------//
//--------------------------------------| MAIN |-------------------------------------//
//-----------------------------------------------------------------------------------//

Fans fans = Fans(0); //D0 is the fans switch
RF rf = RF(A0); //D0 is the fans switch

void setup(){
    fans.setup();
    rf.setup();
}


void loop() {

    rf.receive();        
    delay(250);
}

void say(String toSay) {
    Serial.println(toSay);
}


// //--------------------------------------| MAIN |-------------------------------------//
// // This #include statement was automatically added by the Particle IDE.
// #include <RCSwitch.h>

// #define rfReceivePin A0  //RF Receiver pin = Analog pin 0
// #define ledPin 7        //Onboard LED = digital pin 13

// unsigned int data = 0;   // variable used to store received data
// const unsigned int upperThreshold = 70;  //upper threshold value
// const unsigned int lowerThreshold = 50;  //lower threshold value

// RCSwitch mySwitch = RCSwitch();
// Fans fans = Fans(0); //D0 is the fans switch

// void setup(){
    
//     fans.setup();
//     pinMode(ledPin, OUTPUT);
//     Serial.begin(9600);

//     mySwitch.enableReceive(A0);  // Receiver on interrupt 0 => that is pin #2
    
// }

// unsigned int loops = 0;

// void loop() {
    
//     if (random(0, 10) <= 1) {

//         String str = "";
//         str += loops;
//         str += ") ![PLAY 5]";
//         say(str);

//     } else {
    
//         if (mySwitch.available()) {
//             // Serial.println(mySwitch.getReceivedValue(), mySwitch.getReceivedBitlength(), mySwitch.getReceivedDelay(), mySwitch.getReceivedRawdata(),mySwitch.getReceivedProtocol());
//             String str = "";
//             str += loops + ") ------------------| RECEIVING: " + mySwitch.getReceivedValue();
//             say(str);
//             mySwitch.resetAvailable();
//         } else {
//             String str = "";
//             str += loops;
//             str += ") ...................";
//             say(str);
//         }
//     }
    
//     delay(250);
//     loops++;
// }

// void say(String toSay) {
//     Serial.println(toSay);
    
//     //Serial.println((String)'[|' + str + '|]');
// }
