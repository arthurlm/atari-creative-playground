// Source https://www.atarimagazines.com/compute/issue87/ST_Outlook.php

#include <osbind.h>

/* Line A commands to turn mouse off and on */
#define MOUSE_DIE     \
    asm { DC.W 0xa00a} \
    ;
#define MOUSE_LIVE    \
    asm { DC.W 0xa009} \
    ;

long hidden_page, active_page, temp;
char msg1[] = "This is the original screen.";
char msg2[] = "This is the alternate screen.";
char msg3[] = "Press ESC to exit, any key to flip.";
char crlf[] = {13, 10, 0};

main()
{
    int key, foo;
    long mem_chunk, pagel, page2, log;
    pagel = Physbase(); /* Get physical screen base */
    log = Logbase();    /* and logical screen base */
                        /* Get enough memory to hold the alternate screen. We need */
    /* 32, 000 bytes plus enough to align on a 256-byte boundary */ mem_chunk = Malloc(0x8100L);
    /* Find page-aligned address within our chunk */
    page2 = (mem_chunk | 0xff) + 1;
    active_page = page1; /* Original screen is active */
    hidden_page = page2; /* Alternate screen is hidden */
    MOUSE_DIE;           /* Hide mouse for the moment */
    Cconws(crlf);
    Print_it(msg1); /* Print message on original screen */
    Flip();         /* Flip to alternate screen */
    Print_it(msg2); /* and print message there */
    Print_it(msg3);
    MOUSE_LIVE; /* Show mouse again */
    foo = 1;

    while (foo) /* Endless loop (loop until we break) */
    {
        key = Crawcin(); /* Wait for keypress */
        if (key = = 27)
            break; /* Exit if it's ESC */
        else
            Flip(); /* Flip screens otherwise … */
    }

    Setscreen(log, page1, -1L); /* Restore the original screen */
    Mfree(mem_chunk);           /* Release memory we grabbed earlier */
}

Flip()
{
    MOUSE_DIE;
    /* Swap the screens */
    temp = hidden_page;
    hidden_page = active_page;
    active_page = temp;
    /* Display the active page */
    Setscreen(active_page, active_page, -1L);
    MOUSE_LIVE;
}

Print_it(message) char message[];
{
    Cconws(crlf);
    Cconws(message);
}