/**
 * @file clock.h
 * @brief TODO.
 *
 */




#ifndef CLOCK_H
#define	CLOCK_H




#include "gui.h"
#include "display_manager.h"




//
void clock_set_default_configuration( gui_clock_s * const clock );


//
void clock_render(
        gui_s * const gui,
        gui_clock_s * const clock );



#endif	/* CLOCK_H */
