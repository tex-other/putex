% dvipdfmx_c.ch
@x
    Copyright (C) 2002-2003 by Jin-Hwan Cho and Shunsaku Hirata,
@y
    Copyright (C) 2004 by Chey-Woei Tsay (cwtsay@pu.edu.tw)

    Copyright (C) 2002-2003 by Jin-Hwan Cho and Shunsaku Hirata,
@z

@x
#include "pdfencrypt.h"
@y
#include "pdfencrypt.h"
/* add -- cdi2pdf */
#include "sfnt.h"
#include "tt_table.h"
#include "numbers.h"
/* end -- cdi2pdf */
@z

@x
  if (strlen (dvi_base) < 5 || strncmp (".dvi", dvi_base+strlen(dvi_base)-4, 4)) 
@y
/* mod -- cdi2pdf */
  if (strlen (dvi_base) < 5 || strncmp (".cdi", dvi_base+strlen(dvi_base)-4, 4)) 
/* end -- cdi2pdf */
@z

@x
  fprintf (stdout, "\nThis is %s-%s by the DVIPDFMx project team,\n", PACKAGE, VERSION);
  fprintf (stdout, "an extended version of dvipdfm-0.13.2c developed by Mark A. Wicks.\n");
  fprintf (stdout, "\nCopyright (C) 2002-2003 by the DVIPDFMx project team\n");
@y
/* mod -- cdi2pdf */
  fprintf (stdout, "\nThis is %s-%s by Chey-Woei Tsay,\n", PACKAGE, VERSION);
  fprintf (stdout, "an extended version of dvipdfm and dvipdfmx.\n");
  fprintf (stdout, "\nCopyright (C) 2002-2003 by the DVIPDFMx project team.\n");
  fprintf (stdout, "\nCopyright (C) 2004 by Chey-Woei Tsay.\n");
/* end -- cdi2pdf */
@z

@x
  fprintf (stdout, "-S          \tEnable PDF encryption\n");
@y
  fprintf (stdout, "-S          \tEnable PDF encryption\n");
/* add -- cdi2pdf */
  fprintf (stdout, "-E          \tEmbed fonts ignoring their copyright restrictions\n");
  fprintf (stdout, "-F TTF_file \tShow the ascent and descent of TTF_file\n");
/* end -- cdi2pdf */
@z

@x
static int pdfdecimaldigits = 3;
@y
static int pdfdecimaldigits = 3;
/* add -- cdi2pdf */
int ignore_copyright = 0;
static int ascent = 0, descent = 0;
/* end -- cdi2pdf */
@z

@x
static void do_args (int argc, char *argv[])
@y
/* add -- cdi2pdf */
#define PDFUNIT(x) ((double) ROUND((1000.0*(x)/(head->unitsPerEm)), 1))
void get_ascent_descent(char *name)
{
  char    *fullname;
  sfnt    *sfont;
  unsigned long   offset = 0;
  int font_index = 0;  /* Fix me!! */
  struct tt_head_table *head;
  struct tt_os2__table *os2;

  if ((fullname = kpse_find_file(name, kpse_truetype_format, 1)) == NULL) {
  	fprintf(stderr, "\nCan't locate TTF font file: %s\n", name);
  	exit(1);
  }
  if ((sfont = sfnt_open(fullname)) == NULL) {
  	fprintf(stderr, "\nCan't open TTF font file: %s\n", name);
  	exit(1);
  }
  switch (sfont->type) {
  case SFNT_TYPE_TTC:
    offset = ttc_read_offset(sfont, font_index);
    break;
  case SFNT_TYPE_TRUETYPE:
    if (font_index > 0) {
  	  fprintf(stderr, "\nCan't locate TTF font file: %s\n", name);
  	  exit(1);
    } else
      offset = 0;
    break;
  default:
    sfnt_close(sfont);
  	fprintf(stderr, "\nUnknown TrueType format: %s\n", name);
  	exit(1);
  }
  if (sfnt_read_table_directory(sfont, offset) < 0)
    ERROR("Reading TrueType table directory failed.");
  
  os2  = tt_read_os2__table(sfont);
  head = tt_read_head_table(sfont);
  ascent = PDFUNIT(os2->sTypoAscender);
  descent = PDFUNIT(os2->sTypoDescender);
}
/* end -- cdi2pdf */

static void do_args (int argc, char *argv[])
@z

@x
      default:
@y
/* add -- cdi2pdf */
      case 'E':
        ignore_copyright = 1;
        break;
      case 'F':
        if (argc < 2) {
	      fprintf (stderr, "\missing truetype font file\n\n");
	      usage();
	    }
	    get_ascent_descent(argv[1]);
	    printf("\n%s -a %d -d %d\n", argv[1], ascent, descent);
      	exit(0);
/* end -- cdi2pdf */
      default:
@z
@x
    fprintf (stderr, "\nMultiple dvi filenames?\n\n");
@y
/* mod -- cdi2pdf */
    fprintf (stderr, "\nMultiple cdi filenames?\n\n");
/* end -- cdi2pdf */
@z

@x
    if (strncmp (".dvi", argv[0]+strlen(argv[0])-4, 4)) {
@y
/* mod -- cdi2pdf */
    if (strncmp (".cdi", argv[0]+strlen(argv[0])-4, 4)) {
/* end -- cdi2pdf */
@z

@x
      strcat (dvi_filename, ".dvi");
@y
/* mod -- cdi2pdf */
      strcat (dvi_filename, ".cdi");
/* end -- cdi2pdf */
@z

@x
static char *config_file_name = "dvipdfmx.cfg";
@y
/* mod -- cdi2pdf */
static char *config_file_name = "cdi2pdf.cfg";
/* end -- cdi2pdf */
@z

@x
    fprintf (stderr, "\nNo dvi filename specified.\n\n");
@y
/* mod -- cdi2pdf */
    fprintf (stderr, "\nNo cdi filename specified.\n\n");
/* end -- cdi2pdf */
@z

@x
  /* Check for ".dvi" at end of argument name */
@y
  /* Check for ".cdi" at end of argument name */
@z