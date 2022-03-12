#include "include/PrimesAll.h"
//#include "include/UI.h"

int main()
{
    UI ui;
    while(!ui.terminate())
    {
        ui.getTask();
    }
}