#include <stdio.h>
#include <p33Fj128GP202.h>
#include "C:\Users\Matt\Quadrocopter\MPU6050.h" //For some strange reason I couldn't get this to function when relative to the root directory, so I just gave in and included the whole path
#include "C:\Users\Matt\Quadrocopter\declarations.h"
#define FCY     39613750UL //Required for built in delay function
#include <libpic30.h>  
#include <math.h>

_FPOR(FPWRT_PWR128 & ALTI2C_OFF) //set power on timer to 128ms
_FOSCSEL(FNOSC_FRCPLL) //set clock for internal OSC with PLL
_FOSC(OSCIOFNC_OFF & POSCMD_NONE & IOL1WAY_OFF ) //no clock output, external OSC disabled, multiple peripheral pin changes
//_FWDT(FWDTEN_OFF & WDTPRE_PR32 & WDTPOST_PS64 & WINDIS_OFF) //Watchdog timer off, but can be enabled by software
_FICD(JTAGEN_OFF & ICS_PGD1) //disable JTAG, enable debugging on PGx1 pins


void Setup_Oscillator()
{
	// setup internal clock for 80MHz/40MIPS
	// 7.37/2=3.685*43=158.455/2=79.2275
	CLKDIVbits.PLLPRE=0;        // PLLPRE (N2) 0=/2
	PLLFBD=41;                  // pll multiplier (M) = +2
	CLKDIVbits.PLLPOST=0;       // PLLPOST (N1) 0=/2
	while(!OSCCONbits.LOCK);    // wait for PLL ready	
}

void Zero_Sensors()
{
	float BUFFER_XANGLE = 0;
	float BUFFER_YANGLE = 0;
	int x = 0;
	for(x=0; x<100; x++)
	{
		Get_Accel_Values();
		Get_Accel_Angles();
		BUFFER_XANGLE += ACCEL_XANGLE;
		BUFFER_YANGLE += ACCEL_YANGLE;
		__delay_ms(1);
	}
	COMPLEMENTARY_XANGLE = BUFFER_XANGLE/100.0;
	COMPLEMENTARY_YANGLE = BUFFER_YANGLE/100.0;
	GYRO_XANGLE = BUFFER_XANGLE/100.0;
	GYRO_YANGLE = BUFFER_YANGLE/100.0;
}	

int main(void)
{
	Setup_Oscillator();
	/*if(RCONbits.WDTO == 1) //If watchdog reset has occured
	{
		RCONbits.WDTO = 0;
		//Sets all motors to their stationary value. The watchdog reset will only occur if the reciever recieves no data for a certain period, aka if the signal is lost. This does mean the quad will drop out of the sky, but its better than it flying away and hitting something/someone with blades still spinning.
		Setup_Timer3();
		Setup_OC_Single_Shot();			
		OC1R = 700;
		OC2R = 700;
		OC3R = 700;
		OC4R = 700;
		output_compare_fire();	
			
		Setup_UART1();
		printf("\nSignal Lost");
	}	*/
	
	AD1PCFGL = 0xffff; //digital pins
	TRISAbits.TRISA0 = 0;	
	TRISAbits.TRISA1 = 0;	
	LATAbits.LATA1 = 1; // Set LED high
	
	int error = 1;
	Setup_UART1();
	Setup_I2C();
	do
	{
		Setup_MPU6050();
		MPU6050_Test_I2C();
		error = MPU6050_Check_Registers();
	}
	while(error==1);
	
	
	Setup_Timer1(); //400Hz loop timer
	Setup_Timer2(); //Input capture timer
	Setup_Timer3(); //Output compare timer
	Setup_OC_Single_Shot();
	Calibrate_ESC_Endpoints();
	__delay_ms(5000);
	Calibrate_Gyros();
	Zero_Sensors();
	Setup_IC();
	Setup_Timer4(); //RX timeout timer
	//RCONbits.SWDTEN = 1; //Enable watchdog timer
	IEC0bits.T1IE = 1; //Enable timer1 interrupt
	
	while(1)
	{
		if(U1STAbits.TRMT == 1)
		{
			//printf("\n%d,	%d,	%d,	%d	%f",throttle_input, yaw_input, pitch_input, roll_input, throttle);
			//printf("\n%f	%.4f	%.4f	%.4f", throttle, TARGET_XANGLE, TARGET_YANGLE, TARGET_ZRATE);
			//printf("\n%f	%f	%f	%f", OC1_output, OC2_output, OC3_output, OC4_output);
			//printf("\n%u",TMR5);
			//printf("\n%u",OC1R);
			//printf("\n%f,	%f", PID_ZOUTPUT,ZERROR);
			//printf("\nDATA %.3f", COMPLEMENTARY_XANGLE);
			//printf("\nDATA %.3f,%.3f", COMPLEMENTARY_XANGLE, TARGET_XANGLE);
			//printf("\nDATA %.3f,%.3f,%.3f", GYRO_XANGLE, ACCEL_XANGLE, COMPLEMENTARY_XANGLE);
			//printf("\n%d,%d,%d", ACCEL_XOUT, ACCEL_YOUT, ACCEL_ZOUT);
			//printf("\n%f,%f", ACCEL_XANGLE, ACCEL_YANGLE);
		}
		
		if(U1STAbits.URXDA==1)
			{
			char input = U1RXREG;
			
			if (input == '*')
			{				
				IEC0bits.T1IE = 0; //Disable timer1 interrupt
				IEC0bits.IC1IE = 0; //Disable interrupt
				IEC0bits.IC2IE = 0; //Disable interrupt
				IEC1bits.IC7IE = 0; //Disable interrupt
				IEC1bits.IC8IE = 0; //Disable interrupt
				while(1){}
			}
			else if (input == 'q')
			{
				KP = KP + 1;
				printf("\nKP=%.1f",KP);
			}
			else if (input == 'w')
			{
				KP = KP - 1;
				printf("\nKP=%.1f",KP);
			}
			else if (input == 'a')
			{
				KD = KD + 0.5;
				printf("\nKD=%.1f",KD);
			}
			else if (input == 's')
			{
				KD = KD - 0.5;
				printf("\nKD=%.1f",KD);
			}
			
			else if (input == 'z')
			{
				KI = KI + 0.1;
				printf("\nKI=%.1f",KI);
			}
			else if (input == 'x')
			{
				KI = KI - 0.1;
				printf("\nKI=%.1f",KI);
			}
		}		
	}
}

