/* 
 * Töötab ,testitud 20.07.219
 * ESP8266 
 * OLED SSD1306 128*64
 * 
 * D1-GPIO5 = SCL
 * D2-GPIO4 = SDA
 * D5-GPIO14 = SCLK
 * D7-GPIO13 = MOSI
 * D8-GPIO15 = CS
 */
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_I2C_ADDRESS 0x3c // use i2c scanner to find address 0x3c
#define SCREEN_WIDTH        128 // OLED display width, in pixels
#define SCREEN_HEIGHT        64 // OLED display height, in pixels
#define SCREEN_RESET_PIN     16 // ? not conected D0-GPIO16

/* Buttons */
#define BTN_UP     0  // D3
#define BTN_SELECT 2  // D4
#define BTN_DOWN   12 // D6

#define MENU_FONT_SIZE   2
#define MENU_ROW_HEIGHT 16

/* Menu items */
#define MENU_1_LENGTH    4
const char * menu_1_items[] = {"Page 2", "Page 3", "Page 4", "Exit"};

/* Global variables */
uint8_t menu_pos {0};
uint8_t page {0};

/* Initialize screen */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET_PIN);

/**********************************
 * Custom function declarations
 **********************************/
void taunotxt(uint16_t data, uint8_t fontsize);
bool is_button(int pin);
void display_home ();
void display_page_2 ();
void display_page_3 ();
void display_page_4 ();
void display_menu_page (uint8_t menu_length, const char *menu[]);


/********************************************************/
void setup() {
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS)) { // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
    
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.clearDisplay();
  display.display();
  delay(1000);
}

void loop() {
  // 0 = home
  if (page == 0) {
    display_home ();

    if (is_button(BTN_DOWN)) {
      page = 1; // Go to menu 1 (1)
    }
    
  }
  else if (page == 1) {
    display_menu_page (MENU_1_LENGTH, menu_1_items);
    
    // Move down on menu
    if (is_button(BTN_DOWN)) {
      menu_pos++;
      if (menu_pos > MENU_1_LENGTH - 1) {
        menu_pos = 0;
      }
    }
    // Move up on menu
    else if (is_button(BTN_UP)) {
      if (menu_pos == 0) {
        menu_pos = MENU_1_LENGTH - 1;
      }
      else {
        menu_pos--;
      }
    }
    // Select menu item
    else if (is_button(BTN_SELECT)) {
      if (menu_pos == 0) {
        page = 2;
      }
      else if (menu_pos == 1) { // Exit
        page = 3;
      }
      else if (menu_pos == 2) { // Exit
        page = 4;
      }
      else if (menu_pos == 3) { // Exit
        page = 0;
        menu_pos = 0;
      }
    }

  }
  else if (page == 2) {
    display_page_2 ();
  }
  else if (page == 3) {
    display_page_3 ();
  }
  else if (page == 4) {
    display_page_4 ();
  }
  else { // page = 0 = home
    display_home ();
  }


  
/*
  if (is_button(BTN_UP) && !is_button(BTN_SELECT) ){
    Serial.println("BNT_UP");
    taunotxt(1, 2);
  }
  if (is_button(BTN_UP) && is_button(BTN_SELECT) ){
    Serial.println("BNT_UP + SELECT");
    taunotxt(1, 2);
  }
  else if (is_button(BTN_SELECT) && !is_button(BTN_UP)){
    Serial.println("BNT_SELECT");
    taunotxt(2, 2);
  }
  else if (is_button(BTN_DOWN) ){
    Serial.println("BNT_DOWN");
    taunotxt(3, 2);
  }
*/
} // loop end


/* custom functions */

/*!
 * @brief   Checks whether the button is pressed down or not.
 * @param   pin
 *          Button pin is pulledup (HIGH)
 * @return  True if is pressed down (LOW), else false
 */
bool is_button(int pin) {
  if (digitalRead(pin) == LOW) {
    delay(30);

    if (digitalRead(pin) == LOW) {      
      return true; //kui nupp on all
      delay(30);
    }
    return false;
  }
  return false;
}


/*!
 * @brief   Display menu content
 * @param   menu_length
 *          Elements on char array
 * @param   menu
 *          char array of menu items
 */
void display_menu_page (uint8_t menu_length, const char *menu[]) {
  display.clearDisplay();
  display.setTextSize(MENU_FONT_SIZE);
  display.setTextColor(WHITE);

  for (uint8_t i = 0; i < menu_length; i++)
  {
      display.setCursor(2, MENU_ROW_HEIGHT * i +1);
      if (menu_pos == i) {
        display.setTextColor(BLACK, WHITE);
        display.fillRect(0, MENU_ROW_HEIGHT * i, SCREEN_WIDTH, MENU_ROW_HEIGHT + 1, WHITE);
      }
      else
      {
        display.setTextColor(WHITE);
      }
      display.println(menu[i]);
  }

  display.display();
}

/*!
 * @brief   Display home page content
 */
void display_home () {
  display.clearDisplay();
  display.setTextSize(MENU_FONT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Home"));
  display.display();
}

/*!
 * @brief   Display page content
 */
void display_page_2 () {
  display.clearDisplay();
  display.setTextSize(MENU_FONT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Page 2"));
  display.display();
}

/*!
 * @brief   Display page content
 */
void display_page_3 () {
  display.clearDisplay();
  display.setTextSize(MENU_FONT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Page 3"));
  display.display();
}

/*!
 * @brief   Display page content
 */
void display_page_4 () {
  display.clearDisplay();
  display.setTextSize(MENU_FONT_SIZE);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.println(F("Page 4"));
  display.display();
}