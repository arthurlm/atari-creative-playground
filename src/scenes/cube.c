#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <osbind.h>

#include "const.h"

#include "linea.h"
#include "video.h"
#include "matrix.h"
#include "utils.h"
#include "trigo.h"

// Define constant =================================================================================

#define GRID_SIZE 100000

#ifdef USE_PROJECTION_ORTHOGRAPHIC
#define CAMERA_F 7
#else // USE_PROJECTION_ORTHOGRAPHIC
#define CAMERA_F 4
#endif // USE_PROJECTION_ORTHOGRAPHIC

#define CAMERA_NEAR (-1.8 * GRID_SIZE)
#define CAMERA_FAR (2.5 * GRID_SIZE)
#define CAMERA_X 0
#define CAMERA_Y 0
#define CAMERA_Z -3

#ifdef ENABLE_FAST_CALL
#define CHECK_CALL(XX) \
    {                  \
        (XX);          \
    }
#else // ENABLE_FAST_CALL
#define CHECK_CALL(XX)               \
    {                                \
        if ((XX) != ERR_NO)          \
        {                            \
            panic("Call fail:" #XX); \
        }                            \
    }
#endif // ENABLE_FAST_CALL

#define DRAW_CUBE_SCREEN_SEGMENT(P1, P2)                       \
    {                                                          \
        matdata_t z1 = MAT_AT_UNSAFE(coord_space, (P1), 2);    \
        matdata_t z2 = MAT_AT_UNSAFE(coord_space, (P2), 2);    \
        if (CAMERA_NEAR < z1 && z1 < CAMERA_FAR &&             \
            CAMERA_NEAR < z1 && z2 < CAMERA_FAR)               \
        {                                                      \
            int16_t x1 = MAT_AT_UNSAFE(coord_screen, (P1), 0); \
            int16_t y1 = MAT_AT_UNSAFE(coord_screen, (P1), 1); \
            int16_t x2 = MAT_AT_UNSAFE(coord_screen, (P2), 0); \
            int16_t y2 = MAT_AT_UNSAFE(coord_screen, (P2), 1); \
            linea_draw_line(x1, y1, x2, y2);                   \
        }                                                      \
    }

// Define globals ==================================================================================

static Matrix_t coord_space_original;
static Matrix_t coord_space;
static Matrix_t coord_space_tmp;
static Matrix_t camera_pos;
static Matrix_t coord_proj;
static Matrix_t coord_screen;

static Matrix_t rot_x;
static Matrix_t rot_y;
static Matrix_t rot_z;

static Matrix_t tmp;

static uint16_t rotate_x = 0;
static uint16_t rotate_y = 0;
static uint16_t rotate_z = 0;

static uint16_t counter_interrupt = 0;
static uint16_t counter_frame = 0;

static int flag_new_frame_ready = 0;

// Define utils ====================================================================================

#define SWAP_MATRIX(XXX, YYY) \
    {                         \
        tmp = (XXX);          \
        (XXX) = (YYY);        \
        (YYY) = tmp;          \
    }

// Define scene lifecycle callback =================================================================

void scene_setup()
{
    coord_space_original = Matrix_alloc(8, 4);
    coord_space = Matrix_alloc(8, 4);
    coord_space_tmp = Matrix_alloc(8, 4);
    camera_pos = Matrix_alloc(4, 4);
    coord_proj = Matrix_alloc(8, 4);
    coord_screen = Matrix_alloc(8, 2);

    rot_x = Matrix_alloc(4, 4);
    rot_y = Matrix_alloc(4, 4);
    rot_z = Matrix_alloc(4, 4);

    if (coord_space_original.data == NULL ||
        coord_space.data == NULL ||
        coord_space_tmp.data == NULL ||
        camera_pos.data == NULL ||
        coord_proj.data == NULL ||
        coord_screen.data == NULL ||
        rot_x.data == NULL ||
        rot_y.data == NULL ||
        rot_z.data == NULL)
    {
        panic("Fail to init matrix");
    }

    // Set cube coord
    // NOTE: Scale to make integer computation easyer
    CHECK_CALL(Matrix_set_point(&coord_space_original, 0, -1 * GRID_SIZE, -1 * GRID_SIZE, -1 * GRID_SIZE));
    CHECK_CALL(Matrix_set_point(&coord_space_original, 1, +1 * GRID_SIZE, -1 * GRID_SIZE, -1 * GRID_SIZE));
    CHECK_CALL(Matrix_set_point(&coord_space_original, 2, +1 * GRID_SIZE, +1 * GRID_SIZE, -1 * GRID_SIZE));
    CHECK_CALL(Matrix_set_point(&coord_space_original, 3, -1 * GRID_SIZE, +1 * GRID_SIZE, -1 * GRID_SIZE));
    CHECK_CALL(Matrix_set_point(&coord_space_original, 4, -1 * GRID_SIZE, -1 * GRID_SIZE, +1 * GRID_SIZE));
    CHECK_CALL(Matrix_set_point(&coord_space_original, 5, +1 * GRID_SIZE, -1 * GRID_SIZE, +1 * GRID_SIZE));
    CHECK_CALL(Matrix_set_point(&coord_space_original, 6, +1 * GRID_SIZE, +1 * GRID_SIZE, +1 * GRID_SIZE));
    CHECK_CALL(Matrix_set_point(&coord_space_original, 7, -1 * GRID_SIZE, +1 * GRID_SIZE, +1 * GRID_SIZE));
    CHECK_CALL(Matrix_make_rot_x(&rot_x, 0));
    CHECK_CALL(Matrix_make_rot_y(&rot_y, 0));
    CHECK_CALL(Matrix_make_rot_z(&rot_z, 0));

    // Set camera pos translate matrix
    CHECK_CALL(Matrix_set_identity(&camera_pos));
    MAT_AT_UNSAFE(camera_pos, 3, 0) = CAMERA_X * GRID_SIZE;
    MAT_AT_UNSAFE(camera_pos, 3, 1) = CAMERA_Y * GRID_SIZE;
    MAT_AT_UNSAFE(camera_pos, 3, 2) = CAMERA_Z * GRID_SIZE;
}

inline static void rot_space(Matrix_t *rot)
{
    CHECK_CALL(Matrix_dot(&coord_space, rot, &coord_space_tmp));
    CHECK_CALL(Matrix_div(&coord_space_tmp, float_scale()));
    SWAP_MATRIX(coord_space, coord_space_tmp);
}

void scene_update()
{
    // Recompute rotation matrix based on frame count
    CHECK_CALL(Matrix_make_partial_rot_x(&rot_x, rotate_x));
    CHECK_CALL(Matrix_make_partial_rot_y(&rot_y, rotate_y));
    CHECK_CALL(Matrix_make_partial_rot_z(&rot_z, rotate_z));

    // Apply rotation to space
    CHECK_CALL(Matrix_copy(&coord_space_original, &coord_space));

    rot_space(&rot_z);
    rot_space(&rot_x);
    rot_space(&rot_y);

    CHECK_CALL(Matrix_dot(&coord_space, &camera_pos, &coord_proj));

    // Project matrix to screen
    CHECK_CALL(Matrix_project(&coord_proj, &coord_screen, CAMERA_F, GRID_SIZE));

    // Convert space to screen coord
    for (uint16_t h = 0; h < coord_screen.height; h++)
    {
        MAT_AT_UNSAFE(coord_screen, h, 0) = ((MAT_AT_UNSAFE(coord_screen, h, 0)) / (GRID_SIZE / CAMERA_F)) + (V_SCREEN_WIDTH / 2);
        MAT_AT_UNSAFE(coord_screen, h, 1) = ((MAT_AT_UNSAFE(coord_screen, h, 1)) / (GRID_SIZE / CAMERA_F)) + (V_SCREEN_HEIGHT / 2);
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

void __attribute__((interrupt)) vector_hblank()
{
    if (flag_new_frame_ready)
    {
        video_flip_buffers();
        flag_new_frame_ready = 0;
    }

    rotate_x = (rotate_x + 6) % sizeof_tables();
    rotate_y = (rotate_y + 9) % sizeof_tables();
    rotate_z = (rotate_z + 12) % sizeof_tables();

    // Update counter (debug purposes)
    counter_interrupt++;

    // Mark interruption address as terminated
    *(INTERRUPTION_SERVICE_ADDRESS) &= END_OF_INTERRUPT_TIMER_HBLANK;
}

int main(int argc, char **argv)
{
    // Init sub-systems
    linea_init();
    video_init();

    // Configure Line-A
    linea_set_clip_region(0, 0, 0, 0, 0);
    linea_set_bit_plane(1, 1, 1, 1);

    // Setup globals
    scene_setup();

    // Setup interrupt
    Xbtimer(TIMER_HBLANK, V_SCREEN_HEIGHT, 0, vector_hblank);

    // Draw
    while (true)
    {
        if (!flag_new_frame_ready)
        {
            video_clear_screen();

            video_set_draw_buffer_hidden();
            scene_draw();
            video_set_draw_buffer_primary();

            flag_new_frame_ready = 1;
        }

        scene_update();

        // Update counter (debug purposes)
        counter_frame++;
        // printf("interupt=%d, local=%d\r", counter_interrupt / 50, counter_frame);
    }

    video_quit();
    return 0;
}
