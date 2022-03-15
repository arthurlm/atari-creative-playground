#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <mint/osbind.h>

#include "linea.h"
#include "video.h"

int main(int argc, char **argv)
{
    // Init sub-systems
    linea_init();
    video_init();

    // Configure Line-A
    linea_set_clip_region(0, 0, 0, 0, 0);
    linea_set_bit_plane(1, 0, 1, 0);

    int i = 0;
    int diff = 2;

    // Draw
    while (true)
    {
        video_clear_screen();
        video_flip_buffers();

        linea_draw_line(0, i, 319, 200 - i);

        i += diff;

        if (i >= 200 || i <= 0)
        {
            diff *= -1;
        }

        Vsync();
    }

    // Wait forever
    // while (true)
    // {
    // }

    video_quit();
    return 0;
}
