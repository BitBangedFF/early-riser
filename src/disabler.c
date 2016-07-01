/**
 * @file disabler.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <VG/openvg.h>
#include <VG/vgu.h>
#include <fontinfo.h>
#include <shapes.h>

#include "gui.h"
#include "time_domain.h"
#include "font.h"
#include "disabler.h"




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

//
static float get_window_x( const gui_display_s * const display, const float x )
{
    const float dw = (float) display->win_width / 2.0f;

    const float winx = dw + x;

    return winx;
}

//
static float get_window_y( const gui_display_s * const display, const float y )
{
    const float dh = (float) display->win_height / 2.0f;

    const float winy = dh + y;

    return winy;
}


//
static void update_position( gui_disabler_s * const disabler )
{
    const timestamp_ms now = time_get_monotonic_timestamp();

    const timestamp_ms delta = (now - disabler->last_update);

    const float dt = (float) delta / 1000.0f;

    disabler->position[0] += (disabler->velocity[0] * dt);
    disabler->position[1] += (disabler->velocity[1] * dt);

    // check for change in x direction

    // check for change in y direction

    disabler->last_update = now;
}




// *****************************************************
// public definitions
// *****************************************************

//
void disabler_set_default_configuration( gui_disabler_s * const disabler )
{
    // default disabler configuration
    if( disabler != NULL )
    {
        disabler->enabled = FALSE;
        disabler->enabled_timestamp = 0;
        disabler->last_update = 0;

        disabler->font = TEXT_FONT_SARIF_TYPE_FACE;
        disabler->font_point_size = 30;
        disabler->digit_color[0] = 100.0f;
        disabler->digit_color[1] = 100.0f;
        disabler->digit_color[2] = 100.0f;
        disabler->digit_color[3] = 1.0f;

        disabler->color[0] = 255.0f;
        disabler->color[1] = 0.0f;
        disabler->color[2] = 0.0f;
        disabler->color[3] = 1.0f;

        // get text height
        disabler->font_height = (float) TextHeight(
                *((Fontinfo*) font_get( disabler->font )),
                (int) disabler->font_point_size );

        disabler->radius = 1.5f * disabler->font_height;

        disabler->position[0] = 0.0f;
        disabler->position[1] = 0.0f;

        disabler->velocity[0] = DISABLER_DEFAULT_VELOCITY;
        disabler->velocity[1] = DISABLER_DEFAULT_VELOCITY;
    }
}


//
void disabler_start( gui_disabler_s * const disabler )
{
    if( disabler != NULL )
    {
        disabler_set_default_configuration( disabler );

        disabler->enabled = TRUE;

        disabler->enabled_timestamp = time_get_timestamp();

        disabler->last_update = time_get_monotonic_timestamp();
    }
}


//
void disabler_stop( gui_disabler_s * const disabler )
{
    if( disabler != NULL )
    {
        disabler->enabled = FALSE;
    }
}


//
void disabler_render(
        const gui_s * const gui,
        gui_disabler_s * const disabler )
{
    if( disabler->enabled == TRUE )
    {
        // select font
//        Fontinfo * const font = (Fontinfo*) font_get( disabler->font );

        // stroke width
        StrokeWidth( 0.0f );

        // stroke color
        Stroke( 0, 0, 0, 1.0f );

        // circle color
        Fill(
                (unsigned int) disabler->color[0],
                (unsigned int) disabler->color[1],
                (unsigned int) disabler->color[2],
                disabler->color[3] );

        // render circle
        Circle(
                get_window_x( &gui->display, disabler->position[0] ),
                get_window_y( &gui->display, disabler->position[1] ),
                disabler->radius );
    }
}


//
void disabler_update(
        gui_s * const gui,
        gui_disabler_s * const disabler )
{
    if( disabler->enabled == TRUE )
    {
        update_position( disabler );
    }
}
