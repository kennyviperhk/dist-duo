
void setSpeedController() {
  if (input_value[0] == 11) {
    power = input_value[1];
    Serial.print("Power : ");
    Serial.println(power);
    input_value[0] = 0;
  }
}
