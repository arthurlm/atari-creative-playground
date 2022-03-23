#include "matrix.h"

#include <stdlib.h>
#include <stdio.h>

void panic()
{
    exit(1);
}

#define TEST_CASE_RUN(XX)                                    \
    {                                                        \
        printf("Running test: " #XX " (in progress)\r\n");   \
        (XX)();                                              \
        printf("Running test: " #XX " (completed !)\r\n\n"); \
    }

#define ASSERT_EQUALS(XX, YY)                                                    \
    if ((XX) != (YY))                                                            \
    {                                                                            \
        printf("ASSERT_EQUALS(" #XX ", " #YY ")@%s:%d\n\r", __FILE__, __LINE__); \
        panic();                                                                 \
    }

#define ASSERT_NOT_EQUALS(XX, YY)                                                    \
    if ((XX) == (YY))                                                                \
    {                                                                                \
        printf("ASSERT_NOT_EQUALS(" #XX ", " #YY ")@%s:%d\n\r", __FILE__, __LINE__); \
        panic();                                                                     \
    }

// Test definition =================================================================================

void test_matrix_init_and_destroy()
{
    Matrix_t mat = Matrix_alloc(10, 5);

    ASSERT_EQUALS(mat.height, 10);
    ASSERT_EQUALS(mat.width, 5);
    ASSERT_NOT_EQUALS(mat.data, NULL);

    Matrix_free(&mat);

    ASSERT_EQUALS(mat.height, 0);
    ASSERT_EQUALS(mat.width, 0);
    ASSERT_EQUALS(mat.data, NULL);
}

void test_matrix_empty()
{
    Matrix_t mat = Matrix_alloc(0, 0);

    ASSERT_EQUALS(mat.height, 0);
    ASSERT_EQUALS(mat.width, 0);
    ASSERT_EQUALS(mat.data, NULL);

    Matrix_free(&mat);
}

void test_matrix_copy()
{
    Matrix_t src = Matrix_alloc(3, 2);
    Matrix_fill(&src, 4);
    MAT_AT_UNSAFE(src, 0, 1) = 154;
    MAT_AT_UNSAFE(src, 2, 1) = -9;

    Matrix_t dst;

    ASSERT_EQUALS(Matrix_copy(NULL, NULL), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_copy(&src, NULL), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_copy(NULL, &dst), ERR_MATDATA);

    ASSERT_EQUALS(Matrix_copy(&src, &dst), ERR_NO);

    ASSERT_EQUALS(dst.height, 3);
    ASSERT_EQUALS(dst.width, 2);
    ASSERT_NOT_EQUALS(dst.data, NULL);

    ASSERT_EQUALS(MAT_AT_UNSAFE(src, 0, 0), 4);
    ASSERT_EQUALS(MAT_AT_UNSAFE(src, 0, 1), 154);
    ASSERT_EQUALS(MAT_AT_UNSAFE(src, 1, 0), 4);
    ASSERT_EQUALS(MAT_AT_UNSAFE(src, 1, 1), 4);
    ASSERT_EQUALS(MAT_AT_UNSAFE(src, 2, 0), 4);
    ASSERT_EQUALS(MAT_AT_UNSAFE(src, 2, 1), -9);

    ASSERT_EQUALS(Matrix_free(&src), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&dst), ERR_NO);
}

void test_matrix_double_free()
{
    Matrix_t mat = Matrix_alloc(10, 5);

    ASSERT_EQUALS(mat.height, 10);
    ASSERT_EQUALS(mat.width, 5);
    ASSERT_NOT_EQUALS(mat.data, NULL);

    ASSERT_EQUALS(Matrix_free(&mat), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&mat), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_free(&mat), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_free(&mat), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_free(&mat), ERR_MATDATA);

    ASSERT_EQUALS(mat.height, 0);
    ASSERT_EQUALS(mat.width, 0);
    ASSERT_EQUALS(mat.data, NULL);
}

void test_matrix_clear()
{
    Matrix_t mat = Matrix_alloc(2, 4);
    ASSERT_EQUALS(Matrix_clear(&mat), ERR_NO);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 0), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 1), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 2), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 3), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 0), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 1), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 2), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 3), 0);
    ASSERT_EQUALS(Matrix_free(&mat), ERR_NO);
}

void test_matrix_set_identity_invalid()
{
    Matrix_t mat = Matrix_alloc(2, 4);
    ASSERT_EQUALS(Matrix_set_identity(&mat), ERR_MATSQUARE);

    ASSERT_EQUALS(Matrix_free(&mat), ERR_NO);
    ASSERT_EQUALS(Matrix_set_identity(&mat), ERR_MATDATA);
}

void test_matrix_set_identity()
{
    Matrix_t mat = Matrix_alloc(3, 3);
    ASSERT_EQUALS(Matrix_set_identity(&mat), ERR_NO);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 0), 1);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 1), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 2), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 0), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 1), 1);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 2), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 2, 0), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 2, 1), 0);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 2, 2), 1);
    ASSERT_EQUALS(Matrix_free(&mat), ERR_NO);
}

void test_matrix_shape_equal()
{
    Matrix_t a = Matrix_alloc(2, 4);
    Matrix_t b = Matrix_alloc(2, 4);
    Matrix_t c = Matrix_alloc(4, 3);

    ASSERT_EQUALS(Matrix_shape_equal(NULL, NULL), false);
    ASSERT_EQUALS(Matrix_shape_equal(NULL, &a), false);
    ASSERT_EQUALS(Matrix_shape_equal(&a, NULL), false);
    ASSERT_EQUALS(Matrix_shape_equal(&a, &b), true);
    ASSERT_EQUALS(Matrix_shape_equal(&a, &c), false);

    ASSERT_EQUALS(Matrix_free(&a), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&b), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&c), ERR_NO);
}

void test_matrix_transpose()
{
    Matrix_t a = Matrix_alloc(2, 4);
    Matrix_fill(&a, 5);
    MAT_AT_UNSAFE(a, 0, 1) = -18;
    MAT_AT_UNSAFE(a, 0, 3) = 10;
    MAT_AT_UNSAFE(a, 1, 2) = -3;

    Matrix_t b;

    ASSERT_EQUALS(Matrix_transpose(NULL, NULL), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_transpose(NULL, &a), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_transpose(&a, NULL), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_transpose(&a, &b), ERR_NO);

    ASSERT_EQUALS(a.height, 2);
    ASSERT_EQUALS(a.width, 4);
    ASSERT_EQUALS(b.height, 4);
    ASSERT_EQUALS(b.width, 2);

    ASSERT_EQUALS(MAT_AT_UNSAFE(b, 0, 0), 5);
    ASSERT_EQUALS(MAT_AT_UNSAFE(b, 0, 1), 5);
    ASSERT_EQUALS(MAT_AT_UNSAFE(b, 1, 0), -18);
    ASSERT_EQUALS(MAT_AT_UNSAFE(b, 1, 1), 5);
    ASSERT_EQUALS(MAT_AT_UNSAFE(b, 2, 0), 5);
    ASSERT_EQUALS(MAT_AT_UNSAFE(b, 2, 1), -3);
    ASSERT_EQUALS(MAT_AT_UNSAFE(b, 3, 0), 10);
    ASSERT_EQUALS(MAT_AT_UNSAFE(b, 3, 1), 5);

    ASSERT_EQUALS(Matrix_free(&a), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&b), ERR_NO);
}

void test_matrix_add_const()
{
    Matrix_t mat = Matrix_alloc(2, 4);
    Matrix_fill(&mat, 5);
    MAT_AT_UNSAFE(mat, 0, 1) = -18;
    MAT_AT_UNSAFE(mat, 0, 3) = 10;
    MAT_AT_UNSAFE(mat, 1, 2) = -3;

    ASSERT_EQUALS(Matrix_add_const(NULL, 2), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_add_const(&mat, 2), ERR_NO);

    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 0), 7);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 1), -16);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 2), 7);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 3), 12);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 0), 7);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 1), 7);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 2), -1);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 3), 7);

    ASSERT_EQUALS(Matrix_free(&mat), ERR_NO);
}

void test_matrix_scale()
{
    Matrix_t mat = Matrix_alloc(2, 4);
    Matrix_fill(&mat, 5);
    MAT_AT_UNSAFE(mat, 0, 1) = -18;
    MAT_AT_UNSAFE(mat, 0, 3) = 10;
    MAT_AT_UNSAFE(mat, 1, 2) = -3;

    ASSERT_EQUALS(Matrix_scale(NULL, 2), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_scale(&mat, 2), ERR_NO);

    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 0), 10);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 1), -36);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 2), 10);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 0, 3), 20);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 0), 10);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 1), 10);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 2), -6);
    ASSERT_EQUALS(MAT_AT_UNSAFE(mat, 1, 3), 10);

    ASSERT_EQUALS(Matrix_free(&mat), ERR_NO);
}

void test_matrix_dot_shape()
{
    Matrix_t a = Matrix_alloc(2, 4);
    Matrix_t b = Matrix_alloc(4, 3);
    Matrix_t out = Matrix_alloc(2, 3);

    ASSERT_EQUALS(Matrix_dot(NULL, NULL, NULL), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_dot(NULL, NULL, &out), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_dot(NULL, &b, NULL), ERR_MATDATA);
    ASSERT_EQUALS(Matrix_dot(&a, NULL, NULL), ERR_MATDATA);

    ASSERT_EQUALS(Matrix_dot(&b, &a, &out), ERR_MATSHAPE);
    ASSERT_EQUALS(Matrix_dot(&a, &b, &out), ERR_NO);

    ASSERT_EQUALS(Matrix_free(&a), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&b), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&out), ERR_NO);
}

void test_matrix_dot_data()
{
    Matrix_t a = Matrix_alloc(2, 3);
    MAT_AT_UNSAFE(a, 0, 0) = 1;
    MAT_AT_UNSAFE(a, 0, 1) = 2;
    MAT_AT_UNSAFE(a, 0, 2) = 0;
    MAT_AT_UNSAFE(a, 1, 0) = 4;
    MAT_AT_UNSAFE(a, 1, 1) = 3;
    MAT_AT_UNSAFE(a, 1, 2) = -1;

    Matrix_t b = Matrix_alloc(3, 2);
    MAT_AT_UNSAFE(b, 0, 0) = 5;
    MAT_AT_UNSAFE(b, 0, 1) = 1;
    MAT_AT_UNSAFE(b, 1, 0) = 2;
    MAT_AT_UNSAFE(b, 1, 1) = 3;
    MAT_AT_UNSAFE(b, 2, 0) = 3;
    MAT_AT_UNSAFE(b, 2, 1) = 4;

    Matrix_t out1 = Matrix_alloc(2, 2);
    Matrix_t out2 = Matrix_alloc(3, 3);

    ASSERT_EQUALS(Matrix_dot(&a, &b, &out1), ERR_NO);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out1, 0, 0), 9);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out1, 0, 1), 7);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out1, 1, 0), 23);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out1, 1, 1), 9);

    ASSERT_EQUALS(Matrix_dot(&b, &a, &out2), ERR_NO);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out2, 0, 0), 9);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out2, 0, 1), 13);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out2, 0, 2), -1);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out2, 1, 0), 14);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out2, 1, 1), 13);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out2, 1, 2), -3);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out2, 2, 0), 19);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out2, 2, 1), 18);
    ASSERT_EQUALS(MAT_AT_UNSAFE(out2, 2, 2), -4);

    ASSERT_EQUALS(Matrix_free(&a), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&b), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&out1), ERR_NO);
    ASSERT_EQUALS(Matrix_free(&out2), ERR_NO);
}

// Main ============================================================================================

int main()
{
    TEST_CASE_RUN(test_matrix_init_and_destroy);
    TEST_CASE_RUN(test_matrix_empty);
    TEST_CASE_RUN(test_matrix_copy);
    TEST_CASE_RUN(test_matrix_double_free);
    TEST_CASE_RUN(test_matrix_clear);
    TEST_CASE_RUN(test_matrix_set_identity_invalid);
    TEST_CASE_RUN(test_matrix_set_identity);
    TEST_CASE_RUN(test_matrix_shape_equal);
    TEST_CASE_RUN(test_matrix_transpose);
    TEST_CASE_RUN(test_matrix_add_const);
    TEST_CASE_RUN(test_matrix_scale);
    TEST_CASE_RUN(test_matrix_dot_shape);
    TEST_CASE_RUN(test_matrix_dot_data);

    return 0;
}
