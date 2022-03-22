#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdint.h>

#define ERR_NO 0         // No error
#define ERR_MATDATA -1   // Matrix does not have valid data ptr
#define ERR_MATSQUARE -2 // Matrix is not squared

// Define helper to read matrix content
#define MAT_AT_UNSAFE(MAT, II, JJ) (MAT).data[(II) * (MAT).width + (JJ)]
#define MATPTR_AT_UNSAFE(MAT, II, JJ) (MAT)->data[(II) * (MAT)->width + (JJ)]

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
int8_t Matrix_free(Matrix_t *mat);

/**
 * Clear matrix content.
 */
int8_t Matrix_clear(Matrix_t *mat);

/**
 * Set matrix content to value.
 */
int8_t Matrix_fill(Matrix_t *mat, int16_t value);

/**
 * Fill matrix as identity matrix.
 */
int8_t Matrix_set_identity(Matrix_t *mat);

#endif // _MATRIX_H_
