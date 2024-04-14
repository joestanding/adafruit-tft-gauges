#ifndef CBARGAUGE_H
#define CBARGAUGE_H

#include "Gauge.h"

/* ------------------------------------------------------------------------- */

#define LABEL_SECTION_HEIGHT 40
#define BAR_FRAME_PADDING    8
#define GAUGE_LIMIT_HEIGHT   1

/* ------------------------------------------------------------------------- */

class BarGauge : public Gauge {
public:
    BarGauge(Adafruit_ILI9341 * display, Measurement * easurement,
              char * name, int32_t range_min, int32_t range_max);

    void draw(uint16_t gauge_x, uint16_t gauge_y, uint16_t gauge_w, uint16_t gauge_h) override;
    void draw_limit_bar(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t colour, int32_t value);
    void draw_reading(uint16_t x, uint16_t y, uint16_t width, uint16_t height);
    void draw_name(uint16_t x, uint16_t y, uint16_t width, uint16_t height);

    void set_frame_colour(uint16_t frame_colour);
    void set_limit_bar_height(uint32_t height);
    void set_label_state(uint8_t state);


    uint16_t get_bar_y_for_value(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint32_t value);
    void get_bar_xy(uint16_t gauge_x, uint16_t gauge_y, uint16_t gauge_w, uint16_t gauge_h, uint16_t * dst_x, uint16_t * dst_y, uint16_t bar_height);
    uint16_t get_max_bar_height(uint16_t frame_h);
private:
    uint8_t  last_reading_length;

    uint16_t bar_colour;
    uint16_t bar_colour_high;
    uint16_t bar_colour_low;
    uint16_t last_bar_colour;
    uint16_t frame_colour;

    int32_t  min_bar_value;
    int32_t  max_bar_value;
    uint16_t frame_padding;
    uint16_t label_section_height;
    uint8_t  limit_bar_height;
    uint8_t  label_enabled;
};

/* ------------------------------------------------------------------------- */

#endif // CBARGAUGE_H