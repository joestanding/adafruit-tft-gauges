#include "Layout.h"
#include "Arduino.h"

/* ------------------------------------------------------------------------- */

Layout::Layout(Adafruit_ILI9341 * display) {
  this->display = display;
  this->gauge_count = 0;
  this->title_enabled = 0;
  for(uint32_t i = 0; i < MAX_GAUGES; i++) {
    this->gauges[i] = NULL;
  }
}

/* ------------------------------------------------------------------------- */

void Layout::add_gauge(Gauge * gauge) {
  for(uint8_t i = 0; i < MAX_GAUGES; i++) {
    if(this->gauges[i] == NULL) {
      this->gauges[i] = gauge;
      this->gauge_count++;
      return;
    }
  }
}

/* ------------------------------------------------------------------------- */

void Layout::redraw() {
  for(uint8_t i = 0; i < MAX_GAUGES; i++) {
    if(this->gauges[i] != NULL) {
      this->gauges[i]->set_initialised(0);
    }
  }  
}

/* ------------------------------------------------------------------------- */

void Layout::set_title(char * title) {
  strncpy(this->title, title, sizeof(this->title));
}

/* ------------------------------------------------------------------------- */

void Layout::enable_title() {
  this->title_enabled = 1;
}

/* ------------------------------------------------------------------------- */

void Layout::disable_title() {
  this->title_enabled = 0;
}

/* ------------------------------------------------------------------------- */