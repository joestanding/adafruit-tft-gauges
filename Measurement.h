#ifndef CMEASUREMENT_H
#define CMEASUREMENT_H

#include "Arduino.h"

/* ------------------------------------------------------------------------- */

#define MEASUREMENT_NAME_MAX 16

/* ------------------------------------------------------------------------- */

class Measurement {
public:
  Measurement(char * name);
  void set_data_type(uint32_t data_type);

  int32_t get_value();
  void    set_value(int32_t value);
private:
  uint32_t id;
  uint32_t data_type;
  int32_t value;
  char name[MEASUREMENT_NAME_MAX];
};

/* ------------------------------------------------------------------------- */

#endif // CMEASUREMENT_H