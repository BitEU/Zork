/* supp.c -- support routines for dungeon */
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

#include <stdio.h>

#ifdef unix
#include <sys/types.h>
#endif

#ifdef BSD4_2
#include <sys/time.h>
#else /* ! BSD4_2 */
#include <time.h>
#endif /* ! BSD4_2 */

#include "funcs.h"

/* Define these here to avoid using <stdlib.h> */

extern void exit P((int));
extern int rand P((void));

/* We should have a definition for time_t and struct tm by now.  Make
 * sure we have definitions for the functions we want to call.
 * The argument to localtime should be P((const time_t *)), but Ultrix
 * 4.0 leaves out the const in their prototype.  Damn them.
 */

extern time_t time P((time_t *));
extern struct tm *localtime ();

/* Terminate the game */

void exit_()
{
    fprintf(stderr, "The game is over.\n");
    exit(0);
}

/* Get time in hours, minutes and seconds */

void itime_(hrptr, minptr, secptr)
integer *hrptr;
integer *minptr;
integer *secptr;
{
	time_t timebuf;
	struct tm *tmptr;

	time(&timebuf);
	tmptr = localtime(&timebuf);
	
	*hrptr  = tmptr->tm_hour;
	*minptr = tmptr->tm_min;
	*secptr = tmptr->tm_sec;
}
/* Random number generator */

integer rnd_(maxval)
integer maxval;
{
	return rand() % maxval;
}

/* Terminal support routines for dungeon */
/* By Ian Lance Taylor ian@airs.com or uunet!airs!ian */

/* The dungeon game can often output long strings, more than enough
 * to overwhelm a typical 24 row terminal (I assume that back when
 * dungeon was written people generally used paper terminals (I know
 * I did) so this was not a problem).  The functions in this file
 * provide a very simplistic ``more'' facility.  They are necessarily
 * somewhat operating system dependent, although I have tried to
 * minimize it as much as I could.
 */

/* The following macro definitions may be used to control how these
 * functions work:
 *
 *	MORE_NONE	Don't use the more facility at all
 *	MORE_24		Always assume a 24 row terminal
 *	MORE_TERMCAP	Use termcap routines to get rows of terminal
 *	MORE_TERMINFO	Use terminfo routines to get rows of terminal
 *	MORE_AMOS	Use AMOS monitor calls to get rows of terminal
 *
 * If none of these are defined then this will use termcap routines on
 * unix and AMOS routines on AMOS.
 */

#ifndef MORE_NONE
#ifndef MORE_24
#ifndef MORE_TERMCAP
#ifndef MORE_TERMINFO
#ifndef MORE_AMOS
#ifdef __AMOS__
#define MORE_AMOS
#else /* ! __AMOS__ */
#ifdef unix
#define MORE_TERMCAP
#else /* ! unix */
#define MORE_NONE
#endif /* ! unix */
#endif /* ! __AMOS__ */
#endif /* ! MORE_AMOS */
#endif /* ! MORE_TERMINFO */
#endif /* ! MORE_TERMCAP */
#endif /* ! MORE_24 */
#endif /* ! MORE_NONE */

#ifdef MORE_TERMCAP

extern char *getenv P((const char *));
extern void tgetent P((char *, const char *));
extern int tgetnum P((const char *));

#else /* ! MORE_TERMCAP */

#ifdef MORE_TERMINFO

#include <cursesX.h>
#include <term.h>
extern void setupterm P((const char *, int, int));

#else /* ! MORE_TERMINFO */

#ifdef MORE_AMOS

#include <moncal.h>
#include <unistd.h>

#endif /* MORE_AMOS */
#endif /* ! MORE_TERMINFO */
#endif /* ! MORE_TERMCAP */

/* Initialize the more waiting facility (determine how many rows the
 * terminal has).
 */

static integer crows;
static integer coutput;

void more_init()
{
#ifdef MORE_NONE

    crows = 0;

#else /* ! MORE_NONE */
#ifdef MORE_24

    crows = 24;

#else /* ! MORE_24 */
#ifdef MORE_TERMCAP

    char buf[2048];
    char *term;

    term = getenv("TERM");
    if (term == NULL)
	crows = 0;
    else {
	tgetent(buf, term);
	crows = tgetnum("li");
    }

#else /* ! MORE_TERMCAP */
#ifdef MORE_TERMINFO

    int i;

    setupterm(NULL, 1, &i);
    if (i != 1)
        crows = 0;
    else
	crows = lines;

#else /* ! MORE_TERMINFO */
#ifdef MORE_AMOS

    trm_char st;

    if (isatty(fileno(stdin)) == 0)
	crows = 0;
    else {
	    trmchr(&st, 0);
	    crows = st.row;
    }

#else /* ! MORE_AMOS */

    This should be impossible

#endif /* ! MORE_AMOS */
#endif /* ! MORE_TERMINFO */
#endif /* ! MORE_TERMCAP */
#endif /* ! MORE_24 */
#endif /* ! MORE_NONE */
}

/* The program wants to output a line to the terminal.  If z is not
 * NULL it is a simple string which is output here; otherwise it
 * needs some sort of formatting, and is output after this function
 * returns (if all computers had vprintf I would just it, but they
 * probably don't).
 */

void more_output(z)
const char *z;
{
/* pager code remarked out to allow streamed input and output */
/*
     if (crows > 0  &&  coutput > crows - 2) {
	printf("Press return to continue: ");
	(void) fflush(stdout);
	while (getchar() != '\n')
	    ;
	coutput = 0;
    }
*/
    if (z != NULL)
	printf("%s\n", z);

    coutput++;
}

/* The terminal is waiting for input (clear the number of output lines) */

void more_input()
{
    coutput = 0;
}
