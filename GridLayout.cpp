#include "GridLayout.h"

/* ------------------------------------------------------------------------- */

GridLayout::GridLayout(Adafruit_ILI9341 * display) : Layout(display) {
  this->gauges_wide = 3;
  this->gauges_high = 2;
}

/* ------------------------------------------------------------------------- */

void GridLayout::draw() {
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t width = 320 / this->gauges_wide;
  uint16_t height = 240;

  for(uint8_t i = 0; i < this->gauge_count; i++) {

    if(i == this->gauges_wide) {
      y += height / this->gauges_high;
      x = 0;
    }

    if(this->gauges[i] != NULL) {
      this->gauges[i]->draw(x, y, width, height / this->gauges_high);
      x += width;
    }
  }
}

/* ------------------------------------------------------------------------- */

void GridLayout::set_gauges_high(uint16_t value) {
  this->gauges_high = value;
}

/* ------------------------------------------------------------------------- */

void GridLayout::set_gauges_wide(uint16_t value) {
  this->gauges_wide = value;
}

/* ------------------------------------------------------------------------- */