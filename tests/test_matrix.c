#include "matrix.h"

#include <stdlib.h>

void panic()
{
    exit(1);
}

#define ASSERT_EQUALS(XX, YY) \
    if ((XX) != (YY))         \
    {                         \
        panic();              \
    }

#define ASSERT_NOT_EQUALS(XX, YY) \
    if ((XX) == (YY))             \
    {                             \
        panic();                  \
    }

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

    Matrix_free(&mat);
    Matrix_free(&mat);
}

int main()
{
    test_matrix_init_and_destroy();

    return 0;
}
