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
        clock->font_point_size = 165;
        clock->digit_color_rgb[0] = 28;
        clock->digit_color_rgb[1] = 0;
        clock->digit_color_rgb[2] = 223;
        clock->digit_color_alpha = 1.0f;

        // get text height
        clock->font_height = (float) TextHeight(
                *((Fontinfo*) font_get( clock->font )),
                (int) clock->font_point_size );
    }
}


//
void clock_render(
        gui_s * const gui,
        gui_clock_s * const clock )
{
    // get local time date
    const struct tm * const local_date = time_get_localtime( gui->utc_clock_time );

    // format digits
    if( local_date != NULL )
    {
        // convert 24 hour clock to 12 (AM/PM is implied)
        int am_pm_hour = local_date->tm_hour;

        // 12:00 AM
        if( am_pm_hour == 0 )
        {
            am_pm_hour = 12;
        }
        else if( am_pm_hour > 12 )
        {
            // 1:00 PM
            am_pm_hour -= 12;
        }

        snprintf(
                clock->time_string,
                sizeof(clock->time_string),
                "%02d:%02d",
                am_pm_hour,
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
            clock->digit_color_rgb[0],
            clock->digit_color_rgb[1],
            clock->digit_color_rgb[2],
            clock->digit_color_alpha );

    // render hour/minute text
    TextMid(
            ((VGfloat) gui->display.win_width) / 2.0f,
            ((VGfloat) gui->display.win_height) - (clock->font_height / 1.3f),
            clock->time_string,
            *font,
            (int) clock->font_point_size );
}
