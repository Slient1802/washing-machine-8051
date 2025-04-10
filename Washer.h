//T_Lights.h
#ifndef _WASHER_H
#define _WASHER_H

extern unsigned char trang_thai;
extern unsigned char count;

void function1(void);
void os(void);
void change_state(void);
void function1(void);
void function2(void);

void UsartConfiguration(void);

// Add function prototypes in ANSI style:
void washer_off_all(void);
void washer_program_features(tByte program_selected, tByte features[]);



// ANSI-style prototype:
tByte washer_get_program_step(tByte program, int step);


// If you use any additional washer functions, list them here as well.




#endif