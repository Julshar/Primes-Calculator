#include "include/PrimesAll.h"

int main()
{
    bool* terminate;
    UI ui{terminate};
    while(*terminate)
    {
        ui.getTask();
    }
}