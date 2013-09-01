%% fontmap_h.ch
@x
extern void read_mapfile (char *filename);
@y
/* add -- cdipdfmx */
extern void set_doc_encoding (char *enc_name);
extern void read_cdipdfmx_mapfile (char *filename);
/* end -- cdipdfmx */
extern void read_mapfile (char *filename);
@z

@x
extern int fontmap_mapc (int map_id);
@y
extern int fontmap_mapc (int map_id);
/* add -- cdipdfmx */
extern int fontmap_ascent (int map_id);
extern int fontmap_descent (int map_id);
/* end -- cdipdfmx */
@z