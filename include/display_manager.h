/**
 * @file display_manager.h
 * @brief TODO.
 *
 */




#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H




#include "gui.h"




//
void dm_init(
        const unsigned long x,
        const unsigned long y,
        const unsigned long width,
        const unsigned long height,
        gui_display_s * const display );


//
void dm_release( gui_display_s * const display );


//
void dm_update_gui( gui_s * const gui );




#endif /* DISPLAY_MANAGER_H */
