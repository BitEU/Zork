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

#include "funcs.h"
#include "vars.h"

/* GTTIME-- GET TOTAL TIME PLAYED */

void gttime_(t)
integer *t;
{
    integer h, m, s;

    itime_(&h, &m, &s);
    *t = h * 60 + m - (time_1.shour * 60 + time_1.smin);
    if (*t < 0) {
	*t += 1440;
    }
    *t += time_1.pltime;
    return;
} /* gttime_ */

/* OPNCLS-- PROCESS OPEN/CLOSE FOR DOORS */

/* DECLARATIONS */

logical opncls_(obj, so, sc)
integer obj;
integer so;
integer sc;
{
    /* System generated locals */
    integer i__1;
    logical ret_val;

    ret_val = TRUE_;
/* 						!ASSUME WINS. */
    if (prsvec_1.prsa == vindex_1.closew) {
	goto L100;
    }
/* 						!CLOSE? */
    if (prsvec_1.prsa == vindex_1.openw) {
	goto L50;
    }
/* 						!OPEN? */
    ret_val = FALSE_;
/* 						!LOSE */
    return ret_val;

L50:
    if ((objcts_1.oflag2[obj - 1] & OPENBT) != 0) {
	goto L200;
    }
/* 						!OPEN... IS IT? */
    rspeak_(so);
    objcts_1.oflag2[obj - 1] |= OPENBT;
    return ret_val;

L100:
    if (! ((objcts_1.oflag2[obj - 1] & OPENBT) != 0)) {
	goto L200;
    }
/* 						!CLOSE... IS IT? */
    rspeak_(sc);
    objcts_1.oflag2[obj - 1] &= ~ OPENBT;
    return ret_val;

L200:
    i__1 = rnd_(3) + 125;
    rspeak_(i__1);
/* 						!DUMMY. */
    return ret_val;
} /* opncls_ */

/* LIT-- IS ROOM LIT? */

/* DECLARATIONS */

logical lit_(rm)
integer rm;
{
    /* System generated locals */
    integer i__1, i__2;
    logical ret_val;

    /* Local variables */
    integer i, j, oa;

    ret_val = TRUE_;
/* 						!ASSUME WINS */
    if ((rooms_1.rflag[rm - 1] & RLIGHT) != 0) {
	return ret_val;
    }

    i__1 = objcts_1.olnt;
    for (i = 1; i <= i__1; ++i) {
/* 						!LOOK FOR LIT OBJ */
	if (qhere_(i, rm)) {
	    goto L100;
	}
/* 						!IN ROOM? */
	oa = objcts_1.oadv[i - 1];
/* 						!NO */
	if (oa <= 0) {
	    goto L1000;
	}
/* 						!ON ADV? */
	if (advs_1.aroom[oa - 1] != rm) {
	    goto L1000;
	}
/* 						!ADV IN ROOM? */

/* OBJ IN ROOM OR ON ADV IN ROOM */

L100:
	if ((objcts_1.oflag1[i - 1] & ONBT) != 0) {
	    return ret_val;
	}
	if ((objcts_1.oflag1[i - 1] & VISIBT) == 0 || (
		objcts_1.oflag1[i - 1] & TRANBT) == 0 && (
		objcts_1.oflag2[i - 1] & OPENBT) == 0) {
	    goto L1000;
	}

/* OBJ IS VISIBLE AND OPEN OR TRANSPARENT */

	i__2 = objcts_1.olnt;
	for (j = 1; j <= i__2; ++j) {
	    if (objcts_1.ocan[j - 1] == i && (objcts_1.oflag1[j - 1] & 
		    ONBT) != 0) {
		return ret_val;
	    }
/* L500: */
	}
L1000:
	;
    }
    ret_val = FALSE_;
    return ret_val;
} /* lit_ */

/* WEIGHT- RETURNS SUM OF WEIGHT OF QUALIFYING OBJECTS */

/* DECLARATIONS */

integer weight_(rm, cn, ad)
integer rm;
integer cn;
integer ad;
{
    /* System generated locals */
    integer ret_val, i__1;

    /* Local variables */
    integer i, j;

    ret_val = 0;
    i__1 = objcts_1.olnt;
    for (i = 1; i <= i__1; ++i) {
/* 						!OMIT BIG FIXED ITEMS. */
	if (objcts_1.osize[i - 1] >= 10000) {
	    goto L100;
	}
/* 						!IF FIXED, FORGET IT. */
	if (qhere_(i, rm) && rm != 0 || objcts_1.oadv[i - 1] == ad && ad 
		!= 0) {
	    goto L50;
	}
	j = i;
/* 						!SEE IF CONTAINED. */
L25:
	j = objcts_1.ocan[j - 1];
/* 						!GET NEXT LEVEL UP. */
	if (j == 0) {
	    goto L100;
	}
/* 						!END OF LIST? */
	if (j != cn) {
	    goto L25;
	}
L50:
	ret_val += objcts_1.osize[i - 1];
L100:
	;
    }
    return ret_val;
} /* weight_ */
