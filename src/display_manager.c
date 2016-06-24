/**
 * @file display_manager.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

//
#include <VG/openvg.h>
#include <VG/vgu.h>
#include <fontinfo.h>
#include <shapes.h>

//
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
static void render_display( display_s * const display )
{


    // set background color
    Background( 0, 0, 0 );

    //
    Fill( 28, 0, 223, 1.0f );

    //
    Text( 30.0f, 300.0f, "05:58", SerifTypeface, 150.0f );

    TextMid( (VGfloat) display->win_width / 2.0f, 420.0f, "PM", SerifTypeface, 20.0f );

    TextMid( (VGfloat) display->win_width / 2.0f, 250.0f, "Thursday - 23 June 2016", SerifTypeface, 30.0f );

    Text( 20.0f, 175.0f, "Alarm", SerifTypeface, 15.0f );

    Text( 25.0f, 150.0f, "+ Friday 05:05 AM", SerifTypeface, 15.0f );

    // render to buffer/screen
    End();

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
        //
        finish();
    }
}


//
void dm_update( display_s * const display )
{
    if( display != NULL )
    {
        render_display( display );
    }
}
