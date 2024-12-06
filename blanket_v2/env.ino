void initEnv() {

  clear();

  for (uint8_t count = 0; aht20.begin() != true; count++) {

    String formattedString = "AHT2x fail #" + String(count);
    const char* myCharPointer = formattedString.c_str();
    print(myCharPointer, 20);
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

  if ((status_1 != AHTXX_ERROR && status_2 != AHTXX_ERROR)
      && (status_1 <= 50.0 && status_1 >= 0.0))  //AHTXX_ERROR = 255, library returns 255 if error occurs
  {
    env_values[0] = status_1;
    env_values[1] = status_2;
  } else {
    setBlanketLevel(heater_min_level);
    initEnv();
    return;
  }
}
