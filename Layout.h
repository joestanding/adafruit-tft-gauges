#ifndef CLAYOUT_H
#define CLAYOUT_H

#define MAX_GAUGES 8

#include "BarGauge.h"

class Layout {
public:
  Layout(Adafruit_ILI9341 * display);
  virtual void draw() {}
  void add_gauge(Gauge * gauge);
  void redraw();
  void set_title(char * title);
  void enable_title();
  void disable_title();
protected:
  Adafruit_ILI9341 * display;
  uint16_t gauge_count;
  Gauge * gauges[MAX_GAUGES];
  char title[32];
  uint8_t title_enabled;
};

#endif