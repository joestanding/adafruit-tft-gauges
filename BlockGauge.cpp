#include "BlockGauge.h"

/* ------------------------------------------------------------------------- */

BlockGauge::BlockGauge(Adafruit_ILI9341 * display,
                         Measurement * measurement, char * name,
                         int32_t range_min, int32_t range_max)
: Gauge(display, measurement, name, range_min, range_max) {
}

/* ------------------------------------------------------------------------- */

void BlockGauge::draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  int frame_x = x + this->padding;
  int frame_y = y + this->padding;
  int frame_w = width  - (this->padding * 2);
  int frame_h = height - (this->padding * 2);
  this->display->drawRect(frame_x, frame_y, frame_w, frame_h, WHITE);

  int value_x = x + (width / 2);
  int value_y = y + (height / 2);
  char value_str[8];
  sprintf(value_str, "%d", this->measurement->get_value());

  this->display->setTextSize(4);

  if(strlen(value_str) < this->last_reading_length)
    this->draw_centred_string("    ", value_x, value_y - 5);
  this->draw_centred_string(value_str, value_x, value_y - 5);

  this->display->setTextSize(2);
  this->draw_centred_string(this->name, value_x, value_y + 25);

  this->last_reading_length = strlen(value_str);
}

/* ------------------------------------------------------------------------- */