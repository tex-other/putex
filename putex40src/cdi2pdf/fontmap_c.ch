%% fontmap_c.ch
@x
    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
@y
    Copyright (C) 2004 by Chey-Woei Tsay (cwtsay@pu.edu.tw)

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
@z

@x
static unsigned char verbose = 0;
@y
/* add -- cdipdfmx */
static char *doc_encoding = NULL;
/* end -- cdipdfmx */
static unsigned char verbose = 0;
@z

@x
  int mapc;
@y
  int mapc;
/* add -- cdipdfmx */
  int ascent, descent; /* sTypoAscender and sTypoDescender of the TTF font. */
/* end -- cdipdfmx */
@z

@x
static void init_map_record (struct map_record *r)
@y
/* add -- cdipdfmx */
/* defalut ascent and descent values for most CJK fonts */
#define DEFAULT_CJK_ASCENT 801
#define DEFAULT_CJK_DESCENT -199
/* end -- cdipdfmx */

static void init_map_record (struct map_record *r)
@z

%% init_map_record (...)
@x
  r->mapc  = -1;
@y
  r->mapc  = -1;
/* add -- cdipdfmx */
  r->ascent = DEFAULT_CJK_ASCENT;
  r->descent = DEFAULT_CJK_DESCENT;
/* end -- cdipdfmx */
@z

@x
void read_mapfile (char *filename)
@y
/* add -- cdipdfmx */
void set_doc_encoding (char *enc_name)
{
	doc_encoding = strdup(enc_name);
}
#if 0
void read_cdipdfmx_mapfile (char *filename)
{
  FILE *mapfile = NULL;
  char *full_map_filename, *start = NULL, *end, *p, *tex_name;

#ifdef MIKTEX
  if (!miktex_find_app_input_file("dvipdfm", filename, full_map_filename = work_buffer))
    full_map_filename = NULL;
#else
  full_map_filename = kpse_find_file(filename, kpse_program_text_format, 0);
#endif
  if (full_map_filename == NULL || (mapfile = MFOPEN (full_map_filename, FOPEN_R_MODE)) == NULL)
    fprintf(stderr, "**Warning: Couldn't open the font map file '%s'.\n", filename);

	if (mapfile) {
    while ((start = mfgets(work_buffer, WORK_BUFFER_SIZE, mapfile)) != NULL) {
      end = work_buffer + strlen(work_buffer);
      skip_white(&start, end);
      if (start >= end) continue;
      if (*start == '%') continue;
      if ((tex_name = parse_dbcs_name(&start, end)) == NULL) continue;
      fontmaps_need(num_fontmap + 1);
      init_map_record(fontmap + num_fontmap);
      
			fontmap[num_fontmap].tex_name = tex_name;
#if 0
			fontmap[num_fontmap].enc_name = strdup(doc_encoding);
      skip_white(&start, end);
			fontmap[num_fontmap].font_name = parse_val_ident(&start, end);
#endif
      skip_white(&start, end);
      if (*start != '-') { /* May be NULL */
				fontmap[num_fontmap].enc_name = parse_ident(&start, end);
				skip_white(&start, end);
      }
      if (*start != '-') { /* May be NULL */
      /* parse_val_ident() allows inclusion of '/' */
      	fontmap[num_fontmap].font_name = parse_val_ident(&start, end);
      	skip_white(&start, end);
      }
	    num_fontmap += 1;
    }
	}
}
#endif
/* end -- cdipdfmx */
void read_mapfile (char *filename)
@z

@x
      if ((tex_name = parse_ident(&start, end)) == NULL) continue;
@y
/* mod -- cdipdfmx */
      if ((tex_name = parse_dbcs_name(&start, end)) == NULL) continue;
/* end -- cdipdfmx */
@z

@x
      if ((p = strchr(tex_name, '@')) && p[1] != '\0') {
@y
/* mod -- cdipdfmx (make sure @ is not a part of dbcs code) */
      if ((p = strchr(tex_name, '@')) && p[1] != '\0' && (p == tex_name || p[-1] & 0x80 == 0)) {
/* end -- cdipdfmx */
@z

@x
	default:
@y
/* add -- cdipdfmx */
	case 'a': /* ascent option */
	  start += 2;
	  skip_white(&start, end);
	  if (start < end && (number = parse_number(&start, end))) {
	    fontmap[num_fontmap].ascent = atoi(number);
	    RELEASE (number);
	  } else
	    WARN("Missing ascent value in map file (%s).", tex_name);
	  break;
	case 'd': /* descent option */
	  start += 2;
	  skip_white(&start, end);
	  if (start < end && (number = parse_number(&start, end))) {
	    fontmap[num_fontmap].descent = atoi(number);
	    RELEASE (number);
	  } else
	    WARN("Missing descent value in map file (%s).", tex_name);
	  break;
/* end -- cdipdfmx */
	default:
@z

@x
int fontmap_mapc (int map_id)
@y
/* add -- cdipdfmx */
int fontmap_ascent (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].ascent;
  else
    return -1;
}

int fontmap_descent (int map_id)
{
  if (map_id >= 0 && map_id < num_fontmap)
    return fontmap[map_id].descent;
  else
    return -1;
}

/* end -- cdipdfmx */

int fontmap_mapc (int map_id)
@z