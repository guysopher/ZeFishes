const int MAX_LOOP = 1*2*3*2*5*7*2*3;
const int DELAY = 50;
int loops = 0;

String tmp = "";
String zeFish;

bool isEveryMillis(int m, int is) {
    return ((loops * DELAY) % m) == is;    
}

void say(String toSay) {
    if (isEveryMillis(2000, 0)) {
        Serial.println("[Arduino] " + toSay);
        delay(10);
    }
}

//-----------------------------------------------------------------------------------//
//--------------------------------------| LEDS |-------------------------------------//
//-----------------------------------------------------------------------------------//

class Leds {

    int fastPin;
    int slowPin;

    public:
        Leds(int _slowPin, int _fastPin) {
            slowPin = _slowPin;
            fastPin = _fastPin;
        }

        void setup() {
            pinMode(slowPin, OUTPUT);
            pinMode(fastPin, OUTPUT);
        }
        
        void party() {
            digitalWrite(fastPin, LOW);
            digitalWrite(slowPin, HIGH);
        }

        void sea() {
            digitalWrite(slowPin, LOW);
            digitalWrite(fastPin, HIGH);
        }
        
        void stop() {
            digitalWrite(slowPin, HIGH);
            digitalWrite(fastPin, HIGH);
        }

};

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
          pinMode(3, OUTPUT);
        }
        
        void relax() {
             say("Fans off");
            digitalWrite(3, LOW);
        }
        
        void blow() {
             say("Fans on");
            digitalWrite(3, HIGH);
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
          pinMode(4, OUTPUT);
          pinMode(5, OUTPUT);
          pinMode(6, OUTPUT);
        }
        
        void on() {
             say("Lights on");
            digitalWrite(4, HIGH);
            digitalWrite(5, HIGH);
            digitalWrite(6, HIGH);
        }
        
        void off() {
             say("Lights off");
            digitalWrite(4, LOW);
            digitalWrite(5, LOW);
            digitalWrite(6, LOW);
        }
        
        void dance() {
            if (isEveryMillis(2000, 0)) {
                this-> on();
            } else if (isEveryMillis(2000, 1000)) {
                this->off();
            }
        }

};


//-----------------------------------------------------------------------------------//
//-------------------------------------| ENGINE |------------------------------------//
//-----------------------------------------------------------------------------------//

class Engine {
    int rpmPin1;
    int rpmPin2;
    int pwmPin;
    
    int curPwm = 0;
    int targetPwm = 0;
    // const int maxRpm = 60; //the maximum wanted RPM
    // const int maxPwm = 150; //the pwm signal that generates the maxRpm

    // const int pwmStep = 1; //the "jumps" in pwm signal to generate the spin
    
    public:
        Engine(int _pwmPin, int _rpmPin1, int _rpmPin2) {
            pwmPin = _pwmPin;
            rpmPin1 = _rpmPin1;
            rpmPin2 = _rpmPin2;
        }

        void setup() {
          pinMode(pwmPin, OUTPUT);
        //   pinMode(rpmPin, INPUT);
        }
        
        void spinAt(int pin) {
            
            targetPwm = map(analogRead(pin), 0, 4095, 0, 255);

            if (isEveryMillis(20, 0)) {
                if (targetPwm > curPwm) {
                    curPwm++;
                } else if (targetPwm < curPwm) {
                    curPwm--;
                }
            }

            say("Spinning for pwm: " + (String)targetPwm);
            say("Spinning at pwm: " + (String)curPwm);
            say("Spinning according to read: " + (String)analogRead(pin));
            
            analogWrite(pwmPin, curPwm);
        }
        
        void slow() {
            this->spinAt(rpmPin1);
        }
        
        void fast() {
            this->spinAt(rpmPin2);
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
            String str = "";
            str = str + "RF Pin is: ";
//            str = str + pin;
//            say(str);
        }

        void setup() {
            rfSwitch.enableReceive(0);  // Receiver on interrupt 0 => that is pin #2
        }

                
        void clear() {
            if (rfSwitch.available()) {
                rfSwitch.resetAvailable();
            }
        }
                
        String receive() {
            if (rfSwitch.available()) {
                const String val = (String)rfSwitch.getReceivedValue();
                say("Received Fish #" + val);
                rfSwitch.resetAvailable();
                return val;
            } else {
                return "";
            }
        }
                
        String mockReceive() {
            if (random(0, 20) <= 1) {
                say("Simulating Fish #2");
                return ("2");
            } else {
                return "";
            }

        }
};

//-----------------------------------------------------------------------------------//
//--------------------------------------| MUSIC |-------------------------------------//
//-----------------------------------------------------------------------------------//

class Music {
    public:
    
        String curStatus = "WAITING";
        
        const int minPlayTime = 1000 * 3; //minimum play time for music (used to let the system adjust)
        unsigned long lastPlayStart = 0;
        unsigned long lastIdleStart = 0;
        
        Music() {
        }

        void setup() {

        }

                
        void play(String set) {
            lastPlayStart = millis();
            String str = "![PLAY ";
            str += set;
            str += "]";
            Serial.println(str);
        }
                
        void stop() {
            lastPlayStart = 0;
            String str = "![STOP]";
            Serial.println(str);
        }
                
        void idle() {
            lastIdleStart = millis();
            if (this->isPlaying()) {
                String str = "![IDLE]";
                Serial.println(str);
            }
        }
        
        String setStatus() {
            
            String status = "";

            Serial.setTimeout(50);
            status = (String)Serial.readString();
            // say("Music status is: ");
            // say(status);
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
            if ((millis() - lastPlayStart) < minPlayTime) { // is in min play time
                return true;
            } else if ((millis() - lastIdleStart) < minPlayTime) { // is in min idle time
                return false;
            } else {
                this->setStatus();
                const bool isPlaying = curStatus != "WAITING";
                return isPlaying;
            }
        }
};

//-----------------------------------------------------------------------------------//
//--------------------------------------| MAIN |-------------------------------------//
//-----------------------------------------------------------------------------------//

RF          rf      = RF(2); //D2 is the rf listener pin
Leds        leds    = Leds(8, 7); //fixed on D6 pin
Fans        fans    = Fans(3); //D0 is the fans switch
Music       music   = Music();
Lights      lights  = Lights(4,5,6); //three lights on pins D2, D2, D3
Engine      engine  = Engine(A5, A6, A7); //A5 for pwd. A6, A7 for knob read

void setup(){

    Serial.begin(9600);
    
    rf.setup();
    fans.setup();
    music.setup();
    lights.setup();
//    engine.setup();
//    leds.setup();
}

void party() {
    fans.blow();
    lights.dance();
//    engine.fast();
//    leds.party();
}

void chill() {
    music.idle();
    fans.relax();
    lights.on();
//    engine.slow();
//    leds.sea();
}

void loop() {
    
    if (music.isPlaying()) {
        say("Music still playing...");
        rf.clear();
        party();
    } else {
        zeFish = rf.receive();
        if (zeFish.length() > 0) {
            say("Ze Fish" + (String)zeFish);
            music.play(zeFish);
            party();
        } else {
            say("No fishes...");
            chill();
        }
    }
    
    loops = (loops + 1) % MAX_LOOP;
    
    delay(DELAY);
}

