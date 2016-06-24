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
#include "time_domain.h"
#include "clock.h"
#include "calendar.h"
#include "display_manager.h"




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
static void render_display( gui_s * const gui )
{
    // set background color
    Background(
            gui->background_color_rgb[0],
            gui->background_color_rgb[1],
            gui->background_color_rgb[2] );

#warning "TESTING - red outline of window space"
    // red outline of window space
    Stroke( 255, 0, 0, 1.0f );
    StrokeWidth( 1.0f );
    RectOutline( 5.0f, 5.0f, (VGfloat) gui->display.win_width - 5.0f, (VGfloat) gui->display.win_height - 5.0f );

    // render the digital clock
    clock_render( gui, &gui->clock );

    // render the calendar
    calendar_render( gui, &gui->calendar );

    // render to buffer/screen
    End();

    // update render counter after rendering
    gui->display.render_cnt += 1;
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
        gui_display_s * const display )
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
void dm_release( gui_display_s * const display )
{
    if( display != NULL )
    {
        // release OpenVG
        finish();
    }
}


//
void *dm_get_font( const gui_text_font_kind font_kind )
{
    return (void*) get_font( font_kind );
}


//
void dm_update_gui( gui_s * const gui )
{
    if( gui != NULL )
    {
        // render GUI elements to the screen
        render_display( gui );

#warning "TESTING - sleeping 1 ms"
        time_sleep_ms( 1 );
    }
}
