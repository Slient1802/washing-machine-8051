/*------------------------------------------------------------*
 Delay.C (v1.00)
--------------------------------------------------------------
 Simple software and hardware delay library using 8051 timers.

 Provides:
 - Hardware delay using Timer 0 and Timer 1
 - Software delay loops for ms and short ns-scale delays

 Dependencies:
 - Main.h
 - Port.h
 - Delay.h
--------------------------------------------------------------*/

#include "Main.h"
#include "Port.h"
#include "Delay.h"

//-------------------------------------------------------------
// Function : DELAY_T0_ms
// Purpose  : Generates delay in milliseconds using Timer 0
// Params   : N - delay time in milliseconds
//-------------------------------------------------------------
void DELAY_T0_ms(tWord N)
{
	tWord ms;

	// Configure Timer 0 in 16-bit mode
	TMOD &= 0xF0;      // Clear lower 4 bits (T0 config)
	TMOD |= 0x01;      // Set T0 in 16-bit timer mode
	ET0 = 0;           // Disable Timer 0 interrupt

	for (ms = 0; ms < N; ms++)	
	{
		TH0 = PRELOAD01H; // Load high byte
		TL0 = PRELOAD01L; // Load low byte
		TF0 = 0;          // Clear overflow flag
		TR0 = 1;          // Start Timer 0
		while (TF0 == 0); // Wait for overflow
		TR0 = 0;          // Stop Timer 0
	}
}

//-------------------------------------------------------------
// Function : DELAY_T1_ms
// Purpose  : Generates delay in milliseconds using Timer 1
// Params   : N - delay time in milliseconds
//-------------------------------------------------------------
void DELAY_T1_ms(tWord N)
{
	tWord ms;

	// Configure Timer 1 in 16-bit mode
	TMOD &= 0x0F;      // Clear upper 4 bits (T1 config)
	TMOD |= 0x10;      // Set T1 in 16-bit timer mode
	ET1 = 0;           // Disable Timer 1 interrupt

	for (ms = 0; ms < N; ms++)	
	{
		TH1 = PRELOAD01H;
		TL1 = PRELOAD01L;
		TF1 = 0; // clear overflow flag
		TR1 = 1; // start timer 0
		while (TF1 == 0); // Loop until Timer 0 overflows (TF0 == 1)
		TR1 = 0; // Stop Timer 0
	}
}

//-------------------------------------------------------------
// Function : DELAY_loop_ms
// Purpose  : Simple software-based millisecond delay
// Note     : Crude timing, depends on clock frequency
// Params   : ms - delay duration in milliseconds
//-------------------------------------------------------------
void DELAY_loop_ms(tWord ms)
{
	tWord counter1;
	tByte counter2;

	for (counter1 = 0; counter1 < ms; counter1++)
	{
		for (counter2 = 0; counter2 < 250; counter2++);
	}
}

//-------------------------------------------------------------
// Function : DELAY_loop_ns
// Purpose  : Very short software-based delay loop (ns-range)
// Note     : Use for quick pulse delays or segment stability
// Params   : ns - arbitrary loop count (not real nanoseconds)
//-------------------------------------------------------------
void DELAY_loop_ns(tWord ns)
{
	int counter;
	for (counter = 0; counter < ns; counter++);
}

/*------------------------------------------------------------*
------------------------ END OF FILE --------------------------
*------------------------------------------------------------*/
