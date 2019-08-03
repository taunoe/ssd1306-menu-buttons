/*!
 * Custom function declarations
 * @author Tauno Erik
 */
#include <Arduino.h>

bool is_button(uint8_t pin);
uint8_t flip_num(uint8_t num);
void move_menu_up (uint8_t menu_length);
void move_menu_down (uint8_t menu_length);
void play_note (uint8_t speaker_pin, uint16_t the_tone, uint8_t duration);
void sound_on_off();
void go_page(uint8_t new_page);
void display_home ();
void display_page_2 ();
void display_page_3 ();
void display_page_4 ();
void display_menu_page (uint8_t menu_length, const char *menu[]);