#include "matrix.h"

#include <stdlib.h>

#define _ENSURE_MATRIX_PTR_VALID(MAT)             \
    {                                             \
        if ((mat) == NULL || (mat)->data == NULL) \
        {                                         \
            return ERR_MATDATA;                   \
        }                                         \
    }

#define _ENSURE_MATRIX_SQUARED(mat)    \
    {                                  \
        if (mat->width != mat->height) \
        {                              \
            return ERR_MATSQUARE;      \
        }                              \
    }

Matrix_t Matrix_alloc(uint16_t height, uint16_t width)
{
    if (height == 0 || width == 0)
    {
        Matrix_t mat = {
            .width = 0,
            .height = 0,
            .data = NULL,
        };
        return mat;
    }

    Matrix_t mat = {
        .width = width,
        .height = height,
        .data = malloc(width * height * sizeof(int16_t)),
    };
    return mat;
}

int8_t Matrix_free(Matrix_t *mat)
{
    _ENSURE_MATRIX_PTR_VALID(mat);

    free(mat->data);
    mat->data = NULL;
    mat->width = 0;
    mat->height = 0;

    return ERR_NO;
}

int8_t Matrix_clear(Matrix_t *mat)
{
    return Matrix_fill(mat, 0);
}

int8_t Matrix_fill(Matrix_t *mat, int16_t value)
{
    _ENSURE_MATRIX_PTR_VALID(mat);

    int16_t length = (mat->width * mat->height);
    for (int16_t i = 0; i < length; i++)
    {
        mat->data[i] = value;
    }

    return ERR_NO;
}

int8_t Matrix_set_identity(Matrix_t *mat)
{
    _ENSURE_MATRIX_PTR_VALID(mat);
    _ENSURE_MATRIX_SQUARED(mat);

    for (int16_t h = 0; h < mat->height; h++)
    {
        for (int16_t w = 0; w < mat->width; w++)
        {
            MATPTR_AT_UNSAFE(mat, w, h) = (w == h) ? 1 : 0;
        }
    }

    return ERR_NO;
}