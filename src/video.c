#include "video.h"

#include <stddef.h>
#include <stdint.h>
#include <stdio.h>

#include <osbind.h>

#include "config.h"

#include "linea.h"
#include "utils.h"

void *_v_framebuffer_log = NULL;
void *_v_framebuffer_phy = NULL;
void *_v_framebuffer_vir = NULL;

uint16_t *_v_framebuffer_active = NULL;
uint16_t *_v_framebuffer_hidden = NULL;

uint16_t _v_saved_mode;
uint16_t _v_saved_palette[16];

uint16_t _v_palette[16] = {
    0x000,
    0x300,
    0x030,
    0x003,
    0x330,
    0x033,
    0x303,
    0x333,
    0x000,
    0x700,
    0x070,
    0x007,
    0x770,
    0x077,
    0x707,
    0x777,
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

    Setscreen(_v_framebuffer_active, _v_framebuffer_active, -1);
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