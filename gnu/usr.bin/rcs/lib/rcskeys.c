/* RCS keyword table and match operation */

/* Copyright 1982, 1988, 1989 Walter Tichy
   Copyright 1990, 1991, 1992, 1993 Paul Eggert
   Distributed under license by the Free Software Foundation, Inc.

This file is part of RCS.

RCS is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2, or (at your option)
any later version.

RCS is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with RCS; see the file COPYING.  If not, write to
the Free Software Foundation, 675 Mass Ave, Cambridge, MA 02139, USA.

Report problems and direct all questions to:

    rcs-bugs@cs.purdue.edu

*/

/* $Log: rcskeys.c,v $
/* Revision 1.3  1996/05/31 13:04:51  deraadt
/* retain -Z options to rcs commands even though they do not work (rcs
/* commands spawn children which do not inherit the -Z option...) but also
/* look in the RCSLOCALID environment variable. cvs sets this to "OpenBSD",
/* as read from the options file.
/*
/* Revision 1.2  1996/04/19 12:40:09  mickey
/* -L<string> option added to support LOCALID behaviour.
/* maybe set up in RCSINIT environment variable.
/*
/* Revision 1.1.1.1  1995/10/18 08:41:02  deraadt
/* initial import of NetBSD tree
/*
/* Revision 1.6  1995/02/24 02:25:08  mycroft
/* RCS 5.6.7.4
/*
 * Revision 5.3  1993/11/03 17:42:27  eggert
 * Add Name keyword.
 *
 * Revision 5.2  1991/08/19  03:13:55  eggert
 * Say `T const' instead of `const T'; it's less confusing for pointer types.
 * (This change was made in other source files too.)
 *
 * Revision 5.1  1991/04/21  11:58:25  eggert
 * Don't put , just before } in initializer.
 *
 * Revision 5.0  1990/08/22  08:12:54  eggert
 * Add -k.  Ansify and Posixate.
 *
 * Revision 4.3  89/05/01  15:13:02  narten
 * changed copyright header to reflect current distribution rules
 * 
 * Revision 4.2  87/10/18  10:36:33  narten
 * Updating version numbers. Changes relative to 1.1 actuallyt
 * relative to 4.1
 * 
 * Revision 1.2  87/09/24  14:00:10  narten
 * Sources now pass through lint (if you ignore printf/sprintf/fprintf 
 * warnings)
 * 
 * Revision 4.1  83/05/04  10:06:53  wft
 * Initial revision.
 * 
 */


#include "rcsbase.h"

libId(keysId, "$Id: rcskeys.c,v 1.3 1996/05/31 13:04:51 deraadt Exp $")

char local_id[keylength+1];
char const *Keyword[] = {
    /* This must be in the same order as rcsbase.h's enum markers type. */
	0,
	AUTHOR, DATE, HEADER, IDH,
	LOCKER, LOG, NAME, RCSFILE, REVISION, SOURCE, STATE,
	NULL,
};


	void
setRCSlocalId(string)
	char const *string;
{
	static int num;

	if (strlen(string) > keylength)
		error("LocalId is too long");
	strcpy(local_id, string);
	Keyword[LocalId] = local_id;

	if (num++)
		setenv("RCSLOCALID", local_id, 1);

}

	enum markers
trymatch(string)
	char const *string;
/* function: Checks whether string starts with a keyword followed
 * by a KDELIM or a VDELIM.
 * If successful, returns the appropriate marker, otherwise Nomatch.
 */
{
        register int j;
	register char const *p, *s;
	for (j = sizeof(Keyword)/sizeof(*Keyword);  (--j);  )
		if (Keyword[j]) {
			/* try next keyword */
			p = Keyword[j];
			s = string;
			while (*p++ == *s++) {
				if (!*p)
			    	switch (*s) {
					case KDELIM:
					case VDELIM:
				    	return (enum markers)j;
					default:
				    	return Nomatch;
			    	}
			}
        	}
        return(Nomatch);
}

