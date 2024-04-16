/*****************************************************************************/
/*                       Adafruit TFT Gauge Library                          */
/*                       --------------------------                          */
/* A gauge library for the Adafruit TFT screens, providing configurable bar  */
/* and block gauges for displaying numerical information. The example code   */
/* below initialises a range of demonstration layouts, including simulated   */
/* data from a car's ECU (oil temp., etc). Tapping the touchscreen skips to  */
/* the next layout. This code specifically targets the Adafruit 240x320      */
/* capacitive touchscreen (ILI9341 display controller, FT6206 touchscreen    */
/* controller), but should be easily adaptable for other controllers.        */
/*                                                                           */
/* Documentation can be found at:                                            */
/* https://github.com/joestanding/adafruit-tft-gauges                        */
/*                                                                           */
/*****************************************************************************/

#include "main.h"

/* ------------------------------------------------------------------------- */
/* Measurement Set-up                                                        */
/* ------------------------------------------------------------------------- */

/* Simulated Car Sensor Data */
Measurement coolTemp = Measurement();
Measurement oilTemp  = Measurement();
Measurement oilPres  = Measurement();
Measurement iat      = Measurement();
Measurement boost    = Measurement();

/* Values to test drawing of values in and out of bounds */
Measurement testMax  = Measurement();
Measurement testMin  = Measurement();
Measurement testNeg  = Measurement();
Measurement testBelowMin = Measurement();
Measurement testAboveMax = Measurement();

/* ------------------------------------------------------------------------- */
/* Device Initialisation                                                     */
/* ------------------------------------------------------------------------- */

void setup() {
  /* Serial initialisation */
  Serial.begin(9600);

  /* Display initialisation */
  display.begin();
  display.setRotation(1);
  display.fillScreen(BLACK);
  display.setTextColor(WHITE, BLACK);
  touchscreen.begin();

  /* Coolant temperature bar: ensuring our coolant is not too hot is the
     priority, as this will allow the engine to overheat. We set the bar
     to red in this scenario, but also add a lower limit just to tell us
     when the coolant is still quite cold. */
  BarGauge * coolTBar = new BarGauge(&display, &coolTemp, "CoolT", 0, 100);
  coolTBar->set_limit_higher(100);
  coolTBar->set_limit_lower(50);
  coolTBar->set_colour_low(CYAN);
  coolTBar->set_colour_normal(GREEN);
  coolTBar->set_colour_high(RED);

  /* Oil temperature bar: we want to keep oil within a certain acceptable
     temp. range, as oil that's too cold is not viscous enough, and oil that's
     too hot can degrade and lose its effectiveness. The bar therefore is
     cyan when the oil is below its limits (too cold), and red when the oil
     is above its limits (too hot). When it's in the middle, it's green. */
  BarGauge * oilTBar =  new BarGauge(&display, &oilTemp, "OilT", 0, 150);
  oilTBar->set_limit_higher(120);
  oilTBar->set_limit_lower(70);
  oilTBar->set_colour_low(CYAN);
  oilTBar->set_colour_normal(GREEN);
  oilTBar->set_colour_high(RED);

  /* Oil pressure bar: maintaining pressure is critical, and ensuring that
     it does not drop too low is most important. However, oil pressure that's
     too high is also a sign of a failure (e.g. blockage), so we set the limits
     for both, and make the bar red for either scenario. */
  BarGauge * oilPBar =  new BarGauge(&display, &oilPres, "OilP", 0, 50);
  oilPBar->set_limit_lower(20);
  oilPBar->set_limit_higher(50);
  /* We don't want oil pressure to be too low OR too high */
  oilPBar->set_colour_low(RED);
  oilPBar->set_colour_high(RED);

  /* Intake air temperature bar: Colder air is better for engine performance,
     so we don't set a lower limit. Hotter air will reduce engine performance,
     but is also unlikely to be a major issue, so we set the higher limit but
     assign the colour to orange, as red suggests a failure. */
  BarGauge * iatBar =   new BarGauge(&display, &iat, "IAT", 0, 40);
  iatBar->set_limit_higher(40);
  iatBar->set_colour_high(ORANGE);

  /* Turbo boost: boost levels will be very low at low RPM, so we don't need
     any sort of lower limit. Additionally, low boost isn't necessarily good
     or bad, so we make the bar white by default. Too high boost can definitely
     be a problem, so we set an upper limit and set the bar to red if it's
     exceeded. */
  BarGauge * boostBar = new BarGauge(&display, &boost, "Boost", 0, 30);
  boostBar->set_limit_higher(30);
  boostBar->set_colour_high(RED);
  boostBar->set_colour_normal(WHITE);

  /* Add all of the registered car gauges to the car layout */
  carBarLayout.add_gauge(coolTBar);
  carBarLayout.add_gauge(oilTBar);
  carBarLayout.add_gauge(oilPBar);
  carBarLayout.add_gauge(iatBar);
  carBarLayout.add_gauge(boostBar);

  /* Configure block gauges for car sensors */
  BlockGauge * coolTBlock = new BlockGauge(&display, &coolTemp, "CoolT", 0, 100);
  coolTBlock->set_limit_higher(100);
  coolTBlock->set_limit_lower(84);
  coolTBlock->set_text_colour_normal(DARKGREY);
  coolTBlock->set_text_colour_low(BLACK);
  coolTBlock->set_frame_colour(DARKGREY);
  BlockGauge * oilTBlock = new BlockGauge(&display, &oilTemp, "OilT", 0, 125);
  oilTBlock->set_limit_higher(120);
  BlockGauge * oilPBlock = new BlockGauge(&display, &oilPres, "OilP", 0, 65);
  BlockGauge * iatBlock = new BlockGauge(&display, &iat, "IAT", 0, 50);
  BlockGauge * boostBlock = new BlockGauge(&display, &boost, "Boost", 0, 15);

  /* Add car block gauges */
  carBlockLayout.add_gauge(coolTBlock);
  carBlockLayout.add_gauge(oilTBlock);
  carBlockLayout.add_gauge(oilPBlock);
  carBlockLayout.add_gauge(iatBlock);
  carBlockLayout.add_gauge(boostBlock);

  /* Bounds testing */
  BarGauge * testMinBar = new BarGauge(&display, &testMin, "Min", 0, 100);
  BarGauge * testMaxBar = new BarGauge(&display, &testMax, "Max", 0, 100);
  BarGauge * testNegBar = new BarGauge(&display, &testNeg, "Neg", -10, 10);
  BarGauge * testBMinBar = new BarGauge(&display, &testBelowMin, "BMin", 0, 100);
  BarGauge * testAMaxBar = new BarGauge(&display, &testAboveMax, "AMax", 0, 100);
  testMin.set_value(0);
  testMax.set_value(100);
  testNeg.set_value(-5);
  testBelowMin.set_value(-10);
  testAboveMax.set_value(110);

  testLayout.add_gauge(testMinBar);
  testLayout.add_gauge(testMaxBar);
  testLayout.add_gauge(testNegBar);
  testLayout.add_gauge(testBMinBar);
  testLayout.add_gauge(testAMaxBar);

  /* Register the three layouts for use */
  add_layout(&carBarLayout);
  add_layout(&carBlockLayout);
  add_layout(&testLayout);
}

/* ------------------------------------------------------------------------- */
/* Main Loop                                                                 */
/* ------------------------------------------------------------------------- */
void loop() {
  /* Pressing the touch screen will skip to the next layout */
  if(touchscreen.touched() && last_touch_state != 1) {
    next_layout();
  }
  last_touch_state = touchscreen.touched();

  update_measurements();
  draw();

  delay(MAIN_LOOP_DELAY);
}

/* ------------------------------------------------------------------------- */
/* Drawing                                                                   */
/* ------------------------------------------------------------------------- */

void draw() {
  layouts[current_layout]->draw();
}

/* ------------------------------------------------------------------------- */
/* Data Handling                                                             */
/* ------------------------------------------------------------------------- */

void update_measurements() {
  /* Generate some simulated values for car sensors */
  coolTemp.set_value(random(42, 47));
  oilTemp.set_value(random(132, 133));
  oilPres.set_value(random(37, 38));
  iat.set_value(random(24, 27));
  boost.set_value(random(18, 21));
}

/* ------------------------------------------------------------------------- */
/* Layout Handling                                                           */
/* ------------------------------------------------------------------------- */

void add_layout(Layout * new_layout) {
  for(uint8_t i = 0; i < MAX_LAYOUTS; i++) {
    if(layouts[i] == NULL) {
      layouts[i] = new_layout;
      return;
    }
  }
}

/* ------------------------------------------------------------------------- */

void next_layout() {
  do {
    current_layout = (current_layout + 1) % MAX_LAYOUTS;
  } while (layouts[current_layout] == NULL);

  if (layouts[current_layout] != NULL) {
    display.fillScreen(0);
    layouts[current_layout]->redraw();
  }
}

/* ------------------------------------------------------------------------- */