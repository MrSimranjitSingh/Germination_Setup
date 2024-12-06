void setOutput(float *current_temperature) {
  // Update the input value for the PID controller
  input = *current_temperature;
  // Compute the output value using the PID controller
  pid.Compute();
  // Write the output value to the heating pad
  setBlanketLevel(round(output));
}

uint8_t setBlanketLevel(uint8_t level) {

  if (level == heater_min_level && level != heater_current_level) {

    heater_current_level = heater_min_level;

    digitalWrite(RELAY_PIN, LOW);
    delay(150);
    digitalWrite(RELAY_PIN, HIGH);
    return;
  }

  // Make sure the desired level is within the valid range (0-4)
  if (level >= heater_min_level && level <= heater_max_level) {
    // Cycle through the levels to reach the desired level
    while (heater_current_level != level) {
      // increment current level and wrap around if necessary
      heater_current_level = (heater_current_level + 1) % 5;
      // Send a signal to the heater to change the level
      pulse(OPTOCOUPLER_PIN);
    }
  }
}

void pulse(uint8_t pin) {
  digitalWrite(pin, HIGH);
  delay(130);  //Minimum is 100
  digitalWrite(pin, LOW);
  delay(20);  //Minimum is 10
}

uint8_t beep(uint16_t milliseconds) {
  digitalWrite(BUZZER_PIN, 1);
  delay(milliseconds);
  digitalWrite(BUZZER_PIN, 0);
}

void reset_auto_shut() {
  // Calculate the elapsed time in milliseconds since the Arduino was last reset
  unsigned long currentTime = millis();
  unsigned long elapsedTime = currentTime - startTime;

  // Convert milliseconds to minutes
  unsigned long minutes = elapsedTime / (1000UL * 60UL);

  if (minutes > 120) {
    setBlanketLevel(heater_min_level);
    startTime = millis();  // Record the current time as the start time
  }
}
