#include <stdio.h>
#include <p33Fj128GP202.h>
#include "D:\documents\Matthew\mplab\ControlV4\MPU6050.h"
#include "D:\documents\Matthew\mplab\ControlV4\common.h"
#define FCY     40000000UL 
#include <libpic30.h>  
#include <math.h>
#include <dsp.h>

#define  PPSUnlock                   __builtin_write_OSCCONL(OSCCON & 0xbf) 
#define  PPSLock                     __builtin_write_OSCCONL(OSCCON | 0x40)

//These values are specific to my hobbyking 6ch reciever
#define THROTTLE_MAX 9328
#define THROTTLE_MIN 5543
#define YAW_MAX 8510
#define YAW_MID 7531
#define YAW_MIN 6577
#define PITCH_MAX 8453
#define PITCH_MID 7562
#define PITCH_MIN 6638
#define ROLL_MAX 8558
#define ROLL_MID 7600
#define ROLL_MIN 6620

//The desired quad angle range, 30 = 15 degrees either way
#define XANGLE_RANGE 30.0
#define YANGLE_RANGE 30.0
#define ZANGLE_RANGE 5.0

int trash = 0;

void Setup_IC()
{
	PPSUnlock;
	RPINR7bits.IC1R = 0b1010; //Tie IC1 to RP10	
	RPINR7bits.IC2R = 0b1011; //Tie IC2 to RP11
	RPINR10bits.IC7R = 0b0010; //Tie IC7 to RP2
	RPINR10bits.IC8R = 0b0011; //Tie IC8 to RP3
	PPSLock;
	
	IC1CONbits.ICSIDL = 0; //Continue in idle mode
	IC1CONbits.ICTMR = 1; //Use Timer2
	IC1CONbits.ICI = 0b00; //Interrupt on every capture
	IC1CONbits.ICM = 0b001; //Every edge
	
	IC2CONbits.ICSIDL = 0; //Continue in idle mode
	IC2CONbits.ICTMR = 1; //Use Timer2
	IC2CONbits.ICI = 0b00; //Interrupt on every capture
	IC2CONbits.ICM = 0b001; //Act as interrupt pin only
	
	IC7CONbits.ICSIDL = 0; //Continue in idle mode
	IC7CONbits.ICTMR = 1; //Use Timer2
	IC7CONbits.ICI = 0b00; //Interrupt on every capture
	IC7CONbits.ICM = 0b001; //Act as interrupt pin only
	
	IC8CONbits.ICSIDL = 0; //Continue in idle mode
	IC8CONbits.ICTMR = 1; //Use Timer2
	IC8CONbits.ICI = 0b00; //Interrupt on every capture
	IC8CONbits.ICM = 0b001; //Act as interrupt pin only
	
	IEC0bits.IC1IE = 1; //Enable interrupt
	IPC0bits.IC1IP = 0b110; //Priority 6
	
	IEC0bits.IC2IE = 1; //Enable interrupt
	IPC1bits.IC2IP = 0b110; //Priority 6
	
	IEC1bits.IC7IE = 1; //Enable interrupt
	IPC5bits.IC7IP = 0b110; //Priority 6
	
	IEC1bits.IC8IE = 1; //Enable interrupt
	IPC5bits.IC8IP = 0b110; //Priority 6	
}	


void _ISR _IC1Interrupt(void)
{	
	if(PORTBbits.RB10 == 1)
	{
		Restart_Timer2();
		trash = IC1BUF;
	}	
	else if(PORTBbits.RB10 == 0)
	{
		yaw_input = IC1BUF;
		TARGET_ZANGLE -= ZANGLE_RANGE*(yaw_input - YAW_MID)/(YAW_MAX - YAW_MIN);
	}
	ClrWdt();
	IFS0bits.IC1IF = 0; //Clear IC1 interrupt flag
}	

void _ISR _IC2Interrupt(void)
{	
	if(PORTBbits.RB11 == 1)
	{
		Restart_Timer2();
		trash = IC2BUF;
	}	
	else if(PORTBbits.RB11 == 0)
	{
		throttle_input = IC2BUF;
		if(throttle_input <= THROTTLE_MIN)
		{
			throttle = 0.0;
			TARGET_ZANGLE = GYRO_ZANGLE;
		}
		else
		{			
			throttle = (float)(throttle_input - THROTTLE_MIN)/(THROTTLE_MAX-THROTTLE_MIN);
		}	
	}
	ClrWdt();
	IFS0bits.IC2IF = 0; //Clear IC2 interrupt flag
}	

void _ISR _IC7Interrupt(void)
{	
	if(PORTBbits.RB2 == 1)
	{
		Restart_Timer2();
		trash = IC7BUF;
	}	
	else if(PORTBbits.RB2 == 0)
	{
		roll_input = IC7BUF;
		TARGET_XANGLE = XANGLE_RANGE*(roll_input - ROLL_MID)/(ROLL_MAX - ROLL_MIN);
	}
	ClrWdt();
	IFS1bits.IC7IF = 0; //Clear IC7 interrupt flag
}

void _ISR _IC8Interrupt(void)
{	
	if(PORTBbits.RB3 == 1)
	{
		Restart_Timer2();
		trash = IC8BUF;
	}	
	else if(PORTBbits.RB3 == 0)
	{
		pitch_input = IC8BUF;
		TARGET_YANGLE = YANGLE_RANGE*(float)((float)(pitch_input - PITCH_MID)/(PITCH_MAX - PITCH_MIN));
	}
	ClrWdt();
	IFS1bits.IC8IF = 0; //Clear IC8 interrupt flag
}