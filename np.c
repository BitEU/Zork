/* RDLINE-	READ INPUT LINE */

/*COPYRIGHT 1980, INFOCOM COMPUTERS AND COMMUNICATIONS, CAMBRIDGE MA. 02142*/
/* ALL RIGHTS RESERVED, COMMERCIAL USAGE STRICTLY PROHIBITED */
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
#include <ctype.h>
#include "funcs.h"
#include "vars.h"

/* This declaration is here since many systems don't have <stdlib.h> */

extern int system P((const char *));

static logical lex_ P((char *, integer *, integer *, logical));

void rdline_(buffer, who)
char *buffer;
integer who;
{
    /* Local variables */
    char *z, *zlast;

    /* Function Body */
L5:
    switch (who + 1) {
	case 1:  goto L90;
	case 2:  goto L10;
    }
/* 						!SEE WHO TO PROMPT FOR. */
L10:
    printf(">");
/* 						!PROMPT FOR GAME. */
L90:
    (void) fflush(stdout);
    if (fgets(buffer, 78, stdin) == NULL)
	exit_();
    more_input();

    if (buffer[0] == '!') {
	system(buffer + 1);
	goto L5;
    }

    zlast = buffer - 1;
    for (z = buffer; *z != '\0' && *z != '\n'; z++) {
	if (*z != ' ')
	    zlast = z;
	if (islower(*z))
		*z = toupper(*z);
    }
    z = zlast + 1;
    if (z == buffer)
	goto L5;
    *z = '\0';

    prsvec_1.prscon = 1;
/* 						!RESTART LEX SCAN. */
} /* rdline_ */

/* PARSE-	TOP LEVEL PARSE ROUTINE */

/* THIS ROUTINE DETAILS ON BIT 0 OF PRSFLG */

logical parse_(inbuf, vbflag)
char *inbuf;
logical vbflag;
{
    /* System generated locals */
    integer i__1;
    logical ret_val;

    /* Local variables */
    integer outbuf[40], outlnt;

    /* Parameter adjustments */
    --inbuf;

    /* Function Body */
    ret_val = FALSE_;
/* 						!ASSUME FAILS. */
    prsvec_1.prsa = 0;
/* 						!ZERO OUTPUTS. */
    prsvec_1.prsi = 0;
    prsvec_1.prso = 0;

    if (! lex_(inbuf + 1, outbuf, &outlnt, vbflag)) {
	goto L100;
    }
    if ((i__1 = sparse_(outbuf, outlnt, vbflag)) < 0) {
	goto L100;
    } else if (i__1 == 0) {
	goto L200;
    } else {
	goto L300;
    }
/* 						!DO SYN SCAN. */

/* PARSE REQUIRES VALIDATION */

L200:
    if (! (vbflag)) {
	goto L350;
    }
/* 						!ECHO MODE, FORCE FAIL. */
    if (! synmch_()) {
	goto L100;
    }
/* 						!DO SYN MATCH. */
    if (prsvec_1.prso > 0 & prsvec_1.prso < xsrch_1.xmin) {
	last_1.lastit = prsvec_1.prso;
    }

/* SUCCESSFUL PARSE OR SUCCESSFUL VALIDATION */

L300:
    ret_val = TRUE_;
L350:
    orphan_(0, 0, 0, 0, 0);
/* 						!CLEAR ORPHANS. */
    return ret_val;

/* PARSE FAILS, DISALLOW CONTINUATION */

L100:
    prsvec_1.prscon = 1;
    return ret_val;

} /* parse_ */

/* ORPHAN- SET UP NEW ORPHANS */

/* DECLARATIONS */

void orphan_(o1, o2, o3, o4, o5)
integer o1;
integer o2;
integer o3;
integer o4;
integer o5;
{
    orphs_1.oflag = o1;
/* 						!SET UP NEW ORPHANS. */
    orphs_1.oact = o2;
    orphs_1.oslot = o3;
    orphs_1.oprep = o4;
    orphs_1.oname = o5;
} /* orphan_ */

/* LEX-	LEXICAL ANALYZER */

/* THIS ROUTINE DETAILS ON BIT 1 OF PRSFLAG */

static logical lex_(inbuf, outbuf, op, vbflag)
char *inbuf;
integer *outbuf;
integer *op;
logical vbflag;
{
    /* Initialized data */

    static const char dlimit[9] = { 'A', 'Z', 'A' - 1,
				    '1', '9', '1' - 31,
				    '-', '-', '-' - 27 };

    /* System generated locals */
    logical ret_val;

    /* Local variables */
    integer i;
    char j;
    integer k, j1, j2, cp;

    /* Parameter adjustments */
    --outbuf;
    --inbuf;

    /* Function Body */

    for (i = 1; i <= 40; ++i) {
/* 						!CLEAR OUTPUT BUF. */
	outbuf[i] = 0;
/* L100: */
    }

    ret_val = FALSE_;
/* 						!ASSUME LEX FAILS. */
    *op = -1;
/* 						!OUTPUT PTR. */
L50:
    *op += 2;
/* 						!ADV OUTPUT PTR. */
    cp = 0;
/* 						!CHAR PTR=0. */

L200:
    j = inbuf[prsvec_1.prscon];
/* 						!GET CHARACTER */

    if (j == '\0')
	goto L1000;
/* 						!END OF INPUT? */

    ++prsvec_1.prscon;
/* 						!ADVANCE PTR. */

    if (j == '.') {
	goto L1000;
    }
/* 						!END OF COMMAND? */
    if (j == ',') {
	goto L1000;
    }
/* 						!END OF COMMAND? */
    if (j == ' ') {
	goto L6000;
    }
/* 						!SPACE? */
    for (i = 1; i <= 9; i += 3) {
/* 						!SCH FOR CHAR. */
	if (j >= dlimit[i - 1] & j <= dlimit[i]) {
	    goto L4000;
	}
/* L500: */
    }

    if (vbflag) {
	rspeak_(601);
    }
/* 						!GREEK TO ME, FAIL. */
    return ret_val;

/* END OF INPUT, SEE IF PARTIAL WORD AVAILABLE. */

L1000:
    if (inbuf[prsvec_1.prscon] == '\0') {
	prsvec_1.prscon = 1;
    }
/* 						!FORCE PARSE RESTART. */
    if (cp == 0 & *op == 1) {
	return ret_val;
    }
    if (cp == 0) {
	*op += -2;
    }
/* 						!ANY LAST WORD? */
    ret_val = TRUE_;
    return ret_val;

/* LEGITIMATE CHARACTERS: LETTER, DIGIT, OR HYPHEN. */

L4000:
    j1 = j - dlimit[i + 1];
    if (cp >= 6) {
	goto L200;
    }
/* 						!IGNORE IF TOO MANY CHAR. */
    k = *op + cp / 3;
/* 						!COMPUTE WORD INDEX. */
    switch (cp % 3 + 1) {
	case 1:  goto L4100;
	case 2:  goto L4200;
	case 3:  goto L4300;
    }
/* 						!BRANCH ON CHAR. */
L4100:
    j2 = j1 * 780;
/* 						!CHAR 1... *780 */
    outbuf[k] = outbuf[k] + j2 + j2;
/* 						!*1560 (40 ADDED BELOW). */
L4200:
    outbuf[k] += j1 * 39;
/* 						!*39 (1 ADDED BELOW). */
L4300:
    outbuf[k] += j1;
/* 						!*1. */
    ++cp;
    goto L200;
/* 						!GET NEXT CHAR. */

/* SPACE */

L6000:
    if (cp == 0) {
	goto L200;
    }
/* 						!ANY WORD YET? */
    goto L50;
/* 						!YES, ADV OP. */

} /* lex_ */
