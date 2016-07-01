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
#include <math.h>

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
static float get_rolling_alpha( const timestamp_ms start_time )
{
    float alpha = 1.0f;
    const timestamp_ms delta = time_get_since( start_time );

    alpha = fabs( cos( ((float) delta / 1000.0f) ) );

    return alpha;
}


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
static void update_alarm(
        const gui_s * const gui,
        const unsigned long week_day,
        const unsigned long hour,
        const unsigned long minute,
        gui_alarm_s * const alarm )
{
    if( alarm->enabled == FALSE )
    {
        bool check_date = FALSE;
        if( alarm->week_day == DAY_MONDAY_THROUGH_FRIDAY )
        {
            if( time_is_day_mon_through_fri(week_day) == TRUE )
            {
                check_date = TRUE;
            }
        }
        else if( alarm->week_day == DAY_SATURDAY_AND_SUNDAY )
        {
            if( time_is_day_sat_through_sun(week_day) == TRUE )
            {
                check_date = TRUE;
            }
        }
        else if( week_day == alarm->week_day )
        {
            check_date = TRUE;
        }

        if( check_date == TRUE )
        {
            const timestamp_ms delta = time_get_since( alarm->enabled_timestamp );

            if( delta >= ALARM_DEFAULT_REENABLE_DELAY )
            {
                // only enable when the minute is match so we don't re-enabled after it passes
                if( (hour >= alarm->hour) && (minute == alarm->minute) )
                {
                    alarm->enabled = TRUE;
                    alarm->enabled_timestamp = time_get_timestamp();
                }
            }
        }
    }
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
    if( alarm->enabled == TRUE )
    {
        Fill(
                (unsigned int) config->enabled_digit_color[0],
                (unsigned int) config->enabled_digit_color[1],
                (unsigned int) config->enabled_digit_color[2],
                get_rolling_alpha(alarm->enabled_timestamp) );
    }
    else
    {
        Fill(
                (unsigned int) config->digit_color[0],
                (unsigned int) config->digit_color[1],
                (unsigned int) config->digit_color[2],
                config->digit_color[3] );
    }

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
        config->digit_color[0] = 100.0f;
        config->digit_color[1] = 100.0f;
        config->digit_color[2] = 100.0f;
        config->digit_color[3] = 0.5f;

        config->enabled_digit_color[0] = 255.0f;
        config->enabled_digit_color[1] = 0.0f;
        config->enabled_digit_color[2] = 0.0f;
        config->enabled_digit_color[3] = 1.0f;

        // get text height
        config->font_height = (float) TextHeight(
                *((Fontinfo*) font_get( config->font )),
                (int) config->font_point_size );
    }
}


//
bool alarm_are_any_enabled( const gui_alarm_sequence_s * const alarms )
{
    bool is_enabled = FALSE;
    unsigned long idx = 0;

    for( idx = 0; (idx < alarms->length) && (is_enabled == FALSE); idx += 1 )
    {
        if( alarms->buffer[ idx ].enabled == TRUE )
        {
            is_enabled = TRUE;
        }
    }

    return is_enabled;
}


//
void alarm_disable_all( const gui_alarm_sequence_s * const alarms )
{
    unsigned long idx = 0;

    for( idx = 0; idx < alarms->length; idx += 1 )
    {
        alarms->buffer[ idx ].enabled = FALSE;
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


//
void alarm_update(
        gui_s * const gui,
        gui_alarm_sequence_s * const alarms )
{
    unsigned long idx = 0;
    const unsigned long week_day = time_get_week_day();
    const unsigned long hour = time_get_hour();
    const unsigned long minute = time_get_minute();

    for( idx = 0; idx < alarms->length; idx += 1 )
    {
        gui_alarm_s * const alarm = &alarms->buffer[ idx ];

        update_alarm( gui, week_day, hour, minute, alarm );
    }
}
