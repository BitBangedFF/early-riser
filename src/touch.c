/**
 * @file touch.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <linux/input.h>
#include <fcntl.h>

#include "gui.h"
#include "touch.h"




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************

//
static const char INPUT_DEVICE[] = "/dev/input/event0";




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

//
static void *event_thread_function( void * const user_data )
{
    gui_touch_s * const touch = (gui_touch_s*) user_data;

    if( touch != NULL )
    {
        int input_fd = open( INPUT_DEVICE, O_RDONLY );

        while( input_fd >= 0 )
        {
            struct input_event event_data;

            const int ret = (int) read( input_fd, &event_data, sizeof(event_data) );

            if( ret > 0 )
            {
                if( event_data.type == EV_ABS )
                {
                    if( event_data.code == ABS_X )
                    {
                        printf( "x %d\n", event_data.value );
                    }
                }
                else if( event_data.type == EV_KEY )
                {
//                    printf( "key\n" );
                }




//                printf( "time %ld.%06ld\ttype %d\tcode %d\tvalue %d\n",
//                        event_data.time.tv_sec, event_data.time.tv_usec, event_data.type, event_data.code, event_data.value );
            }
        }

        if( input_fd >= 0 )
        {
            (void) close( input_fd );
        }
    }

    return NULL;
}




// *****************************************************
// public definitions
// *****************************************************

//
void touch_init(
        const unsigned long max_x,
        const unsigned long max_y,
        gui_touch_s * const touch )
{
#warning "testing touch event handler"


    const int test_fd = open( INPUT_DEVICE, O_RDONLY );
    if( test_fd >= 0 )
    {
        (void) close( test_fd );

        const int status = pthread_create(
                &touch->event_thread,
                NULL,
                &event_thread_function,
                (void*) touch );
        if( status == 0 )
        {
            touch->enabled = TRUE;
        }
        else
        {
            printf( "failed to create event thread\n" );
            touch->enabled = FALSE;
        }
    }
    else
    {
        printf( "failed to open '%s'\n", INPUT_DEVICE );
        touch->enabled = FALSE;
    }
}
