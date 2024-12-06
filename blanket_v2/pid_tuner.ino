void init_pid_tuner(unsigned long *loopInterval) {

  uint16_t loopCounter = 0;

  PIDAutotuner autotuner = PIDAutotuner();
  // Initialize your hardware and set up the autotuner parameters
  autotuner.setTargetInputValue(setpoint);
  autotuner.setLoopInterval(*loopInterval);
  autotuner.setOutputRange(heater_min_level, heater_max_level);
  autotuner.setZNMode(PIDAutotuner::ZNModeNoOvershoot);
  autotuner.setTuningCycles(10);

  // This must be called immediately before the tuning loop
  // Must be called with the current time in microseconds
  autotuner.startTuningLoop(micros());
  long microseconds;
  while (!autotuner.isFinished()) {

    // This loop must run at the same speed as the PID control loop being tuned
    long prevMicroseconds = microseconds;
    microseconds = micros();

    readEnv(ahtValue);
    // Get input value here (temperature, encoder position, velocity, etc)
    //double input = ahtValue[0];

    // Call tunePID() with the input value and current time in microseconds
    double output = autotuner.tunePID(ahtValue[0], microseconds);

    // Set the output - tunePid() will return values within the range configured
    // by setOutputRange(). Don't change the value or the tuning results will be
    // incorrect.
    clear();
    setBlanketLevel(round(output));
    loopCounter++;
    printTuner(&ahtValue[0], (heater_current_level * 25), loopInterval, &loopCounter, autotuner.getCycle());


    // This loop must run at the same speed as the PID control loop being tuned
    while (micros() - microseconds < *loopInterval) {
      delayMicroseconds(1);
    }
  }

  // Turn the output off here.
  setBlanketLevel(heater_min_level);

  setPIDParameters(autotuner.getKp(), autotuner.getKi(), autotuner.getKd());
  savePIDParametersToEEPROM(autotuner.getKp(), autotuner.getKi(), autotuner.getKd());
}
