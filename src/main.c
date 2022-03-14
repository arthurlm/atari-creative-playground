#include <stdio.h>
#include <stdbool.h>
#include <stdint.h>

#include <mint/mintbind.h>

#include "linea.h"

int main(int argc, char **argv)
{
    size_t i, j;

    linea_init();

    for (i = 0; i < 16; i++)
        for (j = 0; j < 160; j++)
            linea_put_pixel(j, i, i);

    linea_set_bit_plane(1, 1, 1, 1);
    linea_set_clip_region(0, 0, 0, 0, 0);

    linea_draw_rect_filled(50, 60, 0, 0);
    linea_draw_line(10, 20, 100, 20);

    // linea_printf();

    // Wait forever
    while (true)
    {
    }

    return 0;
}
