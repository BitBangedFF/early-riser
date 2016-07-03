/**
 * @file gui.h
 * @brief TODO.
 *
 */




#ifndef GUI_H
#define GUI_H




#include <stdbool.h>
#include <pthread.h>




#ifndef TRUE
#define TRUE 1
#endif


#ifndef FALSE
#define FALSE 0
#endif


/**
 * @brief Minimum of a and b.
 *
 */
#ifndef MIN
#define MIN(a,b) ((a)<(b)?(a):(b))
#endif


/**
 * @brief Maximum of a and b.
 *
 */
#ifndef MAX
#define MAX(a,b) ((a)>(b)?(a):(b))
#endif


/**
 * @brief Absolute value of x.
 *
 */
#ifndef ABS
#define ABS(x) ((x)>0?(x):-(x))
#endif


/**
 * @brief Square of x.
 *
 */
#ifndef SQ
#define SQ(x) ((x)*(x))
#endif


/**
 * @brief Constrain amount to the range low:high.
 *
 */
#ifndef CONSTRAIN
#define CONSTRAIN(amt,low,high) ((amt)<(low)?(low):((amt)>(high)?(high):(amt)))
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
    // monotonic
    timestamp_ms last_stop;
    //
    //
    unsigned long stop_count;
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
    char display_string[32];
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
    bool enabled;
    //
    //
    pthread_t event_thread;
    //
    //
    unsigned long min_x;
    //
    //
    unsigned long max_x;
    //
    //
    unsigned long min_y;
    //
    //
    unsigned long max_y;
    //
    //
    unsigned long width;
    //
    //
    unsigned long height;
} gui_touch_s;


//
typedef struct
{
    //
    //
    bool enabled;
    //
    //
    pthread_t event_thread;
    //
    //
    char file_path[1024];
} gui_audio_s;


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
    gui_touch_s touch;
    //
    //
    gui_audio_s audio;
    //
    //
    unsigned int background_color_rgb[3];
} gui_s;




#endif /* GUI_H */
