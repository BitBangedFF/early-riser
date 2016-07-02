/**
 * @file touch.h
 * @brief TODO.
 *
 */




#ifndef TOUCH_H
#define	TOUCH_H




#include "gui.h"




//
void touch_init(
        const unsigned long min_x,
        const unsigned long max_x,
        const unsigned long min_y,
        const unsigned long max_y,
        const unsigned long width,
        const unsigned long height,
        gui_touch_s * const touch );


//
void touch_set_pressed_state(
        const bool state,
        gui_touch_s * const touch );


//
bool touch_get_position(
        const gui_touch_s * const touch,
        float * const x,
        float * const y );




#endif	/* TOUCH_H */
