# Adafruit TFT Gauge Library

This library currently provides two types of configurable visual gauges that can be used on an Adafruit TFT screen to display various types of numerical information. The library was developed for my own personal needs, but I've tried to make the gauges fairly configurable so they should also be suitable for many other applications.

## Gauges
---
Two gauge types are currently provided, `BarGauge` and `BlockGauge`. I find the `BarGauge` provides more than enough of the functionality I've needed so far when displaying information on an embedded device.

Gauges will not be fully redrawn upon each `draw()` call - instead, areas that have changed will be calculated, and only those areas will be redrawn. This provides a significant performance improvement, especially with bar gauges which would otherwise have to redraw large filled areas each time.
### Bar Gauge

![Bar gauge visual demonstration](images/bar-gauge-demo.jpg?raw=true)

The bar gauge is XXXXX.

To significantly reduce draw time and eliminate draw flicker, only the delta of the bar is added or removed between each draw call. For example, if a bar was originally drawn at 70% of its maximum size, but the next call required it to be 75%, only the additional 5% will be drawn.

###### Configuration
- `void set_limit_higher(double value)`: Set the upper alert limit.
- `void set_limit_lower(double value)`: Set the lower alert limit.
- `void set_colour_high(uint16_t colour)`: Set the bar colour for when the upper limit has been reached.
- `void set_colour_low(uint16_t colour)`: Set the bar colour for when the lower limit has been reached.
- `void set_colour_normal(uint16_t colour)`: Set the bar colour for no limits are set or no limits have been reached.
- `void set_frame_colour(uint16_t value)`: Sets the colour of the rectangular frame drawn around the bar.
- `void set_limit_bar_height(uint32_t value)`: Sets the height (in pixels) of the bar drawn to indicate upper and lower value limits.
- `void enable_labels()`: Enables textual value label and gauge name label.
- `void disable_labels()`: Disables textual value label and gauge name label.

### Block Gauge

![Block gauge visual demonstration](images/block-gauge-demo.jpg?raw=true)

The block gauge is a simple square, featuring the data reading prominently in the centre of the block with its label beneath it. The block gauge by default features a black background and white outline, but the background colour of the block can be configured to change depending on whether a lower or upper value limit have been set.

###### Configuration
- `void set_limit_higher(double value)`: Set the upper alert limit.
- `void set_limit_lower(double value)`: Set the lower alert limit.
- `void set_colour_high(uint16_t colour)`: Set the background colour for when the upper limit has been reached.
- `void set_colour_low(uint16_t colour)`: Set the background colour for when the lower limit has been reached.
- `void set_text_colour_high(uint16_t colour)`: Set the text colour for when the upper limit has been reached.
- `void set_text_colour_low(uint16_t colour)`: Set the text colour for when the lower limit has been reached.
- `void set_colour_normal(uint16_t colour)`: Set the bar colour for no limits are set or no limits have been reached.
- `void set_frame_colour(uint16_t value)`: Sets the colour of the rectangular frame drawn around the block.

  
## Layouts
---
Layouts are used to define the position and size of gauges. The code features two default layouts, `ColumnLayout` and `GridLayout`. New layouts can be added by creating a subclass of the `Layout` class, and overriding the `draw()` method. 

Layouts are very simple in their implementation, with the main work performed in the `draw()` method. The `draw()`method is expected to iterate through the internal `gauges` list, and call the `draw()` function of each gauge with co-ordinates and dimensions decided by the layout.
### ColumnLayout
In a `ColumnLayout`, each added gauge is drawn as a new vertical column, horizontally adjacent to any previous gauges. No configuration is required, as each gauge's width is scaled relative to how many gauges are being shown. For example, a `ColumnLayout` of three gauges will render them with the full height of the screen, and third of the screen width each.

### GridLayout
In a `GridLayout`, gauges are organised in columns and rows. Unlike `ColumnLayout`, the grid layout features two configuration options, `void set_gauges_wide(uint16_t value)` and `void set_gauges_high(uint16_t value)`. These functions can be used to configure how many rows and columns will be used by the grid layout.

### Custom Layout Example
In the below example code, the first three gauges are rendered in a vertical column that takes up half the width of the screen. The final, fourth, gauge is then rendered in the second vertical column, taking up the full height of the screen.

```cpp
/* An example layout to draw a column of three gauges vertically, then
   another column containing a single gauge on its right. */
void ExampleLayout::draw() {
  /* Set the beginning dimensions for the first three gauges */
  uint16_t x = 0;
  uint16_t y = 0;
  uint16_t w = this->display->width() / 2;
  uint16_t h = this->display->height() / 3;

  /* Iterate through the registered gauges and draw them */
  for(uint8_t i = 0; i < this->gauge_count; i++) {
    this->gauges[i]->draw(x, y, w, h);

    /* If it's one of the first three gauges, shift the next gauge down. */
    if(i < 2) {
      y += h;
    }

    /* If it's the final gauge, shift it to the right, begin from the top of
       the screen, and set the height to the maximum. */
    if(i == 2) {
      x += w;
      y = 0;
      h = this->display->height();
    }
  }
}
```


## Demo Code
---
Demonstration code for the library can be found in the `gauges.ino` file, which initialises several demo layouts, including simulated sensor data from a vehicle.