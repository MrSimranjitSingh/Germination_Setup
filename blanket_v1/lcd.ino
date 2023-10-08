void initLcd() {
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1);  // Rotate the display if needed (adjust as necessary)
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(2);  // Rotate the display if needed (adjust as necessary)
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(3);  // Rotate the display if needed (adjust as necessary)
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(4);  // Rotate the display if needed (adjust as necessary)
  tft.fillScreen(ST7735_BLACK);
  // Initialize data array with initial values
  for (int i = 0; i < dataPoints; i++) {
    data[i] = minValue;  // Initialize with the minimum value
  }


  char str_msg[16];
  strcpy(str_msg, "Starting in");
  print(str_msg, 10);

  for (int i = 3; i >= 1; i--) {
    sprintf(str_msg, "%d seconds.", i);
    print(str_msg, 20);
    delay(1000);
  }
}

void update_names() {
  print_scroll(" C ->    C", 0, 30);
  print_scroll("Power=    %", 15, 0);
  print_scroll("RH:      %", 30, 0);
}

void update_value(char *text, uint8_t x, int16_t y, int width, int height) {
  tft.fillRect(x, 5 + y, width, height, ST7735_BLACK);
  print_scroll(text, y, x);
}

void print(char *text, uint8_t line) {

  tft.fillRect(5, 5 + line, 160, 10, ST7735_BLACK);
  tft.setCursor(5, 5 + line);
  tft.setTextColor(ST7735_WHITE);
  tft.setTextSize(1);
  tft.setTextWrap(true);
  tft.print(text);
}

void print_scroll(char *text, uint8_t line, int16_t x) {
  tft.setCursor(5 + x, 5 + line);
  tft.setTextColor(ST7735_RED);
  tft.setTextSize(1);
  tft.setTextWrap(true);
  tft.print(text);
}

void print_i(char *text, uint8_t line) {
  //TODO
  tft.setCursor(5, 5 + line);
  tft.setTextColor(ST7735_BLUE);
  tft.setTextSize(1);
  tft.setTextWrap(true);
  tft.print(text);
}

void clear() {
  tft.fillScreen(ST7735_BLACK);
}

void drawLineGraph(int data[], int dataPoints, int x, int y, int width, int height) {
  tft.drawRect(x, y, width, height, ST7735_GREEN);  // Draw the graph boundary

  int xStep = width / (dataPoints - 1);  // Calculate the horizontal spacing between data points

  // Draw the data points and connect them with lines
  for (int i = 0; i < dataPoints - 1; i++) {
    int x1 = x + i * xStep;
    int y1 = map(data[i], minValue, maxValue, y + height, y);
    int x2 = x + (i + 1) * xStep;
    int y2 = map(data[i + 1], minValue, maxValue, y + height, y);

    tft.drawLine(x1, y1, x2, y2, ST7735_GREEN);
  }
}
