#ifndef _MATRIX_H_
#define _MATRIX_H_

#include <stdint.h>
#include <stdbool.h>

#define ERR_NO 0         // No error
#define ERR_MATDATA -1   // Matrix does not have valid data ptr
#define ERR_MATSQUARE -2 // Matrix is not squared
#define ERR_MATSHAPE -3  // Invalid matrix shape

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
 * Copy matrix struct.
 *
 * NOTE: dst does not need any allocation.
 */
int8_t Matrix_copy(Matrix_t *src, Matrix_t *dst);

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

/**
 * Check if two matrix have same shape.
 */
bool Matrix_shape_equal(Matrix_t *a, Matrix_t *b);

/**
 * Transpose matrix src to dst.
 *
 * NOTE: dst does not need any allocation.
 */
int8_t Matrix_transpose(Matrix_t *src, Matrix_t *dst);

/**
 * Add const to all matrix elements.
 */
int8_t Matrix_add_const(Matrix_t *mat, int16_t value);

/**
 * Scale all matrix elements.
 */
int8_t Matrix_scale(Matrix_t *mat, int16_t value);

/**
 * @brief Perform dot product of A and B.
 *
 * @param a Input matrix A
 * @param b Input matrix B
 * @param out Output matrix (require allocation with correct shape)
 * @return int8_t Status code
 */
int8_t Matrix_dot(Matrix_t *a, Matrix_t *b, Matrix_t *out);

#endif // _MATRIX_H_
