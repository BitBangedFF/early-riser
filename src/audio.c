/**
 * @file audio.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <stdatomic.h>
#include <ao/ao.h>
#include <mpg123.h>

#include "gui.h"
#include "audio.h"

#ifdef __STDC_NO_ATOMICS__
#error "requires atomic"
#endif




// *****************************************************
// static global types/macros
// *****************************************************

//
#define STATE_CHANGE_WAIT_DELAY (5)




// *****************************************************
// static global data
// *****************************************************

//
static volatile atomic_bool global_is_enabled = ATOMIC_VAR_INIT(0);




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

//
static bool is_file_readable( const char * const path )
{
    if( path == NULL )
    {
        return FALSE;
    }

    if( access( path, R_OK ) != 0 )
    {
        return FALSE;
    }

    return TRUE;
}




// *****************************************************
// public definitions
// *****************************************************

//
void audio_init(
        const char * const file_path,
        gui_audio_s * const audio )
{
    ao_initialize();
    mpg123_init();

    if( audio != NULL )
    {
        atomic_store( &global_is_enabled, FALSE );

        audio->enabled = FALSE;

        const bool read_ok = is_file_readable( file_path );
        if( read_ok == FALSE )
        {
            printf( "failed to read file '%s'\n", file_path );
        }
        else
        {
            strncpy( audio->file_path, file_path, sizeof(audio->file_path) );
        }
    }
}


//
void audio_release( gui_audio_s * const audio )
{
    audio_disable( audio );

    mpg123_exit();
    ao_shutdown();
}


//
void audio_enable( gui_audio_s * const audio )
{
    if( audio != NULL )
    {
        audio->enabled = FALSE;
    }
}


//
void audio_disable( gui_audio_s * const audio )
{
    if( audio != NULL )
    {
        audio->enabled = FALSE;

        atomic_store( &global_is_enabled, FALSE );

        time_sleep_ms( STATE_CHANGE_WAIT_DELAY );
    }
}
