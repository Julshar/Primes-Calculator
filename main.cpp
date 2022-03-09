#include "include/PrimesAll.h"
#include "include/UI.h"

int main()
{
    bool* terminate;
    UI ui{terminate};
    while(*terminate)
    {
        ui.getTask();
    }
}