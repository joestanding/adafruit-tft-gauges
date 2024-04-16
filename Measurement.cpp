#include "Measurement.h"

/* ------------------------------------------------------------------------- */

Measurement::Measurement() {
}

/* ------------------------------------------------------------------------- */

void Measurement::set_data_type(uint32_t data_type) {
  this->data_type = data_type;
}

/* ------------------------------------------------------------------------- */

void Measurement::set_value(double value) {
  this->value = value;
}

/* ------------------------------------------------------------------------- */

double Measurement::get_value() {
  return this->value;
}

/* ------------------------------------------------------------------------- */