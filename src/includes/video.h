#ifndef _CONFIG_H_
#define _CONFIG_H_

/*
 * Thanks to FX Java dev blog
 *
 * https://www.fxjavadevblog.fr/atari-st-4-bitplanes/
 */

#define V_RESOLUTION_LOW 0
#define V_RESOLUTION_MED 1
#define V_RESOLUTION_HIG 2

#define V_SCREEN_WIDTH 320
#define V_SCREEN_HEIGHT 200
#define V_SCREEN_H_BLOCK 20

// There is 4 plane, 8 pixels per byte, so we need to divide screen buffer size by 2
#define V_FRAMEBUFFER_B_SIZE 32000
#define V_FRAMEBUFFER_W_SIZE 16000

/**
 * Save previous video config and init helper.
 */
void video_init();

/**
 * Restore video configuration.
 */
void video_quit();

/**
 * Flip frame buffer
 */
void video_flip_buffers();

/**
 * Clear screen.
 */
void video_clear_screen();

/**
 * Print video all framebuffers address.
 */
void video_print_all_addr();

/**
 * Print active framebuffer addr.
 */
void video_print_active_addr();

#endif // _CONFIG_H_
