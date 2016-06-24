/**
 * @file display_manager.c
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
#include "display_manager.h"
#include "time_domain.h"




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
static Fontinfo *get_font( const gui_text_font_kind font_kind )
{
    Fontinfo *font = NULL;

    if( font_kind == TEXT_FONT_SARIF_TYPE_FACE )
    {
        font = &SerifTypeface;
    }
    else
    {
        font = &SerifTypeface;
    }

    return font;
}

//
static void render_clock(
        display_s * const display,
        gui_s * const gui )
{
    // get local time date
    const struct tm * const local_date = time_get_localtime( gui->clock.utc_time );

    // format digits
    if( local_date != NULL )
    {
        // convert 24 hour clock with 12 (AM/PM is implied)
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
                gui->clock.time_string,
                sizeof(gui->clock.time_string),
                "%02d:%02d",
                am_pm_hour,
                local_date->tm_min );
    }

    // select font
    Fontinfo * const font = get_font( gui->clock.font );

    // get text height
    const VGfloat text_height = TextHeight( *font, (int) gui->clock.font_point_size );

    // stroke width
    StrokeWidth( 0.0f );

    // stroke color
    Stroke( 0, 0, 0, 1.0f );

    // digit color
    Fill(
            gui->clock.digit_color_rgb[0],
            gui->clock.digit_color_rgb[1],
            gui->clock.digit_color_rgb[2],
            gui->clock.digit_color_alpha );

    // render text
    TextMid(
            ((VGfloat) display->win_width) / 2.0f,
            ((VGfloat) display->win_height) - (text_height / 1.3f),
            gui->clock.time_string,
            *font,
            (int) gui->clock.font_point_size );
}


//
static void render_display(
        display_s * const display,
        gui_s * const gui )
{
    // set background color
    Background( 0, 0, 0 );

#warning "TESTING - red outline of window space"
    // red outline of window space
    Stroke( 255, 0, 0, 1.0f );
    StrokeWidth( 1.0f );
    RectOutline( 5.0f, 5.0f, (VGfloat) display->win_width - 5.0f, (VGfloat) display->win_height - 5.0f );

    // render the digital clock
    render_clock( display, gui );

    // render to buffer/screen
    End();

    // update render counter after rendering
    display->render_cnt += 1;
}




// *****************************************************
// public definitions
// *****************************************************

//
void dm_init(
        const unsigned long x,
        const unsigned long y,
        const unsigned long width,
        const unsigned long height,
        display_s * const display )
{
    int screen_width = 0;
    int screen_height = 0;

    display->render_cnt = 0;

    display->win_width = width;
    display->win_height = height;

    // gets used in OpenVG.init()
    initWindowSize(
            (int) x,
            (int) y,
            (unsigned int) width,
            (unsigned int) height );

    //
    init( &screen_width, &screen_height );

    //
    Start( (int) width, (int) height );
}


//
void dm_release( display_s * const display )
{
    if( display != NULL )
    {
        // release OpenVG
        finish();
    }
}


//
void dm_render_gui(
        display_s * const display,
        gui_s * const gui )
{
    if( (display != NULL) && (gui != NULL) )
    {
        // TESTING
        gui->clock.utc_time = time_get_timestamp();

        // render GUI elements to the screen
        render_display( display, gui );
    }
}
