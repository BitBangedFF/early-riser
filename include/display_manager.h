/**
 * @file display_manager.h
 * @brief TODO.
 *
 */




#ifndef DISPLAY_MANAGER_H
#define DISPLAY_MANAGER_H




#include "gui.h"




//
typedef struct
{
    //
    //
    unsigned int win_width;
    //
    //
    unsigned int win_height;
    //
    //
    unsigned long long render_cnt;
} display_s;




//
void dm_init(
        const unsigned long x,
        const unsigned long y,
        const unsigned long width,
        const unsigned long height,
        display_s * const display );


//
void dm_release( display_s * const display );


//
void dm_render_gui(
        display_s * const display,
        gui_s * const gui );




#endif /* DISPLAY_MANAGER_H */
