/*------------------------------------------------------------*
 seven_segment.C (v1.00)
--------------------------------------------------------------
 Simple library for displaying digits on a 7-segment display.

 Supports:
 - Common anode and common cathode displays
 - Single and dual digit handling
 - Conversion from digit to segment pattern
 - Multiplexed display control
 - Conversion of digit array to integer value

 Dependencies:
 - Main.h, Port.h, Seven_segment.h, Delay.h
--------------------------------------------------------------*/

#include "Main.h"
#include "Port.h"
#include "Seven_segment.h"
#include "Delay.h"

//-------------------------------------------------------------
// Lookup tables to convert digits (0–9) to 7-segment values
// for both anode and cathode configurations
//-------------------------------------------------------------
static const tByte bcd_to_7_anode[]   = {0xC0, 0xF9, 0xA4, 0xB0, 0x99,
                                         0x92, 0x82, 0xF8, 0x80, 0x90};

static const tByte bcd_to_7_cathode[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66,
                                         0x6D, 0x7D, 0x07, 0x7F, 0x6F};

//-------------------------------------------------------------
// Function : seg7_display_1_digit
// Purpose  : Displays a single digit on the 7-segment display
// Params   : digit - digit to display (0–9)
//            is_anode - 1 for common anode, 0 for cathode
//-------------------------------------------------------------
void seg7_display_1_digit(tByte digit, bit is_anode)
{
    if (is_anode == 1)
    {
        port_7_segment = bcd_to_7_anode[digit];
    }
    else
    {
        port_7_segment = bcd_to_7_cathode[digit];
    }

    // Small delay for segment stabilization
    DELAY_loop_ns(2);
}

//-------------------------------------------------------------
// Function : seg7_enable
// Purpose  : Enables specific 7-segment digit (for multiplexing)
// Params   : segment_number - which segment to enable (1 or 2)
//            is_anode - 1 for common anode, 0 for cathode
//            disable_rest - whether to disable other digits
//-------------------------------------------------------------
void seg7_enable(int segment_number, bit is_anode, bit disable_rest)
{
    if (segment_number == 1)
    {
        if (is_anode)
        {
            seg7_enable_1 = 1;
            if (disable_rest) seg7_enable_2 = 0;
        }
        else
        {
            seg7_enable_1 = 0;
            if (disable_rest) seg7_enable_2 = 1;
        }
    }
    else if (segment_number == 2)
    {
        if (is_anode)
        {
            seg7_enable_2 = 1;
            if (disable_rest) seg7_enable_1 = 0;
        }
        else
        {
            seg7_enable_2 = 0;
            if (disable_rest) seg7_enable_1 = 1;
        }
    }
}

//-------------------------------------------------------------
// Function : seg7_display_digits
// Purpose  : Displays multiple digits using multiplexing
// Params   : digits - array of digit values to display
//            is_anode - display type: 1 for anode, 0 for cathode
//-------------------------------------------------------------
void seg7_display_digits(tByte digits[seg7_number], bit is_anode)
{
    int i;
    for (i = 0; i < seg7_number; i++)
    {
        seg7_enable(i + 1, is_anode, 1);
        seg7_display_1_digit(digits[i], is_anode);
        DELAY_loop_ms(50);  // Persistence delay for visibility
    }
}

//-------------------------------------------------------------
// Function : array_to_value
// Purpose  : Converts an array of digits to an integer
// Example  : [1, 2, 3] → 123
// Params   : container_array - array of digits
//            array_size - number of digits in the array
// Returns  : integer value composed from the array
//-------------------------------------------------------------
int array_to_value(tByte *container_array, int array_size)
{
    tByte result = 0;
    int i, j, multiplier = 1;

    for (i = 0; i < array_size; i++)
    {
        for (j = 0; j < i; j++)
        {
            multiplier *= 10;
        }
        result += container_array[i] * multiplier;
        multiplier = 1;
    }

    return result;
}

//---------------------------- END OF FILE ----------------------------
