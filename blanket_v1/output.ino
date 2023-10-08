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
      pulse(BLANKET_PIN);
    }
  }
}

void pulse(uint8_t pin) {
  //digitalWrite(ledPin, HIGH);
  digitalWrite(pin, HIGH);
  delay(130);  //Minimum is 100
  digitalWrite(pin, LOW);
  delay(20);  //Minimum is 10
  //digitalWrite(ledPin, LOW);
}

uint8_t beep(uint16_t milliseconds) {
  digitalWrite(BUZZER_PIN, 1);
  delay(milliseconds);
  digitalWrite(BUZZER_PIN, 0);
}
