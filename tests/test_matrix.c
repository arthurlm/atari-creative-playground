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

// Main ============================================================================================

int main()
{
    TEST_CASE_RUN(test_matrix_init_and_destroy);
    TEST_CASE_RUN(test_matrix_empty);
    TEST_CASE_RUN(test_matrix_double_free);
    TEST_CASE_RUN(test_matrix_clear);
    TEST_CASE_RUN(test_matrix_set_identity_invalid);
    TEST_CASE_RUN(test_matrix_set_identity);

    return 0;
}
