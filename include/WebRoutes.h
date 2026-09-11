#ifndef WEB_ROUTES_H
#define WEB_ROUTES_H

#include "RPS.h"
#include "Display.h"
#include "Buzzer.h"

// Registers all HTTP routes ("/", "/style.css", "/jquery.min.js",
// "/play", "/reset") on the global `server` object declared in Wifi.h.
void setupRoutes(RPS &game, Display &display, Buzzer &buzzer, int &roundCounter);

#endif