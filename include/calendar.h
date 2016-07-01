/**
 * @file calendar.h
 * @brief TODO.
 *
 */




#ifndef CALENDAR_H
#define	CALENDAR_H




#include "gui.h"




//
void calendar_set_default_configuration( gui_calendar_s * const calendar );


//
void calendar_render(
        const gui_s * const gui,
        gui_calendar_s * const calendar );



#endif	/* CALENDAR_H */
