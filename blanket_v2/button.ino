void readButton() {

  button.loop();  // MUST call the loop() function first

  if (button.isPressed())
    pressedTime = millis();

  if (button.isReleased()) {
    releasedTime = millis();

    long pressDuration = releasedTime - pressedTime;

    if (pressDuration < SHORT_PRESS_TIME) {
      setpoint++;
      if (setpoint > maxTargetTemperature) {
        setpoint = targetTemperature;
      }
    }

    if (pressDuration > LONG_PRESS_TIME) {
      tunePID();
    }
  }
}

void tunePID() {

  //Turn off Heater
  setBlanketLevel(heater_min_level);

  clear();
  char str_msg[16];

  //Clear the EEPROM
  EEPROM.begin();
  for (int i = 0; i < 512; i++) {
    EEPROM.write(i, 0);
  }
  EEPROM.end();

  //Display message
  strcpy(str_msg, "EEPROM cleared.");
  print(str_msg, 4);
  delay(1000);

  clear();
  init_pid_tuner(&loopInterval);
}
