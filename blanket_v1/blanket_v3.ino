#include <avr/pgmspace.h>
#include <Arduino.h>
#include <Wire.h>
#include <AHTxx.h>
#include <ezButton.h>

#include <pidautotuner.h>
#include <PID_v1.h>
#include <EEPROM.h>

#include <Adafruit_GFX.h>     // Core graphics library
#include <Adafruit_ST7735.h>  // Hardware-specific library
#include <SPI.h>
//#include <Fonts/FreeMono9pt7b.h>


//A6, A7 not working
#define I2C_CLOCK 200
#define BUZZER_PIN A0
#define BUTTON_RESET A1
#define RELAY_PIN 16
#define BLANKET_PIN 17

unsigned long currentMillis = 0;
unsigned long previousSecondMillis = 0;
unsigned long previousFiveSecondMillis = 0;
const unsigned long oneSecondInterval = 1000;
const unsigned long fiveSecondInterval = 5000;
uint8_t secondsCounter = 0;


//For Handling Button
const int SHORT_PRESS_TIME = 500;  // 500 milliseconds
const int LONG_PRESS_TIME = 1500;  // 2000 milliseconds
ezButton button(BUTTON_RESET);     // create ezButton object that attach to pin 7;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;

// For Tuning PID and main loop()
const float targetTemperature = 26.0;         //Celsius
const float maxTargetTemperature = 32.0;      //Celsius
const uint8_t mainLoopInterval = 10;          //seconds
const unsigned long loopInterval = 10000000;  //us

// Define the parameters for the PID controller
double pidParams[3];
//const double DEFAULT_PID_PARAMS[3] = { 7.16, 5.38, 45.82 };
const double DEFAULT_PID_PARAMS[3] = { 4.16, 5.38, 45.82 };
const int EEPROM_PID_PARAMS_ADDRESS = 0;

// Initialize the PID controller
double input, output, setpoint;
PID pid(&input, &output, &setpoint, DEFAULT_PID_PARAMS[0], DEFAULT_PID_PARAMS[1], DEFAULT_PID_PARAMS[2], DIRECT);

//Initialize LCD
// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define TFT_CS 4
#define TFT_RST 8  // you can also connect this to the Arduino reset in which case, set this #define pin to -1!
#define TFT_DC 7
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//Initialize Environment Sensor
AHTxx aht20(AHTXX_ADDRESS_X38, AHT2x_SENSOR);


uint8_t heater_current_level = 0;
const uint8_t heater_min_level = 0;
const uint8_t heater_max_level = 3;
float ahtValue[2] = { 0.0, 0.0 };  //to store T/RH result

unsigned long startTime;  // Variable to store the start time in milliseconds


const int graphX = 5;
const int graphY = 60;
const int graphWidth = 120;
const int graphHeight = 80;
const int dataPoints = 20;
int data[dataPoints];
int minValue = 10;  // Set your desired minimum value
int maxValue = 40;  // Set your desired maximum value

void setup() {
  Serial.begin(115200);

  pinMode(BLANKET_PIN, OUTPUT);         // Set buzzer - pin 9 as an output
  pinMode(RELAY_PIN, OUTPUT);           // Set buzzer - pin 9 as an output
  pinMode(BUZZER_PIN, OUTPUT);          // Set buzzer - pin 9 as an output
  pinMode(BUTTON_RESET, INPUT_PULLUP);  // configure button pin as input with internal pull-up resistor

  digitalWrite(RELAY_PIN, LOW);
  initLcd();
  digitalWrite(RELAY_PIN, HIGH);

  initEnv();

  readEnv(ahtValue);

  setpoint = targetTemperature;
  initPIDController();

  button.setDebounceTime(50);  // set debounce time to 50 milliseconds
  beep(75);

  printEnv(ahtValue, heater_current_level * 25);
  printPID();

  startTime = millis();  // Record the current time as the start time
}

void my_routine() {

  readButton();

  currentMillis = millis();

  // Code to be executed every 1 second
  if (currentMillis - previousSecondMillis >= oneSecondInterval) {
    previousSecondMillis = currentMillis;
    shortInterval();
  }

  // Code to be executed every 5 seconds
  if (currentMillis - previousFiveSecondMillis >= fiveSecondInterval) {
    previousFiveSecondMillis = currentMillis;
    longInterval();
    reset_auto_shut();
  }
}

void shortInterval() {
  printSetpoint();
  printPID();
}

void longInterval() {
  readEnv(ahtValue);
  setOutput(&ahtValue[0]);
  printEnv(ahtValue, heater_current_level * 25);
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

void loop() {
  my_routine();
}