/**
 * @file gui.h
 * @brief TODO.
 *
 */




#ifndef GUI_H
#define GUI_H




#include <stdbool.h>




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
    unsigned int font_point_size;
    //
    //
    unsigned int digit_color_rgb[3];
    //
    //
    float digit_color_alpha;
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
    unsigned int font_point_size;
    //
    //
    unsigned int digit_color_rgb[3];
    //
    //
    float digit_color_alpha;
} gui_calendar_s;


//
typedef struct
{
    //
    //
    char name[256];
    //
    //
    timestamp_ms utc_time;
    //
    //
    char date_string[256];
} gui_alarm_s;


//
typedef struct
{
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
    unsigned int win_width;
    //
    //
    unsigned int win_height;
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
    unsigned int background_color_rgb[3];
} gui_s;




#endif /* GUI_H */
