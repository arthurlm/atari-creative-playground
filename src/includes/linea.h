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

typedef struct __attribute__((__packed__)) linea_vdiesc
{
    int32_t reserved6;        /* -$38E: Reserved                     */
    font_hdr_t *cur_font;     /* -$38A: Pointer->current font header */
    int16_t reserved5[23];    /* -$386: Reserved                     */
    int16_t m_pos_hx;         /* -$358: X-coordinate mouse hot-spot  */
    int16_t m_pos_hy;         /* -$356: Y-coordinate mouse hot-spot  */
    int16_t m_planes;         /* -$354: Write mode for mouse pointer */
                              /*        1=Replace, -1=XOR mode       */
    int16_t m_cdb_bg;         /* -$352: Mouse pointer backgr. colour */
    int16_t m_cdb_fg;         /* -$350: Mouse pointer foregr. colour */
    int16_t mask_form[32];    /* -$34E: Mouse ptr. foregr. and mask  */
                              /*        Alternately 1 WORD           */
                              /*        foreground and mask          */
    int16_t inq_tab[45];      /* -$30E: As vq_extnd                  */
    int16_t dev_tab[45];      /* -$2B4: As v_opnwk                   */
    int16_t gcurx;            /* -$25A: Curr. mouse pointer X-posn.  */
    int16_t gcury;            /* -$258: Curr. mouse pointer Y-posn.  */
    int16_t m_hid_ct;         /* -$256: Number of hide_mouse-calls   */
    int16_t mouse_bt;         /* -$254: Current mouse button status  */
    int16_t req_col[3][16];   /* -$252: Internal data for vq_color   */
    int16_t siz_tab[15];      /* -$1F2: As v_opnwk                   */
    int16_t reserved4[2];     /*        Reserved                     */
    void *cur_work;           /* -$1D0: Attributes current workstn.  */
    font_hdr_t *def_font;     /* -$1CC: Standard system font         */
    font_hdr_t *font_ring[4]; /* -$1C8: System and GDOS font lists   */
                              /*        Chained FONT_HDR structures  */
                              /*        0 as terminator ID           */
    int16_t font_count;       /* -$1B8: Total number of fonts        */
    int16_t reserved3[45];    /*        Reserved                     */
    int8_t cur_ms_stat;       /* -$15C: Mouse status since last      */
                              /*        Bit 0:    Left button (0=up) */
                              /*        Bit 1:    Right button (0=up)*/
                              /*        Bit 2..4: Reserved           */
                              /*        Bit 5:    1=Mouse moved      */
                              /*        Bit 6:    1=State of right   */
                              /*                  button changed     */
                              /*        Bit 7:    1=State of left    */
                              /*                  button changed     */
    int8_t reserved2;         /*        Reserved                     */
    int16_t v_hid_cnt;        /*        Number of Hide_cursor calls  */
    int16_t cur_x;            /* -$158: X-position for mouse pointer */
    int16_t cur_y;            /* -$156: Y-position for mouse pointer */
    int8_t cur_flag;          /* -$154: != 0: Redraw mouse ptr.in VBL*/
    int8_t mouse_flag;        /* -$153: != 0: Mouse interrupt on     */
    int32_t reserved1;        /*        Reserved                     */
    int16_t v_sav_xy[2];      /* -$14E: X-Y coordinates saved by VT52*/
    int16_t save_len;         /* -$14A: Number of screen lines       */
    void *save_addr;          /* -$148: First bufered byte in screen */
                              /*        memory                       */
    int16_t save_stat;        /* -$144: Dirty flag                   */
                              /*        Bit 0: 1=Buffer is valid     */
                              /*        Bit 1: 1=Long, 0=Word bufferd*/
    int32_t save_area[4][16]; /* -$142: Buffer for image under mouse */
    void (*user_tim)(void);   /*  -$42: Timer interrupt vector       */
                              /*        To terminate should jump to  */
                              /*        next_tim                     */
    void (*next_tim)(void);   /*  -$3E: Old timer interrupt          */
    void (*user_but)(void);   /*  -$3A: Mouse button vector          */
    void (*user_cur)(void);   /*  -$36: Mouse rendering vector       */
    void (*user_mot)(void);   /*  -$32: Mouse movement vector        */
    int16_t v_cel_ht;         /*  -$2E: Current text cell height     */
    int16_t v_cel_mx;         /*  -$2C: Maximum column for cursor    */
    int16_t v_cel_my;         /*  -$2A: Maximum line for cursor      */
    int16_t v_cel_wr;         /*  -$28: Bytes betwn. character cells */
    int16_t v_col_bg;         /*  -$26: Text background colour       */
    int16_t v_col_fg;         /*  -$24: Text foreground colour       */
    void *v_cur_ad;           /*  -$22: Address of cursor position   */
    int16_t v_cur_off;        /*  -$1E: Vertical screen offset       */
    int16_t v_cur_xy[2];      /*  -$1C: X-Y position of text cursor  */
    int8_t v_period;          /*  -$18: Current cursor blink rate    */
    int8_t v_cur_ct;          /*  -$17: Countdown timer for blink    */
    void *v_fnt_ad;           /*  -$16: Addr. of monospace font data */
    int16_t v_fnt_nd;         /*  -$12: Last ASCII code in font      */
    int16_t v_fnt_st;         /*  -$10: First ASCII code in font     */
    int16_t v_fnt_wd;         /*   -$E: Width of font form in bytes  */
    int16_t v_rez_hz;         /*   -$C: Horizontal pixel resolution  */
    int16_t *v_off_ad;        /*   -$A: Address of font offset table */
    int16_t reserved;         /*   -$6: Cursorflag (TOS 1.00)/res.   */
                              /*        Bit 0: Blink on/off          */
                              /*        Bit 1: Blink status norm/inv.*/
                              /*        Bit 2: Cursor visible no/yes */
                              /*        Bit 3: Wrapping on/off       */
                              /*        Bit 4: Inverse on/off        */
                              /*        Bit 5: Cursorpos. saved n/y  */
    int16_t v_rez_vt;         /*   -$4: Vertical pixel resolution    */
    int16_t bytes_lin;        /*   -$2: Bytes per screen line        */
} linea_vdiesc_t;

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
 * @see https://freemint.github.io/tos.hyp/en/linea_showmouse.html
 * @see https://freemint.github.io/tos.hyp/en/linea_hidemouse.html
 */
void linea_set_mouse_visible(int8_t visible);

/**
 * Get line A parameter block.
 */
linea_t *linea_get_parameter_block();

/**
 * Get line A font block.
 */
font_hdr_t *linea_get_font_block();

/**
 * Get line A negative parameter block.
 */
linea_vdiesc_t *linea_get_vdiesc_block();

#endif // _LINEA_H_
