/* GHERE--	IS GLOBAL ACTUALLY IN THIS ROOM? */

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

logical ghere_(obj, rm)
integer obj;
integer rm;
{
    /* System generated locals */
    logical ret_val;

    ret_val = TRUE_;
/* 						!ASSUME WINS. */
    switch (obj - star_1.strbit) {
	case 1:  goto L1000;
	case 2:  goto L1000;
	case 3:  goto L1000;
	case 4:  goto L1000;
	case 5:  goto L1000;
	case 6:  goto L1000;
	case 7:  goto L1000;
	case 8:  goto L1000;
	case 9:  goto L1000;
	case 10:  goto L1000;
	case 11:  goto L1000;
	case 12:  goto L2000;
	case 13:  goto L3000;
	case 14:  goto L4000;
	case 15:  goto L5000;
	case 16:  goto L5000;
	case 17:  goto L5000;
	case 18:  goto L6000;
	case 19:  goto L7000;
	case 20:  goto L8000;
	case 21:  goto L9000;
	case 22:  goto L9100;
	case 23:  goto L8000;
	case 24:  goto L10000;
	case 25:  goto L11000;
    }
    bug_(60, obj);

/* 1000--	STARS ARE ALWAYS HERE */

L1000:
    return ret_val;

/* 2000--	BIRD */

L2000:
    ret_val = rm >= rindex_1.fore1 && rm < rindex_1.clear || rm == 
	    rindex_1.mtree;
    return ret_val;

/* 3000--	TREE */

L3000:
    ret_val = rm >= rindex_1.fore1 && rm < rindex_1.clear && rm != 
	    rindex_1.fore3;
    return ret_val;

/* 4000--	NORTH WALL */

L4000:
    ret_val = rm >= rindex_1.bkvw && rm <= rindex_1.bkbox || rm == 
	    rindex_1.cpuzz;
    return ret_val;

/* 5000--	EAST, SOUTH, WEST WALLS */

L5000:
    ret_val = rm >= rindex_1.bkvw && rm < rindex_1.bkbox || rm == 
	    rindex_1.cpuzz;
    return ret_val;

/* 6000--	GLOBAL WATER */

L6000:
    ret_val = (rooms_1.rflag[rm - 1] & RWATER + RFILL) != 0;
    return ret_val;

/* 7000--	GLOBAL GUARDIANS */

L7000:
    ret_val = rm >= rindex_1.mrc && rm <= rindex_1.mrd || rm >= 
	    rindex_1.mrce && rm <= rindex_1.mrdw || rm == rindex_1.inmir;
    return ret_val;

/* 8000--	ROSE/CHANNEL */

L8000:
    ret_val = rm >= rindex_1.mra && rm <= rindex_1.mrd || rm == 
	    rindex_1.inmir;
    return ret_val;

/* 9000--	MIRROR */
/* 9100		PANEL */

L9100:
    if (rm == rindex_1.fdoor) {
	return ret_val;
    }
/* 						!PANEL AT FDOOR. */
L9000:
    ret_val = rm >= rindex_1.mra && rm <= rindex_1.mrc || rm >= 
	    rindex_1.mrae && rm <= rindex_1.mrcw;
    return ret_val;

/* 10000--	MASTER */

L10000:
    ret_val = rm == rindex_1.fdoor || rm == rindex_1.ncorr || rm == 
	    rindex_1.parap || rm == rindex_1.cell;
    return ret_val;

/* 11000--	LADDER */

L11000:
    ret_val = rm == rindex_1.cpuzz;
    return ret_val;

} /* ghere_ */

/* MRHERE--	IS MIRROR HERE? */

/* DECLARATIONS */

integer mrhere_(rm)
integer rm;
{
    /* System generated locals */
    integer ret_val, i__1;

    if (rm < rindex_1.mrae || rm > rindex_1.mrdw) {
	goto L100;
    }

/* RM IS AN E-W ROOM, MIRROR MUST BE N-S (MDIR= 0 OR 180) */

    ret_val = 1;
/* 						!ASSUME MIRROR 1 HERE. */
    if ((rm - rindex_1.mrae) % 2 == findex_1.mdir / 180) {
	ret_val = 2;
    }
    return ret_val;

/* RM IS NORTH OR SOUTH OF MIRROR.  IF MIRROR IS N-S OR NOT */
/* WITHIN ONE ROOM OF RM, LOSE. */

L100:
    ret_val = 0;
    if ((i__1 = findex_1.mloc - rm, abs(i__1)) != 1 || findex_1.mdir % 180 ==
	     0) {
	return ret_val;
    }

/* RM IS WITHIN ONE OF MLOC, AND MDIR IS E-W */

    ret_val = 1;
    if (rm < findex_1.mloc && findex_1.mdir < 180 || rm > findex_1.mloc && 
	    findex_1.mdir > 180) {
	ret_val = 2;
    }
    return ret_val;
} /* mrhere_ */
