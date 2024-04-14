# Adafruit TFT Gauge Library
This library currently provides two types of configurable gauges that can be used on an Adafruit TFT screen to display various types of numerical information. The library was developed for my own personal requirements, but I've tried to make the gauges fairly configurable so should also be suitable for many other applications.

## Bar Gauge
![Bar gauge visual demonstration](images/bar-gauge-demo.jpg?raw=true)

Bar gauges currently feature:
- Configurable upper and lower limits, with optional limit marker bar
- Configurable bar colour depending on whether any of the limits have been hit
- Toggleable label


#### Block Gauge
![Block gauge visual demonstration](images/block-gauge-demo.jpg?raw=true)

The block gauge is a simple square, featuring the data reading prominently in the centre of the block with its label beneath it. The block gauge by default features a black background and white outline, but the background colour of the block can be configured to change depending on whether a lower or upper value limit have been set.