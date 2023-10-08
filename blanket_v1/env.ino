void initEnv() {

  clear();
  const char* const msg_aht_fail[] PROGMEM = { "AHT2x fail #" };
  char* buffer;
  buffer = (char*)malloc(strlen(msg_aht_fail[0]) + 2);  // 2 is the max number of digits

  for (uint8_t count = 0; aht20.begin() != true; count++) {
    // check if memory allocation was successful
    if (buffer == NULL) {
      // handle error here (e.g. print error message and exit program)
    } else {
      sprintf(buffer, "%s%d", msg_aht_fail[0], count);
      print(buffer, 0);
      free(buffer);
    }

    delay(1000);
    if (count >= 99) {
      count = 0;
      clear();
    }
  }
}

void readEnv(float* env_values) {

  float status_1 = aht20.readTemperature(AHTXX_FORCE_READ_DATA);  //read 6-bytes via I2C, takes 80 milliseconds
  float status_2 = aht20.readHumidity(AHTXX_USE_READ_DATA);       //use 6-bytes from temperature reading, takes zero milliseconds!!!


  if (status_1 != AHTXX_ERROR && status_2 != AHTXX_ERROR)  //AHTXX_ERROR = 255, library returns 255 if error occurs
  {
    env_values[0] = status_1;
    env_values[1] = status_2;
  } else {
    setBlanketLevel(heater_min_level);
    initEnv();
    return;
  }
}
