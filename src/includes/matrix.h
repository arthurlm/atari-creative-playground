#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdint.h>

typedef struct Matrix
{
    uint16_t width;
    uint16_t height;
    int16_t *data;
} Matrix_t;

/**
 * Allocate struct.
 *
 * @param height Height
 * @param width Width
 */
Matrix_t Matrix_alloc(uint16_t height, uint16_t width);

/**
 * Free struct.
 */
void Matrix_free(Matrix_t *mat);

#endif // _MATRIX_H_
