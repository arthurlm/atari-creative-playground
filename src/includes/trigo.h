#ifndef _TRIGO_H_
#define _TRIGO_H_

#include <stdint.h>

typedef int16_t softfloat_t;

softfloat_t float_scale();
int16_t sizeof_tables();

softfloat_t soft_cos(softfloat_t value);
softfloat_t soft_sin(softfloat_t value);

#endif // _TRIGO_H_