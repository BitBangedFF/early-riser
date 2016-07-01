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
#include "clock.h"
#include "calendar.h"
#include "alarm.h"
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
    gui_s *gui = NULL;

    global_exit_signal = 0;

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

    //
    if( gui != NULL )
    {
        dm_init(
                0,
                0,
                640,
                480,
                &gui->display );

        // default background color
        gui->background_color_rgb[0] = 0;
        gui->background_color_rgb[1] = 0;
        gui->background_color_rgb[2] = 0;

        // default clock configuration
        clock_set_default_configuration( &gui->clock );

        // default calendar configuration
        calendar_set_default_configuration( &gui->calendar );

        // default alarm configuration
        alarm_set_default_configuration( &gui->alarms.config );

#warning "TESTING alarm"
        alarm_add(
                "test alarm 1",
                DAY_MONDAY_THROUGH_FRIDAY,
                13,
                25,
                &gui->alarms );

        alarm_add(
                "test alarm 2",
                DAY_MONDAY,
                8,
                22,
                &gui->alarms );
    }

    // main loop
    while( global_exit_signal == 0 )
    {
        // update current clock time
        gui->utc_clock_time = time_get_timestamp();

        //
        dm_update_gui( gui );
    }

    if( gui != NULL )
    {
        dm_release( &gui->display );

        alarm_release( &gui->alarms );

        free( gui );
        gui = NULL;
    }

    return 0;
}
