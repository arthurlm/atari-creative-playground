#include "matrix.h"

#include <stdlib.h>

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

void Matrix_free(Matrix_t *mat)
{
    if (mat == NULL || mat->data == NULL)
    {
        return;
    }

    free(mat->data);
    mat->data = NULL;
    mat->width = 0;
    mat->height = 0;
}