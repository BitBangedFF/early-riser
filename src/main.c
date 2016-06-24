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
    gui_s gui;

    global_exit_signal = 0;
    memset( &display, 0, sizeof(display) );
    memset( &gui, 0, sizeof(gui) );

    // hook up the control-c signal handler, sets exit signaled flag
    signal( SIGINT, sig_handler );

    // allow signals to interrupt
    siginterrupt( SIGINT, 1 );

    //
    dm_init(
            0,
            0,
            800,
            400,
            &display );

    // main loop
    while( global_exit_signal == 0 )
    {
        //
        dm_render_gui( &display, &gui );
    }

    //
    dm_release( &display );

    return 0;
}
