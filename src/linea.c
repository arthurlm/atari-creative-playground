#include "linea.h"

#include <stddef.h>
#include <stdio.h>

linea_t *p_linea_parameter_block = NULL;
linea_vdiesc_t *p_plinea_vdiesc_block = NULL;
font_hdr_t *p_linea_font_hdr = NULL;

// See m68k arch: https://en.wikipedia.org/wiki/Motorola_68000#Architecture
// data register
#define D0_D2 "d0", "d1", "d2"
// address register
#define A0_A2 "a0", "a1", "a2"
// condition code register
#define AND_ASM_CC , "cc"
// do we need hint for this ;)
#define AND_ASM_MEMORY , "memory"

#define _line_a_call(op_code)                                         \
    __asm__ volatile(                                                 \
        "move.l    %0,A0\n"                                           \
        "dc.w  " op_code "\n"                                         \
        :                                        /* outputs */        \
        : "r"(p_linea_parameter_block)           /* inputs */         \
        : D0_D2, A0_A2 AND_ASM_CC AND_ASM_MEMORY /* side effect on */ \
    )

void linea_init()
{
    __asm__ volatile(
        "dc.w      0xA000\n" // Line-A opcode
        "move.l    A0,%0\n"  // Pointer parameter block LINEA
        "move.l    A1,%1\n"  // Pointer system fonts
        /* outputs */
        : "=r"(p_linea_parameter_block),
          "=r"(p_linea_font_hdr)
        /* inputs */
        :
        /* side effect on */
        : D0_D2, A0_A2 AND_ASM_CC AND_ASM_MEMORY);

    p_plinea_vdiesc_block = (void *)(p_linea_parameter_block) - sizeof(linea_vdiesc_t);
}

void linea_set_bit_plane(int8_t p1, int8_t p2, int8_t p3, int8_t p4)
{
    p_linea_parameter_block->fg_bp_1 = p1;
    p_linea_parameter_block->fg_bp_2 = p2;
    p_linea_parameter_block->fg_bp_3 = p3;
    p_linea_parameter_block->fg_bp_4 = p4;
}

void linea_set_clip_region(int16_t enabled, int16_t x_min, int16_t y_min, int16_t x_max, int16_t y_max)
{
    p_linea_parameter_block->clip = enabled;
    p_linea_parameter_block->xmn_clip = x_min;
    p_linea_parameter_block->ymn_clip = y_min;
    p_linea_parameter_block->xmx_clip = x_max;
    p_linea_parameter_block->ymx_clip = y_max;
}

void linea_put_pixel(int16_t x, int16_t y, int16_t color)
{
    int16_t intin[] = {color};
    p_linea_parameter_block->intin = intin;

    int16_t ptsin[] = {x, y};
    p_linea_parameter_block->ptsin = ptsin;

    _line_a_call("0xA001");
}

void linea_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    // Coordinate
    p_linea_parameter_block->x1 = x1;
    p_linea_parameter_block->y1 = y1;
    p_linea_parameter_block->x2 = x2;
    p_linea_parameter_block->y2 = y2;

    // Bit pattern for the line type; the bit pattern is to be interpreted as a dot pattern
    p_linea_parameter_block->ln_mask = 0xFFFF;

    // Writing mode; the following modes are possible:
    // - REPLACE 0 : Replace
    // - TRANS 1   : Transparent (OR)
    // - XOR 2     : Inverse (XOR)
    // - INVERS 3  : Inverse Transparent (XOR with not(LN_MASK))
    p_linea_parameter_block->wrt_mode = 0;

    // Is the last pixel of the line to be drawn (1) or not (0)
    p_linea_parameter_block->lstlin = 1;

    _line_a_call("0xA003");
}

void linea_draw_rect_filled(int16_t x1, int16_t y1, int16_t x2, int16_t y2)
{
    // Coordinate
    p_linea_parameter_block->x1 = x1;
    p_linea_parameter_block->y1 = y1;
    p_linea_parameter_block->x2 = x2;
    p_linea_parameter_block->y2 = y2;

    // Writing mode; the following modes are possible:
    // - REPLACE 0 : Replace
    // - TRANS 1   : Transparent (OR)
    // - XOR 2     : Inverse (XOR)
    // - INVERS 3  : Inverse Transparent (XOR with not(LN_MASK))
    p_linea_parameter_block->wrt_mode = 0;

    // Fill pattern
    uint16_t pattern[] = {
        0x1000,
    };
    p_linea_parameter_block->patptr = pattern;
    p_linea_parameter_block->patmsk = 0;

    _line_a_call("0xA005");
}

void linea_set_mouse_visible(int8_t visible)
{
    if (visible)
    {
        int16_t intin[] = {0};
        p_linea_parameter_block->intin = intin;

        _line_a_call("0xA009");
    }
    else
    {
        _line_a_call("0xA00A");
    }
}

linea_t *linea_get_parameter_block()
{
    return p_linea_parameter_block;
}

font_hdr_t *linea_get_font_block()
{
    return p_linea_font_hdr;
}

linea_vdiesc_t *linea_get_vdiesc_block()
{
    return p_plinea_vdiesc_block;
}