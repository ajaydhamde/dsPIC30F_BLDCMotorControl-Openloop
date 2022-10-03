/*
 * File:   main.c
 * Author: admin
 *
 * Created on October 3, 2022, 4:02 PM
 */


#include "xc.h"
#include "peripheral.h"
#include <stdint.h>
#include <stdio.h>

int main(void) {
    
    ChangeOnPin_Init();
    MCPWM_Init();
    Timer_Init();
    while(1)
    { 
        while (!S2); // wait for start key hit
        while (S2) // wait till key is released
        DelayNmSec(10);
        // read hall position sensors on PORTB
        MC.HallValue = PORTB & 0x0038; // mask RB3,4 & 5
        MC.HallValue = MC.HallValue >> 3; // shift right to get value 1, 2 ... 6
        OVDCON = StateLoTable[MC.HallValue]; // Load the overide control register
        PWMCON1 = 0x0777; // enable PWM outputs
        MC.Flags.RunMotor = 1; // set flag
        while (MC.Flags.RunMotor) // while motor is running
        if (S2) // if S2 is pressed
        {
            PWMCON1 = 0x0700; // disable PWM outputs
            OVDCON = 0x0000; // overide PWM low.
            MC.Flags.RunMotor = 0; // reset run flag
            while (S2) // wait for key release
            DelayNmSec(10);
        }
    } // end of while (1)
    return 0;
}
