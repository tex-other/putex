% dvicodes_h.ch
@x
#define DVI_ID             2    /* ID Byte for current DVI file */
#define DVIV_ID             3    /* with Ascii pTeX VW mode extension */
@y
#define DVI_ID             2    /* ID Byte for current DVI file */
#define DVIV_ID             3    /* with Ascii pTeX VW mode extension */
#define OLD_CDI_ID	2	/* old CDI file ID Byte value */
#define CDI_ID		100	/* new CDI file ID Byte value */
@z

@x
#define DIR 255 /* Ascii pTeX DIR command */

#endif /* _DVICODES_H_ */
@y
#define DIR 255 /* Ascii pTeX DIR command */

/* cdi commands			argvs : */
#define CFNT 250	  /* k[2] */
#define CFNT_DEF 251  /* k[2] a[1] n[a] c[1] size[4] dsize[4] weight[2] style[1]
                         width[4] height[4] depth[4]
                         fw_width[4] fw_height[4] fw_depth[4] */

#endif /* _DVICODES_H_ */
@z