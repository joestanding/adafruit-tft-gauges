#include "ColumnLayout.h"

/* ------------------------------------------------------------------------- */

ColumnLayout::ColumnLayout(Adafruit_ILI9341 * display) : Layout(display) {

}

/* ------------------------------------------------------------------------- */

void ColumnLayout::draw() {
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t width = this->display->width() / this->gauge_count;
  uint16_t height = this->display->height();

  if(this->title_enabled) {
    y = 27;
    height -= 27;
    this->display->setTextColor(WHITE, BLACK);
    this->display->setTextSize(2);
    this->display->setCursor(7, 7);
    this->display->print(this->title);
  }

  for(uint8_t i = 0; i < this->gauge_count; i++) {
    if(this->gauges[i] != NULL) {
      this->gauges[i]->draw(x, y, width, height);
      x += width;
    }
  }
}

/* ------------------------------------------------------------------------- */