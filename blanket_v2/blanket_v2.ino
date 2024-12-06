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

#define I2C_CLOCK 200
#define BUZZER_PIN A0
#define BUTTON_CYCLE A1
#define RELAY_PIN A2
#define OPTOCOUPLER_PIN A3

#define TFT_CS 4
#define TFT_RST 8  // you can also connect this to the Arduino reset in which case, set this #define pin to -1!
#define TFT_DC 7

#define SHORT_PRESS_TIME 500  // 500 milliseconds
#define LONG_PRESS_TIME 1500  // 2000 milliseconds

unsigned long currentMillis = 0;
unsigned long previousMillis = 0;
const unsigned long interval = 1000;
uint8_t intervalCounter = 0;

//For Handling Button
ezButton button(BUTTON_CYCLE);  // create ezButton object that attach to pin 7;
unsigned long pressedTime = 0;
unsigned long releasedTime = 0;

// For Tuning PID and main loop()
const float targetTemperature = 24.0;         //Celsius
const float maxTargetTemperature = 45.0;      //Celsius
const uint8_t mainLoopInterval = 10;          //seconds
const unsigned long loopInterval = 10000000;  //us

// Define the parameters for the PID controller
double pidParams[3];
const double DEFAULT_PID[3] = { 4.16, 5.38, 45.82 };
#define EEPROM_PID_ADDRESS 0

// Initialize the PID controller
double input, output, setpoint;
PID pid(&input, &output, &setpoint, DEFAULT_PID[0], DEFAULT_PID[1], DEFAULT_PID[2], DIRECT);

//Initialize LCD
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

//Initialize Environment Sensor
AHTxx aht20(AHTXX_ADDRESS_X38, AHT2x_SENSOR);

uint8_t heater_current_level = 0;
#define heater_min_level 0
#define heater_max_level 4
float ahtValue[2] = { 0.0, 0.0 };  //to store T/RH result

unsigned long startTime;  // Variable to store the start time in milliseconds


bool dataReceived = false;  // Initialize a flag to track if data has been received


void setup() {
  Serial.begin(115200);

  pinMode(OPTOCOUPLER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(BUTTON_CYCLE, INPUT_PULLUP);


  digitalWrite(RELAY_PIN, HIGH);

  initLcd();
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

void loop() {

  readButton();

  currentMillis = millis();

  // Code to be executed every 1 second
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    intervalCounter++;
    printSetpoint();
    printPID();
  }

  // Code to be executed every 5 seconds
  if (intervalCounter >= 5) {
    readEnv(ahtValue);
    setOutput(&ahtValue[0]);
    printEnv(ahtValue, heater_current_level * 25);
    //printGraph(ahtValue);
    reset_auto_shut();
    intervalCounter = 0;
  }
}