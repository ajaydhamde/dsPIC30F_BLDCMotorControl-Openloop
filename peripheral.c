/*
 * File:   peripheral.c
 * Author: admin
 *
 * Created on October 3, 2022, 4:13 PM
 */


#include "peripheral.h"
/*************************************************************
Low side driver table is as below. In this StateLoTable,
the Low side driver is PWM while the high side driver is
either on or off. This table is used in this exercise
*************************************************************/
uint16_t StateLoTable[] = {0x0000, 0x0210, 0x2004, 0x0204,0x0801, 0x0810, 0x2001, 0x0000};
/****************************************************************/

motor_control_t MC; 
/*
 * brief    : _ISR_CNInterrupt
 * pin change interrupt ISR
 */
//void interrupt  ISR_CNInterrupt()
void __attribute__ (( interrupt, no_auto_psv )) _CNInterrupt ( void )
{
    IFS0bits.CNIF = 0; // clear flag
    MC.HallValue = PORTB & 0x0038; // mask RB3,4 & 5
    MC.HallValue = MC.HallValue >> 3; // shift right 3 times
    OVDCON = StateLoTable[MC.HallValue];
}
/*
 * brief    : _ISR_CNInterrupt
 * pin change interrupt ISR
 */
/*********************************************************************
The ADC interrupt loads the PDCx registers with the demand pot value.
This is only done when the motor is running.
*********************************************************************/
//void interrupt ISR_ADCInterrupt()
void __attribute__ ( ( __interrupt__ , auto_psv, weak ) ) _ADCInterrupt ( void )
{
    IFS0bits.ADIF = 0;
    if (MC.Flags.RunMotor)
    {
    PDC1 = ADCBUF0; // get value ...
    PDC2 = PDC1; // and load all three PWMs ...
    PDC3 = PDC1; // duty cycles
    }
}

void ChangeOnPin_Init(){
    CNEN1 = 0x00E0; // CN5,6 and 7 enabled
    CNPU1 = 0x00E0; // enable internal pullups
    IFS0bits.CNIF = 0; // clear CNIF
    IEC0bits.CNIE = 1; // enable CN interrupt
}
void ADC_Channels_Init(){
    ADPCFG = 0xFFF8; // all PORTB = Digital;RB0 to RB2 = analog
    ADCON1 = 0x0064; // PWM starts conversion
    ADCON2 = 0x0200; // simulataneous sample 4 channels
    ADCHS = 0x0002; // Connect RB2/AN2 as CH0 = pot ..
    // ch1 = Vbus, Ch2 = Motor, Ch3 = pot
    ADCON3 = 0x0080; // Tad = internal RC (4uS)
    IFS0bits.ADIF = 0;
    IEC0bits.ADIE = 1;
    ADCON1bits.ADON = 1; // turn ADC ON
}
/********************************************************************
InitMCPWM, intializes the PWM as follows:
1. FPWM = 16000 hz
2. Independant PWMs
3. Control outputs using OVDCON
4. Set Duty Cycle with the ADC value read from pot
5. Set ADC to be triggered by PWM special trigger
*********************************************************************/
void MCPWM_Init(){
    LATE |= 0x0000;
    TRISE |= 0xFFC0; // PWMs are outputs
    PTPER = FCY/FPWM - 1;
    
    PWMCON1 = 0x0700; // disable PWMs
    OVDCON = 0x0000; // allow control using OVD
    PDC1 = 100; // init PWM 1, 2 and 3 to 100
    PDC2 = 100;
    PDC3 = 100;
    SEVTCMP = PTPER;
    PWMCON2 = 0x0F00; // 16 postscale values
    PTCON = 0x8000; // start PWM
}
void Buttons_Init(){
    
}
/************************************************************************
Tmr3 is used to determine the speed so it is set to count using Tcy/256
*************************************************************************/
void Timer_Init()
{
    T3CON = 0x0030; // internal Tcy/256 clock
    TMR3 = 0;
    PR3 = 0x8000;
}
/************************************************************************
GetSpeed, determins the exact speed of the motor by using the value in
TMR3 for every mechanical cycle.
*************************************************************************/
void GetSpeed(void)
{
    uint16_t Speed;
    if (MC.Timer3 > 23000) // if TMR3 is large ignore reading
        return;
    if (MC.Timer3 > 0)
        Speed = RPMConstant/(long)MC.Timer3;// get speed in RPM
    MC.ActualSpeed += Speed;
    MC.ActualSpeed = MC.ActualSpeed >> 1;
//    if (++SpeedCount == 1)
//    {SpeedCount = 0;CalculateDC();}
}
//---------------------------------------------------------------------
// This is a generic 1ms delay routine to give a 1mS to 65.5 Seconds delay
// For N = 1 the delay is 1 mS, for N = 65535 the delay is 65,535 mS.
// Note that FCY is used in the computation. Please make the necessary
// Changes(PLLx4 or PLLx8 etc) to compute the right FCY as in the define
// statement above.
void DelayNmSec(unsigned int N)
{
unsigned int j;
while(N--)
for(j=0;j < MILLISEC;j++);
}
