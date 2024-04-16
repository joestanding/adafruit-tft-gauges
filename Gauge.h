#ifndef CGAUGE_H
#define CGAUGE_H

#include <math.h>
#include "Arduino.h"
#include "SPI.h"
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include "Measurement.h"

#define GAUGE_LIMIT_MULTIPLIER 1.25
#define GAUGE_PADDING_DEFAULT 3

#define BLACK ILI9341_BLACK
#define NAVY ILI9341_NAVY
#define DARKGREEN ILI9341_DARKGREEN
#define DARKCYAN ILI9341_DARKCYAN
#define MAROON ILI9341_MAROON
#define PURPLE ILI9341_PURPLE
#define OLIVE ILI9341_OLIVE
#define LIGHTGREY ILI9341_LIGHTGREY
#define DARKGREY ILI9341_DARKGREY
#define BLUE ILI9341_BLUE
#define GREEN ILI9341_GREEN
#define CYAN ILI9341_CYAN
#define RED ILI9341_RED
#define MAGENTA ILI9341_MAGENTA
#define YELLOW ILI9341_YELLOW
#define WHITE ILI9341_WHITE
#define ORANGE ILI9341_ORANGE
#define GREENYELLOW ILI9341_GREENYELLOW
#define PINK ILI9341_PINK

const double EPSILON = 0.001; 

/* ------------------------------------------------------------------------- */

class Gauge {

public:

  Gauge(Adafruit_ILI9341 * display, Measurement * measurement, char * name,
         int32_t range_min, int32_t range_max);

  void draw_frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                  uint16_t colour);
  void redraw();

  void set_limit_lower(double value);
  void set_limit_higher(double value);
  void set_value(double value);
  void set_padding(uint8_t value);
  void set_colour_normal(uint16_t value);
  void set_colour_high(uint16_t value);
  void set_colour_low(uint16_t value);

  virtual void draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  }

protected:

  void draw_centred_string(const char *buf, int x, int y);
  
  double value;
  double value_min;
  double value_max;
  double value_last;

  uint16_t colour_normal;
  uint16_t colour_low;
  uint16_t colour_high;
  uint16_t last_colour;

  uint8_t has_limit_lower;
  uint8_t has_limit_higher;
  double limit_lower;
  double limit_higher;

  Measurement * measurement;

  uint8_t padding;

  char name[16];

  uint8_t  requires_redraw;
  Adafruit_ILI9341 * display;
};

/* ------------------------------------------------------------------------- */

#endif CGAUGE_H