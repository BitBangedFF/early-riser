/**
 * @file time_domain.h
 * @brief TODO.
 *
 */




#ifndef TIME_DOMAIN_H
#define	TIME_DOMAIN_H




#include <time.h>
#include "gui.h"




/**
 * @brief Convert seconds to milliseconds. [unsigned long long]
 *
 */
#define SEC_TO_MILLI(time) (time * 1000ULL)


/**
 * @brief Convert milliseconds to seconds. [unsigned long long]
 *
 */
#define MILLI_TO_SEC(time) (time / 1000ULL)


/**
 * @brief Convert nanoseconds to milliseconds. [unsigned long long]
 *
 */
#define NANO_TO_MILLI(time) (time / 1000000ULL)




//
void time_sleep_ms( const timestamp_ms interval );


//
timestamp_ms time_get_timestamp( void );


//
timestamp_ms time_get_monotonic_timestamp( void );


//
timestamp_ms time_get_since( const timestamp_ms const value );


//
timestamp_ms time_get_until( const timestamp_ms const value );


//
struct tm *time_get_localtime( const timestamp_ms const timestamp );




#endif	/* TIME_DOMAIN_H */
