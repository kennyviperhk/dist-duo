/*
  Controller MZBL4801F

*/


#include "PinAssignment.h" //Pin Config

long input_value[Input_size];

int brightness = 100;    // how bright the motorPWMPin is
int increment = 10;    // how many points to fade the motorPWMPin by

bool clockwise = true;
bool enableMotor = true;

bool oneSide = true;

bool quick = true;

long Hz;

unsigned long previousMillis = 0;        // will store last time LED was updated
long intervals = 10000;           // interval at which to blink (milliseconds)

bool prevDirection = true;
bool currDirection = true;
long changeDirectionMillis = 0;
long changeDirectionInterval = 100;
int changeDirectionStage = 0;

//
unsigned long timestamp = 0L ;
long prev_pulses = 0L ;
int num_of_poles = 3;

// the setup routine runs once when you press reset:
void setup() {
  // declare pin 9 to be an output:
  pinMode(motorPWMPin, OUTPUT);
  pinMode(motorEnablePin, OUTPUT);
  digitalWrite(motorEnablePin, LOW); //HIGH to enable
  pinMode(motorDirPin, OUTPUT);

  Serial.begin(115200);
  analogWrite(motorPWMPin, 50);
  delay(1000);
  analogWrite(motorPWMPin, 30);
  delay(1000);
  analogWrite(motorPWMPin, 40);
  delay(1000);


  //
  pinMode (motorHzPin, INPUT_PULLUP) ; // just in case its open-collector
  attachInterrupt (0, int_func, RISING) ;  // interrupt channel 0 is pin 2 (for Uno etc)


  Serial.println("Starting");

}

// the loop routine runs over and over again forever:
void loop() {
  // Essentials
  unsigned long currentMillis = millis();

  /* Error Checking */
  if (brightness <= 0)
  {
    brightness = 0;
  }
  if (brightness >= 255)
  {
    brightness = 255;
  }

  if (clockwise) {
    digitalWrite(motorDirPin, HIGH);
  } else {
    digitalWrite(motorDirPin, LOW);
  }


  if (enableMotor) {
    digitalWrite(motorEnablePin, LOW);
  }
  else {
    digitalWrite(motorEnablePin, HIGH);
  }

  if (prevDirection != currDirection) {
    changeDirectionStage = 0;
  }

  if (currentMillis - changeDirectionMillis >= changeDirectionInterval) {
    if (changeDirectionStage == 0) {
      changeDirectionInterval = random(100,1000);
      enableMotor = false;
      changeDirectionStage = 1;
      changeDirectionMillis = currentMillis;
      prevDirection = currDirection;
      Serial.println("Changing Direction");
    } else if (changeDirectionStage == 1) {
      enableMotor = true;
      changeDirectionStage = 2;
      Serial.println("Change Direction Done");
      changeDirectionMillis = currentMillis;
    }else{
      }
  }


  /*
    if (currentMillis - previousMillis >= intervals) {
      // save the last time you blinked the LED
      previousMillis = currentMillis;

      brightness = random(40, 60);

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
       //   Serial.println("slow");
          analogWrite(motorPWMPin, brightness);
              oneSide = !oneSide;
        } else {
       //   Serial.println("quick");
          analogWrite(motorPWMPin, 100);
        }

       // Serial.println("ACW");
      } else {

        digitalWrite(motorDirPin, HIGH);
        if (quick) {
        //  Serial.println("slow");
          analogWrite(motorPWMPin, brightness);

        } else {
        //  Serial.println("quick");
          analogWrite(motorPWMPin, 100);
                    oneSide = !oneSide;
        }

       // Serial.println("CW");
      }

    } else {

    // Serial.print( "Hz: " );
    // Serial.println(Hz);
    }

    // Hz = pulseInLong(5, HIGH);


  */




  if (currentMillis - timestamp >= 1000) { // triggered every second
    {
      timestamp += 1000 ;  // setup for next time

      long now_pulses = get_pulsecount () ;  // sample the count
      long count = now_pulses - prev_pulses ; // take difference from last sample
      prev_pulses = now_pulses ;  // update the previous value

      Serial.println (count * 60.0 / num_of_poles) ;
    }

  }

  analogWrite(motorPWMPin, brightness);
  //Serial.println(brightness);


  serial_decode();
  check_update();

  currDirection = clockwise;

}



