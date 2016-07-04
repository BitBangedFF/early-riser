/**
 * @file audio.h
 * @brief TODO.
 *
 */




#ifndef AUDIO_H
#define	AUDIO_H




#include "gui.h"




//
#define AUDIO_DEFAULT_VOLUME (50)




//
void audio_init(
        const char * const file_path,
        gui_audio_s * const audio );


//
void audio_release( gui_audio_s * const audio );


//
void audio_enable( gui_audio_s * const audio );


//
void audio_disable( gui_audio_s * const audio );




#endif	/* AUDIO_H */
