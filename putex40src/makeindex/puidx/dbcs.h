/*  FILE: dbcs.h
 *
 *  This file is part of
 *	PUIdx - A formatter and format independent DBCS index processor
 *
 *  Copyright (C) 1998 by Chey-Woei Tsay
 *
 *  Author:
 *	Chey-Woei Tsay (cwtsay@pu.edu.tw)
 *	Providence University
 *	Taiwan, ROC
 *
 *  Contributors:
 *	Please refer to the CONTRIB file that comes with this release
 *	for a list of people who have contributed to this and/or previous
 *	release(s) of MakeIndex.
 *
 *  All rights reserved by the copyright holders.  See the copyright
 *  notice distributed with this software for a complete description of
 *  the conditions under which it is made available.
 *
 */

#ifndef DBCS_H_
#define DBCS_H_

#define IS_DBCHAR (hi_byte != 0)

extern int hi_byte, low_byte;
extern unsigned char dbcs_sk[65536];

extern int getdbc (FILE *fp);
extern void setup_dbcs_sk (FILE *fp);

#endif