#ifndef CBLOCKGAUGE_H
#define CBLOCKGAUGE_H

#include "Gauge.h"

/* ------------------------------------------------------------------------- */

class BlockGauge : public Gauge {
public:
    BlockGauge(Adafruit_ILI9341 * display, Measurement * measurement,
                char * name, double range_min, double range_max);
    void draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height) override;
    void set_text_colour_low(uint16_t value);
    void set_text_colour_high(uint16_t value);
    void set_text_colour_normal(uint16_t value);
    void set_frame_colour(uint16_t value);
  private:
    uint8_t  last_reading_length;
    uint16_t text_colour_low;
    uint16_t text_colour_high;
    uint16_t text_colour_normal;
    uint16_t frame_colour;
};

/* ------------------------------------------------------------------------- */

#endif