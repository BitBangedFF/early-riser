/**
 * @file disabler.h
 * @brief TODO.
 *
 */




#ifndef DISABLER_H
#define DISABLER_H




#include "gui.h"




// pixels/second
#define DISABLER_DEFAULT_VELOCITY (200.0f)


// number of counts/button-presses before disabling the alarms
#define DISABLER_DEFAULT_STOP_COUNT (5)


// ms
#define DISABLER_DEFAULT_INTER_STOP_DELAY (1000)




//
void disabler_set_default_configuration(
        const unsigned long win_width,
        const unsigned long win_height,
        gui_disabler_s * const disabler );


//
void disabler_start( gui_disabler_s * const disabler );


//
bool disabler_stop( gui_disabler_s * const disabler );


//
void disabler_render(
        const gui_s * const gui,
        gui_disabler_s * const disabler );


//
void disabler_update(
        gui_s * const gui,
        gui_disabler_s * const disabler );




#endif /* DISABLER_H */
