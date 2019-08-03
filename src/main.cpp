/*************************************************************
 * 
 * @author Tauno Erik
 * Töötab ,testitud 23.07.219
 * 
 * ESP8266 
 * OLED SSD1306 128*64
 * 
 * D1-GPIO5  = SCL
 * D2-GPIO4  = SDA
 * D5-GPIO14 = SCLK
 * D7-GPIO13 = MOSI
 * D8-GPIO15 = CS
 * 
 **************************************************************/
#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "pitches.h"
#include "functions.h"

#define SCREEN_I2C_ADDRESS 0x3c  // use i2c scanner to find address 0x3c

const uint8_t SPEAKER_PIN  {16};  // D0


const uint8_t SCREEN_WIDTH        {128};  // OLED display width, in pixels
const uint8_t SCREEN_HEIGHT        {64};  // OLED display height, in pixels
const uint8_t SCREEN_RESET_PIN     {10};  // 16 ? not conected D0-GPIO16

const uint8_t BTN_UP                {0};  // D3
const uint8_t BTN_SELECT            {2};  // D4
const uint8_t BTN_DOWN             {12};  // D6

const uint8_t MENU_FONT_SIZE        {2};
const uint8_t MENU_ROW_HEIGHT      {16};

const uint8_t MENU_1_LENGTH         {4};
const uint8_t MENU_4_LENGTH         {3};

const char *menu_1_items[] = {"Page 2", "Page 3", "Sound", "Exit"};
const char *menu_4_items[] = {"Buttons", "Light", "Exit"};
uint8_t  menu_4_states[] = {1 , 0, 0};

/* Global variables */
uint8_t menu_pos {0};
uint8_t page {0};



/* Initialize screen */
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, SCREEN_RESET_PIN);

/********************************************************/
void setup() {
  Serial.begin(115200);

  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_SELECT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_I2C_ADDRESS)) { // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // loop forever
  }

  // Adafruit splash screen.
  display.clearDisplay();
  display.display();
  delay(1000);
}

void loop() {
 Serial.println(menu_4_states[0]);

  if (page == 0) {
      display_home();

      if (is_button(BTN_DOWN)) {   // Go to menu 1
          go_page(1); 
      }
  }
  else if (page == 1) {
      display_menu_page (MENU_1_LENGTH, menu_1_items);
    
      if (is_button(BTN_DOWN)) {
          move_menu_down(MENU_1_LENGTH);
      }
      else if (is_button(BTN_UP)) {
          move_menu_up(MENU_1_LENGTH);
      }
      else if (is_button(BTN_SELECT)) {
          if (menu_pos == 0) {
              go_page(2);
          }
          else if (menu_pos == 1) {
              go_page(3);
          }
          else if (menu_pos == 2) {
              go_page(4);
          }
          else if (menu_pos == 3) {
              go_page(0);
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
      display_page_4();                     // sound page
      if (is_button(BTN_DOWN)) {
          move_menu_down(MENU_4_LENGTH);
      }
      else if (is_button(BTN_UP)) {
          move_menu_up(MENU_4_LENGTH);
      }
      else if (is_button(BTN_SELECT)) {
          if (menu_pos == 0) {
              menu_4_states[menu_pos] = flip_num(menu_4_states[menu_pos]);
          }
          else if (menu_pos == 1) {
              menu_4_states[menu_pos] = flip_num(menu_4_states[menu_pos]);
          }
          else if (menu_pos == 2) {
              go_page(0);
          }
      }
  }
  else { // page = 0 = home
      display_home ();
  }

} // loop end


/**************************
 **** Custom Functions ****
 **************************/



void move_menu_up (uint8_t menu_length) {
  if (menu_pos == 0) {
    menu_pos = menu_length - 1;
  }
  else {
    menu_pos--;
  }

  sound_on_off();
}


void move_menu_down (uint8_t menu_length) {
  menu_pos++;
  if (menu_pos > menu_length - 1) {
    menu_pos = 0;
  }

  sound_on_off();
}

/*!
 * @brief   Play sound if sate is 1
 */
void sound_on_off() {
  if (menu_4_states[0] > 0) {
    play_note(SPEAKER_PIN, NOTE_F7, 4);
  }
}

/*!
 * @brief   Play note
 * @param   speaker_pin
 * @param   the_tone
 *          define on pitches.h
 * @param   duration
 *          1 to 7
 */
void play_note (uint8_t speaker_pin, uint16_t the_tone, uint8_t duration) {
  long time = 500 / pow(2, duration - 1);
  tone(speaker_pin, the_tone, time);
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
 * @note    Sound settings
 */
void display_page_4 () {
  display.clearDisplay();
  display.setTextSize(MENU_FONT_SIZE);
  display.setTextColor(WHITE);
  

  for (uint8_t i = 0; i < MENU_4_LENGTH; i++)
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
      display.print(menu_4_items[i]);
      display.setCursor(95, MENU_ROW_HEIGHT * i +1);
      display.print(menu_4_states[i]);
  }

  display.display();
}


/*!
 * @brief   Change page nr and set menu position 0
 * @note    new_page
 */
void go_page(uint8_t new_page) {
  page = new_page;
  menu_pos = 0;
}