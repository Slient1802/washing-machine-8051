#ifndef _SIMPLE_EOS_H
#define _SIMPLE_EOS_H

// Initializes Timer2 to generate system ticks (in milliseconds)
void sEOS_Init_Timer2(const tByte TICK_MS);

// Puts the system into idle/sleep mode
void sEOS_Go_To_Sleep(void);

#endif // _SIMPLE_EOS_H
