#include "BlockGauge.h"

/* ------------------------------------------------------------------------- */

BlockGauge::BlockGauge(Adafruit_ILI9341 * display,
                       Measurement * measurement, char * name,
                       double range_min, double range_max)
: Gauge(display, measurement, name, range_min, range_max) {
  this->text_colour_high = WHITE;
  this->text_colour_low = WHITE;
  this->text_colour_normal = WHITE;
  this->frame_colour = WHITE;
}

/* ------------------------------------------------------------------------- */

void BlockGauge::draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height) {
  int frame_x = x + this->padding;
  int frame_y = y + this->padding;
  int frame_w = width  - (this->padding * 2);
  int frame_h = height - (this->padding * 2);

  /* Draw outer frame */
  if(this->requires_redraw)
    this->display->drawRect(frame_x, frame_y, frame_w, frame_h, WHITE);

  /* Determine block colour */
  int16_t block_colour = BLACK;
  int16_t text_colour = this->text_colour_normal;
  if(this->has_limit_higher && (this->measurement->get_value() >= this->limit_higher)) {
    block_colour = this->colour_high;
    text_colour = this->text_colour_high;
  }

  if(this->has_limit_lower && (this->measurement->get_value() < this->limit_lower)) {
    block_colour = this->colour_low;
    text_colour = this->text_colour_low;
  }

  if(this->requires_redraw || (block_colour != this->last_colour)) {
    this->display->fillRect(frame_x + 1,
                            frame_y + 1,
                            frame_w - 2,
                            frame_h - 2,
                            block_colour);
  }

  int value_x = x + (width / 2);
  int value_y = y + (height / 2);
  double value = this->measurement->get_value();
  char str[20];  // Ensure the buffer is large enough for the number and decimal places
  dtostrf(value, 0, 0, str);  // Convert double to string with 3 decimal places

  this->display->setTextColor(text_colour, block_colour);

  /* Draw the value reading */
  if(this->requires_redraw || fabs(this->measurement->get_value() - this->value_last) > EPSILON) {
    this->display->setTextSize(4);
    if(strlen(str) < this->last_reading_length) {
      char clearing_str[this->last_reading_length + 1];
      memset(clearing_str, ' ', this->last_reading_length);
      clearing_str[this->last_reading_length] = '\0';
      this->draw_centred_string(clearing_str, value_x, value_y - 5);
    }
    this->draw_centred_string(str, value_x, value_y - 5);
  }

  /* Draw the measurement name */
  if(this->requires_redraw || (block_colour != this->last_colour)) {
    this->display->setTextSize(1);
    this->draw_centred_string(this->name, value_x, value_y + 25);
  }

  this->last_reading_length = strlen(str);
  this->requires_redraw = false;
  this->last_colour = block_colour;
}

/* ------------------------------------------------------------------------- */

void BlockGauge::set_text_colour_low(uint16_t value) {
  this->text_colour_low = value;
}

/* ------------------------------------------------------------------------- */

void BlockGauge::set_text_colour_high(uint16_t value) {
  this->text_colour_high = value;
}

/* ------------------------------------------------------------------------- */

void BlockGauge::set_text_colour_normal(uint16_t value) {
  this->text_colour_normal = value;
}

/* ------------------------------------------------------------------------- */

void BlockGauge::set_frame_colour(uint16_t value) {
  this->frame_colour = value;
}

/* ------------------------------------------------------------------------- */