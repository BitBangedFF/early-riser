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
#include "time_domain.h"
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


//
static volatile atomic_bool global_thread_running = ATOMIC_VAR_INIT(0);




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


//
static void *audio_thread_function( void * const user_data )
{
    ao_device *device = NULL;
    mpg123_handle *decoder = NULL;
    unsigned char *buffer = NULL;
    size_t buffer_size = 0;
    size_t bytes_read = 0;
    int channels = 0;
    int encoding = 0;
    long rate = 0;
    int ao_driver = 0;
    int decoder_err = 0;
    ao_sample_format format;
    char  file_path[1024];
    const gui_audio_s * const audio = (gui_audio_s*) user_data;

    atomic_store( &global_thread_running, TRUE );

    memset( &format, 0, sizeof(format) );

    strncpy( file_path, audio->file_path, sizeof(file_path) );

    if( audio != NULL )
    {
        decoder = mpg123_new( NULL, &decoder_err );
        if( decoder == NULL )
        {
            atomic_store( &global_is_enabled, FALSE );
        }
        else
        {
            buffer_size = mpg123_outblock( decoder );
            buffer = malloc( buffer_size * sizeof(*buffer) );

            // default driver
            ao_driver = ao_default_driver_id();

            // open the file and get the decoding format
            (void) mpg123_open( decoder, file_path );
            (void) mpg123_getformat( decoder, &rate, &channels, &encoding );

            // set the output format and open the output device
            format.bits = mpg123_encsize(encoding) * 8;
            format.rate = rate;
            format.channels = channels;
            format.byte_format = AO_FMT_NATIVE;
            format.matrix = 0;
            device = ao_open_live( ao_driver, &format, NULL );
        }

        bool is_enabled = (bool) atomic_load( &global_is_enabled );

        while( is_enabled == TRUE )
        {
            bytes_read = 0;

            const int read_stat = mpg123_read(
                    decoder,
                    buffer,
                    buffer_size,
                    &bytes_read );

            if( (read_stat == MPG123_OK) || (read_stat == MPG123_DONE) )
            {
                if( bytes_read > 0 )
                {
                    (void) ao_play(
                            device,
                            (char*) buffer,
                            bytes_read );
                }
            }

            // reset/loop on end of stream
            if( (read_stat == MPG123_DONE) && (bytes_read > 0) )
            {
                (void) mpg123_close( decoder );
                time_sleep_ms( STATE_CHANGE_WAIT_DELAY );
                (void) mpg123_open( decoder, file_path );
            }

            is_enabled = (bool) atomic_load( &global_is_enabled );
        }

        if( device != NULL )
        {
            (void) ao_close( device );
            device = NULL;
        }

        if( decoder != NULL )
        {
            (void) mpg123_close( decoder );
            (void) mpg123_delete( decoder );
            decoder = NULL;
        }

        if( buffer != NULL )
        {
            free( buffer );
            buffer = NULL;
        }
    }

    atomic_store( &global_thread_running, FALSE );

    return NULL;
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
        if( audio->enabled == FALSE )
        {
            atomic_store( &global_is_enabled, TRUE );

            const int status = pthread_create(
                    &audio->thread,
                    NULL,
                    &audio_thread_function,
                    (void*) audio );
            if( status == 0 )
            {
                audio->enabled = TRUE;
            }
            else
            {
                atomic_store( &global_is_enabled, FALSE );
                audio->enabled = FALSE;
                printf( "failed to create audio thread\n" );
            }
        }
    }
}


//
void audio_disable( gui_audio_s * const audio )
{
    if( audio != NULL )
    {
        if( audio->enabled == TRUE )
        {
            audio->enabled = FALSE;

            atomic_store( &global_is_enabled, FALSE );

            while( ((bool) atomic_load( &global_thread_running )) == TRUE )
            {
                time_sleep_ms( STATE_CHANGE_WAIT_DELAY );
            }
        }
    }
}
