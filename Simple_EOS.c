// Simple_EOS.C (v1.00)
// Includes core headers for system functionality
#include "Main.H"
#include "Simple_EOS.H"
#include "Washer.H"

//------------------------------------------------------------------------------
// Timer 2 Interrupt Service Routine for simple Embedded Operating System (sEOS)
//------------------------------------------------------------------------------
void sEOS_ISR() interrupt INTERRUPT_Timer_2_Overflow
{
    // Clear Timer 2 overflow flag
    TF2 = 0;

    // Call system state change or dummy task handler
    change_state();
}

//------------------------------------------------------------------------------
// Initializes Timer 2 for sEOS with a user-defined tick interval in milliseconds
// This function sets up periodic interrupts for the operating system scheduler
//------------------------------------------------------------------------------
void sEOS_Init_Timer2(const tByte TICK_MS)
{
    tLong Inc;             // Timer tick increment value
    tWord Reload_16;       // 16-bit reload value for Timer 2
    tByte Reload_08H;      // High byte of reload value
    tByte Reload_08L;      // Low byte of reload value

    // Ensure Timer 2 is disabled and configured in default mode
    T2CON = 0x00;

    // Calculate the number of timer increments based on tick duration
    Inc = ((tLong)TICK_MS * (OSC_FREQ / 1000)) / (tLong)OSC_PER_INST;

    // Calculate the reload value to achieve desired tick interval
    Reload_16 = (tWord)(65536UL - Inc);
    Reload_08H = (tByte)(Reload_16 / 256);  // Extract high byte
    Reload_08L = (tByte)(Reload_16 % 256);  // Extract low byte

    // Load calculated values into Timer 2 registers
    TH2 = Reload_08H;      // Timer 2 high byte
    RCAP2H = Reload_08H;   // Auto-reload high byte
    TL2 = Reload_08L;      // Timer 2 low byte
    RCAP2L = Reload_08L;   // Auto-reload low byte

    // Enable Timer 2 interrupt
    ET2 = 1;

    // Start Timer 2
    TR2 = 1;

    // Enable global interrupts
    EA = 1;
}

//------------------------------------------------------------------------------
// Places the CPU into idle mode to save power
// Will wake up automatically on enabled interrupt (e.g., Timer 2)
//------------------------------------------------------------------------------
void sEOS_Go_To_Sleep(void)
{
    PCON |= 0x01;  // Set idle mode bit (IDLE = 1)
}
