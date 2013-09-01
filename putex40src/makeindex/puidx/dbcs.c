/*
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

#include <stdio.h>
#include "dbcs.h"
#include "mkind.h"

typedef struct {
	unsigned short code;
	unsigned short sk;
} entity;

int hi_byte, low_byte;	/* of the current double-byte character */

unsigned char dbcs_sk[65536];

int getdbc (FILE *fp)
{
  hi_byte = getc(fp);
  if (hi_byte > 127) {
    low_byte = getc(fp);
    return hi_byte * 256 + low_byte;
  }
  else if (hi_byte == EOF)
    return EOF;
  else {
    low_byte = hi_byte;
    hi_byte = 0;
    return low_byte;
  }
}

void setup_dbcs_sk (FILE *fp)
{
	entity e;

	memset(dbcs_sk, 0, 65536);
	fread(&e, sizeof(entity), 1, fp);
	while (!feof(fp)) {
		dbcs_sk[e.code] = e.sk+'A'-1;  /* sk can't be larger than 60 */
		fread(&e, sizeof(entity), 1, fp);
	}
}
