#include "linea.h"

#include <stddef.h>
#include <stdio.h>

linea_t *p_linea_parameter_block = NULL;
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

void linea_printf()
{
    printf("===== LINE A CONTENT ====\n");
    printf("p_linea_parameter_block: %p\n", p_linea_parameter_block);
    printf("p_linea_font_hdr: %p\n", p_linea_font_hdr);

    if (p_linea_parameter_block)
    {
        printf("\n\r");
        printf(">> parameter_block (sizeof: %ld)\n", sizeof(linea_t));
        printf("v_planes   : %d\n\r", p_linea_parameter_block->v_planes);
        printf("v_lin_wr   : %d\n\r", p_linea_parameter_block->v_lin_wr);
        printf("contrl     : %p\n\r", p_linea_parameter_block->contrl);
        printf("intin      : %p\n\r", p_linea_parameter_block->intin);
        printf("ptsin      : %p\n\r", p_linea_parameter_block->ptsin);
        printf("intout     : %p\n\r", p_linea_parameter_block->intout);
        printf("ptsout     : %p\n\r", p_linea_parameter_block->ptsout);
        printf("fg_bp      : [%d %d %d %d]\n\r",
               p_linea_parameter_block->fg_bp_1,
               p_linea_parameter_block->fg_bp_2,
               p_linea_parameter_block->fg_bp_3,
               p_linea_parameter_block->fg_bp_4);
        printf("lstlin     : %d\n\r", p_linea_parameter_block->lstlin);
        printf("ln_mask    : %d\n\r", p_linea_parameter_block->ln_mask);
        printf("wrt_mode   : %d\n\r", p_linea_parameter_block->wrt_mode);
        printf("xy         : (%d %d) (%d %d)\n\r",
               p_linea_parameter_block->x1,
               p_linea_parameter_block->y1,
               p_linea_parameter_block->x2,
               p_linea_parameter_block->y2);
        printf("patptr     : %p\n\r", p_linea_parameter_block->patptr);
        printf("patmsk     : %d\n\r", p_linea_parameter_block->patmsk);
        printf("multifill  : %d\n\r", p_linea_parameter_block->multifill);
        printf("clip flag  : %d\n\r", p_linea_parameter_block->clip);
        printf("clip rect  : (%d %d) (%d %d)\n\r",
               p_linea_parameter_block->xmn_clip,
               p_linea_parameter_block->ymn_clip,
               p_linea_parameter_block->xmx_clip,
               p_linea_parameter_block->ymx_clip);
        printf("xacc_dda   : %d\n\r", p_linea_parameter_block->xacc_dda);
        printf("dda_inc    : %d\n\r", p_linea_parameter_block->dda_inc);
        printf("t_sclsts   : %d\n\r", p_linea_parameter_block->t_sclsts);
        printf("mono_status: %d\n\r", p_linea_parameter_block->mono_status);
        printf("source     : (%d %d)\n\r", p_linea_parameter_block->sourcex, p_linea_parameter_block->sourcey);
        printf("dest       : (%d %d)\n\r", p_linea_parameter_block->destx, p_linea_parameter_block->desty);
        printf("del        : (%d %d)\n\r", p_linea_parameter_block->delx, p_linea_parameter_block->dely);
        printf("fwidth     : %d\n\r", p_linea_parameter_block->fwidth);
        printf("style      : %d\n\r", p_linea_parameter_block->style);
        printf("litemask   : %d\n\r", p_linea_parameter_block->litemask);
        printf("skewmask   : %d\n\r", p_linea_parameter_block->skewmask);
        printf("weight     : %d\n\r", p_linea_parameter_block->weight);
        printf("r_off      : %d\n\r", p_linea_parameter_block->r_off);
        printf("l_off      : %d\n\r", p_linea_parameter_block->l_off);
        printf("scale      : %d\n\r", p_linea_parameter_block->scale);
        printf("chup       : %d\n\r", p_linea_parameter_block->chup);
        printf("text_fg    : %d\n\r", p_linea_parameter_block->text_fg);
        printf("text_bg    : %d\n\r", p_linea_parameter_block->text_bg);
        printf("scrtchp    : %p\n\r", p_linea_parameter_block->scrtchp);
        printf("scrpt2     : %d\n\r", p_linea_parameter_block->scrpt2);
        printf("copy_tran  : %d\n\r", p_linea_parameter_block->copy_tran);
    }

    if (p_linea_font_hdr)
    {
        printf("\n\r");
        printf(">> font hdr (sizeof: %ld)\n\r", sizeof(font_hdr_t));
        printf("font_id    : %d\n\r", p_linea_font_hdr->font_id);
        printf("point      : %d\n\r", p_linea_font_hdr->point);
        printf("first_ade  : %c\n\r", p_linea_font_hdr->first_ade);
        printf("last_ade   : %c\n\r", p_linea_font_hdr->last_ade);
    }

    printf("=========================\n");
}