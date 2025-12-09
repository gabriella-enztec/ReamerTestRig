#ifndef BUTTONS_H
#define BUTTONS_H

#include <Bounce2.h>

// Button pin definitions
#define DOWN_BUTTON 2
#define UP_BUTTON 4
#define ZERO_BUTTON 8
#define REAM_BUTTON 6
#define TEST_BUTTON 7

// Button indices
#define BTN_DOWN 0
#define BTN_UP 1
#define BTN_ZERO 2
#define BTN_REAM 3
#define BTN_TEST 4

// External declarations
extern Bounce btn[];
extern bool upPressed;
extern bool downPressed;

// Function declarations
void buttons_init();
void checkButtons();
bool get_butt();

#endif // BUTTONS_H