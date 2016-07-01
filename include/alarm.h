/**
 * @file alarm.h
 * @brief TODO.
 *
 */




#ifndef ALARM_H
#define	ALARM_H




#include "gui.h"




//
void alarm_set_default_configuration( gui_alarm_s * const alarm );


//
void alarm_release( gui_alarm_sequence_s * const alarms );


//
void alarm_add(
        const char * const name,
        const unsigned long week_day,
        const unsigned long hour,
        const unsigned long minute,
        gui_alarm_sequence_s * const alarms );


//
void alarm_render(
        gui_s * const gui,
        gui_alarm_sequence_s * const alarms );




#endif	/* ALARM_H */
