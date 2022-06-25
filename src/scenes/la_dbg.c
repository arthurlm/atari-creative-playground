#include <mint/osbind.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include <stdio.h>

#include "const.h"
#include "linea.h"
#include "video.h"

void print_linea_parameter_block()
{
    printf("===== LINE A CONTENT ====\n\r");

    linea_t *block = linea_get_parameter_block();
    printf("block: %p\n\r", block);

    if (block)
    {
        printf("\n\r");
        printf(">> parameter_block (sizeof: %ld)\n", sizeof(linea_t));
        printf("v_planes   : %d\n\r", block->v_planes);
        printf("v_lin_wr   : %d\n\r", block->v_lin_wr);
        printf("contrl     : %p\n\r", block->contrl);
        printf("intin      : %p\n\r", block->intin);
        printf("ptsin      : %p\n\r", block->ptsin);
        printf("intout     : %p\n\r", block->intout);
        printf("ptsout     : %p\n\r", block->ptsout);
        printf("fg_bp      : [%d %d %d %d]\n\r",
               block->fg_bp_1,
               block->fg_bp_2,
               block->fg_bp_3,
               block->fg_bp_4);
        printf("lstlin     : %d\n\r", block->lstlin);
        printf("ln_mask    : %d\n\r", block->ln_mask);
        printf("wrt_mode   : %d\n\r", block->wrt_mode);
        printf("xy         : (%d %d) (%d %d)\n\r",
               block->x1,
               block->y1,
               block->x2,
               block->y2);
        printf("patptr     : %p\n\r", block->patptr);
        printf("patmsk     : %d\n\r", block->patmsk);
        printf("multifill  : %d\n\r", block->multifill);
        printf("clip flag  : %d\n\r", block->clip);
        printf("clip rect  : (%d %d) (%d %d)\n\r",
               block->xmn_clip,
               block->ymn_clip,
               block->xmx_clip,
               block->ymx_clip);
        printf("xacc_dda   : %d\n\r", block->xacc_dda);
        printf("dda_inc    : %d\n\r", block->dda_inc);
        printf("t_sclsts   : %d\n\r", block->t_sclsts);
        printf("mono_status: %d\n\r", block->mono_status);
        printf("source     : (%d %d)\n\r", block->sourcex, block->sourcey);
        printf("dest       : (%d %d)\n\r", block->destx, block->desty);
        printf("del        : (%d %d)\n\r", block->delx, block->dely);
        printf("fwidth     : %d\n\r", block->fwidth);
        printf("style      : %d\n\r", block->style);
        printf("litemask   : %d\n\r", block->litemask);
        printf("skewmask   : %d\n\r", block->skewmask);
        printf("weight     : %d\n\r", block->weight);
        printf("r_off      : %d\n\r", block->r_off);
        printf("l_off      : %d\n\r", block->l_off);
        printf("scale      : %d\n\r", block->scale);
        printf("chup       : %d\n\r", block->chup);
        printf("text_fg    : %d\n\r", block->text_fg);
        printf("text_bg    : %d\n\r", block->text_bg);
        printf("scrtchp    : %p\n\r", block->scrtchp);
        printf("scrpt2     : %d\n\r", block->scrpt2);
        printf("copy_tran  : %d\n\r", block->copy_tran);
    }
}

void print_linea_font_block()
{
    printf("===== FONT BLOCK CONTENT ====\n\r");

    font_hdr_t *block = linea_get_font_block();
    printf("block: %p\n\r", block);

    if (block)
    {
        printf("\n\r");
        printf(">> font hdr (sizeof: %ld)\n\r", sizeof(font_hdr_t));
        printf("font_id    : %d\n\r", block->font_id);
        printf("point      : %d\n\r", block->point);
        printf("first_ade  : %c\n\r", block->first_ade);
        printf("last_ade   : %c\n\r", block->last_ade);
    }
}

void print_linea_vdiesc_block()
{
    printf("===== LINE-A NEG BLOCK CONTENT ====\n\r");

    linea_vdiesc_t *block = linea_get_vdiesc_block();
    printf("block: %p\r\n", block);

    if (block)
    {
        printf("font_count: %d\r\n", block->font_count);
        printf("save_addr: %p\r\n", block->save_addr);
        printf("v_rez_hz: %d\r\n", block->v_rez_hz);
        printf("v_rez_vt: %d\r\n", block->v_rez_vt);
        printf("v_off_ad: %p\r\n", block->v_off_ad);
    }
}

int main(int argc, char **argv)
{
    linea_init();
    video_init();

    // print_linea_parameter_block();
    // print_linea_font_block();
    print_linea_vdiesc_block();

    // Wait forever
    while (true)
    {
    }

    video_quit();
    return 0;
}
