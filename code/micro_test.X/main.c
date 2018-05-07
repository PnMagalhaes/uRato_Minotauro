// imports
#include <stdio.h>
#include <stdlib.h>
#include <p32xxxx.h>
#include <stdint.h>
#include <time.h>

#include "DinotauroDrivers.h"


int main()
{
    INTEnableSystemMultiVectoredInt();
    //init funcs
    initPIC32();
    
    led(1, 1);
    led(2, 1);
    led(3, 1);
    led(4, 1);    
    
    setVel2(50, 50);
    
    delay(5000);
    setVel2(10,20);
    delay(5000);

    return 0;
}