/**
 * @file main.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#include "gui.h"
#include "time_domain.h"
#include "display_manager.h"




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************

//
static sig_atomic_t global_exit_signal;




// *****************************************************
// static declarations
// *****************************************************

//
static void sig_handler( int sig )
{
    if( sig == SIGINT )
    {
        global_exit_signal = 1;
    }
}




// *****************************************************
// static definitions
// *****************************************************




// *****************************************************
// public definitions
// *****************************************************

//
int main( int argc, char **argv )
{
    display_s display;
    gui_s *gui = NULL;

    global_exit_signal = 0;
    memset( &display, 0, sizeof(display) );

    // hook up the control-c signal handler, sets exit signaled flag
    signal( SIGINT, sig_handler );

    // allow signals to interrupt
    siginterrupt( SIGINT, 1 );

    // create GUI
    gui = calloc( 1, sizeof(*gui) );
    if( gui == NULL )
    {
        printf( "failed to allocate GUI\n" );
        global_exit_signal = 1;
    }

    // 800x400 or 640x480 ?
    if( gui != NULL )
    {
        dm_init(
                0,
                0,
                640,
                480,
                &display );

        // default clock configuration
        gui->clock.font = TEXT_FONT_SARIF_TYPE_FACE;
        gui->clock.font_point_size = 165;
        gui->clock.digit_color_rgb[0] = 28;
        gui->clock.digit_color_rgb[1] = 0;
        gui->clock.digit_color_rgb[2] = 223;
        gui->clock.digit_color_alpha = 1.0f;
    }

    // main loop
    while( global_exit_signal == 0 )
    {
        //
        dm_render_gui( &display, gui );
    }

    //
    dm_release( &display );

    if( gui != NULL )
    {
        free( gui );
        gui = NULL;
    }

    return 0;
}
