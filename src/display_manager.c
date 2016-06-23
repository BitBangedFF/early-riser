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
    int win_width = 0;
    int win_height = 0;


    //
    init( &win_width, &win_height );

    //
    Start( win_width, win_height );

    //
    Background( 0x00, 0x00, 0xFF );

    //
    Fill( 0x00, 0xFF, 0x00, 0.5f );

    //
    Circle( win_width / 2, 0, win_width );

    //
    End();

    (void) sleep( 3 );

    //
    finish();
}
