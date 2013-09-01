%% tt_aux_c.ch -- change file for tt_aux.c
@x
    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.
@y
		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		!!!  Don't modify this file. It is generated from __tt_aux.c and tt_aux_c.ch
		!!!  using the tie program.
		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
    This is cdipdfmx, an eXtended version of dvipdfmx.
@z

@x
ULONG ttc_read_offset (sfnt *sfont, int face)
@y
/* add -- cdipdfmx */
extern int ignore_copyright; /* defined in dvipdfmx.c */
/* end -- cdipdfmx */

ULONG ttc_read_offset (sfnt *sfont, int face)
@z

@x
      fprintf(stderr, "\n*** Embedding disabled due to licensing restriction ***\n");
      *embed = 0;
@y
/* add -- cdipdfmx */
      if (ignore_copyright)
        fprintf(stderr, "\n*** It is illegal to embed a copyright font ***\n");
      else
        fprintf(stderr, "\n*** Embedding disabled due to licensing restriction ***\n");
      *embed = ignore_copyright;
/* end -- cdipdfmx */
@z
