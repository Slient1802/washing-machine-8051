#include "Main.h"
#include "Seven_segment.h"
#include "Port.h"
#include "Switch.h"
#include "Delay.h"
#include "SOES.h"
#include "Washer.h"

#define ONE_SEC 2
tByte program_number[seg7_number] = {0};
int time_ticks = 0;
int time_ticks_internal = 0;
tByte program_selected = 0;
int program_step = 0;
int program_state = 0;
int curret_state = INIT;
	
tByte features[3] = {FALSE};

void program_inc(tByte *displayed_array, bit is_dec);
// Define some basic macros
#define FALSE 0
#define TRUE 1
#define OFF 0
#define ON 1

// Time base definition for 1 second
#define ONE_SEC 2  // This assumes ONE_SEC is defined as 2 ticks

// Global Variables
tByte program_number[seg7_number] = {0};   // Holds the currently selected program number (2-digit)
int time_ticks = 0;                        // Time counter for delays
int time_ticks_internal = 0;              // Secondary time counter for sub-timings
tByte program_selected = 0;               // Holds final selected program number as integer
int program_step = 0;                     // Tracks the current step of the running program
int program_state = 0;                    // Holds the current state inside a program
int curret_state = INIT;                  // State machine for the main program (INIT, PROGRAM_SELECT, RUN_PROGRAM)

tByte features[3] = {FALSE};              // Flags for optional features (heating, drying, double cycle)

// Function to increment or decrement displayed program number
void program_inc(tByte *displayed_array, bit is_dec);

// Main function
void main()
{
    // Initialize 7-segment display (anode type)
    seg7_enable(1, IS_ANODE, 1);
    
    finished = OFF;

    // Initialize and then sleep the SOES (EtherCAT slave)
    SOES_voidInitialize();
    SOES_voidGoToSleep();
}

// Handles increment or decrement of 2-digit number stored in array
void program_inc(tByte *displayed_array, bit is_dec)
{
    if(is_dec)
    {
        // Decrement logic
        if(displayed_array[0] > 0)
        {
            displayed_array[0]--;
        }
        else if(displayed_array[1] > 0)
        {
            displayed_array[0] = 9;
            displayed_array[1]--;
        }
    } 
    else 
    {
        // Increment logic with limit check (max 19)
        if((displayed_array[0] < 9 && displayed_array[1] == 0) || (displayed_array[0] < 8 && displayed_array[1] == 1))
        {
            displayed_array[0]++;
        }
        else if(displayed_array[1] == 0)
        {
            displayed_array[0] = 0;
            displayed_array[1] = 1;
        }
    }
}

// Main washer program loop
void run()
{
    // Display selected program number on 7-segment
    seg7_display_digits(program_number, IS_ANODE);

    switch (curret_state)
    {
        // Initialization state
        case INIT:
        {
            washer_off_all();                 // Turn off all washer parts
            program_step = 0;
            time_ticks = 0;
            time_ticks_internal = 0;
            program_selected = 0;
            curret_state = PROGRAM_SELECT;   // Go to program selection
            break;
        }

        // User selects the program
        case PROGRAM_SELECT:
        {
            // If UP button pressed
            if(SWITCH_state(1, DEBOUNCE_PERIOD) == SWITCH_PRESSED)
            {
                program_inc(program_number, 0); // Increment number
            }

            // If DOWN button pressed
            if(SWITCH_state(2, DEBOUNCE_PERIOD) == SWITCH_PRESSED)
            {
                program_inc(program_number, 1); // Decrement number
            }

            // If START button pressed
            if(SWITCH_state(3, DEBOUNCE_PERIOD) == SWITCH_PRESSED)
            {
                // Check if door is closed
                if(door_closed_pin == SWITCH_PRESSED)
                {
                    // Get selected program number from array
                    program_selected = array_to_value(program_number, seg7_number);
                    if(program_selected != 0)
                    {
                        door_lock = ON;                           // Lock the door
                        curret_state = RUN_PROGRAM;               // Go to run state
                        washer_program_features(program_selected, features); // Load program features
                    }
                }
            }
            break;
        }

        // Program is running
        case RUN_PROGRAM:
        {
            // Check for door open or stop button pressed
            if(door_closed_pin != SWITCH_PRESSED || SWITCH_state(3, DEBOUNCE_PERIOD) != SWITCH_PRESSED)
            {
                washer_off_all();
                error = ON;
                break;
            }
            else 
            {
                error = OFF;
                finished = OFF;
                door_lock = ON;
            }

            // Fetch current step of program
            program_state = washer_get_program_step(program_selected, program_step);

            // State machine for each program step
            switch (program_state)
            {
                case FINISH:
                {
                    washer_off_all();
                    finished = ON;
                    door_lock = OFF;
                    program_number[0] = 0;
                    program_number[1] = 0;
                    curret_state = INIT;
                    break;
                }

                case ADD_DETERGENT:
                {
                    detergent_valve = ON;
                    if (++time_ticks >= (5 * ONE_SEC))
                    {
                        detergent_valve = OFF;
                        time_ticks = 0;
                        program_step++;
                    }
                    break;
                }

                case ADD_WATER:
                {
                    water_valve = ON;
                    if (water_level_full == SENSOR_ON)
                    {
                        water_valve = OFF;
                        program_step++;
                    }
                    break;
                }

                case HEAT:
                {
                    if(features[0] == FALSE)
                    {
                        program_step++;
                        break;
                    }
                    heater = ON;
                    if (temprature_sensor == SENSOR_ON)
                    {
                        heater = OFF;
                        program_step++;
                    }
                    break;
                }

                case SPIN_MOTOR:
                {
                    motor_slow = ON;
                    if (++time_ticks >= (5 * ONE_SEC))
                    {
                        motor_slow = OFF;
                        time_ticks = 0;
                        program_step++;
                    }
                    break;
                }

                case TOGGLE_MOTOR:
                {
                    // Alternate direction
                    if(time_ticks_internal < (2 * ONE_SEC)) 
                    {
                        motor_reverse = OFF;
                        DELAY_loop_ms(10);
                        motor_slow = ON;
                        time_ticks_internal++;
                    } 
                    else if(time_ticks_internal < (4 * ONE_SEC)) 
                    {
                        motor_slow = OFF;
                        DELAY_loop_ms(10);
                        motor_reverse = ON;
                        time_ticks_internal++;
                    } 
                    else 
                    {
                        time_ticks_internal = 0;
                        motor_reverse = OFF;
                        motor_slow = ON;
                    }

                    // After 8 seconds, stop toggling
                    if (++time_ticks >= (8 * ONE_SEC))
                    {
                        motor_reverse = OFF;
                        motor_slow = OFF;
                        time_ticks = 0;
                        program_step++;
                    }
                    break;
                }

                case DRY:
                {
                    if(features[1] == FALSE)
                    {
                        program_step++;
                        break;
                    }
                    if (time_ticks_internal < (5 * ONE_SEC))
                    {
                        motor_fast = ON;
                        time_ticks_internal++;
                        break;
                    } 
                    else 
                    {
                        motor_fast = OFF;
                    }
                    // Fallthrough to DRAIN_WATER
                }

                case DRAIN_WATER:
                {
                    water_drain = ON;
                    if (water_level_empty == SENSOR_ON)
                    {
                        water_drain = OFF;
                        time_ticks_internal = 0;
                        program_step++;
                    }
                    break;
                }

                case REPEAT:
                {
                    if(features[2] == FALSE)
                    {
                        program_step++;
                    } 
                    else 
                    {
                        features[2] = FALSE;
                        program_step = 0;
                    }
                    break;
                }
            }
            break;
        }
    }
}
