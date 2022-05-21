#include "matrix.h"

#include <stdlib.h>

#ifdef ENABLE_STDLIB
#include <stdio.h>
#endif // ENABLE_STDLIB

#include "trigo.h"

#define _ENSURE_VALID_PTR(PTR)  \
    {                           \
        if ((PTR) == NULL)      \
        {                       \
            return ERR_MATDATA; \
        }                       \
    }

#define _ENSURE_MATRIX_PTR_VALID(MAT)             \
    {                                             \
        if ((MAT) == NULL || (MAT)->data == NULL) \
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

#define _ENSURE_CALL_SUCCESS(XXX) \
    {                             \
        int8_t err = (XXX);       \
        if (err != ERR_NO)        \
        {                         \
            return err;           \
        }                         \
    }

#define _MATPTR_LENGTH(MAT) ((MAT)->width * (MAT)->height)

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
        .data = malloc(width * height * sizeof(matdata_t)),
    };
    return mat;
}

int8_t Matrix_copy(Matrix_t *src, Matrix_t *dst)
{
    _ENSURE_VALID_PTR(dst);
    _ENSURE_MATRIX_PTR_VALID(src);

    *dst = Matrix_alloc(src->height, src->width);

    return Matrix_transfer(src, dst);
}

int8_t Matrix_transfer(Matrix_t *src, Matrix_t *dst)
{
    _ENSURE_MATRIX_PTR_VALID(src);
    _ENSURE_MATRIX_PTR_VALID(dst);

    uint16_t length = _MATPTR_LENGTH(src);
    for (uint16_t i = 0; i < length; i++)
    {
        dst->data[i] = src->data[i];
    }

    return ERR_NO;
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

int8_t Matrix_fill(Matrix_t *mat, matdata_t value)
{
    _ENSURE_MATRIX_PTR_VALID(mat);

    uint16_t length = _MATPTR_LENGTH(mat);
    for (uint16_t i = 0; i < length; i++)
    {
        mat->data[i] = value;
    }

    return ERR_NO;
}

int8_t Matrix_set_identity(Matrix_t *mat)
{
    _ENSURE_MATRIX_PTR_VALID(mat);
    _ENSURE_MATRIX_SQUARED(mat);

    uint16_t length = _MATPTR_LENGTH(mat);
    for (uint16_t i = 0; i < length; i++)
    {
        mat->data[i] = 0;
    }

    for (uint16_t i = 0; i < mat->width; i++)
    {
        MATPTR_AT_UNSAFE(mat, i, i) = 1;
    }

    return ERR_NO;
}

bool Matrix_shape_equal(Matrix_t *a, Matrix_t *b)
{
    return a != NULL && b != NULL && a->width == b->width && a->height == b->height;
}

int8_t Matrix_transpose(Matrix_t *src, Matrix_t *dst)
{
    _ENSURE_VALID_PTR(dst);
    _ENSURE_MATRIX_PTR_VALID(src);

    *dst = Matrix_alloc(src->width, src->height);
    _ENSURE_MATRIX_PTR_VALID(dst);

    for (uint16_t h = 0; h < src->height; h++)
    {
        for (uint16_t w = 0; w < src->width; w++)
        {
            MATPTR_AT_UNSAFE(dst, w, h) = MATPTR_AT_UNSAFE(src, h, w);
        }
    }

    return ERR_NO;
}

int8_t Matrix_add_const(Matrix_t *mat, matdata_t value)
{
    _ENSURE_MATRIX_PTR_VALID(mat);

    uint16_t length = _MATPTR_LENGTH(mat);
    for (uint16_t i = 0; i < length; i++)
    {
        mat->data[i] += value;
    }

    return ERR_NO;
}

int8_t Matrix_mul(Matrix_t *mat, matdata_t value)
{
    _ENSURE_MATRIX_PTR_VALID(mat);

    uint16_t length = _MATPTR_LENGTH(mat);
    for (uint16_t i = 0; i < length; i++)
    {
        mat->data[i] *= value;
    }

    return ERR_NO;
}

int8_t Matrix_div(Matrix_t *mat, matdata_t value)
{
    _ENSURE_MATRIX_PTR_VALID(mat);

    uint16_t length = _MATPTR_LENGTH(mat);
    for (uint16_t i = 0; i < length; i++)
    {
        mat->data[i] /= value;
    }

    return ERR_NO;
}

int8_t Matrix_dot(Matrix_t *a, Matrix_t *b, Matrix_t *out)
{
    _ENSURE_MATRIX_PTR_VALID(a);
    _ENSURE_MATRIX_PTR_VALID(b);
    _ENSURE_MATRIX_PTR_VALID(out);
    if (a->width != b->height || a->height != out->height || b->width != out->width)
    {
        return ERR_MATSHAPE;
    }

    for (uint16_t i = 0; i < out->height; i++)
    {
        for (uint16_t j = 0; j < out->width; j++)
        {
            matdata_t v = 0;

            for (uint16_t k = 0; k < a->width; k++)
            {
                v += MATPTR_AT_UNSAFE(a, i, k) * MATPTR_AT_UNSAFE(b, k, j);
            }

            MATPTR_AT_UNSAFE(out, i, j) = v;
        }
    }

    return ERR_NO;
}

int8_t Matrix_set_point(Matrix_t *mat, uint16_t row, matdata_t x, matdata_t y, matdata_t z)
{
    _ENSURE_MATRIX_PTR_VALID(mat);
    if (mat->width != 4 || row >= mat->height)
    {
        return ERR_MATSHAPE;
    }

    MATPTR_AT_UNSAFE(mat, row, 0) = x;
    MATPTR_AT_UNSAFE(mat, row, 1) = y;
    MATPTR_AT_UNSAFE(mat, row, 2) = z;
    MATPTR_AT_UNSAFE(mat, row, 3) = 1;

    return ERR_NO;
}

int8_t Matrix_project(Matrix_t *space, Matrix_t *plane, matdata_t f, matdata_t grid_size)
{
    _ENSURE_MATRIX_PTR_VALID(space);
    _ENSURE_MATRIX_PTR_VALID(plane);
    if (plane->height != space->height || plane->width != 2 || space->width < 3)
    {
        return ERR_MATSHAPE;
    }

    for (uint16_t h = 0; h < space->height; h++)
    {
        // FIXME: too much issue with perspective projection rounding for now
        // Switching to orthographic projection.

#ifdef USE_PROJECTION_ORTHOGRAPHIC
        matdata_t s = f;
#else  // USE_PROJECTION_ORTHOGRAPHIC
        matdata_t s = MATPTR_AT_UNSAFE(space, h, 2) * f / grid_size;
#endif // USE_PROJECTION_ORTHOGRAPHIC

        matdata_t x = s * MATPTR_AT_UNSAFE(space, h, 0);
        matdata_t y = s * MATPTR_AT_UNSAFE(space, h, 1);

        MATPTR_AT_UNSAFE(plane, h, 0) = x;
        MATPTR_AT_UNSAFE(plane, h, 1) = y;
    }

    return ERR_NO;
}

int8_t Matrix_make_rot_x(Matrix_t *mat, matdata_t theta)
{
    _ENSURE_CALL_SUCCESS(Matrix_clear(mat));
    _ENSURE_CALL_SUCCESS(Matrix_make_partial_rot_x(mat, theta));
    return ERR_NO;
}

int8_t Matrix_make_partial_rot_x(Matrix_t *mat, matdata_t theta)
{
    _ENSURE_MATRIX_PTR_VALID(mat);
    if (mat->width != 4 || mat->height != 4)
    {
        return ERR_MATSHAPE;
    }

    matdata_t cosT = soft_cos(theta);
    matdata_t sinT = soft_sin(theta);

    //              (mat, row, col) = v
    MATPTR_AT_UNSAFE(mat, 0, 0) = float_scale();

    MATPTR_AT_UNSAFE(mat, 1, 1) = cosT;
    MATPTR_AT_UNSAFE(mat, 1, 2) = -sinT;

    MATPTR_AT_UNSAFE(mat, 2, 1) = sinT;
    MATPTR_AT_UNSAFE(mat, 2, 2) = cosT;

    MATPTR_AT_UNSAFE(mat, 3, 3) = float_scale();

    return ERR_NO;
}

int8_t Matrix_make_rot_y(Matrix_t *mat, matdata_t theta)
{
    _ENSURE_CALL_SUCCESS(Matrix_clear(mat));
    _ENSURE_CALL_SUCCESS(Matrix_make_partial_rot_y(mat, theta));
    return ERR_NO;
}

int8_t Matrix_make_partial_rot_y(Matrix_t *mat, matdata_t theta)
{
    _ENSURE_MATRIX_PTR_VALID(mat);
    if (mat->width != 4 || mat->height != 4)
    {
        return ERR_MATSHAPE;
    }

    matdata_t cosT = soft_cos(theta);
    matdata_t sinT = soft_sin(theta);

    //              (mat, row, col) = v
    MATPTR_AT_UNSAFE(mat, 0, 0) = cosT;
    MATPTR_AT_UNSAFE(mat, 0, 2) = sinT;

    MATPTR_AT_UNSAFE(mat, 1, 1) = float_scale();

    MATPTR_AT_UNSAFE(mat, 2, 0) = -sinT;
    MATPTR_AT_UNSAFE(mat, 2, 1) = 0;
    MATPTR_AT_UNSAFE(mat, 2, 2) = cosT;

    MATPTR_AT_UNSAFE(mat, 3, 3) = float_scale();

    return ERR_NO;
}

int8_t Matrix_make_rot_z(Matrix_t *mat, matdata_t theta)
{
    _ENSURE_CALL_SUCCESS(Matrix_clear(mat));
    _ENSURE_CALL_SUCCESS(Matrix_make_partial_rot_z(mat, theta));
    return ERR_NO;
}

int8_t Matrix_make_partial_rot_z(Matrix_t *mat, matdata_t theta)
{
    _ENSURE_MATRIX_PTR_VALID(mat);
    if (mat->width != 4 || mat->height != 4)
    {
        return ERR_MATSHAPE;
    }

    matdata_t cosT = soft_cos(theta);
    matdata_t sinT = soft_sin(theta);

    //              (mat, row, col) = v
    MATPTR_AT_UNSAFE(mat, 0, 0) = cosT;
    MATPTR_AT_UNSAFE(mat, 0, 1) = -sinT;

    MATPTR_AT_UNSAFE(mat, 1, 0) = sinT;
    MATPTR_AT_UNSAFE(mat, 1, 1) = cosT;

    MATPTR_AT_UNSAFE(mat, 2, 2) = float_scale();

    MATPTR_AT_UNSAFE(mat, 3, 3) = float_scale();

    return ERR_NO;
}

#ifdef ENABLE_STDLIB

int8_t Matrix_dump(Matrix_t *mat)
{
    _ENSURE_MATRIX_PTR_VALID(mat);

    printf("[\r\n");
    for (uint16_t i = 0; i < mat->height; i++)
    {
        for (uint16_t j = 0; j < mat->width; j++)
        {
            matdata_t v = MATPTR_AT_UNSAFE(mat, i, j);
            if (v >= 0)
            {
                printf("+");
            }
            printf("%d, ", (int)(v));
        }

        printf("\r\n");
    }
    printf("]\r\n");

    return ERR_NO;
}

#endif // ENABLE_STDLIB