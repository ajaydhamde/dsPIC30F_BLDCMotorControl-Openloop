#ifndef  __PERIPHERAL_H__
#define  __PERIPHERAL_H__
#include "xc.h"
#include <stdint.h>
#define FCY                     10000000// xtal = 5.0Mhz; PLLx8
#define MILLISEC                FCY/10000// 1 mSec delay constant
#define FPWM                    16000
#define RPMConstant             60*(FCY/256)
#define S2                      !PORTCbits.RC14
/********************************************************************
 * GPIO PIN's are Common for open loop and Closed Loop Control
 ********************************************************************/
/********************************************************************
 * User IO  PIN         Description
 *          RC14    :   Push Button(RC5)
 *          RB2     :   Potentiometer(Speed Control)
 *          RB3     :   Hall A  Sensor
 *          RB4     :   Hall B  Sensor
 *          RB5     :   Hall C  Sensor
 *          RE8     :   Fault INPUT
 * 
 *          RE0     :   PWM0
 *          RE1     :   PWM1
 *          RE2     :   PWM2
 *          RE3     :   PWM3
 *          RE4     :   PWM4
 *          RE5     :   PWM5
 * 
 *          RC13    :   LED Output
 * 
 *          RF2     :   RX-Serial
 *          RF3     :   TX-Serial
 *********************************************************************/
/*
 * @brief   :   User Configuration descriptions
 */
extern  uint16_t StateLoTable[];
/*  machine states  */
enum MC_STATE{
    INIT = 0,
    READY_RUN,
    RUNING,
    STOP
    
};
/*  Flags Operation events  */
typedef struct {
    uint8_t RunMotor     :   1;
    uint8_t DirMotor     :   1;
    uint8_t StopMotor    :   1;
    uint8_t FaltMotor    :   1;
    uint8_t res          :   8;
}flags_t;
/*  motor control structure parameters  */
typedef struct{
    uint8_t     cmd;
    uint8_t     response;
    uint8_t     direction;
    flags_t     Flags; 
    uint16_t    PWM_Duty;
    uint16_t    HallValue;
    uint16_t    ActualSpeed;
    uint16_t    Timer3;
    
}motor_control_t;
/*  global object for motor control    */
extern motor_control_t MC; 
/*
 * brief    : Peripherals Configuration
 */
void ChangeOnPin_Init();
void ADC_Channels_Init();
void MCPWM_Init();
void Buttons_Init();
void Timer_Init();
void DelayNmSec(unsigned int );
void GetSpeed(void);
#endif  //__PERIPHERAL_H__