/*


#include "PinAssignment.h" //Pin Config

long input_value[Input_size];

int power = 80;    // how bright the motorPWMPin is
int increment = 10;    // how many points to fade the motorPWMPin by

bool oneSide = true;

bool quick = true;

long Hz;

unsigned long previousMillis = 0;        // will store last time LED was updated
long intervals = 10000;           // interval at which to blink (milliseconds)





// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(motorPWMPin, OUTPUT);
  pinMode(motorEnablePin, INPUT);
  digitalWrite(motorEnablePin, HIGH); //HIGH to enable
  pinMode(motorDirPin, OUTPUT);
  digitalWrite(motorDirPin, LOW);
  Serial.begin(115200);
  analogWrite(motorPWMPin, 50);
  delay(1000);
  analogWrite(motorPWMPin, 30);
  delay(1000);
  analogWrite(motorPWMPin, 40);
  delay(1000);

  Serial.println("Starting");

}

// the loop routine runs over and over again forever:
void loop() {
  // set the power of pin 9:

  if (power <= 0)
  {
    power = 0;
  }
  if (power >= 255)
  {
    power = 255;
  }
  analogWrite(motorPWMPin, power);

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= intervals) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    power = random(40, 60);

    if (quick) {
      intervals = random(500, 800);
      quick = !quick;
    } else {
      intervals = random(2000, 3000);
      quick = !quick;
    }
    if (oneSide) {
      digitalWrite(motorDirPin, LOW);
      if (quick) {
        Serial.println("slow");
        analogWrite(motorPWMPin, power);
            oneSide = !oneSide;
      } else {
        Serial.println("quick");
        analogWrite(motorPWMPin, 100);
      }

      Serial.println("ACW");
    } else {

      digitalWrite(motorDirPin, HIGH);
      if (quick) {
        Serial.println("slow");
        analogWrite(motorPWMPin, power);
  
      } else {
        Serial.println("quick");
        analogWrite(motorPWMPin, 100);
                  oneSide = !oneSide;
      }

      Serial.println("CW");
    }

  } else {

   Serial.print( "Hz: " );
   Serial.println(Hz);
  }

  Hz = pulseInLong(5, HIGH);

}





*/
