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
    // TODO

    // set background color
    Background( 0, 0, 0 );

    // TODO

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
    if( display != NULL )
    {
        render_display( display );
    }
}
