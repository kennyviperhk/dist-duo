
void setSpeedController() {
  if (input_value[0] == 11) {
    brightness = input_value[1];
    Serial.print("Power : ");
    Serial.println(brightness);
    input_value[0] = 0;
  }
}
