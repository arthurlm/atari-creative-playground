enable_testing()

set(TEST_SOURCES
    ../src/matrix.c
    )

add_executable(TestMatrix.tos test_matrix.c "${TEST_SOURCES}")
add_test(NAME TestMatrix.tos COMMAND TestMatrix.tos)
