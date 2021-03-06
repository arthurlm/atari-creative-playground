#include "video.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <osbind.h>

#include "linea.h"
#include "utils.h"

static void *_v_framebuffer_log = NULL;
static void *_v_framebuffer_phy = NULL;
static void *_v_framebuffer_vir = NULL;

static uint16_t *_v_framebuffer_active = NULL;
static uint16_t *_v_framebuffer_hidden = NULL;

static uint16_t _v_saved_mode;
static uint16_t _v_saved_palette[16];

static uint16_t _v_palette[16] = {
    0x000, // 0b0000 BLACK

    0x300, // 0b0001 DARK_RED
    0x030, // 0b0010 DARK_GREEN
    0x003, // 0b0011 DARK_BLUE
    0x330, // 0b0100
    0x033, // 0b0101
    0x303, // 0b0110

    0x333, // 0b0111 GRAY
    0x000, // 0b1000 LIGHT_BLACK <= This can be improved

    0x700, // 0b1001 LIGHT_RED
    0x070, // 0b1010 LIGHT_GREEN
    0x007, // 0b1011 LIGHT_BLUE
    0x770, // 0b1100
    0x077, // 0b1101
    0x707, // 0b1110

    0x777, // 0b1111 WHITE
};

static void _video_clear_fb(uint16_t *framebuffer)
{
    for (size_t i = 0; i < V_FRAMEBUFFER_W_SIZE; i++)
    {
        framebuffer[i] = 0x0000;
    }
}

void video_init()
{
    // Reserve space for double buffering frame buffer
    _v_framebuffer_vir = (void *)Malloc(V_FRAMEBUFFER_B_SIZE + 0x100);
    if (!_v_framebuffer_vir)
    {
        panic("Fail to init double framebuffer");
    }

    // Init global variables
    _v_framebuffer_log = Logbase();
    _v_framebuffer_phy = Physbase();

    // Save screen resolution and palette
    _v_saved_mode = Getrez();

    for (size_t i = 0; i < 16; i++)
    {
        _v_saved_palette[i] = Setcolor(i, -1);
    }

    // Disable mouse
    linea_set_mouse_visible(0);

    // Configure frame buffer addr
    // NOTE: we need to align page:
    // - 0xFF: go to page end
    // - +1  : go to next page
    _v_framebuffer_active = _v_framebuffer_phy;
    _v_framebuffer_hidden = ((uint32_t)(_v_framebuffer_vir) | 0xFF) + 1;

    // Clear framebuffers
    _video_clear_fb(_v_framebuffer_active);
    _video_clear_fb(_v_framebuffer_hidden);

    // Update screen and palette
    Setpalette(_v_palette);
    Setscreen(_v_framebuffer_active, _v_framebuffer_active, V_RESOLUTION_LOW);
}

void video_quit()
{
    // Restore screen resolution and palette
    Setscreen(_v_framebuffer_log, _v_framebuffer_phy, _v_saved_mode);
    Setpalette(_v_saved_palette);

    // Enable mouse
    linea_set_mouse_visible(1);

    // Free memory
    Mfree(_v_framebuffer_vir);

    // Cleanup global variables
    _v_framebuffer_log = NULL;
    _v_framebuffer_phy = NULL;
    _v_framebuffer_vir = NULL;
    _v_framebuffer_active = NULL;
    _v_framebuffer_hidden = NULL;
}

void video_flip_buffers()
{
    void *temp = _v_framebuffer_hidden;
    _v_framebuffer_hidden = _v_framebuffer_active;
    _v_framebuffer_active = temp;

    video_set_draw_buffer_primary();
}

void video_clear_screen()
{
    _video_clear_fb(_v_framebuffer_hidden);
}

void video_print_all_addr()
{
#ifdef ENABLE_STDLIB
    printf("logical addr : %p\n\r", _v_framebuffer_log);
    printf("physical addr: %p\n\r", _v_framebuffer_phy);
    printf("virtual addr : %p\n\r", _v_framebuffer_vir);
    printf("\r\n");
    printf("FB active: %p\n\r", (void *)_v_framebuffer_active);
    printf("FB hidden: %p\n\r", (void *)_v_framebuffer_hidden);
#endif // ENABLE_STDLIB
}

void video_print_active_addr()
{
#ifdef ENABLE_STDLIB
    printf("active=%p fb=[%p %p]\r",
           _v_framebuffer_active,
           _v_framebuffer_log,
           _v_framebuffer_vir);
#endif // ENABLE_STDLIB
}

void video_set_draw_buffer_primary()
{
    Setscreen(_v_framebuffer_active, _v_framebuffer_active, -1);
}

void video_set_draw_buffer_hidden()
{
    Setscreen(_v_framebuffer_hidden, _v_framebuffer_hidden, -1);
}