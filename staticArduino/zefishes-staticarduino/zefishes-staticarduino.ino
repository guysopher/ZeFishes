//-----------------------------------------------------------------------------------//
//--------------------------------------| LEDS |-------------------------------------//
//-----------------------------------------------------------------------------------//

#include <FastLED.h>
FASTLED_USING_NAMESPACE //The FASTLED_USING_NAMESPACE macro will allow your code to work in environments where FastLED is using a namespace vs. places where it isn't. Also, the name of the namespace isn't finalized.

#define NUM_LEDS 60
#define LEDS_PIN 6 //in FastLED pin must be const

class Leds {
    
    CRGB leds[NUM_LEDS];

    public:
        Leds() {
        }

        void setup() {
            FastLED.addLeds<NEOPIXEL, LEDS_PIN>(leds, NUM_LEDS);
        }
        
        void relax() {
            
        }
        
        void dance() {
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
          pinMode(pin, OUTPUT);
        }
        
        void relax() {
            // say("Fans off");
            digitalWrite(pin, LOW);
        }
        
        void blow() {
            // say("Fans on");
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
            // say("Lights on");
            digitalWrite(pin1, HIGH);
            digitalWrite(pin2, HIGH);
            digitalWrite(pin3, HIGH);
        }
        
        void off() {
            // say("Lights off");
            digitalWrite(pin1, LOW);
            digitalWrite(pin2, LOW);
            digitalWrite(pin3, LOW);
        }
        
        void dance(int loops) {
            // String str = ("Dancing! ");
            // str += loops;
            // say(str);
            digitalWrite(pin1, ((loops % 3) == 0));
            digitalWrite(pin2, ((loops % 3) == 1));
            digitalWrite(pin3, ((loops % 3) == 2));
        }

};


//-----------------------------------------------------------------------------------//
//-------------------------------------| ENGINE |------------------------------------//
//-----------------------------------------------------------------------------------//

class Engine {
    int rpmPin;
    int pwmPin;
    
    int curPwm = 0;
    
    const int maxRpm = 60; //the maximum wanted RPM
    const int maxPwm = 150; //the pwm signal that generates the maxRpm

    const int pwmStep = 1; //the "jumps" in pwm signal to generate the spin
    
    public:
        Engine(int _pwmPin, int _rpmPin) {
            pwmPin = _pwmPin;
            rpmPin = _rpmPin;
        }

        void setup() {
          pinMode(pwmPin, OUTPUT);
          pinMode(rpmPin, INPUT);
        }
        
        int getRpm() {
            return analogRead(rpmPin);
            //todo - calc rpm by somthing...
        }
        
        void spinAt(int rpm) {

            rpm = min(maxRpm, rpm);

            say("Spinning at: " + (String)rpm);
            const int curRpm = this->getRpm();
            const int diffRpm = rpm - curRpm;
            if (diffRpm > 0) {
                curPwm += pwmStep;    
            } else if (diffRpm < 0) {
                curPwm -= pwmStep;
            }
            curPwm = max(0, curPwm);
            curPwm = min(maxPwm, curPwm);

            say("Engine RPM is: " + (String)curRpm);
            say("Engine PWM is: " + (String)curPwm);
            
            analogWrite(pwmPin, curPwm);
        }
        
        void slow() {
            this->spinAt(30);
        }
        
        void fast() {
            this->spinAt(15);
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
        int lastPlayStart = 0;
        int lastIdleStart = 0;
        
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
            String str = "![IDLE]";
            Serial.println(str);
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
        // say("Music still playing...");
        rf.clear();
        fans.blow();
        lights.dance(loops);
    } else {
        zeFish = rf.receive();
        if (zeFish.length() > 0) {
            music.play(zeFish);
            fans.blow();
            lights.dance(loops);
        } else {
            // say("No fishes...");
            music.idle();
            fans.relax();
            lights.off();
        }
    }
    
    loops = (loops + 1) % MAX_LOOP;
    delay(200);
}

void say(String toSay) {
    Serial.println("<Arduino> " + toSay);
    delay(10);
}
