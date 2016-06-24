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
static void redraw( display_s * const display )
{
    //
    Start( (int) display->win_width, (int) display->win_height );

    //
    Background( 0, 0, 0 );

    //
    Fill( 28, 0, 223, 1.0f );

    //
    Text( 30.0f, 300.0f, "05:58", SerifTypeface, 150.0f );

    TextMid( (VGfloat) display->win_width / 2.0f, 420.0f, "PM", SerifTypeface, 20.0f );

    TextMid( (VGfloat) display->win_width / 2.0f, 250.0f, "Thursday - 23 June 2016", SerifTypeface, 30.0f );

    Text( 20.0f, 175.0f, "Alarm", SerifTypeface, 15.0f );

    Text( 25.0f, 150.0f, "+ Friday 05:05 AM", SerifTypeface, 15.0f );


    //
//    End();
    SaveEnd( "/tmp/early-riser.raw" );
}




// *****************************************************
// public definitions
// *****************************************************

//
void dm_test( display_s * const display )
{
    int screen_width = 0;
    int screen_height = 0;

    const unsigned int win_width = 640;
    const unsigned int win_height = 480;



    display->win_width = win_width;
    display->win_height = win_height;


    //
    initWindowSize( 0, 0, win_width, win_height );

    //
    init( &screen_width, &screen_height );


    redraw( display );





    //
//    Start( win_width, win_height );
//
//    //
//    Background( 70, 70, 70 );
//
//    //
//    Stroke( 255, 0, 0, 1.0f );
//
//    //
//    StrokeWidth( 2.0f );
//
//    //
//    RoundrectOutline( 2.0f, 2.0f, (VGfloat) win_width - 2.0f, (VGfloat) win_height - 2.0f, 2.0f, 2.0f );
//
//    //
//    Stroke( 0, 0, 0, 1.0f );
//
//    //
//    StrokeWidth( 1.0f );
//
//    //
//    Fill( 0, 255, 0, 1.0f );
//
//    //
//    Circle( win_width / 2.0f, 0.0f, win_width );
//

//    End();






    //
    (void) sleep( 20 );

    //
    finish();
}
