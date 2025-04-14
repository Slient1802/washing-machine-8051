#include "Main.H"
#include "Port.H"
#include "Washer.H"

// Global state variables
unsigned char trang_thai = 0;   // System state
unsigned char count = 0;        // Tick counter

// Function declarations
void function1(void);
void function2(void);
void os(void);

//-------------------------------------------------------------
// Function: UsartConfiguration
// Purpose : Configures the UART for serial communication
//-------------------------------------------------------------
void UsartConfiguration()
{
    SCON = 0x50;      // UART Mode 1: 8-bit data, 1 start bit, 1 stop bit, REN enabled
    TMOD |= 0x20;     // Timer1 in mode 2: 8-bit auto-reload
    TH1 = 0xFD;       // Baud rate 9600 for 11.0592 MHz crystal
    TR1 = 1;          // Start Timer1
    EA = 1;           // Enable global interrupts
}

//-------------------------------------------------------------
// Function: change_state
// Purpose : Changes state based on tick count, then calls OS handler
//-------------------------------------------------------------
void change_state()
{
    count++;

    switch (count) {
        case 20:
            trang_thai = 1;  // Switch to function1
            break;
        case 30:
            trang_thai = 2;  // Switch to function2
            count = 0;       // Reset count after cycle
            break;
        default:
            break;
    }

    os();  // Call task dispatcher
}

//-------------------------------------------------------------
// Function: os
// Purpose : Simple task dispatcher based on `trang_thai` state
//-------------------------------------------------------------
void os()
{
    switch (trang_thai) {
        case 1:
            function1();     // Execute function1
            trang_thai = 3;  // Set to idle
            break;
        case 2:
            function2();     // Execute function2
            trang_thai = 3;  // Set to idle
            break;
        case 3:
            // Idle or do nothing
            break;
    }
}

//-------------------------------------------------------------
// Function: txdata
// Purpose : Transmits a single character over UART
//-------------------------------------------------------------
void txdata(char c)
{
    SBUF = c;         // Load data into UART buffer
    while (!TI);      // Wait for transmission to complete
    TI = 0;           // Clear transmit interrupt flag
}

//-------------------------------------------------------------
// Function: txString
// Purpose : Sends a null-terminated string over UART
//-------------------------------------------------------------
void txString(unsigned char *str)
{
    while (*str != '\0') {
        txdata(*str);
        ++str;
    }
}

//-------------------------------------------------------------
// Function: txInt
// Purpose : Converts and sends an integer as ASCII characters
//-------------------------------------------------------------
void txInt(int num)
{
    unsigned char i = 0, c[10];  // Max 10 digits
    int temp = num;

    if (temp != 0) {
        if (temp < 0) {
            txdata('-');         // Send minus sign
            temp = -temp;
        }
        while (temp) {
            c[i++] = temp % 10;  // Extract digit
            temp /= 10;
        }
        while (i)
            txdata(c[--i] + '0');  // Send digits in correct order
    }
    else {
        txdata('0');  // If number is 0
    }
}

//-------------------------------------------------------------
// Function: function1
// Purpose : Task 1 - prints count value to UART
//-------------------------------------------------------------
void function1()
{
    txString("		;	Ham 1: ");  // "Ham" = Function in Vietnamese
    txInt(count);
}

//-------------------------------------------------------------
// Function: function2
// Purpose : Task 2 - prints count value to UART
//-------------------------------------------------------------
void function2()
{
    txString("		;	Ham 2: ");
    txInt(count);
}
