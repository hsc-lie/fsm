#include "unistd.h"
#include <windows.h>

#include "man.h"


int main(void)
{
    Man_t man;
    Man_Init(&man);
    SetConsoleOutputCP(65001);
    for(;;)
    {
        Man_Life(&man);
        usleep(20*1000);
    }

    return 0;
}
