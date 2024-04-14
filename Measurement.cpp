#include "Measurement.h"

/* ------------------------------------------------------------------------- */

Measurement::Measurement(char * name) {
  strncpy(this->name, name, MEASUREMENT_NAME_MAX);
}

/* ------------------------------------------------------------------------- */

void Measurement::set_data_type(uint32_t data_type) {
  this->data_type = data_type;
}

/* ------------------------------------------------------------------------- */

void Measurement::set_value(int32_t value) {
  this->value = value;
}

/* ------------------------------------------------------------------------- */

int32_t Measurement::get_value() {
  return this->value;
}

/* ------------------------------------------------------------------------- */