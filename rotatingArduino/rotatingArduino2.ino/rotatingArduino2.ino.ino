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
    unsigned long lastBubblesTime = 0;
    const int bubblingDuration = 15 * 1000; //each bubbling on time
    bool isBubbling = false;
    
    public:
        Bubbles(int _pin) {
            pin = _pin;
        }

        void setup() {
            pinMode(pin, OUTPUT); 
        }

                
        void makeBubbles() {
          if (!isBubbling) {
            Serial.println("Making Bubbles!!!");
            isBubbling = true;
            lastBubblesTime = millis();
          }
          digitalWrite(pin, HIGH);
        }

        void stopBubbles() {
          if (isBubbling) {
            Serial.println("Stopping Bubbles!!!");
            isBubbling = false;
          }
          digitalWrite(pin, LOW);
        }

        void stopBubblesWhenNeeded() {
          if ((millis() - lastBubblesTime) > bubblingDuration) {
            //enough time has passed, stopping
            this->stopBubbles();
          } else {
            //continue bubbling 
            
          }
        }
};

#include <Bounce2.h>

#define TOTAL_FISHES 8

int zeFish = 0;
unsigned long lastFishTime = 0;
const int tween = 5 * 1000; //grace period until more fish can be caught

class Fishes {
    
    int fish = 0;
    int pins[TOTAL_FISHES];
    Bounce bouncers[TOTAL_FISHES];
    
    public:
        Fishes(int fromPin) {
          for (int i = 0; i < TOTAL_FISHES; i++) {
            pins[i] = fromPin + i;
            bouncers[i] = Bounce( fromPin + i , 1 ); 
          }
        }

        void setup() {
          for (int i = 0; i < TOTAL_FISHES; i++) {
            pinMode(pins[i], INPUT_PULLUP);
            bouncers[i].update();
          }
        }

                
        int getFish() {

          if (fish > 0 && ((millis() - lastFishTime) < tween)) {
            //still transmitting last fish
            for (int i = 0; i < TOTAL_FISHES; i++) {
              bouncers[i].update();
            }
//            Serial.print("Still transmitting fish #");
//            Serial.println(fish);
          } else {
            fish = 0;
            String fishes = ("[");
            for (int i = 0; i < TOTAL_FISHES; i++) {
              bouncers[i].update();
//              fishes += (digitalRead(pins[i]);
              fishes += (bouncers[i].read());
              if (bouncers[i].read() == LOW) {
//              if (digitalRead(pins[i]) == LOW) {
                fish = i + 1;
              }
            }
            fishes += ("]");
            
            if (fish > 0) {
              lastFishTime = millis();
              Serial.print("Found fish! ");
              Serial.print(fish);
              Serial.print(" -> ");
              Serial.println(fishes);
            }
          }
          return fish;
        }

};

RF      rf      = RF(10); //D10 is the rf transmitter pin
Fishes  fishes  = Fishes(2); //D2 is the first fish pin (2-9)
Bubbles bubbles = Bubbles(11);

void setup() {

  Serial.begin(9600);

  rf.setup();
  fishes.setup();
  bubbles.setup();

  delay(3000);
  
}

void loop() {

  zeFish = fishes.getFish();

  if (zeFish > 0) {
    rf.sendFish(zeFish);
    bubbles.makeBubbles();
  } else {
    bubbles.stopBubblesWhenNeeded();
  }

  delay(1);
}
