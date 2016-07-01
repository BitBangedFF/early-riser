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
    NULL
};


//
static const char MONDAY_THROUGH_FRIDAY[] = "Mon-Fri";




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
        gui_s * const gui,
        gui_alarm_s * const alarm,
        const float ypos )
{
    // get local time date
    const struct tm * const local_date = time_get_localtime( alarm->utc_time );

    // format digits
    if( local_date != NULL )
    {
        // convert 24 hour clock with 12 (AM/PM is implied)
        int am_pm_hour = local_date->tm_hour;
        bool is_pm = false;

        // 12:00 AM
        if( am_pm_hour == 0 )
        {
            am_pm_hour = 12;
        }
        else if( am_pm_hour > 12 )
        {
            // 1:00 PM
            am_pm_hour -= 12;
        }

        if( local_date->tm_hour >= 12 )
        {
            is_pm = TRUE;
        }

        snprintf(
                alarm->time_string,
                sizeof(alarm->time_string),
                "%02d:%02d %s",
                am_pm_hour,
                local_date->tm_min,
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
        if( alarm->is_monday_through_friday == TRUE )
        {
            // mon-fri
            strncat(
                    alarm->display_string,
                    MONDAY_THROUGH_FRIDAY,
                    sizeof(alarm->display_string) );
        }
        else
        {
            strncat(
                    alarm->display_string,
                    WEEK_DAY_TABLE[ local_date->tm_wday ],
                    sizeof(alarm->display_string) );
        }

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
    }

    // select font
    Fontinfo * const font = (Fontinfo*) font_get( alarm->font );

    // stroke width
    StrokeWidth( 0.0f );

    // stroke color
    Stroke( 0, 0, 0, 1.0f );

    // text color
    Fill(
            alarm->digit_color_rgb[0],
            alarm->digit_color_rgb[1],
            alarm->digit_color_rgb[2],
            alarm->digit_color_alpha );

    // render text
    Text(
            20.0f,
            ypos,
            alarm->display_string,
            *font,
            (int) alarm->font_point_size );
}




// *****************************************************
// public definitions
// *****************************************************

//
void alarm_set_default_configuration( gui_alarm_s * const alarm )
{
    // default alarm configuration
    if( alarm != NULL )
    {
        alarm->font = TEXT_FONT_SARIF_TYPE_FACE;
        alarm->font_point_size = 20;
        alarm->digit_color_rgb[0] = 100;
        alarm->digit_color_rgb[1] = 100;
        alarm->digit_color_rgb[2] = 100;
        alarm->digit_color_alpha = 0.5f;
        alarm->is_monday_through_friday = FALSE;
        alarm->enabled = FALSE;

        // get text height
        alarm->font_height = (float) TextHeight(
                *((Fontinfo*) font_get( alarm->font )),
                (int) alarm->font_point_size );
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

    alarm_set_default_configuration( alarm );

    strncpy( alarm->name, name, sizeof(alarm->name) );

//    alarm->utc_time = utc_time;

//    alarm->is_monday_through_friday = is_mon_through_fri;

    // enable if time hasn't already past
//    if( time_get_timestamp() < utc_time )
//    {
//        alarm->enabled = TRUE;
//    }
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

        if( alarm->enabled == TRUE )
        {
            float ypos = 0.0f;

            if( base_y > dy )
            {
                ypos = base_y - dy;
            }

            render( gui, alarm, ypos );

            dy += 2.0f * alarm->font_height;
        }
    }
}
