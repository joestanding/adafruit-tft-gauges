#include "BarGauge.h"
#include "Arduino.h"

/* ------------------------------------------------------------------------- */
/* Constructor                                                               */
/* ------------------------------------------------------------------------- */

BarGauge::BarGauge(Adafruit_ILI9341 * display, Measurement * measurement,
                     char * name, double range_min, double range_max)
: Gauge(display, measurement, name, range_min, range_max) {

  this->bar_colour      = GREEN;
  this->bar_colour_low  = CYAN;
  this->bar_colour_high = RED;
  this->frame_colour    = DARKGREY;

  this->frame_padding = 7;
  this->label_section_height = LABEL_SECTION_HEIGHT;
  this->limit_bar_height = 1;
  this->label_enabled = 1;
}

/* ------------------------------------------------------------------------- */
/* Helper Functions                                                          */
/* ------------------------------------------------------------------------- */

uint16_t BarGauge::get_bar_y_for_value(uint16_t x, uint16_t y, uint16_t w,
                                        uint16_t h, double value) {
  if (this->value_max == this->value_min) {
    return y + this->padding;
  }

  uint16_t max_avail_height =
    (h > (this->padding * 2 + this->label_section_height)) ?
    h - (this->padding * 2 + this->label_section_height) : 0;

  double range = this->value_max - this->value_min;
  double scaled_value = (value - this->value_min) / range;
  uint16_t new_value_height = round(scaled_value * max_avail_height);

  return y + h - new_value_height - this->padding - this->label_section_height;
}

/* ------------------------------------------------------------------------- */

void BarGauge::get_bar_xy(uint16_t gauge_x, uint16_t gauge_y,
                           uint16_t gauge_w, uint16_t gauge_h,
                           uint16_t * dst_x, uint16_t * dst_y,
                           uint16_t bar_height) {
  *dst_x = gauge_x + this->frame_padding;
  *dst_y = (gauge_y + this->frame_padding + this->get_max_bar_height(gauge_h))
           - bar_height;
}

/* ------------------------------------------------------------------------- */

uint16_t BarGauge::get_max_bar_height(uint16_t frame_h) {
  return frame_h - (this->frame_padding * 2) - this->label_section_height;
}

/* ------------------------------------------------------------------------- */
/* Component Rendering                                                       */
/* ------------------------------------------------------------------------- */

void BarGauge::draw_limit_bar(uint16_t x, uint16_t y, uint16_t w,
                               uint16_t h, uint16_t colour, double value) {
  int limit_x1 = x;
  int limit_x2 = x + w - 1;
  int limit_y = this->get_bar_y_for_value(x,
                                          y,
                                          w,
                                          h,
                                          value);

  for(uint8_t i = 0; i < this->limit_bar_height; i++)
    this->display->drawLine(limit_x1,
                            limit_y + i,
                            limit_x2,
                            limit_y + i,
                            colour);
}

/* ------------------------------------------------------------------------- */

void BarGauge::draw_reading(uint16_t x, uint16_t y, uint16_t width,
                             uint16_t height) {
  int x2 = x + (width / 2);
  int y2 = y + height - 34;
  
  this->display->setTextSize(2);

  double value = this->measurement->get_value();
  char str[8]; 
  dtostrf(value, 0, 0, str);

  /* If we're writing a shorter string than last time we'll need to redraw
     the area */
  if(strlen(str) < last_reading_length)
    this->draw_centred_string("    ", x2, y2);

  this->draw_centred_string(str, x2, y2);

  this->last_reading_length = strlen(str);
}

/* ------------------------------------------------------------------------- */

void BarGauge::draw_name(uint16_t x, uint16_t y, uint16_t width,
                          uint16_t height) {
  int x2 = x + (width / 2);
  int y2 = y + height - 17;

  display->setTextColor(WHITE, BLACK);

  this->display->setTextSize(1);
  this->draw_centred_string(this->name, x2, y2);
}

/* ------------------------------------------------------------------------- */
/* Main Drawing Function                                                     */
/* ------------------------------------------------------------------------- */

/**
  * draw() - Draw the bar gauge to the screen.
  * @x:      The x co-ordinate to draw the bar gauge to.
  * @y:      The y co-ordinate to draw the bar gauge to.
  * @width:  The width of the gauge to be drawn.
  * @height: The height of the gauge to be drawn.
  */
void BarGauge::draw(uint16_t gauge_x, uint16_t gauge_y, uint16_t gauge_w,
                    uint16_t gauge_h) {
  /* --------------- */
  /* Bounds Clamping */
  /* --------------- */

  this->value = this->measurement->get_value();

  double bar_value = 0;
  if(this->value > this->value_max) {
    bar_value = this->value_max;
  } else {
    bar_value = this->value;
  }

  if(this->value < this->value_min) {
    bar_value = this->value_min;
  }

  /* --------------------------------------- */
  /* Dimensions and Co-ordinates Calculation */
  /* --------------------------------------- */

  uint16_t frame_x = gauge_x + this->padding;
  uint16_t frame_y = gauge_y + this->padding;
  uint16_t frame_w = gauge_w - (this->padding * 2);
  uint16_t frame_h = gauge_h - (this->padding * 2);

  /* Although the gauge has total height (e.g. 100), with padding present
     we want to calculate the actual, usable height */
  uint16_t max_bar_height = this->get_max_bar_height(frame_h);
  uint16_t bar_width      = frame_w - (this->frame_padding * 2);

  /* Calculate the height of the new bar */
  int bar_height = ((bar_value - this->value_min) * max_bar_height) /
                   (this->value_max - this->value_min);
  int prev_bar_height = ((this->value_last - this->value_min) * max_bar_height) /
                        (this->value_max - this->value_min);

  /* Calculate the X and Y co-ordinates of where our bar will begin */
  uint16_t bar_x, bar_y, prev_x, prev_y = 0;
  this->get_bar_xy(frame_x, frame_y, frame_w, frame_h, &bar_x, &bar_y,
                   bar_height);
  this->get_bar_xy(frame_x, frame_y, frame_w, frame_h, &prev_x, &prev_y,
                   prev_bar_height);

  /* ---------------- */
  /* Bar Border Frame */
  /* ---------------- */

  uint16_t bar_frame_width  = bar_width + (BAR_FRAME_PADDING * 2);
  uint16_t bar_frame_height = max_bar_height + BAR_FRAME_PADDING;
  /* Draw a frame around the bar bounds */
  if(this->requires_redraw) {
    this->display->drawRect(frame_x,
                            frame_y,
                            frame_w,
                            frame_h,
                            this->frame_colour);
  }

  /* ------------- */
  /* Bar Rendering */
  /* ------------- */

  uint16_t bar_colour = this->colour_normal;
  uint16_t text_colour = WHITE;
  if(this->has_limit_higher && (this->value >= this->limit_higher)) {
    bar_colour = this->colour_high;
  }

  if(this->has_limit_lower && (this->value < this->limit_lower)) {
    bar_colour = this->colour_low;
  }

  /* If the bar has changed colour, we'll have to re-render the whole bar */
  if(bar_colour != this->last_colour) {
    this->display->fillRect(bar_x,
                            bar_y,
                            bar_width,
                            bar_height,
                            bar_colour);
  }

  /* If this is the first time rendering, render the full bar and do no more 
     checks */
  if(this->requires_redraw == 1) {
    this->display->fillRect(bar_x,
                            bar_y,
                            bar_width,
                            bar_height,
                            bar_colour);
    this->value_last = bar_value;
  }

  /* If the value hasn't changed, we don't need to render anything */
  if(bar_height == prev_bar_height) {
    
  }

  /* If the new value is higher, we only need to add the delta to the bar */
  if(bar_height > prev_bar_height) {
    this->display->fillRect(bar_x,
                            bar_y,
                            bar_width,
                            prev_y - bar_y,
                            bar_colour);
  }

  /* If the new value is lower, we need to delete the delta from the bar */
  if((bar_height < prev_bar_height) && prev_bar_height >= 0) {
    uint16_t new_bar_height = bar_y - prev_y;
    this->display->fillRect(bar_x,
                            prev_y,
                            bar_width,
                            new_bar_height,
                            BLACK);
  }

  /* ------------------- */
  /* Limit Bar Rendering */
  /* ------------------- */
  
  /* The limit bar indicates where the alarm limit is on the bar, and changes
     colour when the limit has been reached. */
  if(this->requires_redraw
     || (bar_value >= limit_higher-(limit_higher * 0.05) && bar_value <= limit_higher+(limit_higher * 0.05)
     || bar_colour != this->last_colour)) {
    if(this->has_limit_higher) {
      if(this->value >= this->limit_higher) {
        this->draw_limit_bar(bar_x - BAR_FRAME_PADDING,
                            gauge_y,
                            bar_frame_width,
                            gauge_h,
                            YELLOW,
                            this->limit_higher);
      } else {
        this->draw_limit_bar(bar_x - BAR_FRAME_PADDING,
                            gauge_y,
                            bar_frame_width,
                            gauge_h,
                            WHITE,
                            this->limit_higher);
      }
    }
  }

  if(this->requires_redraw
     || (bar_value >= limit_lower-(limit_lower * 0.05) && bar_value <= limit_lower+(limit_lower * 0.05))
     || bar_colour != this->last_colour) {
    if(this->has_limit_lower) {
      this->draw_limit_bar(bar_x - BAR_FRAME_PADDING,
                          gauge_y,
                          bar_frame_width,
                          gauge_h,
                          WHITE,
                          this->limit_lower);
    }
  }

  /* --------------- */
  /* Label Rendering */
  /* --------------- */

  if(this->label_enabled) {
    if(this->requires_redraw
       || fabs(this->measurement->get_value() - this->value_last) > EPSILON)
      this->draw_reading(gauge_x, gauge_y, gauge_w, gauge_h);
    if(this->requires_redraw)
      this->draw_name(gauge_x, gauge_y, gauge_w, gauge_h);
  }

  this->value_last = bar_value;
  this->last_colour = bar_colour;
  this->requires_redraw = false;
}

/* ------------------------------------------------------------------------- */
/* Setters and Getters                                                       */
/* ------------------------------------------------------------------------- */

void BarGauge::set_frame_colour(uint16_t value) {
  this->frame_colour = value;
}

/* ------------------------------------------------------------------------- */

void BarGauge::set_limit_bar_height(uint32_t value) {
  this->limit_bar_height = value;
}

/* ------------------------------------------------------------------------- */

void BarGauge::enable_labels() {
  this->label_enabled = true;
  this->label_section_height = LABEL_SECTION_HEIGHT;
}

/* ------------------------------------------------------------------------- */

void BarGauge::disable_labels() {
  this->label_enabled = false;
  this->label_section_height = 0;
}

/* ------------------------------------------------------------------------- */