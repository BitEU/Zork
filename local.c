/* local.c -- dungeon functions which need local definition */
/* WRITTEN BY R. M. SUPNIK */

/*
 * Program Name: Zork
 * Program Release Year: 2025
 * Program Author: Steven S.
 * Program Link: https://github.com/BitEU/Zork
 * 
 * Original Name: Zork
 * Original Release Year: 1977
 * Original Author: Tim Anderson, Marc Blank, Bruce Daniels, and Dave Lebling
 * Original Link: N/A, but a good article exists here https://www.technologyreview.com/2017/08/22/149560/the-enduring-legacy-of-zork/
 * Original System: PDP-10
 * 
 * Rewrite Name: Zork I
 * Rewrite Release Year: 1991
 * Rewrite Author: Ian Lance Taylor, devshane, ChandlerSwift, jaredledvina, justwheel, and Jan200101
 * Rewrite Link: https://github.com/devshane/zork
 * 
 * Extensive version history:
 *  11-NOV-25  Modified to run on UNIVAC 1219
 * 11-MAR-91	Converted to C (V2.7)
 * 14-SEP-87	Converted to f77/Unix for pdps and Vaxen (V2.6B)
 * 18-JUL-80	Transportable data base file (V2.5A).
 * 28-FEB-80	Compressed text file (V2.4A).
 * 15-NOV-79	Bug fixes (V2.3A).
 * 18-JAN-79	Revised DECUS version (V2.2A).
 * 10-OCT-78	Puzzle Room (V2.1A).
 * 10-SEP-78	Endgame (V2.0A).
 * 10-AUG-78	DECUS version (V1.1B).
 * 14-JUN-78	Public version with parser (V1.1A).
 * 4-MAR-78	Debugging version (V1.0A).
 *
 * Major changes from rewrite: Added UNIVAC 1219 support
 */

#include "funcs.h"

#ifdef __AMOS__
#include <moncal.h>
#endif

/* This function should return TRUE_ if it's OK for people to play the
 * game, FALSE_ otherwise.  If you have a working <time.h> library,
 * you can define NONBUSINESS to disallow play Monday to Friday, 9-5
 * (this is only checked at the start of the game, though).  For more
 * complex control you will have to write your own version of this
 * function.
 */

#if defined(NONBUSINESS) && !defined(UNIVAC)
#ifdef BSD4_2
#include <sys/timeb.h>
#else /* ! BSD4_2 */
#include <time.h>
#endif /* ! BSD4_2 */
#endif /* NONBUSINESS && !UNIVAC */

logical protected()
{
#ifndef NONBUSINESS

    return TRUE_;

#else /* NONBUSINESS */

    time_t t;
    struct tm *q;

    (void)time(&t);
    q = localtime(&t);

    /* Return TRUE_ if it's Sunday or Saturday or before 9 or after 5 */

    if (q->tm_wday == 0 || q->tm_wday == 6)
	return TRUE_;
    else if (q->tm_hour < 9 || q->tm_hour >= 17)
	return TRUE_;
    else
	return FALSE_;

#endif /* NONBUSINESS */

}

#ifdef ALLOW_GDT

/* This function should return TRUE_ if the user is allowed to invoke the
 * game debugging tool by typing "gdt".  This isn't very useful without
 * the source code, and it's mainly for people trying to debug the game.
 * You can define WIZARDID to specify a user id on a UNIX system.  On a
 * non AMOS, non unix system this function will have to be changed if
 * you want to use gdt.
 */

#ifndef WIZARDID
#define WIZARDID (0)
#endif

logical wizard()
{
#ifdef __AMOS__
    if (jobidx()->jobusr == 0x102)
	return TRUE_;
#else
#if defined(unix) && !defined(UNIVAC)
    if (getuid() == 0 || getuid() == WIZARDID)
	return TRUE_;
#endif
#endif

    /* On UNIVAC and other systems, allow wizard mode by default */
    return TRUE_;
}

#endif
