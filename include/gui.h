/**
 * @file gui.h
 * @brief TODO.
 *
 */




#ifndef GUI_H
#define GUI_H




#include <stdbool.h>




#ifndef TRUE
#define TRUE 1
#endif


#ifndef FALSE
#define FALSE 0
#endif




//
typedef unsigned long long timestamp_ms;




//
typedef enum
{
    //
    //
    TEXT_FONT_SARIF_TYPE_FACE = 0,
    //
    //
    TEXT_FONT_SARIF_TYPE_KIND_COUNT
} gui_text_font_kind;




//
typedef struct
{
    //
    //
    char time_string[32];
    //
    //
    gui_text_font_kind font;
    //
    //
    float font_height;
    //
    //
    unsigned int font_point_size;
    //
    //
    float digit_color[4];
} gui_clock_s;


//
typedef struct
{
    //
    //
    char date_string[256];
    //
    //
    gui_text_font_kind font;
    //
    //
    float font_height;
    //
    //
    unsigned int font_point_size;
    //
    //
    float digit_color[4];
} gui_calendar_s;


//
typedef struct
{
    //
    //
    char name[256];
    //
    //
    bool enabled;
    //
    //
    timestamp_ms enabled_timestamp;
    //
    //
    unsigned long week_day;
    //
    //
    unsigned long hour;
    //
    //
    unsigned long minute;
    //
    //
    char display_string[1024];
    //
    //
    char time_string[256];
} gui_alarm_s;


//
typedef struct
{
    //
    //
    gui_text_font_kind font;
    //
    //
    float font_height;
    //
    //
    unsigned int font_point_size;
    //
    //
    float digit_color[4];
    //
    //
    float enabled_digit_color[4];
} gui_alarm_config_s;


//
typedef struct
{
    gui_alarm_config_s config;
    //
    //
    unsigned long length;
    //
    //
    unsigned long maximum;
    //
    //
    gui_alarm_s *buffer;
} gui_alarm_sequence_s;


//
typedef struct
{
    //
    //
    bool enabled;
    //
    //
    timestamp_ms enabled_timestamp;
    //
    // monotonic
    timestamp_ms last_update;
    //
    //
    float position[2];
    //
    //
    float radius;
    //
    //
    float velocity[2];
    //
    //
    float min_bounds[2];
    //
    //
    float max_bounds[2];
    //
    //
    unsigned long win_width;
    //
    //
    unsigned long win_height;
    //
    //
    float color[4];
    //
    //
    gui_text_font_kind font;
    //
    //
    float font_height;
    //
    //
    unsigned int font_point_size;
    //
    //
    float digit_color[4];
} gui_disabler_s;


//
typedef struct
{
    //
    //
    unsigned long win_width;
    //
    //
    unsigned long win_height;
    //
    //
    unsigned long long render_cnt;
} gui_display_s;


//
typedef struct
{
    //
    //
    timestamp_ms utc_clock_time;
    //
    //
    gui_display_s display;
    //
    //
    gui_clock_s clock;
    //
    //
    gui_calendar_s calendar;
    //
    //
    gui_alarm_sequence_s alarms;
    //
    //
    gui_disabler_s disabler;
    //
    //
    unsigned int background_color_rgb[3];
} gui_s;




#endif /* GUI_H */
