#include <RCSwitch.h>

class RF {
    
    int pin;
    RCSwitch rfSwitch = RCSwitch();
    
    public:
        RF(int rfPin) {
            pin = rfPin;
        }

        void setup() {
            rfSwitch.enableTransmit(pin); 
        }

                
        void sendFish(int fish) {
          rfSwitch.send(fish, 24);
          Serial.print("Sending fish: ");
          Serial.println(fish);
        }

};

class Bubbles {
    
    int pin;
    
    public:
        Bubbles(int _pin) {
            pin = _pin;
        }

        void setup() {
            pinMode(pin, OUTPUT); 
        }

                
        void makeBubbles() {
          Serial.println("Making Bubbles!!!");
          digitalWrite(pin, HIGH);
        }

        void stopBubbles() {
          digitalWrite(pin, LOW);
        }

};

#include <Bounce2.h>

class Fishes {

    const int totalFishes = 8;
    int pins[8];
    Bounce bouncers[8];
    
    public:
        Fishes(int fromPin) {
          for (int i = 0; i < totalFishes; i++) {
            pins[i] = fromPin + i;
            bouncers[i] = Bounce( fromPin + i , 5 ); 
          }
        }

        void setup() {
          for (int i = 0; i < totalFishes; i++) {
            pinMode(pins[i], INPUT_PULLUP);
          }
        }

                
        int getFish() {
          int fish = 0;
          String fishes = ("Getting fishes : [");
          for (int i = 0; i < totalFishes; i++) {
            bouncers[i].update();
            fishes += (bouncers[i].read());
            if (bouncers[i].read() == LOW) {
              fish = i + 1;
            }
          }
          fishes += ("]");
          
          if (fish > 0) {
            Serial.print("Found fish! ");
            Serial.print(fish);
            Serial.print(" -> ");
            Serial.println(fishes);
          }
          
          return fish;
        }

};

RF      rf      = RF(10); //D10 is the rf transmitter pin
Fishes  fishes  = Fishes(2); //D2 is the first fish pin (2-9)
Bubbles bubbles = Bubbles(11);

int zeFish = 0;

const int tween = 5 * 1000; //grace period until more fish can be caught

void setup() {

  Serial.begin(9600);

  rf.setup();
  fishes.setup();
  bubbles.setup();
  
}

void loop() {

  zeFish = fishes.getFish();

  if (zeFish > 0) {
    rf.sendFish(zeFish);
    bubbles.makeBubbles();
    
    delay(tween);
    
  } else {
    bubbles.stopBubbles();
  }

  delay(250);
}
