uint8_t initPIDController() {

  // Read the PID parameters from EEPROM
  EEPROM.get(EEPROM_PID_ADDRESS, pidParams);

  // Check if the PID parameters are valid
  bool pidParamsValid = true;
  for (int i = 0; i < 3; i++) {
    if (isnan(pidParams[i]) || pidParams[i] == 0) {
      pidParamsValid = false;
      break;
    }
  }

  // Use the defaults if parameters are not valid
  if (!pidParamsValid) {
    for (int i = 0; i < 3; i++) {
      pidParams[i] = DEFAULT_PID[i];
    }
  }

  // Set the PID controller parameters
  pid.SetTunings(pidParams[0], pidParams[1], pidParams[2]);

  pid.SetMode(AUTOMATIC);
  pid.SetOutputLimits(heater_min_level, heater_max_level);
  pid.SetSampleTime(5 * 1000);  //Set to 5 seconds because our system is slow.
}

void setPIDParameters(double kp, double ki, double kd) {
  // Save the PID parameters
  pidParams[0] = kp;
  pidParams[1] = ki;
  pidParams[2] = kd;

  // Update the PID controller
  pid.SetTunings(kp, ki, kd);
}

void savePIDParametersToEEPROM(double kp, double ki, double kd) {
  // Save the PID parameters
  pidParams[0] = kp;
  pidParams[1] = ki;
  pidParams[2] = kd;

  EEPROM.put(EEPROM_PID_ADDRESS, pidParams);
}