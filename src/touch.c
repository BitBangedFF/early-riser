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
#include <stdatomic.h>

#include "gui.h"
#include "touch.h"

#ifdef __STDC_NO_ATOMICS__
#error "requires atomic"
#endif




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************

//
static const char INPUT_DEVICE[] = "/dev/input/event0";


//
static volatile atomic_ulong global_touch_x = ATOMIC_VAR_INIT(0);


//
static volatile atomic_ulong global_touch_y = ATOMIC_VAR_INIT(0);


//
static volatile atomic_bool global_touch_pressed = ATOMIC_VAR_INIT(0);




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

//
static float map_f(
        const float x,
        const float in_min,
        const float in_max,
        const float out_min,
        const float out_max )
{
    return ((x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min);
}


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
                        atomic_store( &global_touch_x, (unsigned long) event_data.value );
                    }
                    else if( event_data.code == ABS_Y )
                    {
                        atomic_store( &global_touch_y, (unsigned long) event_data.value );
                    }
                }
                else if( event_data.type == EV_KEY )
                {
                    if( event_data.code == BTN_TOUCH )
                    {
                        if( event_data.value == 1 )
                        {
                            atomic_store( &global_touch_pressed, 1 );
                        }
                        else
                        {
                            atomic_store( &global_touch_pressed, 0 );
                        }
                    }
                }
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
        const unsigned long min_x,
        const unsigned long max_x,
        const unsigned long min_y,
        const unsigned long max_y,
        const unsigned long width,
        const unsigned long height,
        gui_touch_s * const touch )
{
    touch->min_x = min_x;
    touch->max_x = max_x;
    touch->min_y = min_y;
    touch->max_y = max_y;
    touch->width = width;
    touch->height = height;

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


//
void touch_set_pressed_state(
        const bool state,
        gui_touch_s * const touch )
{
    atomic_store( &global_touch_pressed, state );
}


//
bool touch_get_position(
        const gui_touch_s * const touch,
        float * const x,
        float * const y )
{
    const unsigned long t_x = (unsigned long) atomic_load( &global_touch_x );
    const unsigned long t_y = (unsigned long) atomic_load( &global_touch_y );
    const bool is_pressed = (bool) atomic_load( &global_touch_pressed );

    if( is_pressed == TRUE )
    {
        if( x != NULL )
        {
            (*x) = map_f(
                    (float) CONSTRAIN(t_x, touch->min_x, touch->max_x),
                    (float) touch->min_x,
                    (float) touch->max_x,
                    (float) 0.0f,
                    (float) touch->width );
        }

        if( y != NULL )
        {
            (*y) = map_f(
                    (float) CONSTRAIN(t_y, touch->min_y, touch->max_y),
                    (float) touch->min_y,
                    (float) touch->max_y,
                    (float) 0.0f,
                    (float) touch->height );
        }
    }

    return is_pressed;
}
