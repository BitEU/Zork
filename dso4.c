/* ROBADV-- STEAL WINNER'S VALUABLES */

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

integer robadv_(adv, nr, nc, na)
integer adv;
integer nr;
integer nc;
integer na;
{
    /* System generated locals */
    integer ret_val, i__1;

    /* Local variables */
    integer i;

    ret_val = 0;
/* 						!COUNT OBJECTS */
    i__1 = objcts_1.olnt;
    for (i = 1; i <= i__1; ++i) {
	if (objcts_1.oadv[i - 1] != adv || objcts_1.otval[i - 1] <= 0 || (
		objcts_1.oflag2[i - 1] & SCRDBT) != 0) {
	    goto L100;
	}
	newsta_(i, 0, nr, nc, na);
/* 						!STEAL OBJECT */
	++ret_val;
L100:
	;
    }
    return ret_val;
} /* robadv_ */

/* ROBRM-- STEAL ROOM VALUABLES */

/* DECLARATIONS */

integer robrm_(rm, pr, nr, nc, na)
integer rm;
integer pr;
integer nr;
integer nc;
integer na;
{
    /* System generated locals */
    integer ret_val, i__1, i__2;

    /* Local variables */
    integer i;


/* OBJECTS */




    ret_val = 0;
/* 						!COUNT OBJECTS */
    i__1 = objcts_1.olnt;
    for (i = 1; i <= i__1; ++i) {
/* 						!LOOP ON OBJECTS. */
	if (! qhere_(i, rm)) {
	    goto L100;
	}
	if (objcts_1.otval[i - 1] <= 0 || (objcts_1.oflag2[i - 1] & 
		SCRDBT) != 0 || (objcts_1.oflag1[i - 1] & 
		VISIBT) == 0 || ! prob_(pr, pr)) {
	    goto L50;
	}
	newsta_(i, 0, nr, nc, na);
	++ret_val;
	objcts_1.oflag2[i - 1] |= TCHBT;
	goto L100;
L50:
	if ((objcts_1.oflag2[i - 1] & ACTRBT) != 0) {
	    i__2 = oactor_(i);
	    ret_val += robadv_(i__2, nr, nc, na);
	}
L100:
	;
    }
    return ret_val;
} /* robrm_ */

/* WINNIN-- SEE IF VILLAIN IS WINNING */

/* DECLARATIONS */

logical winnin_(vl, hr)
integer vl;
integer hr;
{
    /* System generated locals */
    logical ret_val;

    /* Local variables */
    integer ps, vs;


/* OBJECTS */



    vs = objcts_1.ocapac[vl - 1];
/* 						!VILLAIN STRENGTH */
    ps = vs - fights_(hr, 1);
/* 						!HIS MARGIN OVER HERO */
    ret_val = prob_(90, 100);
    if (ps > 3) {
	return ret_val;
    }
/* 						!+3... 90% WINNING */
    ret_val = prob_(75, 85);
    if (ps > 0) {
	return ret_val;
    }
/* 						!>0... 75% WINNING */
    ret_val = prob_(50, 30);
    if (ps == 0) {
	return ret_val;
    }
/* 						!=0... 50% WINNING */
    ret_val = prob_(25, 25);
    if (vs > 1) {
	return ret_val;
    }
/* 						!ANY VILLAIN STRENGTH. */
    ret_val = prob_(10, 0);
    return ret_val;
} /* winnin_ */

/* FIGHTS-- COMPUTE FIGHT STRENGTH */

/* DECLARATIONS */

integer fights_(h, flg)
integer h;
logical flg;
{
    /* Initialized data */

    const integer smin = 2;
    const integer smax = 7;

    /* System generated locals */
    integer ret_val;

    ret_val = smin + ((smax - smin) * advs_1.ascore[h - 1] + state_1.mxscor /
	     2) / state_1.mxscor;
    if (flg) {
	ret_val += advs_1.astren[h - 1];
    }
    return ret_val;
} /* fights_ */

/* VILSTR-	COMPUTE VILLAIN STRENGTH */

/* DECLARATIONS */

integer vilstr_(v)
integer v;
{
    /* System generated locals */
    integer ret_val, i__1, i__2, i__3;

    /* Local variables */
    integer i;

    ret_val = objcts_1.ocapac[v - 1];
    if (ret_val <= 0) {
	return ret_val;
    }
    if (v != oindex_1.thief || ! findex_1.thfenf) {
	goto L100;
    }
    findex_1.thfenf = FALSE_;
/* 						!THIEF UNENGROSSED. */
    ret_val = min(ret_val,2);
/* 						!NO BETTER THAN 2. */

L100:
    i__1 = vill_1.vlnt;
    for (i = 1; i <= i__1; ++i) {
/* 						!SEE IF  BEST WEAPON. */
	if (vill_1.villns[i - 1] == v && prsvec_1.prsi == vill_1.vbest[i - 1]
		) {
/* Computing MAX */
	    i__2 = 1, i__3 = ret_val - 1;
	    ret_val = max(i__2,i__3);
	}
/* L200: */
    }
    return ret_val;
} /* vilstr_ */
