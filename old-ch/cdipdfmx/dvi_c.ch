%%
%% dvi_c.ch -- the change file for __dvi.c 
%%
@x
    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.
@y
		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		!!!  Don't modify this file. It is generated from __dvi.c and dvi_c.ch
		!!!  using the tie program.
		!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		
    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.
@z
@x
    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
@y
    Copyright (C) 2004 by Chey-Woei Tsay (cwtsay@pu.edu.tw)

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
@z

@x
static FILE *dvi_file;
@y
/* add -- cdi2pdf */
static int cdi_id, cdi_charset;
/* end -- cdi2pdf */
static FILE *dvi_file;
@z

@x
} *def_fonts = NULL;
@y
} *def_fonts = NULL;

/* add -- cdi2pdf */
static struct cjkfont_def
{
  signed long tex_id;
  char *name;
  Ubyte charset;
  spt_t size;
  spt_t dsize;
  short weight;
  Ubyte style;
  SIGNED_QUAD width, height, depth;
  SIGNED_QUAD fw_width, fw_height, fw_depth;
  int dev_font_id; /* index of device font in dev_fonts array */
  int map_id;
  int used;
  spt_t v_offset; /* vertical offset */
} *def_cjkfonts = NULL;
/* end -- cdi2pdf */
@z

@x
static int num_def_fonts = 0, max_def_fonts = 0;
@y
static int num_def_fonts = 0, max_def_fonts = 0;
/* add -- cdi2pdf */
static int num_def_cjkfonts = 0, max_def_cjkfonts = 0;
/* end -- cdi2pdf */
@z

@x
  ERROR ("dvi_init:  Something is wrong.  Are you sure this is a DVI file?\n");
@y
/* mod -- cdi2pdf */
  ERROR ("dvi_init:  Something is wrong.  Are you sure this is a CDI file?\n");
/* end -- cdi2pdf */
@z

%%
%% static void find_post (void)
%%
@x
  if (dvi_file_size - current < 4 ||
      current == 0 || !(read_byte == DVI_ID || read_byte == DVIV_ID)) {
    fprintf (stderr, "DVI ID = %d\n", read_byte);
@y
/* mod -- cdi2pdf */
  cdi_id = read_byte;
  if (dvi_file_size - current < 4 || current == 0 || read_byte != CDI_ID) {
    if (read_byte == OLD_CDI_ID)
      fprintf (stderr, "Old CDI file format is not supported by cdi2pdf\n");
    fprintf (stderr, "CDI ID = %d\n", read_byte);
/* end -- cdi2pdf */
@z

@x
  current = current - 5;
@y
  current = current - 6;
@z

@x
static void get_a_font_record (SIGNED_QUAD tex_id)
@y
/* add -- cdi2pdf */
static void dump_cjkfont_info (void)
{
  unsigned i;
  fprintf (stderr, "\nCDI file cjkfont info\n");
  for (i=0; i<num_def_cjkfonts; i++) {
    fprintf (stderr, "name: %10s, ", def_cjkfonts[i].name);
    fprintf (stderr, "TeX/CDI ID: %5ld, ", def_cjkfonts[i].tex_id);
    fprintf (stderr, "charset ID: %5ld, ", def_cjkfonts[i].charset);
    fprintf (stderr, "size: %5.2f pt, ", def_cjkfonts[i].size*dvi2pts);
    fprintf (stderr, "weight: %d, ", def_cjkfonts[i].weight);
    fprintf (stderr, "style: %d, ", def_cjkfonts[i].style);
    fprintf (stderr, "width: %5.2f pt, ", def_cjkfonts[i].width*dvi2pts);
    fprintf (stderr, "height: %5.2f pt, ", def_cjkfonts[i].height*dvi2pts);
    fprintf (stderr, "depth: %5.2f pt, ", def_cjkfonts[i].depth*dvi2pts);
    /*
    fprintf (stderr, "fw_width: %5.2f pt, ", def_cjkfonts[i].fw_width*dvi2pts);
    fprintf (stderr, "fw_height: %5.2f pt, ", def_cjkfonts[i].fw_height*dvi2pts);
    fprintf (stderr, "fw_depth: %5.2f pt, ", def_cjkfonts[i].fw_depth*dvi2pts);
    */
    fprintf (stderr, "\n");
  }
}
/* end -- cdi2pdf */
static void get_a_font_record (SIGNED_QUAD tex_id)
@z

@x
static void get_dvi_fonts (void)
@y
/* add -- cdi2pdf */
static void get_a_cjkfont_record (void)
{
  UNSIGNED_BYTE name_length;
  char *name;
  struct cjkfont_def *this_cjkfont;

#ifdef DEBUG
  fprintf (stderr, "get_a_cjkfont_record\n");
#endif
  if (num_def_cjkfonts >= max_def_cjkfonts) {
    max_def_cjkfonts += MAX_FONTS;
    def_cjkfonts = RENEW (def_cjkfonts, max_def_cjkfonts, struct cjkfont_def);
  }
  
  this_cjkfont = def_cjkfonts + num_def_cjkfonts; /* should be set after RENEW */

	this_cjkfont->tex_id = get_unsigned_pair (dvi_file);
	
  name_length = get_unsigned_byte (dvi_file);
  name = NEW (name_length+1, char);
  if (fread (name, 1, name_length, dvi_file) != name_length) {
    invalid_signature();
  }
  name[name_length] = 0;
  
  
  this_cjkfont->name = name;
  this_cjkfont->charset = get_unsigned_byte (dvi_file);
  this_cjkfont->size = get_unsigned_quad (dvi_file);
  this_cjkfont->dsize = get_unsigned_quad (dvi_file);
  this_cjkfont->weight = get_unsigned_pair (dvi_file);
  this_cjkfont->style = get_unsigned_byte (dvi_file);
  this_cjkfont->width = get_signed_quad (dvi_file);
  this_cjkfont->height = get_signed_quad (dvi_file);
  this_cjkfont->depth = get_signed_quad (dvi_file);
  this_cjkfont->fw_width = get_signed_quad (dvi_file);
  this_cjkfont->fw_height = get_signed_quad (dvi_file);
  this_cjkfont->fw_depth = get_signed_quad (dvi_file);
  this_cjkfont->dev_font_id = -1;
  this_cjkfont->map_id = -1;
  this_cjkfont->used = 0;
  this_cjkfont->v_offset = 0;

  num_def_cjkfonts +=1;
  return;
}
/* end -- cdi2pdf */
static void get_dvi_fonts (void)
@z

@x
  while ((code = get_unsigned_byte(dvi_file)) != POST_POST) {
@y
/* mod -- cdi2pdf */
  while (1) {
  	code = get_unsigned_byte(dvi_file);
    if (code == CFNT_DEF || code == POST_POST) break;
/* end -- cdi2pdf */
@z

@x
  if (verbose > 2) dump_font_info();
@y
/* add -- cdi2pdf */
  while (code == CFNT_DEF) {
  	get_a_cjkfont_record();
  	code = get_unsigned_byte(dvi_file);
  }
/* end -- cdi2pdf */
/* mod -- cdi2pdf */
  if (verbose > 2) {
  	dump_font_info();
  	dump_cjkfont_info();
  }
/* end -- cdi2pdf */
@z

@x
static void get_comment (void)
@y
/* add -- cdi2pdf */
/*
 * TODO
 */
enum {UCS = 0, BIG5, GBK, Shift_JIS, KS_C_5601_1992, Unknown_Enc};
static void get_cdi_charset (void)
{	
  seek_absolute (dvi_file, 2);
  cdi_charset = get_unsigned_byte(dvi_file);
}
/* end -- cdi2pdf */
static void get_comment (void)
@z

@x
  seek_absolute (dvi_file, 14);
@y
/* mod -- cdi2pdf */
  seek_absolute (dvi_file, 15);
/* end -- cdi2pdf */
@z

@x
static int current_font;
@y
/* mod -- cdi2pdf */
static int current_font, current_cjkfont;
/* end -- cdi2pdf */
@z


%%
%% static void clear_state (void)
%%
@x
  current_font = -1;
@y
  current_font = -1;
/* add -- cdi2pdf */
  current_cjkfont = -1;
/* end -- cdi2pdf */
@z

@x
int dvi_locate_font (char *tfm_name, spt_t ptsize)
@y
/* add -- cdi2pdf */
void dvi_locate_cjkfont (int cjkfont_idx)
{
  int thisfont, map_id, subfont_id, dev_font_id;
  int ttf_ascent, ttf_descent;
  double tex_depth_ratio, ttf_depth_ratio;
  char *tex_name;
  
  struct cjkfont_def *this_cjkfont = def_cjkfonts + cjkfont_idx;
  
  map_id = get_map_record(this_cjkfont->name, &subfont_id);
    if (map_id < 0) {
    fprintf(stderr, "** Fatal error: Could not map '%s' to a physical CJK font file.", this_cjkfont->name);
    ERROR ("\n");
  }
  if ((dev_font_id = dev_locate_font(this_cjkfont->name, this_cjkfont->size, map_id)) < 0) {
    fprintf(stderr, "** Fatal error: Could not locate a CJK font file for '%s'.", this_cjkfont->name);
    ERROR ("\n");
  }
  this_cjkfont->dev_font_id = dev_font_id;
  this_cjkfont->map_id = map_id;
  
  /* compute v_offset */
  ttf_ascent = fontmap_ascent (map_id);
  ttf_descent = -fontmap_descent (map_id); /* should be positive now */
  ttf_depth_ratio = ((double) ttf_descent) / (ttf_ascent + ttf_descent);
  this_cjkfont->v_offset = this_cjkfont->depth - this_cjkfont->size * ttf_depth_ratio;
}
/* end -- cdi2pdf */
int dvi_locate_font (char *tfm_name, spt_t ptsize)
@z

@x
void dvi_set (SIGNED_QUAD ch)
@y
/* add -- cdi2pdf */
static void dvi_cjkset ()
{
  spt_t width, height = 0, depth = 0;
  unsigned char wch[2] = {'\0', '\0'};
  struct cjkfont_def *p;
	
  wch[0] = get_unsigned_byte(dvi_file);
  wch[1] = get_unsigned_byte(dvi_file);
  
  p = def_cjkfonts+current_cjkfont;
  width = sqxfw (p->size, p->fw_width);
  dev_set_string (dvi_state.h, -(dvi_state.v + p->v_offset), wch, 2, width, p->dev_font_id, 2);
  if (compute_boxes) {
    height = sqxfw (p->size, p->fw_height);
    depth = sqxfw (p->size, p->fw_depth);
    dev_expand_box (width, height, depth);
  }
  if (!dvi_state.d) {
    dvi_state.h += width;
  } else {
    dvi_state.v += width;
  }
}
/* end -- cdi2pdf */
void dvi_set (SIGNED_QUAD ch)
@z
@x
void dvi_put (SIGNED_QUAD ch)
@y
/* add -- cdi2pdf */
void dvi_cjkput ()
{
  spt_t width, height = 0, depth = 0;
  unsigned char wch[2] = {'\0', '\0'};
  struct cjkfont_def *p;
  
  wch[0] = get_unsigned_byte(dvi_file);
  wch[1] = get_unsigned_byte(dvi_file);
  
  p = def_cjkfonts+current_cjkfont;
  width = sqxfw (p->size, p->fw_width);
  dev_set_string (dvi_state.h, -dvi_state.v, wch, 2, width, p->dev_font_id, 2);
  if (compute_boxes) {
    height = sqxfw (p->size, p->fw_height);
    depth = sqxfw (p->size, p->fw_depth);
    dev_expand_box (width, height, depth);
  }
}
/* end -- cdi2pdf */
void dvi_put (SIGNED_QUAD ch)
@z

@x
static void do_set2(void)
{
  dvi_set (get_unsigned_pair(dvi_file));
}
@y
/* mod -- cdi2pdf */
#if 0
static void do_set2(void)
{
  dvi_set (get_unsigned_pair(dvi_file));
}
#endif
/* end -- cdi2pdf */
@z

@x
static void do_put2(void)
{
  dvi_put (get_unsigned_pair(dvi_file));
}
@y
/* mod -- cdi2pdf */
#if 0
static void do_put2(void)
{
  dvi_put (get_unsigned_pair(dvi_file));
}
#endif
/* end -- cdi2pdf */
@z

@x
static void do_fntdef(void)
@y
/* add -- cdi2pdf */
static void do_cjkfntdef(void)
{
  int name_len, i;
  
  get_unsigned_pair(dvi_file); /* cjkfont num */
  name_len =   get_unsigned_byte(dvi_file);

  seek_relative(dvi_file, name_len + 36); /* skip the cjkfont data */
}

/* end -- cdi2pdf */
static void do_fntdef(void)
@z

@x
static void do_fnt (SIGNED_QUAD tex_id)
@y
/* add -- cdi2pdf */
static void do_cjkfnt ()
{
	int i, tex_id = get_unsigned_pair(dvi_file);
	
  for (i=0; i<num_def_cjkfonts; i++) {
    if (def_cjkfonts[i].tex_id == tex_id)
      break;
  }
  if (i == num_def_cjkfonts) {
    fprintf (stderr, "cjkfontid: %ld\n", tex_id);
    ERROR ("dvi_do_cjkfnt:  Tried to select a cjk font that hasn't been defined");
  }
  if (!def_cjkfonts[i].used) {
  	def_cjkfonts[i].used= 1;
  	dvi_locate_cjkfont (i);
  }
	current_cjkfont = i;
}
/* end -- cdi2pdf */
static void do_fnt (SIGNED_QUAD tex_id)
@z

%%
%% dvi_do_page(unsigned n)
%%
@x
    /* If we are here, we have an opcode that is something
       other than SET_CHAR */
@y
    /* If we are here, we have an opcode that is something
       other than SET_CHAR */
/* add -- cdi2pdf */
	if (opcode == SET2) {
		dvi_cjkset();
		continue;
	}
	if (opcode == PUT2) {
		dvi_cjkput();
		continue;
	}
    if (opcode == CFNT) {
      do_cjkfnt ();
      continue;
    }
	if (opcode == CFNT_DEF) {
		do_cjkfntdef();
		continue;
	}
		
/* end -- cdi2pdf */
@z

@x
      case SET2:
	do_set2();
	break;
@y
/* mod -- cdi2pdf */
#if 0
      case SET2:
	do_set2();
	break;
#endif
/* end -- cdi2pdf */
@z

@x
      case PUT2:
	do_put2();
	break;
@y
/* mod -- cdi2pdf */
#if 0
      case PUT2:
	do_put2();
	break;
#endif
/* end -- cdi2pdf */
@z

@x
  get_comment();
@y
/* add -- cdi2pdf */
	get_cdi_charset();
/* end -- cdi2pdf */
  get_comment();
@z

%%
%% dvi_paper_size
%%
@x
      case SET2: case PUT2: case RIGHT2: case DOWN2: case W2: case X2: case Y2: case Z2: case FNT2:
@y
/* mod -- cdi2pdf */
      case CFNT: case SET2: case PUT2: case RIGHT2: case DOWN2: case W2: case X2: case Y2: case Z2: case FNT2:
/* end -- cdi2pdf */
@z

@x
      case FNT_DEF1:
@y
/* add -- cdi2pdf */
			case CFNT_DEF: do_cjkfntdef(); break;
/* end -- cdi2pdf */
      case FNT_DEF1:
@z