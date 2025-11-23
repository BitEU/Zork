/* MOVETO- MOVE PLAYER TO NEW ROOM */

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
#include "funcs.h"
#include "vars.h"

logical moveto_(nr, who)
integer nr;
integer who;
{
    /* System generated locals */
    logical ret_val;

    /* Local variables */
    integer j;
    logical lhr;
    logical lnr, nlv;
    integer bits;

    ret_val = FALSE_;
/* 						!ASSUME FAILS. */
    lhr = (rooms_1.rflag[play_1.here - 1] & RLAND) != 0;
    lnr = (rooms_1.rflag[nr - 1] & RLAND) != 0;
    j = advs_1.avehic[who - 1];
/* 						!HIS VEHICLE */

    if (j != 0) {
	goto L100;
    }
/* 						!IN VEHICLE? */
    if (lnr) {
	goto L500;
    }
/* 						!NO, GOING TO LAND? */
    rspeak_(427);
/* 						!CAN'T GO WITHOUT VEHICLE. */
    return ret_val;

L100:
    bits = 0;
/* 						!ASSUME NOWHERE. */
    if (j == oindex_1.rboat) {
	bits = RWATER;
    }
/* 						!IN BOAT? */
    if (j == oindex_1.ballo) {
	bits = RAIR;
    }
/* 						!IN BALLOON? */
    if (j == oindex_1.bucke) {
	bits = RBUCK;
    }
/* 						!IN BUCKET? */
    nlv = (rooms_1.rflag[nr - 1] & bits) == 0;
    if (! lnr && nlv || lnr && lhr && nlv && bits != RLAND) {
	goto L800;
    }

L500:
    ret_val = TRUE_;
/* 						!MOVE SHOULD SUCCEED. */
    if ((rooms_1.rflag[nr - 1] & RMUNG) == 0) {
	goto L600;
    }
    rspeak_(rrand[nr - 1]);
/* 						!YES, TELL HOW. */
    return ret_val;

L600:
    if (who != aindex_1.player) {
	newsta_(advs_1.aobj[who - 1], 0, nr, 0, 0);
    }
    if (j != 0) {
	newsta_(j, 0, nr, 0, 0);
    }
    play_1.here = nr;
    advs_1.aroom[who - 1] = play_1.here;
    scrupd_(rooms_1.rval[nr - 1]);
/* 						!SCORE ROOM */
    rooms_1.rval[nr - 1] = 0;
    return ret_val;

L800:
    rspsub_(428, objcts_1.odesc2[j - 1]);
/* 						!WRONG VEHICLE. */
    return ret_val;
} /* moveto_ */

/* SCORE-- PRINT OUT CURRENT SCORE */

/* DECLARATIONS */

void score_(flg)
logical flg;
{
    /* Initialized data */

    static const integer rank[10] = { 20,19,18,16,12,8,4,2,1,0 };
    static const integer erank[5] = { 20,15,10,5,0 };

    /* System generated locals */
    integer i__1;

    /* Local variables */
    integer i, as;

    as = advs_1.ascore[play_1.winner - 1];

    if (findex_1.endgmf) {
	goto L60;
    }
/* 						!ENDGAME? */
    more_output(NULL);
    printf("Your score ");
    if (flg)
	printf("would be");
    else
	printf("is");
    printf(" %d [total of %d points], in %d move", as, state_1.mxscor,
	    state_1.moves);
    if (state_1.moves != 1)
	printf("s");
    printf(".\n");

    for (i = 1; i <= 10; ++i) {
	if (as * 20 / state_1.mxscor >= rank[i - 1]) {
	    goto L50;
	}
/* L10: */
    }
L50:
    i__1 = i + 484;
    rspeak_(i__1);
    return;

L60:
    more_output(NULL);
    printf("Your score in the endgame ");
    if (flg)
	printf("would be");
    else
	printf("is");
    printf(" %d [total of %d points], in %d moves.\n", state_1.egscor,
	   state_1.egmxsc, state_1.moves);

    for (i = 1; i <= 5; ++i) {
	if (state_1.egscor * 20 / state_1.egmxsc >= erank[i - 1]) {
	    goto L80;
	}
/* L70: */
    }
L80:
    i__1 = i + 786;
    rspeak_(i__1);
} /* score_ */

/* SCRUPD- UPDATE WINNER'S SCORE */

/* DECLARATIONS */

void scrupd_(n)
integer n;
{
    if (findex_1.endgmf) {
	goto L100;
    }
/* 						!ENDGAME? */
    advs_1.ascore[play_1.winner - 1] += n;
/* 						!UPDATE SCORE */
    state_1.rwscor += n;
/* 						!UPDATE RAW SCORE */
    if (advs_1.ascore[play_1.winner - 1] < state_1.mxscor - state_1.deaths * 
	    10) {
	return;
    }
    cevent_1.cflag[cindex_1.cevegh - 1] = TRUE_;
/* 						!TURN ON END GAME */
    cevent_1.ctick[cindex_1.cevegh - 1] = 15;
    return;

L100:
    state_1.egscor += n;
/* 						!UPDATE EG SCORE. */
} /* scrupd_ */
