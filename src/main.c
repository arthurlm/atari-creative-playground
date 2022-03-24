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

#define DRAW_CUBE_SCREEN_SEGMENT(P1, P2)                   \
    {                                                      \
        int16_t x1 = MAT_AT_UNSAFE(coord_screen, (P1), 0); \
        int16_t y1 = MAT_AT_UNSAFE(coord_screen, (P1), 1); \
        int16_t x2 = MAT_AT_UNSAFE(coord_screen, (P2), 0); \
        int16_t y2 = MAT_AT_UNSAFE(coord_screen, (P2), 1); \
        linea_draw_line(x1, y1, x2, y2);                   \
    }

// Define globals ==================================================================================

Matrix_t coord_space;
Matrix_t camera_pos;
Matrix_t coord_proj;
Matrix_t coord_screen;

// Define scene lifecycle callback =================================================================

void scene_setup()
{
    coord_space = Matrix_alloc(8, 4);
    camera_pos = Matrix_alloc(4, 4);
    coord_proj = Matrix_alloc(8, 4);
    coord_screen = Matrix_alloc(8, 2);

    if (coord_space.data == NULL ||
        camera_pos.data == NULL ||
        coord_proj.data == NULL ||
        coord_screen.data == NULL)
    {
        panic("Fail to init matrix");
    }

    // Set cube coord
    // NOTE: Scale to make integer computation easyer
    CHECK_CALL(Matrix_set_point(&coord_space, 0, -1000, -1000, -1000));
    CHECK_CALL(Matrix_set_point(&coord_space, 1, +1000, -1000, -1000));
    CHECK_CALL(Matrix_set_point(&coord_space, 2, +1000, +1000, -1000));
    CHECK_CALL(Matrix_set_point(&coord_space, 3, -1000, +1000, -1000));
    CHECK_CALL(Matrix_set_point(&coord_space, 4, -1000, -1000, +1000));
    CHECK_CALL(Matrix_set_point(&coord_space, 5, +1000, -1000, +1000));
    CHECK_CALL(Matrix_set_point(&coord_space, 6, +1000, +1000, +1000));
    CHECK_CALL(Matrix_set_point(&coord_space, 7, -1000, +1000, +1000));

    // Set camera pos translate matrix
    CHECK_CALL(Matrix_set_identity(&camera_pos));
    MAT_AT_UNSAFE(camera_pos, 3, 2) = -3000;
}

void scene_update()
{
    CHECK_CALL(Matrix_dot(&coord_space, &camera_pos, &coord_proj));

    // Project matrix to screen
    CHECK_CALL(Matrix_project(&coord_proj, &coord_screen, CAMERA_F, 1000));

    // Convert space to screen coord
    // NOTE: bellow call are computed from screen size and FRACTIONAL_NUMBER_PRECISION_SCALE
    for (uint16_t h = 0; h < coord_screen.height; h++)
    {
        MAT_AT_UNSAFE(coord_screen, h, 0) = ((MAT_AT_UNSAFE(coord_screen, h, 0) + 4000) / 80) + 120;
        MAT_AT_UNSAFE(coord_screen, h, 1) = ((MAT_AT_UNSAFE(coord_screen, h, 1) + 4000) / 80) + 50;
    }
}

void scene_draw()
{
    DRAW_CUBE_SCREEN_SEGMENT(0, 1);
    DRAW_CUBE_SCREEN_SEGMENT(1, 2);
    DRAW_CUBE_SCREEN_SEGMENT(2, 3);
    DRAW_CUBE_SCREEN_SEGMENT(3, 0);

    DRAW_CUBE_SCREEN_SEGMENT(4, 5);
    DRAW_CUBE_SCREEN_SEGMENT(5, 6);
    DRAW_CUBE_SCREEN_SEGMENT(6, 7);
    DRAW_CUBE_SCREEN_SEGMENT(7, 4);

    DRAW_CUBE_SCREEN_SEGMENT(0, 4);
    DRAW_CUBE_SCREEN_SEGMENT(1, 5);
    DRAW_CUBE_SCREEN_SEGMENT(2, 6);
    DRAW_CUBE_SCREEN_SEGMENT(3, 7);
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
