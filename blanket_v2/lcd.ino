void initLcd() {
  
  tft.initR(INITR_BLACKTAB);
  tft.setRotation(1); 
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(2); 
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(3);
  tft.fillScreen(ST7735_BLACK);
  tft.setRotation(4);
  tft.fillScreen(ST7735_BLACK);
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
