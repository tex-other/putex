/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/cmap.h,v 1.1 2003/12/27 18:03:16 csc Exp $

    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
    the dvipdfmx project team <dvipdfmx@project.ktug.or.kr>

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307 USA.
*/

#ifndef _CMAP_H_
#define _CMAP_H_

#include "cid.h"

#include <sys/types.h>

typedef unsigned short CID;
typedef unsigned short UCV16;

/* Limits */
#define CID_MAX_CID  65535
#define CID_MAX      CID_MAX_CID

#define CID_NOTDEF_CHAR   "\0\0"
#define CID_NOTDEF        0

#define UCS_NOTDEF_CHAR   "\377\375"
#define UCS_NOTDEF        0xfffd

/*
 * TYPE_IDENTITY and TYPE_CID_TO_CODE is not defined in the CMap spec.
 */
#define CMAP_TYPE_IDENTITY    0
#define CMAP_TYPE_CODE_TO_CID 1
#define CMAP_TYPE_TO_UNICODE  2
#define CMAP_TYPE_CID_TO_CODE 3

#define CMAP_PROF_TYPE_INBYTES_MIN  0
#define CMAP_PROF_TYPE_INBYTES_MAX  1
#define CMAP_PROF_TYPE_OUTBYTES_MIN 2
#define CMAP_PROF_TYPE_OUTBYTES_MAX 3

typedef struct CMap CMap;



/************************** CMAP_MAIN **************************/

void CMap_set_verbose (void);

extern CMap  *CMap_new     (void);
extern void   CMap_release (CMap *cmap);

extern int    CMap_is_valid    (CMap *cmap);
extern int    CMap_is_Identity (CMap *cmap);
extern int    CMap_get_profile (CMap *cmap, int type);

extern CIDSysInfo *CMap_get_CIDSysInfo(CMap *cmap);
extern char       *CMap_get_name (CMap *cmap);
extern int         CMap_get_type (CMap *cmap);
extern int         CMap_get_wmode(CMap *cmap);

extern void   CMap_set_name   (CMap *cmap, char *name);
extern void   CMap_set_type   (CMap *cmap, int type);
extern void   CMap_set_wmode  (CMap *cmap, int wmode);
extern void   CMap_set_CIDSysInfo (CMap *cmap, CIDSysInfo *csi);
extern void   CMap_set_usecmap(CMap *cmap, CMap *ucmap);

/* charName not supported */
extern int   CMap_add_bfchar  (CMap *cmap, unsigned char *src, int srcdim, unsigned char *dest, int destdim);
extern int   CMap_add_cidchar (CMap *cmap, unsigned char *src, int srcdim, CID dest);
extern int   CMap_add_bfrange (CMap *cmap,
			       unsigned char *srclo, unsigned char *srchi, int srcdim,
			       unsigned char *dest, int destdim);
extern int   CMap_add_cidrange(CMap *cmap,
			       unsigned char *srclo, unsigned char *hi, int srcdim,
			       CID base);

extern int CMap_add_notdefchar  (CMap *cmap, unsigned char *src, int srcdim, CID dst);
extern int CMap_add_notdefrange (CMap *cmap, unsigned char *srclo, unsigned char *srchi, int srcdim, CID dst);

extern int  CMap_add_codespacerange (CMap *cmap, unsigned char *codelo, unsigned char *codehi, int dim);
extern int  CMap_match_codespace    (CMap *cmap, unsigned char *c, int dim);

extern long CMap_decode (CMap *cmap,
			 unsigned char **inbuf,  long *inbytesleft,
			 unsigned char **outbuf, long *outbytesleft);

/* Convenience functin */
extern unsigned short CMap_lookup_char2 (CMap *cmap, unsigned short code);

/************************** CMAP_CACHE **************************/

extern void        CMap_cache_init        (void);
extern CMap       *CMap_cache_get         (int id);
extern int         CMap_cache_find        (const char *cmap_name);
extern void        CMap_cache_close       (void);

/************************** CMAP_COMPAT **************************/
#ifndef WITHOUT_CMAP_COMPAT

#define CMAP_NO_INVALID      (1 << 1)
#define CMAP_NO_MISSING      (1 << 2)
#define CMAP_NO_WARN_INVALID (1 << 3)
#define CMAP_NO_WARN_MISSING (1 << 4)

#define CMAP_IDENTITY_H 0
#define CMAP_IDENTITY_V 1

extern void cmap_set_lookup (int flag); /* set flag */

/* be verbose */
extern void cmap_set_verbose (void);

/* read or release CMap tables */
extern int  get_cmap (const char *cmap_name);
extern void cmap_close_all (void);

/* accessing CMap info */
extern char *cmap_cmapname (int cmap_id);
extern int   cmap_wmode (int cmap_id);
extern CIDSysInfo *cmap_sysinfo (int cmap_id);

/* lookup character mapping table */
extern unsigned short cmap_lookup (unsigned char hi, unsigned char lo,
				   int cmap_id);
#endif /* WITOUT_CMAP_COMPAT */

#endif /* _CMAP_H_ */
