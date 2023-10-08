void printEnv(float *envValues, uint8_t heater_power) {
  String formattedString = String(envValues[0]);  // Concatenate an integer to the string
  const char *myCharPointer = formattedString.c_str();
  update_value(myCharPointer, 0, 0, 38, 8);

  formattedString = String(heater_power);  // Concatenate an integer to the string
  myCharPointer = formattedString.c_str();
  update_value(myCharPointer, 40, 15, 25, 8);

  formattedString = String(envValues[1]);  // Concatenate an integer to the string
  myCharPointer = formattedString.c_str();
  update_value(myCharPointer, 20, 30, 38, 8);

  update_names();

  //Graph*******************
  // Simulate real-time data acquisition (replace with your actual data source)
  int newData = envValues[0];

  // Shift the data array to the left
  for (int i = 0; i < dataPoints - 1; i++) {
    data[i] = data[i + 1];
  }

  // Add the new data point to the data array
  data[dataPoints - 1] = newData;

  // Clear the graph area
  tft.fillRect(graphX, graphY, graphWidth, graphHeight, ST7735_BLACK);

  // Draw the updated line graph
  drawLineGraph(data, dataPoints, graphX, graphY, graphWidth, graphHeight);

  //Graph*******************
}

void printSetpoint() {
  String formattedString = String((int)setpoint);
  const char *myCharPointer = formattedString.c_str();
  update_value(myCharPointer, 65, 0, 22, 8);
}

void printTuner(float *temperature, uint8_t heater_power, unsigned long *time, uint16_t *counter, uint8_t cycle) {

  char buffer_0[16] PROGMEM = { "Tuning PID:" };
  print(buffer_0, 0);

  // Allocate 16 bytes of memory dynamically
  char *buffer_1 = (char *)malloc(16);
  // Check if memory allocation was successful
  if (buffer_1 != NULL) {
    sprintf(buffer_1, "%d %%", heater_power);
    print_i(buffer_1, 3);
    sprintf(buffer_1, "#%d", cycle);
    print(buffer_1, 30);
    sprintf(buffer_1, "%d * %lu ms", *counter, *time / 1000);
    print(buffer_1, 40);

    // Free the allocated memory
    free(buffer_1);
  }

  float values[] = { *temperature, setpoint };

  String formattedString = String(values[0]) + " / " + String(values[1]);
  const char *myCharPointer = formattedString.c_str();
  print(myCharPointer, 2);
}

void printPID() {

  static int16_t print_state = 0;  // Start off-screen
  String formattedString;
  switch (print_state) {
    case 0:
      formattedString = "kp: " + String(pidParams[0]);
      break;
    case 1:
      formattedString = "ki: " + String(pidParams[1]);
      break;
    case 2:
      formattedString = "kd: " + String(pidParams[2]);
      break;
    default:
      formattedString = "error...";
      break;
  }
  print_state++;
  if (print_state > 2) {
    print_state = 0;
  }

  const char *myCharPointer = formattedString.c_str();
  print(myCharPointer, 145);
}
