#ifndef _LINEA_H_
#define _LINEA_H_

#include <stdint.h>

typedef struct __attribute__((__packed__)) font_hdr
{
    int16_t font_id;            /* Font number                        */
    int16_t point;              /* Size in points                     */
    int8_t name[32];            /* Name of the font                   */
    uint16_t first_ade;         /* First character in font            */
    uint16_t last_ade;          /* Last character in font             */
    uint16_t top;               /* Distance: Top line    <-> Baseline */
    uint16_t ascent;            /* Distance: Ascent line <-> Baseline */
    uint16_t half;              /* Distance: Half line   <-> Baseline */
    uint16_t descent;           /* Distance: Descent line<-> Baseline */
    uint16_t bottom;            /* Distance: Bottom line <-> Baseline */
    uint16_t max_char_width;    /* Largest character width            */
    uint16_t max_cell_width;    /* Largest character cell width       */
    uint16_t left_offset;       /* Left offset for italic (skewed)    */
    uint16_t right_offset;      /* Right offset for italic (skewed)   */
    uint16_t thicken;           /* Thickening factor for bold         */
    uint16_t ul_size;           /* Width of underline                 */
    uint16_t lighten;           /* Mask for light (0x5555)            */
    uint16_t skew;              /* Mask for italic (0x5555)           */
    uint16_t flags;             /* Various flags:
                                   Set for system font
                                    Bit 1: Set if horizontal offset
                                           table is in use
                                    Bit 2: Set if Motorola format
                                    Bit 3: Set if non-proportional    */
    uint8_t *hor_table;         /* Pointer to horizontal offset table */
    uint16_t *off_table;        /* Pointer to character offset table  */
    uint16_t *dat_table;        /* Pointer to font image              */
    uint16_t form_width;        /* Width of the font image            */
    uint16_t form_height;       /* Height of the font image           */
    struct font_hdr *next_font; /* Pointer to next font header        */
} font_hdr_t;

typedef struct __attribute__((__packed__)) linea
{
    int16_t v_planes;            /*   0: # Bit planes (1, 2 or 4)     */
    int16_t v_lin_wr;            /*   2: # bytes/scanline             */
    int16_t *contrl;             /* */
    int16_t *intin;              /* */
    int16_t *ptsin;              /*  12: Coordinates input            */
    int16_t *intout;             /* */
    int16_t *ptsout;             /*  20: Coordinates output           */
    int16_t fg_bp_1;             /*  24: Plane 0                      */
    int16_t fg_bp_2;             /*  26: Plane 1                      */
    int16_t fg_bp_3;             /*  28: Plane 2                      */
    int16_t fg_bp_4;             /*  30: Plane 3                      */
    int16_t lstlin;              /*  32: Draw last pixel of a line    */
                                 /*      (1) or don't draw it (0)     */
    uint16_t ln_mask;            /*  34: Line pattern                 */
    int16_t wrt_mode;            /*  36: Writing modes                */
    int16_t x1, y1, x2, y2;      /*  38: Coordinate                   */
    void *patptr;                /*  46: Fill pattern                 */
    uint16_t patmsk;             /*  50: Fill pattern "mask"          */
    int16_t multifill;           /*  52: Fill pattern for planes      */
    int16_t clip;                /*  54: Flag for clipping            */
    int16_t xmn_clip, ymn_clip;  /* */
    int16_t xmx_clip, ymx_clip;  /*  60: Clipping rectangle           */
                                 /*      Rest for text_blt:           */
    int16_t xacc_dda;            /*  64: Set to 0x8000 before text    */
                                 /*      output                       */
    int16_t dda_inc;             /*  66: Scaling increment            */
    int16_t t_sclsts;            /*  68: Scaling direction            */
    int16_t mono_status;         /*  70: Proportional font            */
    int16_t sourcex, sourcey;    /*  72: Coordinates in font          */
    int16_t destx, desty;        /*  76: Screen coordinates           */
    int16_t delx, dely;          /*  80: Width and height of character*/
    font_hdr_t *fbase;           /*  84: Pointer to font data         */
    int16_t fwidth;              /*  88: Width of font form           */
    int16_t style;               /*  90: Font style effect            */
    uint16_t litemask;           /*  92: Mask for light               */
    int16_t skewmask;            /*  94: Mask for italic              */
    int16_t weight;              /*  96: Width for bold               */
    int16_t r_off;               /*  98: Italic offset right          */
    int16_t l_off;               /* 100: Italic offset left           */
    int16_t scale;               /* 102: Scaling flag yes/no          */
    int16_t chup;                /* 104: Character rotation angle *10 */
    int16_t text_fg;             /* 106: Text foreground colour       */
    void *scrtchp;               /* 108: Pointer to 2 contiguous      */
                                 /*      scratch buffers              */
    int16_t scrpt2;              /* 112: Index in buffer              */
    int16_t text_bg;             /* 114: Unused                       */
    int16_t copy_tran;           /* 116: --                           */
    int16_t (*fill_abort)(void); /* 118: Tests seedfill               */
} linea_t;

typedef struct __attribute__((__packed__)) bitblt
{
    int16_t b_wd;      /* Width of the block in pixels    */
    int16_t b_ht;      /* Height of the block in pixels   */
    int16_t plane_ct;  /* Number of colour planes to blit */
    int16_t fg_col;    /* Foreground colour               */
    int16_t bg_col;    /* Background colour               */
    int8_t op_tab[4];  /* Combination (for each plane)    */
    int16_t s_xmin;    /* X offset to source top left     */
    int16_t s_ymin;    /* Y offset to source top left     */
    void *s_form;      /* Address of the source form      */
    int16_t s_nxwd;    /* Offset to next word             */
    int16_t s_nxln;    /* Byte width of form              */
    int16_t s_nxpl;    /* Byte offset between planes      */
    int16_t d_xmin;    /* X offset to destination top L   */
    int16_t d_ymin;    /* Y offset to destination top L   */
    void *d_form;      /* Address of the destination form */
    int16_t d_nxwd;    /* Offset to next word             */
    int16_t d_nxln;    /* Byte width of form              */
    int16_t d_nxpl;    /* Byte offset to next plane       */
    void *p_addr;      /* 16-bit masks for ANDing         */
    int16_t p_nxln;    /* Width of the mask in bytes      */
    int16_t p_nxpl;    /* Byte offset to next plane       */
    int16_t p_mask;    /* Height of mask in lines         */
    int8_t filler[24]; /* Internal buffer                 */
} bitblt_t;

/**
 * Setup global variables to init Line-A subsystem.
 *
 * @brief Init Line-A subsystem
 * @see http://toshyp.atari.org/en/006005.html#Bindings_20for_20linea_init
 */
void linea_init();

/**
 * @brief Set if bit plane should be enable or not.
 *
 * @param p1 enable plane 1
 * @param p2 enable plane 2
 * @param p3 enable plane 3
 * @param p4 enable plane 4
 */
void linea_set_bit_plane(int8_t p1, int8_t p2, int8_t p3, int8_t p4);

/**
 * @brief Configure clip region
 *
 * @param enabled 1 if clipping is desired, else 0
 * @param x_min X-coordinate of the top left corner of the clipping region, if clipping is switched on
 * @param y_min Y-coordinate of the top left corner of the clipping region, if clipping is switched on
 * @param x_max X-coordinate of the bottom right corner of the clipping region, if clipping is switched on
 * @param y_max Y-coordinate of the bottom right corner of the clipping region, if clipping is switched on
 */
void linea_set_clip_region(int16_t enabled, int16_t x_min, int16_t y_min, int16_t x_max, int16_t y_max);

/**
 * @brief Set pixel value.
 *
 * @param x X coord
 * @param y Y coord
 * @param color
 */
void linea_put_pixel(int16_t x, int16_t y, int16_t color);

/**
 * Draw line between P1 and P2.
 *
 * Color and filling is configured through planes.
 *
 * @brief Draw arbitrary line.
 *
 * @param x1 X source
 * @param y1 Y source
 * @param x2 X dest
 * @param y2 Y dest
 */
void linea_draw_line(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

/**
 * @brief Draw filled rectangle.
 *
 * @param x1 X-coordinate of the top left corner of the rectangle
 * @param y1 Y-coordinate of the top left corner of the rectangle
 * @param x2 X-coordinate of the bottom right corner of the rectangle
 * @param y2 Y-coordinate of the bottom right corner of the rectangle
 * @see http://toshyp.atari.org/en/00600a.html
 */
void linea_draw_rect_filled(int16_t x1, int16_t y1, int16_t x2, int16_t y2);

/**
 * @brief Update mouse visibility.
 *
 * @param visible Set visible / hidden.
 * @see https://freemint.github.io/tos.hyp/en/linea_showmouse.html#Bindings_20for_20show_mouse
 * @see https://freemint.github.io/tos.hyp/en/linea_hidemouse.html
 */
void linea_set_mouse_visible(int8_t visible);

/**
 * @brief Print Line-A struct informations.
 */
void linea_printf();

#endif // _LINEA_H_
