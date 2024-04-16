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

/* Analog to Digital Converter Pin Values */
Measurement adc0Data = Measurement("ADC0");
Measurement adc1Data = Measurement("ADC1");
Measurement adc2Data = Measurement("ADC2");
Measurement adc3Data = Measurement("ADC3");
Measurement adc4Data = Measurement("ADC4");

/* Simulated Car Sensor Data */
Measurement coolTemp = Measurement("CoolT");
Measurement oilTemp  = Measurement("OilT");
Measurement oilPres  = Measurement("OilP");
Measurement iat      = Measurement("IAT");
Measurement boost    = Measurement("Boost");

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

  /* ADC bar gauge initialisation */
  BarGauge * adcGauge0 = new BarGauge(&display, &adc0Data, "ADC0", 0, 512);
  adcGauge0->set_colour_normal(RED);
  BarGauge * adcGauge1 = new BarGauge(&display, &adc1Data, "ADC1", 0, 512);
  adcGauge1->set_colour_normal(GREEN);
  BarGauge * adcGauge2 = new BarGauge(&display, &adc2Data, "ADC2", 0, 512);
  adcGauge2->set_colour_normal(BLUE);
  BarGauge * adcGauge3 = new BarGauge(&display, &adc3Data, "ADC3", 0, 512);
  adcGauge3->set_colour_normal(YELLOW);
  BarGauge * adcGauge4 = new BarGauge(&display, &adc4Data, "ADC4", 0, 512);
  adcGauge4->set_colour_normal(ORANGE);
  adcLayout.add_gauge(adcGauge0);
  adcLayout.add_gauge(adcGauge1);
  adcLayout.add_gauge(adcGauge2);
  adcLayout.add_gauge(adcGauge3);
  adcLayout.add_gauge(adcGauge4);

  /* Car bar gauge initialisation */
  BarGauge * coolTBar = new BarGauge(&display, &coolTemp, "CoolT", 0, 100);
  coolTBar->set_limit_higher(100);
  coolTBar->set_limit_lower(70);
  coolTBar->set_colour_low(CYAN);
  coolTBar->set_colour_normal(GREEN);
  coolTBar->set_colour_high(RED);
  BarGauge * oilTBar =  new BarGauge(&display, &oilTemp, "OilT",   0, 150);
  oilTBar->set_limit_higher(120);
  oilTBar->set_limit_lower(70);
  oilTBar->set_colour_low(CYAN);
  oilTBar->set_colour_normal(GREEN);
  oilTBar->set_colour_high(RED);
  BarGauge * oilPBar =  new BarGauge(&display, &oilPres, "OilP",   0, 50);
  oilPBar->set_limit_lower(20);
  oilPBar->set_limit_higher(50);
  /* We don't want oil pressure to be too low OR too high */
  oilPBar->set_colour_low(RED);
  oilPBar->set_colour_high(RED);
  BarGauge * iatBar =   new BarGauge(&display, &iat, "IAT",        0, 50);
  iatBar->set_colour_normal(WHITE);
  BarGauge * boostBar = new BarGauge(&display, &boost, "Boost",    0, 15);
  boostBar->set_colour_normal(WHITE);
  carBarLayout.add_gauge(coolTBar);
  carBarLayout.add_gauge(oilTBar);
  carBarLayout.add_gauge(oilPBar);
  carBarLayout.add_gauge(iatBar);
  carBarLayout.add_gauge(boostBar);

  /* Car block gauge initialisation */
  BlockGauge * coolTBlock = new BlockGauge(&display, &coolTemp, "CoolT", 0, 100);
  BlockGauge * oilTBlock = new BlockGauge(&display, &oilTemp, "OilT", 0, 140);
  BlockGauge * oilPBlock = new BlockGauge(&display, &oilPres, "OilP", 0, 65);
  BlockGauge * iatBlock = new BlockGauge(&display, &iat, "IAT", 0, 50);
  BlockGauge * boostBlock = new BlockGauge(&display, &boost, "Boost", 0, 15);
  carBlockLayout.add_gauge(coolTBlock);
  carBlockLayout.add_gauge(oilTBlock);
  carBlockLayout.add_gauge(oilPBlock);
  carBlockLayout.add_gauge(iatBlock);
  carBlockLayout.add_gauge(boostBlock);

  /* Register the three layouts for use */
  add_layout(&carBarLayout);
  add_layout(&adcLayout);
  add_layout(&carBlockLayout);
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

  /* We want to handle touchscreen events very quickly, but don't want to 
     call the intensive draw function that often. */
  if(millis() - last_draw >= DRAW_DELAY) {
    update_measurements();
    draw();
    last_draw = millis();
  }

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
  adc0Data.set_value(analogRead(A0));
  adc1Data.set_value(analogRead(A1));
  adc2Data.set_value(analogRead(A2));
  adc3Data.set_value(analogRead(A3));
  adc4Data.set_value(analogRead(A4));
  adc4Data.set_value(analogRead(A5));

  /* Generate some simulated values for car sensors */
  coolTemp.set_value(random(87, 92));
  oilTemp.set_value(random(105, 108));
  oilPres.set_value(random(20, 35));
  oilPres.set_value(random(37, 42));
  iat.set_value(random(32, 35));
  boost.set_value(random(7, 12));
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