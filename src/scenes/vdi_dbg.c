#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>

#include <osbind.h>
#include <gem.h>

#include "utils.h"

int16_t work_in[] = {
    1,
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    true,
    2,    // Coordinate type flag
    0,    // Page format
    0, 0, // Pointer to a GEMDOS file name (C string)
    0, 0  // Reserved
};
VdiHdl handle;
int16_t work_out[57];

int16_t gr_hwchar;
int16_t gr_hhchar;
int16_t gr_hwbox;
int16_t gr_hhbox;

void open_vwork()
{
    handle = graf_handle(&gr_hwchar, &gr_hhchar, &gr_hwbox, &gr_hhbox);
    v_opnvwk(work_in, &handle, work_out);
}

void draw_circles()
{
    int16_t color, radius = 100;

    v_clrwk(handle);
    vsl_type(handle, 1);
    vsl_width(handle, 1);
    for (color = 1; color < 8; color++)
    {
        vsf_color(handle, color);
        vsl_color(handle, color);
        v_circle(handle, 150, 100, radius);
        radius -= 15;
    }
}

void draw_lines()
{
    int16_t points[] = {10, 20, 100, 50, 12, 32, 14, 80};
    vsl_width(handle, 7);
    v_pline(handle, 4, points);
}

int main(int argc, char **argv)
{
    // Init VDI
    _AESversion = 0;
    appl_init();
    if (_AESversion == 0)
    {
        panic("AES not ready yet\r\n");
    }
    open_vwork();
    v_hide_c(handle); // Hide mouse

    // Do some work
    // draw_circles();
    draw_lines();
    v_gtext(handle, 100, 50, "Hello world");

    // Wait user input
    Cconin();

    // Close VDI
    v_show_c(handle, false); // show mouse
    v_clsvwk(handle);
    appl_exit();

    return 0;
}