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
            say("Fans off");
            digitalWrite(pin, LOW);
        }
        
        void blow() {
            say("Fans on");
            digitalWrite(pin, HIGH);
        }

};

//-----------------------------------------------------------------------------------//
//-------------------------------------| LIGHTS |------------------------------------//
//-----------------------------------------------------------------------------------//

class Lights {
    int pin1;
    int pin2;
    int pin3;

    public:
        Lights(int _pin1, int _pin2, int _pin3) {
            pin1 = _pin1;
            pin2 = _pin2;
            pin3 = _pin3;
        }

        void setup() {
          pinMode(pin1, OUTPUT);
          pinMode(pin2, OUTPUT);
          pinMode(pin3, OUTPUT);
        }
        
        void on() {
            say("Lights on");
            digitalWrite(pin1, HIGH);
            digitalWrite(pin2, HIGH);
            digitalWrite(pin3, HIGH);
        }
        
        void off() {
            say("Lights off");
            digitalWrite(pin1, LOW);
            digitalWrite(pin2, LOW);
            digitalWrite(pin3, LOW);
        }
        
        void dance(int loops) {
            String str = ("Dancing! ");
            str += loops;
            say(str);
            digitalWrite(pin1, ((loops % 3) == 0));
            digitalWrite(pin2, ((loops % 3) == 1));
            digitalWrite(pin3, ((loops % 3) == 2));
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
                rfSwitch.resetAvailable();
                return (String)rfSwitch.getReceivedValue();
            } else {
                return "";
            }
        }
                
        String mockReceive() {
            if (random(0, 20) <= 1) {
                say("Simulating Fish #2");
                return ("![PLAY 2]");
            } else {
                return "";
            }

        }
};

//-----------------------------------------------------------------------------------//
//--------------------------------------| MAIN |-------------------------------------//
//-----------------------------------------------------------------------------------//

class Music {
    public:
    
        String curStatus = "WAITING";
        
        Music() {
        }

        void setup() {

        }

                
        void play(String set) {
            String str = "![PLAY ";
            str += set;
            str += "]";
            Serial.println(str);
        }
                
        void stop() {
            String str = "![STOP]";
            Serial.println(str);
        }
                
        void idle() {
            String str = "![IDLE]";
            Serial.println(str);
        }
        
        String setStatus() {
            
            String status = "";

            Serial.setTimeout(50);
            status = (String)Serial.readString();
            say("Music status is: ");
            say(status);
            say((String)status.indexOf("PLAYING"));
            say((String)status.indexOf("WAITING"));
            if (status.indexOf("PLAYING") >= 0) {

                curStatus = "PLAYING";
                say("Current Status is: " + curStatus);
            } else if (status.indexOf("WAITING") >= 0) {
                curStatus = "WAITING";
                say("Current Status is: " + curStatus);
            }
            return status;
        }

        bool isPlaying() {
            this->setStatus();
            const bool isPlaying = curStatus != "WAITING";
            say("isPlaying: " + (String)isPlaying);
            return isPlaying;
        }
};

//-----------------------------------------------------------------------------------//
//--------------------------------------| MAIN |-------------------------------------//
//-----------------------------------------------------------------------------------//

RF      rf      = RF(A0); //A0 is the rf listener pin
Fans    fans    = Fans(0); //D0 is the fans switch
Music   music   = Music();
Lights  lights  = Lights(1,2,3); //three lights on pins D2, D2, D3

const int MAX_LOOP = 1*2*3*4*5*6*7*8*9*10;
int loops = 0;

String zeFish;

void setup(){
    rf.setup();
    fans.setup();
    music.setup();
    lights.setup();
}


void loop() {
    
    if (music.isPlaying()) {
        say("Music still playing...");
        fans.blow();
        lights.dance(loops);
    } else {
        zeFish = rf.mockReceive();
        if (zeFish.length() > 0) {
            say("Received fish: " + zeFish);
            music.play(zeFish);
            fans.blow();
            lights.dance(loops);
        } else {
            say("No fishes...");
            music.idle();
            fans.relax();
            lights.off();
        }
    }
    
    loops = (loops + 1) % MAX_LOOP;
    delay(200);
}

void say(String toSay) {
    Serial.println(" <>===<><>---------->> " + toSay);
    delay(10);
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
