#include "Gauge.h"

/* ------------------------------------------------------------------------- */
/* Constructor                                                               */
/* ------------------------------------------------------------------------- */

Gauge::Gauge(Adafruit_ILI9341 * display, Measurement * measurement,
               char * name, int32_t range_min, int32_t range_max) {
  this->display = display;
  this->value = 0;
  this->value_last = 0;
  this->value_min = range_min;
  this->value_max = range_max;
  this->has_limit_higher = 0;
  this->has_limit_lower = 0;
  this->limit_lower = 0;
  this->limit_higher = 0;
  this->padding = GAUGE_PADDING_DEFAULT;
  this->measurement = measurement;
  this->initialised = 0;
  this->colour_high = RED;
  this->colour_low = CYAN;
  this->colour_normal = GREEN;
  strncpy(this->name, name, 16);
}

/* ------------------------------------------------------------------------- */
/* Drawing                                                                   */
/* ------------------------------------------------------------------------- */

void Gauge::draw_frame(uint16_t x, uint16_t y, uint16_t w, uint16_t h,
                        uint16_t colour) {
  this->display->drawRect(x, y, w, h, colour);
}

/* ------------------------------------------------------------------------- */

void Gauge::initial_draw() {
}

/* ------------------------------------------------------------------------- */

void Gauge::draw_centred_string(const char *buf, int x, int y)
{
    int16_t x1, y1;
    uint16_t w, h;
    this->display->setTextWrap(0);
    this->display->getTextBounds(buf, x, y, &x1, &y1, &w, &h);
    int16_t final_x = x - (w / 2);
    int16_t final_y = y - (h / 2);
    this->display->setCursor(final_x, final_y + 1 );
    this->display->print(buf);
}

/* ------------------------------------------------------------------------- */
/* Setters and Getters                                                       */
/* ------------------------------------------------------------------------- */

void Gauge::set_limit_lower(int32_t value) {
  this->has_limit_lower = 1;
  this->limit_lower = value;
}

/* ------------------------------------------------------------------------- */

void Gauge::set_limit_higher(int32_t value) {
  this->has_limit_higher = 1;
  this->limit_higher = value;
  this->value_max = this->limit_higher * GAUGE_LIMIT_MULTIPLIER;
}

/* ------------------------------------------------------------------------- */

void Gauge::set_value(uint32_t value) {
  this->value = value;
}

/* ------------------------------------------------------------------------- */

void Gauge::set_padding(uint16_t padding) {
  this->padding = padding;
}

/* ------------------------------------------------------------------------- */

void Gauge::set_initialised(uint8_t state) {
  this->initialised = state;
}

/* ------------------------------------------------------------------------- */

void Gauge::set_colour_normal(uint16_t value) {
  this->colour_normal = value;
}

/* ------------------------------------------------------------------------- */

void Gauge::set_colour_low(uint16_t value) {
  this->colour_low = value;
}

/* ------------------------------------------------------------------------- */

void Gauge::set_colour_high(uint16_t value) {
  this->colour_high = value;
}

/* ------------------------------------------------------------------------- */