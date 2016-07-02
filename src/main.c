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
#include <time.h>

#include "gui.h"
#include "time_domain.h"
#include "clock.h"
#include "calendar.h"
#include "alarm.h"
#include "disabler.h"
#include "touch.h"
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

    // seed random numbers
    srand( time(NULL) );

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
                800,
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

        // default disabler configuration
        disabler_set_default_configuration(
                gui->display.win_width,
                gui->display.win_height,
                &gui->disabler );

        // initialize the touch handler
        touch_init(
                700,
                32000,
                1300,
                32000,
                gui->display.win_width,
                gui->display.win_height,
                &gui->touch );

#warning "TESTING example alarms"
        alarm_add(
                "test alarm 1",
                DAY_MONDAY_THROUGH_FRIDAY,
                13,
                30,
                &gui->alarms );

        alarm_add(
                "test alarm 2",
                DAY_SATURDAY,
                time_get_hour(),
                time_get_minute() + 1,
                &gui->alarms );

        alarm_add(
                "test alarm 3",
                time_get_week_day(),
                time_get_hour(),
                time_get_minute() + 2,
                &gui->alarms );
    }

    // main loop
    while( global_exit_signal == 0 )
    {
        // update current UTC clock time
        gui->utc_clock_time = time_get_timestamp();

        // start the disabler if not already enabled and alarm(s) are enabled
        if( gui->disabler.enabled == FALSE )
        {
            const bool alarms_enabled = alarm_are_any_enabled( &gui->alarms );

            if( alarms_enabled == TRUE )
            {
                disabler_start( &gui->disabler );
            }
        }

        // check for a disable touch events
        if( gui->disabler.enabled == TRUE )
        {
            float touch_x = 0.0f;
            float touch_y = 0.0f;

            const bool touch_pressed = touch_get_position(
                    &gui->touch,
                    &touch_x,
                    &touch_y );

            if( touch_pressed == TRUE )
            {
                // check if event is within the disabler object
                const bool is_contained = disabler_is_contained(
                        &gui->disabler,
                        touch_x,
                        touch_y );

                if( is_contained == TRUE )
                {
                    const bool should_disable = disabler_stop( &gui->disabler );

                    if( should_disable == TRUE )
                    {
                        alarm_disable_all( &gui->alarms );
                    }
                }

                touch_set_pressed_state( FALSE, &gui->touch );
            }
        }

        // update state of the alarms
        alarm_update( gui, &gui->alarms );

        // update state of the disabler
        disabler_update( gui, &gui->disabler );

        // update display manager
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
