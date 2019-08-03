/*
 * Custom Functions
 */
#include "functions.h"

/*!
 * @brief   Checks whether the button is pressed down or not.
 * @param   pin
 *          Button pin is pulledup (HIGH)
 * @return  True if is pressed down (LOW), else false
 */

bool is_button(uint8_t pin) {
  if (digitalRead(pin) == LOW) {
    delay(45);

    if (digitalRead(pin) == LOW) {      
      delay(45);
      return true; //kui nupp on all
    }
    return false;
  }
  return false;
}

uint8_t flip_num(uint8_t num) {
  if (num > 0) {
    return 0;
  }
  else {
    return 1;
  }
}
