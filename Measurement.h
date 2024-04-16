#ifndef CMEASUREMENT_H
#define CMEASUREMENT_H

#include "Arduino.h"

/* ------------------------------------------------------------------------- */

class Measurement {
public:
  Measurement();
  void    set_data_type(uint32_t data_type);
  double  get_value();
  void    set_value(double value);
private:
  uint32_t id;
  uint32_t data_type;
  double value;
};

/* ------------------------------------------------------------------------- */

#endif // CMEASUREMENT_H