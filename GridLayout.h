#ifndef CGRIDLAYOUT_H
#define CGRIDLAYOUT_H

#include "Layout.h"

/* ------------------------------------------------------------------------- */

class GridLayout : public Layout {
public:
  GridLayout(Adafruit_ILI9341 * display);
  void draw() override;

  void set_gauges_wide(uint16_t value);
  void set_gauges_high(uint16_t value);
private:
  uint16_t gauges_wide;
  uint16_t gauges_high;
};

/* ------------------------------------------------------------------------- */

#endif
