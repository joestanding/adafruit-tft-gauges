#ifndef CBLOCKGAUGE_H
#define CBLOCKGAUGE_H

#include "Gauge.h"

/* ------------------------------------------------------------------------- */

class BlockGauge : public Gauge {
public:
    BlockGauge(Adafruit_ILI9341 * display, Measurement * measurement,
                char * name, int32_t range_min, int32_t range_max);
    void draw(uint16_t x, uint16_t y, uint16_t width, uint16_t height) override;
  private:
    uint8_t  last_reading_length;
};

/* ------------------------------------------------------------------------- */

#endif