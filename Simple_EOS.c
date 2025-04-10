//Simple_EOS.C (v1.00)
#include "Main.H"
#include "Simple_EOS.H"
#include "Washer.H"
  // Header for dummy task
 

//timer2
void sEOS_ISR() interrupt INTERRUPT_Timer_2_Overflow
{
	// Reset Timer 2 overflow flag
  TF2 = 0;
  // Call dummy task
	change_state();
}

 //Sets up Timer 2 for sEOS with a tick interval in ms.
void sEOS_Init_Timer2(const tByte TICK_MS)
{
    tLong Inc;
    tWord Reload_16;
    tByte Reload_08H, Reload_08L;
	
    T2CON = 0x00; //
    Inc = ((tLong)TICK_MS * (OSC_FREQ / 1000)) / (tLong)OSC_PER_INST;
    Reload_16 = (tWord)(65536UL - Inc);

    Reload_08H = (tByte)(Reload_16 / 256);
    Reload_08L = (tByte)(Reload_16 % 256);

    TH2 = Reload_08H;
    RCAP2H = Reload_08H;
    TL2 = Reload_08L;
    RCAP2L = Reload_08L;

    ET2 = 1;
    TR2 = 1;
    EA = 1;
}

void sEOS_Go_To_Sleep(void)
{
    PCON |= 0x01;  
}