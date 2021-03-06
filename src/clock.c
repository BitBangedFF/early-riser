/**
 * @file clock.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <VG/openvg.h>
#include <VG/vgu.h>
#include <fontinfo.h>
#include <shapes.h>

#include "gui.h"
#include "time_domain.h"
#include "font.h"
#include "clock.h"




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




// *****************************************************
// public definitions
// *****************************************************

//
void clock_set_default_configuration( gui_clock_s * const clock )
{
    // default clock configuration
    if( clock != NULL )
    {
        clock->font = TEXT_FONT_SARIF_TYPE_FACE;
        clock->font_point_size = 200;
        clock->digit_color[0] = 28.0f;
        clock->digit_color[1] = 0.0f;
        clock->digit_color[2] = 223.0f;
        clock->digit_color[3] = 1.0f;

        // get text height
        clock->font_height = (float) TextHeight(
                *((Fontinfo*) font_get( clock->font )),
                (int) clock->font_point_size );
    }
}


//
void clock_render(
        const gui_s * const gui,
        gui_clock_s * const clock )
{
    // get local time date
    const struct tm * const local_date = time_get_localtime( gui->utc_clock_time );

    // format digits
    if( local_date != NULL )
    {
        // convert 24 hour clock to 12 (AM/PM is implied)
        const unsigned long ampm_hour = time_get_ampm_hour(
                (unsigned long) local_date->tm_hour,
                NULL );

        snprintf(
                clock->time_string,
                sizeof(clock->time_string),
                "%02lu:%02d",
                ampm_hour,
                local_date->tm_min );
    }

    // select font
    Fontinfo * const font = (Fontinfo*) font_get( clock->font );

    // stroke width
    StrokeWidth( 0.0f );

    // stroke color
    Stroke( 0, 0, 0, 1.0f );

    // digit color
    Fill(
            (unsigned int) clock->digit_color[0],
            (unsigned int) clock->digit_color[1],
            (unsigned int) clock->digit_color[2],
            clock->digit_color[3] );

    // render hour/minute text
    TextMid(
            ((VGfloat) gui->display.win_width) / 2.0f,
            ((VGfloat) gui->display.win_height) - (clock->font_height / 1.3f),
            clock->time_string,
            *font,
            (int) clock->font_point_size );
}
