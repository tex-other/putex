/*  $Header: /cvsroot/miktex/miktex/dvipdfmx/pngimage.c,v 1.1 2003/12/27 18:03:16 csc Exp $

    This is dvipdfmx, an eXtended version of dvipdfm by Mark A. Wicks.

    Copyright (C) 2002 by Jin-Hwan Cho and Shunsaku Hirata,
    the dvipdfmx project team <dvipdfmx@project.ktug.or.kr>
    
    Copyright (C) 1998, 1999 by Mark A. Wicks <mwicks@kettering.edu>

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

#if HAVE_CONFIG_H
#include "config.h"
#endif

/*
 * PNG SUPPORT
 *
 *  All bitdepth less than 16 is supported.
 *  Supported color types are: PALETTE, RGB, GRAY, RGB_ALPHA, GRAY_ALPHA.
 *  Supported ancillary chunks: tRNS, cHRM + gAMA, (sRGB)
 * 
 *  gAMA support is available only when cHRM exists. cHRM support is not
 *  tested well. CalRGB/CalGray colorspace is used for PNG images that
 *  have cHRM chunk (but not sRGB).
 *
 * LIMITATIONS
 *
 *   Recent version of PDF (>= 1.5) support 16 bpc, but 16 bit bitdepth PNG
 *   images are automatically converted to 8 bit bitpedth image.
 *   ICCP, pHYS are currently not supported.
 *
 * TODO
 *
 *  sBIT ? iTXT, tEXT and tIME as MetaData ? iCCP, pHYS (see below)
 *  16 bpc support for PDF-1.5. JBIG compression for monochrome image.
 *  Predictor for deflate ?
 */

#include "system.h"
#include "error.h"
#include "mem.h"

#include "pdfobj.h"

#define PNG_DEBUG_STR "PNG"
#define PNG_DEBUG     3

#ifdef HAVE_LIBPNG
#include <png.h>

#define PDF_TRANS_TYPE_NONE   0
#define PDF_TRANS_TYPE_BINARY 1
#define PDF_TRANS_TYPE_ALPHA  2

/* ColorSpace   */
static pdf_obj *create_cspace_Indexed (png_structp png_ptr, png_infop info_ptr);
static pdf_obj *create_cspace_CalRGB  (png_structp png_ptr, png_infop info_ptr);
static pdf_obj *create_cspace_CalGray (png_structp png_ptr, png_infop info_ptr);
static pdf_obj *create_param_Cal      (png_structp png_ptr, png_infop info_ptr,
				       png_byte color_type);

/* Rendering Intent, from sRGB chunk */
static pdf_obj *get_rendering_intent  (png_structp png_ptr, png_infop info_ptr);

/* Transparency */
static int      check_transparency   (png_structp png_ptr, png_infop info_ptr);
/* Colorkey-Mask */
static pdf_obj *create_cmask_for_PLT (png_structp png_ptr, png_infop info_ptr);
static pdf_obj *create_cmask_for_RGB (png_structp png_ptr, png_infop info_ptr);
static pdf_obj *create_cmask_for_GRA (png_structp png_ptr, png_infop info_ptr);
static pdf_obj *create_smask_for_PLT (png_structp png_ptr, png_infop info_ptr,
				      png_bytep image_data_ptr,
				      png_uint_32 width, png_uint_32 height);
/* Soft-Mask */
static pdf_obj *separate_smask_for_RGBA (png_structp png_ptr, png_infop info_ptr,
					 png_bytep image_data_ptr,
					 png_uint_32p rowbytes_ptr,
					 png_uint_32 width, png_uint_32 height);
static pdf_obj *separate_smask_for_GRAA (png_structp png_ptr, png_infop info_ptr,
					 png_bytep image_data_ptr,
					 png_uint_32p rowbytes_ptr,
					 png_uint_32 width, png_uint_32 height);
/* Read image body */
static void read_image_data (png_structp png_ptr, png_infop info_ptr,
			     png_bytep dest_ptr,
			     png_uint_32 height, png_uint_32 rowbytes);

/* check_for_xxx, start_xxx_image */
int
check_for_png (FILE *png_file) 
{
  unsigned char sigbytes[4];

  rewind (png_file);
  if (fread (sigbytes, 1, sizeof(sigbytes), png_file) !=
      sizeof(sigbytes) ||
      (!png_check_sig (sigbytes, sizeof(sigbytes))))
    return 0;
  else
    return 1;
}

pdf_obj *
start_png_image (FILE *png_file, char *res_name)
{
  pdf_obj  *result = NULL;
  pdf_obj  *dict   = NULL, *colorspace = NULL, *mask = NULL, *intent = NULL;
  png_bytep stream_data_ptr;
  int       trans_type;
  /* Libpng stuff */
  png_structp png_ptr;
  png_infop   info_ptr;
  png_byte    bpc, color_type;
  png_uint_32 width, height, rowbytes;

  rewind (png_file);
  png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
  if (png_ptr == NULL || 
      (info_ptr = png_create_info_struct (png_ptr)) == NULL) {
    WARN("%s: Creating Libpng read/info struct failed.", PNG_DEBUG_STR);
    if (png_ptr)
      png_destroy_read_struct(&png_ptr, NULL, NULL);
    return NULL;
  }
  
  /* Inititializing file IO. */
  png_init_io (png_ptr, png_file);

  /* Read PNG info-header and get some info. */
  png_read_info(png_ptr, info_ptr);
  color_type = png_get_color_type  (png_ptr, info_ptr);
  width      = png_get_image_width (png_ptr, info_ptr);
  height     = png_get_image_height(png_ptr, info_ptr);
  bpc        = png_get_bit_depth   (png_ptr, info_ptr);

  /* We do not need 16-bpc color. Ask libpng to convert down to 8-bpc. */
  if (bpc > 8) {
    png_set_strip_16(png_ptr);
    bpc = 8;
  }

  trans_type = check_transparency(png_ptr, info_ptr);
  /* check_transparency() does not do updata_info() */
  png_read_update_info(png_ptr, info_ptr);
  rowbytes = png_get_rowbytes(png_ptr, info_ptr);

  result = pdf_new_stream(STREAM_COMPRESS);
  dict   = pdf_stream_dict(result);
  /* Values listed below will not be modified in the remaining process. */
  pdf_add_dict(dict, pdf_new_name("Type"),             pdf_new_name("XObject"));
  pdf_add_dict(dict, pdf_new_name("Subtype"),          pdf_new_name("Image"));
  pdf_add_dict(dict, pdf_new_name("Width"),            pdf_new_number(width));
  pdf_add_dict(dict, pdf_new_name("Height"),           pdf_new_number(height));
  pdf_add_dict(dict, pdf_new_name("BitsPerComponent"), pdf_new_number(bpc));

  stream_data_ptr = (png_bytep) NEW(rowbytes*height, png_byte);
  read_image_data (png_ptr, info_ptr, stream_data_ptr, height, rowbytes);

  intent = get_rendering_intent(png_ptr, info_ptr);
  switch (color_type) {
  case PNG_COLOR_TYPE_PALETTE:
    colorspace = create_cspace_Indexed(png_ptr, info_ptr);
    switch (trans_type) {
    case PDF_TRANS_TYPE_BINARY:
      /* Color-key masking */
      mask = create_cmask_for_PLT(png_ptr, info_ptr);
      break;
    case PDF_TRANS_TYPE_ALPHA:
      /* Soft mask */
      mask = create_smask_for_PLT(png_ptr, info_ptr, stream_data_ptr, width, height);
      break;
    default:
      /*
       * Nothing to be done here.
       * No tRNS chunk or image already composited with background color.
       */
      break;
    }
    break;
  case PNG_COLOR_TYPE_RGB:
    if (intent == NULL) {
      if ((colorspace = create_cspace_CalRGB(png_ptr, info_ptr)) == NULL)
	colorspace = pdf_new_name("DeviceRGB");
    } else
      colorspace = pdf_new_name("DeviceRGB");
    if (trans_type == PDF_TRANS_TYPE_BINARY)
      mask = create_cmask_for_RGB(png_ptr, info_ptr);
    break;
  case PNG_COLOR_TYPE_RGB_ALPHA:
    if (intent == NULL) {
      if ((colorspace = create_cspace_CalRGB(png_ptr, info_ptr)) == NULL)
	colorspace = pdf_new_name("DeviceRGB");
    } else
      colorspace = pdf_new_name("DeviceRGB");
    /* rowbytes changes 4 to 3 at here */
    if (trans_type == PDF_TRANS_TYPE_ALPHA)
      mask = separate_smask_for_RGBA(png_ptr, info_ptr, stream_data_ptr, &rowbytes, width, height);
    /* else { already converted to RGB } */
    break;
  case PNG_COLOR_TYPE_GRAY:
    if (intent == NULL) {
      if ((colorspace = create_cspace_CalGray(png_ptr, info_ptr)) == NULL)
	colorspace = pdf_new_name("DeviceGray");
    } else
      colorspace = pdf_new_name("DeviceGray");
    if (trans_type == PDF_TRANS_TYPE_BINARY)
      mask = create_cmask_for_GRA(png_ptr, info_ptr);
    break;
  case PNG_COLOR_TYPE_GRAY_ALPHA:
    if (intent == NULL) {
      if ((colorspace = create_cspace_CalGray(png_ptr, info_ptr)) == NULL)
	colorspace = pdf_new_name("DeviceGray");
    } else
      colorspace = pdf_new_name("DeviceGray");
    if (trans_type == PDF_TRANS_TYPE_ALPHA)
      mask = separate_smask_for_GRAA(png_ptr, info_ptr, stream_data_ptr, &rowbytes, width, height);
    /* else { already converted to Gray } */
    break;
  default:
    WARN("%s: Unknown PNG colortype %d.", PNG_DEBUG_STR, color_type);
  }
  pdf_add_dict(dict, pdf_new_name("ColorSpace"), colorspace);
  pdf_add_stream(result, (char *)stream_data_ptr, rowbytes*height);
  RELEASE(stream_data_ptr);
  if (mask) {
    if (trans_type == PDF_TRANS_TYPE_BINARY)
      pdf_add_dict(dict, pdf_new_name("Mask"), mask);
    else if (trans_type == PDF_TRANS_TYPE_ALPHA) {
      pdf_add_dict(dict, pdf_new_name("SMask"), pdf_ref_obj(mask));
      pdf_release_obj(mask);
    } else {
      WARN("%s: You found a bug in pngimage.c.", PNG_DEBUG_STR);
      pdf_release_obj(mask);
    }
  }

  png_read_end(png_ptr, NULL);

  /* Cleanup */
  if (info_ptr)
    png_destroy_info_struct(png_ptr, &info_ptr);
  if (png_ptr)
    png_destroy_read_struct(&png_ptr, NULL, NULL);

  return result;
}

/* 
 * The returned value trans_type is the type of transparency to be used for
 * this image. Possible values are:
 *
 *   PDF_TRANS_TYPE_NONE    No Masking will be used/required.
 *   PDF_TRANS_TYPE_BINARY  Pixels are either fully opaque/fully transparent.
 *   PDF_TRANS_TYPE_ALPHA   Uses alpha channel, requies SMask.(PDF-1.4)
 *
 *   check_transparency() must check the current setting of output PDF version
 * and must choose appropriate trans_type value according as the PDF version of
 * current output PDF document.
 *   If the PDF version is less than 1.3, no transparency is supported for this
 * version of PDF, hence PDF_TRANS_TYPE_NONE must be returned. And when the PDF
 * version is equal to 1.3, possible retrun values are PDF_TRANS_TYPE_BINARY or
 * PDF_TRANS_TYPE_NONE. The latter case arises when PNG file uses alpha channel
 * explicitly (color type PNG_COLOR_TYPE_XXX_ALPHA), or the tRNS chunk for the
 * PNG_COLOR_TYPE_PALETTE image contains intermediate values of opacity.
 *   Finally, in the case of PDF version 1.4, all kind of translucent pixels
 * can be represented with Soft-Mask.
 */

static int
check_transparency (png_structp png_ptr, png_infop info_ptr)
{
  int           trans_type;
  unsigned      pdf_version;
  png_byte      color_type;
  png_color_16p trans_values;
  png_bytep     trans;
  int           num_trans;

  pdf_version = pdf_get_version();
  color_type  = png_get_color_type(png_ptr, info_ptr);

  /*
   * First we set trans_type to appropriate value for PNG image.
   */
  if (color_type == PNG_COLOR_TYPE_RGB_ALPHA ||
      color_type == PNG_COLOR_TYPE_GRAY_ALPHA) {
    trans_type = PDF_TRANS_TYPE_ALPHA;
  } else if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) &&
	     png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, &trans_values)) {
    /* Have valid tRNS chunk. */
    switch (color_type) {
    case PNG_COLOR_TYPE_PALETTE:
      /* Use color-key mask if possible. */ 
      trans_type = PDF_TRANS_TYPE_BINARY;
      while (num_trans-- > 0) {
	if (trans[num_trans] != 0x00 && trans[num_trans] != 0xff) {
	  /* This seems not binary transparency */
	  trans_type = PDF_TRANS_TYPE_ALPHA;
	  break;
	}
      }
      break;
    case PNG_COLOR_TYPE_GRAY:
    case PNG_COLOR_TYPE_RGB:
      /* RGB or GRAY, single color specified by trans_values is transparent. */
      trans_type = PDF_TRANS_TYPE_BINARY;
      break;
    default:
      /* Else tRNS silently ignored. */
      trans_type = PDF_TRANS_TYPE_NONE;
    }
  } else { /* no transparency */
    trans_type = PDF_TRANS_TYPE_NONE;
  }

  /*
   * Now we check PDF version.
   * We can convert alpha cahnnels to explicit mask via user supplied alpha-
   * threshold value. But I will not do that.
   */
  if (( pdf_version < 3 && trans_type != PDF_TRANS_TYPE_NONE   ) ||
      ( pdf_version < 4 && trans_type == PDF_TRANS_TYPE_ALPHA )) {
    /*
     *   No transparency supported but PNG uses transparency, or Soft-Mask
     * required but no support for it is available in this version of PDF.
     * We must do pre-composition of image with the background image here. But,
     * we cannot do that in general since dvipdfmx is not a rasterizer. What we
     * can do here is to composite image with a rectangle filled with the
     * background color. However, images are stored as an Image XObject which
     * can be referenced anywhere in the PDF document content. Hence, we cannot
     * know the correct background color at this time. So we will choose white
     * as background color, which is most probable color in our cases.
     * We ignore bKGD chunk.
     */
    png_color_16 bg;
    bg.red = 255; bg.green = 255; bg.blue  = 255; bg.gray = 255; bg.index = 0;
    png_set_background(png_ptr, &bg, PNG_BACKGROUND_GAMMA_SCREEN, 0, 1.0);
    WARN("%s: Transparency will be ignored. (no support in PDF ver. < 1.3)", PNG_DEBUG_STR);
    if (pdf_version < 3)
      WARN("%s: Please use -V 3 option to enable binary transparency support.", PNG_DEBUG_STR);
    if (pdf_version < 4)
      WARN("%s: Please use -V 4 option to enable full alpha channel support.", PNG_DEBUG_STR);
    trans_type = PDF_TRANS_TYPE_NONE;
  }

  return trans_type;
}

/*
 * sRGB:
 *
 *   If sRGB chunk is present, cHRM and gAMA chunk must be ignored.
 *
 */
static pdf_obj *
get_rendering_intent (png_structp png_ptr, png_infop info_ptr)
{
  pdf_obj *intent;
  int      srgb_intent;

  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_sRGB) &&
      png_get_sRGB (png_ptr, info_ptr, &srgb_intent)) {
    switch (srgb_intent) {
    case PNG_sRGB_INTENT_SATURATION:
      intent = pdf_new_name("Saturation");
      break;
    case PNG_sRGB_INTENT_PERCEPTUAL:
      intent = pdf_new_name("Perceptual");
      break;
    case PNG_sRGB_INTENT_ABSOLUTE:
      intent = pdf_new_name("AbsoluteColorimetric");
      break;
    case PNG_sRGB_INTENT_RELATIVE:
      intent = pdf_new_name("RelativeColorimetric");
      break;
    default:
      WARN("%s: Invalid value in PNG sRGB chunk: %d", PNG_DEBUG_STR, srgb_intent);
      intent = NULL;
    }
  } else
    intent = NULL;

  return intent;
}

/*
 * gAMA, cHRM:
 *
 *   If cHRM is present, we use CIE-Based color space. gAMA is also used here
 * if available.
 *
 * Some part of the following codes are taken from LGPL licensed software
 *
 *  com.sixlegs.image.png - Java package to read and display PNG images
 *  Copyright (C) 1998, 1999, 2001 Chris Nokleberg
 *
 *  http://www.sixlegs.com/software/png/
 *
 */

static pdf_obj *
create_cspace_CalRGB (png_structp png_ptr, png_infop info_ptr)
{
  pdf_obj *colorspace;
  pdf_obj *calrgb_param;

  calrgb_param = create_param_Cal (png_ptr, info_ptr, PNG_COLOR_TYPE_RGB);

  if (calrgb_param == NULL)
    return NULL;

  colorspace = pdf_new_array();
  pdf_add_array(colorspace, pdf_new_name("CalRGB"));
  pdf_add_array(colorspace, calrgb_param);

  return colorspace;
}

static pdf_obj *
create_cspace_CalGray (png_structp png_ptr, png_infop info_ptr)
{
  pdf_obj *colorspace;
  pdf_obj *calrgb_param;

  calrgb_param = create_param_Cal (png_ptr, info_ptr, PNG_COLOR_TYPE_GRAY);

  if (calrgb_param == NULL)
    return NULL;

  colorspace = pdf_new_array();
  pdf_add_array(colorspace, pdf_new_name("CalGray"));
  pdf_add_array(colorspace, calrgb_param);

  return colorspace;
}

static pdf_obj *
create_param_Cal (png_structp png_ptr, png_infop info_ptr, png_byte color_type)
{
  pdf_obj *cal_param;
  pdf_obj *white_point, *matrix, *gamma;
  double Xw, Yw, Zw; /* Yw = 1.0 */
  double Xr, Xg, Xb, Yr, Yb, Yg, Zr, Zg, Zb;
  double xw, yw, xr, yr, xg, yg, xb, yb;
  double g;

  if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_cHRM) ||
      !png_get_cHRM(png_ptr, info_ptr, &xw, &yw, &xr, &yr, &xg, &yg, &xb, &yb))
    return NULL;

  if (xw <= 0 || yw < 1.0e-10 ||
      xr < 0 || yr < 0 || xg < 0 || yg < 0 || xb < 0 || yb < 0) {
    WARN("%s: Invalid cHRM chunk parameters found.", PNG_DEBUG_STR);
    return NULL;
  }

#ifndef ABS
#define ABS(x) ((x) < 0 ? -(x) : (x))
#endif
  /*
   * TODO: Check validity
   */
  {
    double zw, zr, zg, zb;
    double fr, fg, fb;
    double det;

    /* WhitePoint */
    zw = 1 - (xw + yw);
    zr = 1 - (xr + yr); zg = 1 - (xg + yg); zb = 1 - (xb + yb);
    Xw = xw/yw; Yw = 1.0; Zw = zw/yw;

    /* Matrix */
    det = xr*(yg*zb-zg*yb)-xg*(yr*zb-zr*yb)+xb*(yr*zg-zr*yg);
    if (ABS(det) < 1.0e-10)
      return NULL;
    fr  = (Xw*(yg*zb-zg*yb)-xg*(zb-Zw*yb)+xb*(zg-Zw*yg))/det;
    fg  = (xr*(zb-Zw*yb)-Xw*(yr*zb-zr*yb)+xb*(yr*Zw-zr))/det;
    fb  = (xr*(yg*Zw-zg)-xg*(yr*Zw-zr)+Xw*(yr*zg-zr*yg))/det;
    Xr = fr * xr; Yr = fr * yr; Zr = fr * zr;
    Xg = fg * xg; Yg = fg * yg; Zg = fg * zg;
    Xb = fb * xb; Yb = fb * yb; Zb = fb * zb;
  }

  cal_param   = pdf_new_dict();
  white_point = pdf_new_array();
  matrix      = pdf_new_array();

  /* White point is always required. */
  pdf_add_array(white_point, pdf_new_number(Xw));
  pdf_add_array(white_point, pdf_new_number(Yw));
  pdf_add_array(white_point, pdf_new_number(Zw));
  pdf_add_dict(cal_param, pdf_new_name("WhitePoint"), white_point);

  /* Matrix - default: Identity */ 
  pdf_add_array(matrix,      pdf_new_number(Xr));
  pdf_add_array(matrix,      pdf_new_number(Yr));
  pdf_add_array(matrix,      pdf_new_number(Zr));
  pdf_add_array(matrix,      pdf_new_number(Xg));
  pdf_add_array(matrix,      pdf_new_number(Yg));
  pdf_add_array(matrix,      pdf_new_number(Zg));
  pdf_add_array(matrix,      pdf_new_number(Xb));
  pdf_add_array(matrix,      pdf_new_number(Yb));
  pdf_add_array(matrix,      pdf_new_number(Zb));
  pdf_add_dict(cal_param, pdf_new_name("Matrix"),     matrix     );

  /* Gamma - default: 1.0 */
  if (png_get_valid(png_ptr, info_ptr, PNG_INFO_gAMA) &&
      png_get_gAMA(png_ptr, info_ptr, &g) &&
      g > 0 && g != 1.0) { /* Default 1.0 */
    switch(color_type) {
    case PNG_COLOR_TYPE_RGB:
    case PNG_COLOR_TYPE_RGB_ALPHA:
    case PNG_COLOR_TYPE_PALETTE:
      gamma = pdf_new_array();
      pdf_add_array(gamma, pdf_new_number(g));
      pdf_add_array(gamma, pdf_new_number(g));
      pdf_add_array(gamma, pdf_new_number(g));
      break;
    case PNG_COLOR_TYPE_GRAY:
    case PNG_COLOR_TYPE_GRAY_ALPHA:
      gamma = pdf_new_number(g);
      break;
    default:
      gamma = NULL;
    }
    if (gamma)
      pdf_add_dict(cal_param, pdf_new_name("Gamma"), gamma);
  }

  return cal_param;
}

/*
 * Set up Indexed ColorSpace for color-type PALETTE:
 *
 *  PNG allows only RGB color for base color space. If gAMA and/or cHRM
 *  chunk is available, we can use CalRGB color space instead of DeviceRGB
 *  for base color space.
 *
 */
static pdf_obj *
create_cspace_Indexed(png_structp png_ptr, png_infop info_ptr)
{
  pdf_obj   *colorspace;
  pdf_obj   *base, *lookup;
  png_byte  *data_ptr;
  png_colorp plte;
  int        num_plte, i;

  if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_PLTE) ||
      !png_get_PLTE(png_ptr, info_ptr, &plte, &num_plte)) {
    WARN("%s: PNG does not have valid PLTE chunk but PLTE is requested.", PNG_DEBUG_STR);
    return NULL;
  }

  /* Order is important. */
  colorspace = pdf_new_array ();
  pdf_add_array(colorspace, pdf_new_name("Indexed"));
  /* cHRM and gAMA ignored if sRGB is present. */
  if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_sRGB)) {
    if ((base = create_cspace_CalRGB(png_ptr, info_ptr)) == NULL)
      base = pdf_new_name("DeviceRGB");
  } else
    base = pdf_new_name("DeviceRGB");
  pdf_add_array(colorspace, base);
  pdf_add_array(colorspace, pdf_new_number(num_plte-1));
  data_ptr = NEW(num_plte*3, png_byte);
  for (i = 0; i < num_plte; i++) {
    data_ptr[3*i]   = plte[i].red;
    data_ptr[3*i+1] = plte[i].green;
    data_ptr[3*i+2] = plte[i].blue;
  }
  lookup = pdf_new_stringx(data_ptr, num_plte*3);
  RELEASE(data_ptr);
  pdf_add_array(colorspace, lookup);

  return colorspace;
}

/*
 * pHYs: no support
 *
 *  pngimage.c is not responsible for adjusting image size.
 *  Higher layer must do something for this.
 */

/*
 * Colorkey Mask: array
 *
 *  [component_0_min component_0_max ... component_n_min component_n_max]
 *
 */

static pdf_obj *
create_cmask_for_PLT (png_structp png_ptr, png_infop info_ptr)
{
  pdf_obj  *colorkeys;
  png_bytep trans;
  int       num_trans, i;

  if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) ||
      !png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, NULL)) {
    WARN("%s: PNG does not have valid tRNS chunk but tRNS is requested.", PNG_DEBUG_STR);
    return NULL;
  }

  colorkeys = pdf_new_array();
  for (i = 0; i < num_trans; i++) {
    if (trans[i] == 0x00) {
      pdf_add_array(colorkeys, pdf_new_number(i));
      pdf_add_array(colorkeys, pdf_new_number(i));
    } else if (trans[i] != 0xff)
      WARN("%s: You found a bug in pngimage.c.", PNG_DEBUG_STR);
  }

  return colorkeys;
}

static pdf_obj *
create_cmask_for_RGB (png_structp png_ptr, png_infop info_ptr)
{
  pdf_obj       *colorkeys;
  png_color_16p trans_values;

  if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) ||
      !png_get_tRNS(png_ptr, info_ptr, NULL, NULL, &trans_values)) {
    WARN("%s: PNG does not have valid tRNS chunk but tRNS is requested.", PNG_DEBUG_STR);
    return NULL;
  }

  colorkeys = pdf_new_array();
  pdf_add_array(colorkeys, pdf_new_number(trans_values->red));
  pdf_add_array(colorkeys, pdf_new_number(trans_values->red));
  pdf_add_array(colorkeys, pdf_new_number(trans_values->green));
  pdf_add_array(colorkeys, pdf_new_number(trans_values->green));
  pdf_add_array(colorkeys, pdf_new_number(trans_values->blue));
  pdf_add_array(colorkeys, pdf_new_number(trans_values->blue));

  return colorkeys;
}

static pdf_obj *
create_cmask_for_GRA  (png_structp png_ptr, png_infop info_ptr)
{
  pdf_obj       *colorkeys;
  png_color_16p trans_values;

  if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) ||
      !png_get_tRNS(png_ptr, info_ptr, NULL, NULL, &trans_values)) {
    WARN("%s: PNG does not have valid tRNS chunk but tRNS is requested.", PNG_DEBUG_STR);
    return NULL;
  }

  colorkeys = pdf_new_array();
  pdf_add_array(colorkeys, pdf_new_number(trans_values->gray));
  pdf_add_array(colorkeys, pdf_new_number(trans_values->gray));

  return colorkeys;
}

/*
 * Soft-Mask: stream
 *
 *   <<
 *      /Type             /XObject
 *      /Subtype          /Image
 *      /Width            -int-
 *      /Height           -int-
 *      /BitsPerComponent bpc
 *   >>
 *   stream .... endstream
 *
 *   ColorSpace, Mask, SMask must be absent. ImageMask must be false or absent.
 */

static pdf_obj *
create_smask_for_PLT (png_structp png_ptr, png_infop info_ptr,
		      png_bytep image_data_ptr, png_uint_32 width, png_uint_32 height)
{
  pdf_obj    *smask, *dict;
  png_bytep   smask_data_ptr;
  png_bytep   trans;
  int         num_trans;
  png_uint_32 i;

  if (!png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS) ||
      !png_get_tRNS(png_ptr, info_ptr, &trans, &num_trans, NULL)) {
    WARN("%s: PNG does not have valid tRNS chunk but tRNS is requested.", PNG_DEBUG_STR);
    return NULL;
  }

  smask = pdf_new_stream(STREAM_COMPRESS);
  dict  = pdf_stream_dict(smask);
  smask_data_ptr = (png_bytep) NEW(width*height, png_byte);
  pdf_add_dict(dict, pdf_new_name("Type"),    pdf_new_name("XObjcect"));
  pdf_add_dict(dict, pdf_new_name("Subtype"), pdf_new_name("Image"));
  pdf_add_dict(dict, pdf_new_name("Width"),      pdf_new_number(width));
  pdf_add_dict(dict, pdf_new_name("Height"),     pdf_new_number(height));
  pdf_add_dict(dict, pdf_new_name("ColorSpace"), pdf_new_name("DeviceGray"));
  pdf_add_dict(dict, pdf_new_name("BitsPerComponent"), pdf_new_number(8));
  for (i = 0; i < width*height; i++) {
    png_byte idx = image_data_ptr[i];
    smask_data_ptr[i] = (idx < num_trans) ? trans[idx] : 0xff;
  }
  pdf_add_stream(smask, (char *)smask_data_ptr, width*height);
  RELEASE(smask_data_ptr);

  return smask;
}

/* bitdepth is always 8 (16 is not supported) */
static pdf_obj *
separate_smask_for_RGBA (png_structp png_ptr, png_infop info_ptr,
			 /* next two values will be modified */
			 png_bytep image_data_ptr, png_uint_32p rowbytes_ptr,
			 png_uint_32 width, png_uint_32 height)
{
  pdf_obj   *smask, *dict;
  png_bytep  smask_data_ptr;
  unsigned long i;

  if (*rowbytes_ptr != 4*width*sizeof(png_byte)) { /* Something wrong */
    WARN("%s: Inconsistent rowbytes value.", PNG_DEBUG_STR);
    return NULL;
  }
    
  smask = pdf_new_stream(STREAM_COMPRESS);
  dict  = pdf_stream_dict(smask);
  pdf_add_dict(dict, pdf_new_name("Type"),    pdf_new_name("XObjcect"));
  pdf_add_dict(dict, pdf_new_name("Subtype"), pdf_new_name("Image"));
  pdf_add_dict(dict, pdf_new_name("Width"),      pdf_new_number(width));
  pdf_add_dict(dict, pdf_new_name("Height"),     pdf_new_number(height));
  pdf_add_dict(dict, pdf_new_name("ColorSpace"), pdf_new_name("DeviceGray"));
  pdf_add_dict(dict, pdf_new_name("BitsPerComponent"), pdf_new_number(8));
  smask_data_ptr = (png_bytep) NEW(width*height, png_byte);
  for (i = 0; i < width*height; i++) {
    memmove(image_data_ptr+(3*i), image_data_ptr+(4*i), 3);
    smask_data_ptr[i] = image_data_ptr[4*i+3];
  }
  pdf_add_stream(smask, (char *)smask_data_ptr, width*height);
  RELEASE(smask_data_ptr);

  *rowbytes_ptr = 3*width*sizeof(png_byte);
  return smask;
}

static pdf_obj *
separate_smask_for_GRAA (png_structp png_ptr, png_infop info_ptr,
			 /* next two values will be modified. */
			 png_bytep image_data_ptr, png_uint_32p rowbytes_ptr,
			 png_uint_32 width, png_uint_32 height)
{
  pdf_obj    *smask, *dict;
  png_bytep   smask_data_ptr;
  png_uint_32 i;

  if (*rowbytes_ptr != 2*width*sizeof(png_byte)) { /* Something wrong */
    WARN("%s: Inconsistent rowbytes value.", PNG_DEBUG_STR);
    return NULL;
  }

  smask = pdf_new_stream(STREAM_COMPRESS);
  dict  = pdf_stream_dict(smask);
  smask_data_ptr = (png_bytep) NEW(width*height, png_byte);
  pdf_add_dict(dict, pdf_new_name("Type"),    pdf_new_name("XObjcect"));
  pdf_add_dict(dict, pdf_new_name("Subtype"), pdf_new_name("Image"));
  pdf_add_dict(dict, pdf_new_name("Width"),      pdf_new_number(width));
  pdf_add_dict(dict, pdf_new_name("Height"),     pdf_new_number(height));
  pdf_add_dict(dict, pdf_new_name("ColorSpace"), pdf_new_name("DeviceGray"));
  pdf_add_dict(dict, pdf_new_name("BitsPerComponent"), pdf_new_number(8));
  for (i = 0; i < width*height; i++) {
    image_data_ptr[i] = image_data_ptr[2*i];
    smask_data_ptr[i] = image_data_ptr[2*i+1];
  }
  pdf_add_stream(smask, (char *) smask_data_ptr, width*height);
  RELEASE(smask_data_ptr);

  *rowbytes_ptr = width*sizeof(png_byte);
  return smask;
}

static void
read_image_data (png_structp png_ptr, png_infop info_ptr, /* info_ptr unused */
		 png_bytep dest_ptr, png_uint_32 height, png_uint_32 rowbytes)
{
  png_bytepp  rows_p;
  png_uint_32 i;

  rows_p = (png_bytepp) NEW (height, png_bytep);
  for (i=0; i< height; i++)
    rows_p[i] = dest_ptr + (rowbytes * i);
  png_read_image(png_ptr, rows_p);
  RELEASE(rows_p);
}

#endif /* HAVE_LIBPNG */
