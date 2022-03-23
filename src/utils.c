#include "utils.h"

#include <osbind.h>

void panic(const char *reason)
{
    (void)(Cconws(reason));
    (void)(Cconws(">> Press any key to continue <<\r\n"));
    Cnecin();
    Pterm(-1);
}