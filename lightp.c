/* LIGHTP-	LIGHT PROCESSOR */

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

logical lightp_(obj)
integer obj;
{
    /* System generated locals */
    logical ret_val;

    /* Local variables */
    integer flobts;
    integer i;

    ret_val = TRUE_;
/* 						!ASSUME WINS */
    flobts = FLAMBT + LITEBT + ONBT;
    if (obj != oindex_1.candl) {
	goto L20000;
    }
/* 						!CANDLE? */
    if (findex_1.orcand != 0) {
	goto L19100;
    }
/* 						!FIRST REF? */
    findex_1.orcand = 1;
/* 						!YES, CANDLES ARE */
    cevent_1.ctick[cindex_1.cevcnd - 1] = 50;
/* 						!BURNING WHEN SEEN. */

L19100:
    if (prsvec_1.prsi == oindex_1.candl) {
	goto L10;
    }
/* 						!IGNORE IND REFS. */
    if (prsvec_1.prsa != vindex_1.trnofw) {
	goto L19200;
    }
/* 						!TURN OFF? */
    i = 513;
/* 						!ASSUME OFF. */
    if ((objcts_1.oflag1[oindex_1.candl - 1] & ONBT) != 0) {
	i = 514;
    }
/* 						!IF ON, DIFFERENT. */
    cevent_1.cflag[cindex_1.cevcnd - 1] = FALSE_;
/* 						!DISABLE COUNTDOWN. */
    objcts_1.oflag1[oindex_1.candl - 1] &= ~ ONBT;
    rspeak_(i);
    return ret_val;

L19200:
    if (prsvec_1.prsa != vindex_1.burnw && prsvec_1.prsa != vindex_1.trnonw) {

	goto L10;
    }
    if ((objcts_1.oflag1[oindex_1.candl - 1] & LITEBT) != 0) {
	goto L19300;
    }
    rspeak_(515);
/* 						!CANDLES TOO SHORT. */
    return ret_val;

L19300:
    if (prsvec_1.prsi != 0) {
	goto L19400;
    }
/* 						!ANY FLAME? */
    rspeak_(516);
/* 						!NO, LOSE. */
    prsvec_1.prswon = FALSE_;
    return ret_val;

L19400:
    if (prsvec_1.prsi != oindex_1.match || ! ((objcts_1.oflag1[oindex_1.match 
	    - 1] & ONBT) != 0)) {
	goto L19500;
    }
    i = 517;
/* 						!ASSUME OFF. */
    if ((objcts_1.oflag1[oindex_1.candl - 1] & ONBT) != 0) {
	i = 518;
    }
/* 						!IF ON, JOKE. */
    objcts_1.oflag1[oindex_1.candl - 1] |= ONBT;
    cevent_1.cflag[cindex_1.cevcnd - 1] = TRUE_;
/* 						!RESUME COUNTDOWN. */
    rspeak_(i);
    return ret_val;

L19500:
    if (prsvec_1.prsi != oindex_1.torch || ! ((objcts_1.oflag1[oindex_1.torch 
	    - 1] & ONBT) != 0)) {
	goto L19600;
    }
    if ((objcts_1.oflag1[oindex_1.candl - 1] & ONBT) != 0) {
	goto L19700;
    }
/* 						!ALREADY ON? */
    newsta_(oindex_1.candl, 521, 0, 0, 0);
/* 						!NO, VAPORIZE. */
    return ret_val;

L19600:
    rspeak_(519);
/* 						!CANT LIGHT WITH THAT. */
    return ret_val;

L19700:
    rspeak_(520);
/* 						!ALREADY ON. */
    return ret_val;

L20000:
    if (obj != oindex_1.match) {
	bug_(6, obj);
    }
    if (prsvec_1.prsa != vindex_1.trnonw || prsvec_1.prso != oindex_1.match) {

	goto L20500;
    }
    if (findex_1.ormtch != 0) {
	goto L20100;
    }
/* 						!ANY MATCHES LEFT? */
    rspeak_(183);
/* 						!NO, LOSE. */
    return ret_val;

L20100:
    --findex_1.ormtch;
/* 						!DECREMENT NO MATCHES. */
    objcts_1.oflag1[oindex_1.match - 1] |= flobts;
    cevent_1.ctick[cindex_1.cevmat - 1] = 2;
/* 						!COUNTDOWN. */
    rspeak_(184);
    return ret_val;

L20500:
    if (prsvec_1.prsa != vindex_1.trnofw || (objcts_1.oflag1[oindex_1.match - 
	    1] & ONBT) == 0) {
	goto L10;
    }
    objcts_1.oflag1[oindex_1.match - 1] &= ~ flobts;
    cevent_1.ctick[cindex_1.cevmat - 1] = 0;
    rspeak_(185);
    return ret_val;

/* HERE FOR FALSE RETURN */

L10:
    ret_val = FALSE_;
    return ret_val;
} /* lightp_ */
