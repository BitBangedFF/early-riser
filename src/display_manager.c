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
void dm_test( void )
{
    int screen_width = 0;
    int screen_height = 0;

    const unsigned int win_width = 600;
    const unsigned int win_height = 400;


    //
    initWindowSize( 200, 200, win_width, win_height );

    //
    init( &screen_width, &screen_height );

    //
    Start( win_width, win_height );

    //
    Background( 70, 70, 70 );

    //
    Stroke( 255, 0, 0, 1.0f );

    //
    StrokeWidth( 2.0f );

    //
    RoundrectOutline( 2.0f, 2.0f, (VGfloat) win_width - 2.0f, (VGfloat) win_height - 2.0f, 2.0f, 2.0f );

    //
    Stroke( 0, 0, 0, 1.0f );

    //
    StrokeWidth( 1.0f );

    //
    Fill( 0, 255, 0, 1.0f );

    //
    Circle( win_width / 2.0f, 0.0f, win_width );

    //
    End();

    //
    (void) sleep( 3 );

    //
    finish();
}
