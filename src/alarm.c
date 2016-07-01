/**
 * @file alarm.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

#include <VG/openvg.h>
#include <VG/vgu.h>
#include <fontinfo.h>
#include <shapes.h>

#include "gui.h"
#include "time_domain.h"
#include "font.h"
#include "alarm.h"




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************

//
static const char *WEEK_DAY_TABLE[] =
{
    "Sun",
    "Mon",
    "Tue",
    "Wed",
    "Thur",
    "Fri",
    "Sat",
    "Mon-Fri",
    "Sat-Sun",
    NULL
};




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

//
static void resize_sequence(
        gui_alarm_sequence_s * const seq,
        const unsigned long length )
{
    if( length > seq->maximum )
    {
        seq->buffer = realloc( seq->buffer, (length * sizeof(*seq->buffer)) );
        if( seq->buffer == NULL )
        {
            printf( "failed to allocate sequence\n" );
            exit( 1 );
        }

        seq->maximum = length;
    }

    seq->length = length;
}


//
static void render(
        const gui_s * const gui,
        const gui_alarm_config_s * const config,
        gui_alarm_s * const alarm,
        const float ypos )
{

    bool is_pm = FALSE;

    const unsigned long ampm_hour = time_get_ampm_hour(
            alarm->hour,
            &is_pm );

    // hr:min AM/PM
    snprintf(
            alarm->time_string,
            sizeof(alarm->time_string),
            "%02lu:%02lu %s",
            ampm_hour,
            alarm->minute,
            (is_pm == TRUE) ? "PM" : "AM" );

    // add alarm name
    strncpy(
            alarm->display_string,
            alarm->name,
            sizeof(alarm->display_string) );

    // add ' - ' token
    strncat(
            alarm->display_string,
            " - ",
            sizeof(alarm->display_string) );

    // add week day
    strncat(
            alarm->display_string,
            WEEK_DAY_TABLE[ alarm->week_day ],
            sizeof(alarm->display_string) );

    // add ' ' token
    strncat(
            alarm->display_string,
            " ",
            sizeof(alarm->display_string) );

    // add time in 12 hour (am/pm) format
    strncat(
            alarm->display_string,
            alarm->time_string,
            sizeof(alarm->display_string) );

    // select font
    Fontinfo * const font = (Fontinfo*) font_get( config->font );

    // stroke width
    StrokeWidth( 0.0f );

    // stroke color
    Stroke( 0, 0, 0, 1.0f );

    // text color
    Fill(
            config->digit_color_rgb[0],
            config->digit_color_rgb[1],
            config->digit_color_rgb[2],
            config->digit_color_alpha );

    // render text
    Text(
            20.0f,
            ypos,
            alarm->display_string,
            *font,
            (int) config->font_point_size );
}




// *****************************************************
// public definitions
// *****************************************************

//
void alarm_set_default_configuration( gui_alarm_config_s * const config )
{
    // default alarm configuration
    if( config != NULL )
    {
        config->font = TEXT_FONT_SARIF_TYPE_FACE;
        config->font_point_size = 20;
        config->digit_color_rgb[0] = 100;
        config->digit_color_rgb[1] = 100;
        config->digit_color_rgb[2] = 100;
        config->digit_color_alpha = 0.5f;

        // get text height
        config->font_height = (float) TextHeight(
                *((Fontinfo*) font_get( config->font )),
                (int) config->font_point_size );
    }
}


//
void alarm_release( gui_alarm_sequence_s * const alarms )
{
    if( alarms != NULL )
    {
        if( alarms->buffer != NULL )
        {
            free( alarms->buffer );
            alarms->buffer = NULL;
        }
        alarms->maximum = 0;
        alarms->length = 0;
    }
}


//
void alarm_add(
        const char * const name,
        const unsigned long week_day,
        const unsigned long hour,
        const unsigned long minute,
        gui_alarm_sequence_s * const alarms )
{
    resize_sequence( alarms, alarms->length + 1 );

    gui_alarm_s * const alarm = &alarms->buffer[ alarms->length - 1 ];

    memset( alarm, 0, sizeof(*alarm) );

    strncpy( alarm->name, name, sizeof(alarm->name) );
    alarm->week_day = week_day;
    alarm->hour = hour;
    alarm->minute = minute;
}


//
void alarm_render(
        gui_s * const gui,
        gui_alarm_sequence_s * const alarms )
{
    unsigned long idx = 0;
    float dy = 0.0f;

    const float base_y = ((float) gui->display.win_height) - (gui->clock.font_height / 1.25f) - gui->calendar.font_height - 60.0f;

    for( idx = 0; idx < alarms->length; idx += 1 )
    {
        gui_alarm_s * const alarm = &alarms->buffer[ idx ];

        float ypos = 0.0f;

        if( base_y > dy )
        {
            ypos = base_y - dy;
        }

        render( gui, &alarms->config, alarm, ypos );

        dy += 2.0f * alarms->config.font_height;
    }
}
