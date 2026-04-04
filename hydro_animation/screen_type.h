#ifndef SCREEN_TYPE_H
#define SCREEN_TYPE_H

#define SCREEN_W 1400
#define SCREEN_H 900

typedef enum {
    MENU_SCREEN,
    SIMULATION_SCREEN,
    RAIN_INFO_SCREEN,
    SHAPE_INFO_SCREEN,
    ABOUT_SCREEN
} Screen;

extern Screen currentScreen;

#endif