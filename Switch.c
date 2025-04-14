/*------------------------------------------------------------
 seven_segment.C (v1.00)
--------------------------------------------------------------
 Simple library for detecting switch presses with debouncing
 and returning current switch states.

 Includes:
 - Debounced detection of switch press
 - Raw switch state reading

 Dependencies:
 - Main.h, Port.h, Switch.h, Delay.h

-------------------------------------------------------------*/

#include "Main.h"
#include "Port.h"
#include "Switch.h"
#include "Delay.h"

//-------------------------------------------------------------
// Function: SWITCH_state
// Purpose : Detect if a switch is pressed, with software debouncing
// Params  : switch_number - identifier for the switch (1, 2, 3)
//           debounce_period - time in ms to wait for stable input
// Returns : SWITCH_PRESSED (1) or SWITCH_NOT_PRESSED (0)
//-------------------------------------------------------------
bit SWITCH_state(int switch_number, tByte debounce_period)
{
    bit Return_value = SWITCH_NOT_PRESSED;

    // Check initial switch state
    if (SWITCH_current_state(switch_number) == 0)
    {
        // Possible press detected
        // Wait for debounce period to confirm
        DELAY_loop_ms(debounce_period);

        // Check switch state again
        if (SWITCH_current_state(switch_number) == 0)
        {
            // Confirmed press after debounce
            Return_value = SWITCH_PRESSED;
        }
    }

    // Return final evaluated switch state
    return Return_value;
}

//-------------------------------------------------------------
// Function: SWITCH_current_state
// Purpose : Return the raw state of a given switch
// Params  : switch_number - identifier for the switch (1, 2, 3)
// Returns : state of the switch (1 = not pressed, 0 = pressed)
// Notes   : Assumes switches are active-low (0 = pressed)
//-------------------------------------------------------------
bit SWITCH_current_state(int switch_number)
{
    if (switch_number == 1)
    {
        up_pin = 1;         // Read logic level of 'up' switch
        return up_pin;
    }
    else if (switch_number == 2)
    {
        down_pin = 1;       // Read logic level of 'down' switch
        return down_pin;
    }
    else if (switch_number == 3)
    {
        start_pin = 1;      // Read logic level of 'start' switch
        return start_pin;
    }
    else
    {
        // Invalid switch number
        return 0;
    }
}

//---------------------------- END OF FILE ----------------------------
