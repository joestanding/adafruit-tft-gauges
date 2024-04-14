#include "SPI.h"
#include "Adafruit_GFX.h"
#include <Adafruit_ILI9341.h>
#include <Adafruit_FT6206.h>
#include <Wire.h>

#include "Measurement.h"
#include "BarGauge.h"
#include "BlockGauge.h"
#include "ColumnLayout.h"
#include "GridLayout.h"

/* ------------------------------------------------------------------------- */

#define TFT_DC 9
#define TFT_CS 10
#define MAX_LAYOUTS 32
#define MAIN_LOOP_DELAY 50
#define DRAW_DELAY 200

/* ------------------------------------------------------------------------- */

Adafruit_ILI9341 display     = Adafruit_ILI9341(TFT_CS, TFT_DC);
Adafruit_FT6206  touchscreen = Adafruit_FT6206();

uint8_t current_layout = 0;
Layout * layouts[MAX_LAYOUTS] = {NULL};

ColumnLayout adcLayout      = ColumnLayout(&display);
ColumnLayout carBarLayout   = ColumnLayout(&display);
GridLayout   carBlockLayout = GridLayout(&display);

/* Used to ensure we know whether the touch event is new or not */
uint8_t last_touch_state = 0;

/* Used to call the draw() function less frequently than the main loop fires */
uint32_t last_draw = 0;

/* ------------------------------------------------------------------------- */

void update_measurements();
void add_layout(Layout * new_layout);
void next_layout();

/* ------------------------------------------------------------------------- */