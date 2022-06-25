#include <mint/osbind.h>

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#include "const.h"
#include "linea.h"
#include "video.h"

static uint32_t counter_interrupt;
static bool flag_new_frame;

void __attribute__((interrupt)) vector_hblank()
{
    counter_interrupt++;

    if (counter_interrupt % 10 == 0)
    {
        flag_new_frame = true;
    }

    // Mark interruption address as terminated
    *(INTERRUPTION_SERVICE_ADDRESS) &= END_OF_INTERRUPT_TIMER_HBLANK;
}

int main(int argc, char **argv)
{
    linea_init();
    video_init();

    uint16_t palette[16];
    for (size_t i = 0; i < 16; i++)
    {
        palette[i] = 0;
    }

    flag_new_frame = false;
    counter_interrupt = 0;

    Xbtimer(TIMER_HBLANK, V_SCREEN_HEIGHT, 0, vector_hblank);

    while (true)
    {
        // printf("%ld\r", counter_interrupt);

        if (flag_new_frame)
        {
            palette[0]++;
            Setpalette(palette);

            flag_new_frame = false;
        }
    }

    video_quit();
    return 0;
}
