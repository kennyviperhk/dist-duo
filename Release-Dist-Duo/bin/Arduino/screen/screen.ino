
#include <AccelStepper.h>


#include "PinAssignment.h" //Pin Config

long input_value[Input_size];

AccelStepper stepper(AccelStepper::DRIVER, stepPin, dirPin);

int encoderMidPoint = 180;
int sensorPin = A5;    // select the input pin for the potentiometer
int sensorValue = 0;  // variable to store the value coming from the sensor
int outputVal;
bool ok = true;

bool calibration = true;
bool quickCalibration = false;

long prevMillis;

long pos = 0;
void setup() {

  pinMode(enPin, OUTPUT);
  digitalWrite(enPin, LOW);

  stepper.setMaxSpeed(3000);
  stepper.setAcceleration(3000);
  Serial.begin(115200);

  Serial.println("Calibration...");
}

void loop() {

  sensorValue = analogRead(sensorPin);
  outputVal = map(sensorValue, 0, 1023, 0, 360);
  //Serial.print("sensor val: ");
  //Serial.print(outputVal);


  if (calibration) {
   // Serial.println(outputVal);

    if (outputVal > encoderMidPoint)
    {

      stepper.setMaxSpeed(1000);
      if (quickCalibration) {
        stepper.setSpeed(400);
      } else {
        stepper.setSpeed(100);
      }

    }
    else if (outputVal < encoderMidPoint) {
      stepper.setMaxSpeed(1000);
      if (quickCalibration) {
        stepper.setSpeed(-400);
      } else {
        stepper.setSpeed(-100);
      }
    }
    else if (outputVal == encoderMidPoint) {
      Serial.println("Calibration Done");
      stepper.setCurrentPosition(0);
      calibration = false;

    }
    stepper.runSpeed();
  }
  else {

    if (stepper.distanceToGo() == 0)
    {

      stepper.moveTo(pos);


    }
    stepper.run();

  }

  serial_decode();
  check_update();

long currMillis = millis();
  if (currMillis - prevMillis > 1000) {
    Serial.print("screen \t");
    Serial.print(pos);
    Serial.print("\t");
    Serial.println(outputVal);

    prevMillis = currMillis;
  }
}

