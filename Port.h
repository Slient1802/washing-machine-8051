#ifndef PORT_H
#define PORT_H

#include <REG52.H>    // or <REG52.H>, depending on your device

/**************************************************
 * 7-Segment Display Connections
 **************************************************/
// If your 7-segment data lines are on Port 1 (via RN1):
#define port_7_segment P1

// Digit enable pins (assuming you have two digits):
sbit seg7_enable_1 = P2^0;
sbit seg7_enable_2 = P2^1;

/**************************************************
 * Washer Outputs (relays, motors, valves, etc.)
 **************************************************/
sbit motor_reverse   = P0^7; 
sbit motor_slow      = P0^6;  
sbit motor_fast      = P0^5;
sbit water_drain     = P0^4;   

sbit door_lock       = P3^1;  
sbit heater          = P3^2; 
sbit water_valve     = P3^3;  
sbit detergent_valve = P3^4;  

/**************************************************
 * Indicators (LEDs)
 **************************************************/
sbit error    = P3^5;  
sbit finished = P3^6;  

/**************************************************
 * Sensor Inputs
 **************************************************/
sbit door_closed_pin   = P2^5;   
sbit water_level_full  = P2^6;  
sbit water_level_empty = P2^7; 
sbit temprature_sensor = P3^0;  

/**************************************************
 * Up /Down pin/ Start pin Inputs
 **************************************************/
 
sbit up_pin = P2^2;  
sbit down_pin = P2^3; 
sbit start_pin = P2^4; 



/**************************************************
 * Useful Macros
 **************************************************/
// Logical states
#define TRUE   1
#define FALSE  0

// Relay or signal states
#define ON     1
#define OFF    0

#endif
