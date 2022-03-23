#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <osbind.h>

#include "const.h"

#include "linea.h"
#include "video.h"
#include "matrix.h"
#include "utils.h"

// Define constant =================================================================================

#define CAMERA_F 1

#define CHECK_CALL(XX)               \
    {                                \
        if ((XX) != ERR_NO)          \
        {                            \
            panic("Call fail:" #XX); \
        }                            \
    }

// Define globals ==================================================================================

Matrix_t coord_space;
Matrix_t coord_proj;
Matrix_t coord_screen;

// Define scene lifecycle callback =================================================================

void scene_setup()
{
    coord_space = Matrix_alloc(8, 4);
    coord_proj = Matrix_alloc(8, 2);
    coord_screen = Matrix_alloc(8, 2);

    if (coord_space.data == NULL ||
        coord_proj.data == NULL ||
        coord_screen.data == NULL)
    {
        panic("Fail to init matrix");
    }

    CHECK_CALL(Matrix_set_point(&coord_space, 0, -1, -1, -1));
    CHECK_CALL(Matrix_set_point(&coord_space, 1, +1, -1, -1));
    CHECK_CALL(Matrix_set_point(&coord_space, 2, +1, +1, -1));
    CHECK_CALL(Matrix_set_point(&coord_space, 3, -1, +1, -1));

    CHECK_CALL(Matrix_set_point(&coord_space, 4, -1, -1, +1));
    CHECK_CALL(Matrix_set_point(&coord_space, 5, +1, -1, +1));
    CHECK_CALL(Matrix_set_point(&coord_space, 6, +1, +1, +1));
    CHECK_CALL(Matrix_set_point(&coord_space, 7, -1, +1, +1));

    CHECK_CALL(Matrix_mul(&coord_space, FRACTIONAL_NUMBER_PRECISION_SCALE));
}

void scene_update()
{
    // Project matrix to screen
    CHECK_CALL(Matrix_project(&coord_space, &coord_screen, CAMERA_F));

    // Convert space to screen coord
    // NOTE: bellow call are computed from screen size and FRACTIONAL_NUMBER_PRECISION_SCALE
    CHECK_CALL(Matrix_add_const(&coord_screen, 10000));
    CHECK_CALL(Matrix_div(&coord_screen, 100));
}

void scene_draw()
{
    static int i = 0;
    static int diff = 2;

    linea_draw_line(0, i, 319, 200 - i);

    i += diff;

    if (i >= 200 || i <= 0)
    {
        diff *= -1;
    }
}

// Define main =====================================================================================

int main(int argc, char **argv)
{
    // Init sub-systems
    linea_init();
    video_init();

    // Configure Line-A
    linea_set_clip_region(0, 0, 0, 0, 0);
    linea_set_bit_plane(1, 0, 0, 1);

    // Setup globals
    scene_setup();

    // Draw
    while (true)
    {
        video_clear_screen();
        video_flip_buffers();

        scene_update();
        scene_draw();

        Vsync();
    }

    // Wait forever
    // while (true)
    // {
    // }

    video_quit();
    return 0;
}
