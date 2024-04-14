#ifndef CCOLUMNLAYOUT_H
#define CCOLUMNLAYOUT_H

#include "Layout.h"

/* ------------------------------------------------------------------------- */

class ColumnLayout : public Layout {
public:
    ColumnLayout(Adafruit_ILI9341 * display);
    void draw() override;
};

/* ------------------------------------------------------------------------- */

#endif
