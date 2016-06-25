/**
 * @file calendar.c
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
#include "calendar.h"




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
void calendar_set_default_configuration( gui_calendar_s * const calendar )
{
    // default calendar configuration
    if( calendar != NULL )
    {
        calendar->font = TEXT_FONT_SARIF_TYPE_FACE;
        calendar->font_point_size = 30;
        calendar->digit_color_rgb[0] = 28;
        calendar->digit_color_rgb[1] = 0;
        calendar->digit_color_rgb[2] = 223;
        calendar->digit_color_alpha = 0.8f;

        // get text height
        calendar->font_height = (float) TextHeight(
                *((Fontinfo*) font_get( calendar->font )),
                (int) calendar->font_point_size );
    }
}


//
void calendar_render(
        gui_s * const gui,
        gui_calendar_s * const calendar )
{
    // get local time date
    const struct tm * const local_date = time_get_localtime( gui->utc_clock_time );

    // format digits
    if( local_date != NULL )
    {
        char * const ascii_date = asctime( local_date );

        strncpy(
                calendar->date_string,
                ascii_date,
                sizeof(calendar->date_string) );

        // remove '\n' character if one exist
        char * const nl = strchr( calendar->date_string, (int) '\n' );
        if( nl != NULL )
        {
            (*nl) = '\0';
        }
    }

    // select font
    Fontinfo * const font = (Fontinfo*) font_get( calendar->font );

    // stroke width
    StrokeWidth( 0.0f );

    // stroke color
    Stroke( 0, 0, 0, 1.0f );

    // text color
    Fill(
            calendar->digit_color_rgb[0],
            calendar->digit_color_rgb[1],
            calendar->digit_color_rgb[2],
            calendar->digit_color_alpha );

    // render date text
    TextMid(
            ((VGfloat) gui->display.win_width) / 2.0f,
            ((VGfloat) gui->display.win_height) - (gui->clock.font_height / 1.25f) - calendar->font_height,
            calendar->date_string,
            *font,
            (int) calendar->font_point_size );
}
