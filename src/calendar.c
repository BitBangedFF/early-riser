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
#include "display_manager.h"
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
    calendar->font = TEXT_FONT_SARIF_TYPE_FACE;
    calendar->font_point_size = 30;
    calendar->digit_color_rgb[0] = 28;
    calendar->digit_color_rgb[1] = 0;
    calendar->digit_color_rgb[2] = 223;
    calendar->digit_color_alpha = 1.0f;
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
        snprintf(
                calendar->date_string,
                sizeof(calendar->date_string),
                "Thursday - %02d:%02d",
                local_date->tm_min,
                local_date->tm_sec );
    }

    // select font
    Fontinfo * const font = (Fontinfo*) dm_get_font( calendar->font );

    // get text height
    const VGfloat text_height = TextHeight( *font, (int) calendar->font_point_size );

    // get text height of clock
    const VGfloat clock_text_height = TextHeight(
            *((Fontinfo*) dm_get_font( gui->clock.font )),
            (int) gui->clock.font_point_size );

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
            ((VGfloat) gui->display.win_height) - (clock_text_height / 1.3f) - text_height,
            calendar->date_string,
            *font,
            (int) calendar->font_point_size );
}
