/**
 * @file font.c
 * @brief TODO.
 *
 */




#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <VG/openvg.h>
#include <VG/vgu.h>
#include <fontinfo.h>
#include <shapes.h>

#include "gui.h"
#include "font.h"




// *****************************************************
// static global types/macros
// *****************************************************




// *****************************************************
// static global data
// *****************************************************




// *****************************************************
// static declarations
// *****************************************************




// *****************************************************
// static definitions
// *****************************************************

//
static Fontinfo *get_font( const gui_text_font_kind font_kind )
{
    Fontinfo *font = NULL;

    if( font_kind == TEXT_FONT_SARIF_TYPE_FACE )
    {
        font = &SerifTypeface;
    }
    else
    {
        font = &SerifTypeface;
    }

    return font;
}




// *****************************************************
// public definitions
// *****************************************************

//
void *font_get( const gui_text_font_kind font_kind )
{
    return (void*) get_font( font_kind );
}
