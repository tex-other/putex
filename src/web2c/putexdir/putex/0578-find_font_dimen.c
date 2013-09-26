/* The following routine is used to implement `\.{\\fontdimen} |n| |f|'.
The boolean parameter |writing| is set |true| if the calling program
intends to change the parameter value. */
void find_font_dimen(boolean writing)
/* sets |cur_val| to |font_info| location */
{
  internal_font_number f;
  integer n; /* the parameter number */
  scan_int(); n=cur_val; scan_font_ident(); f=cur_val;
  if (n<=0) cur_val=fmem_ptr;
  else {
    if (writing && n<=space_shrink_code &&
        n>=space_code && font_glue[f]!=0) {
      delete_glue_ref(font_glue[f]);
      font_glue[f]=0;
    }
    if (n>font_params[f]) {
      if (f<font_ptr) cur_val=fmem_ptr;
      else {
        do {
          if (fmem_ptr==font_mem_size)
            overflow("font memory",font_mem_size);
          font_info[fmem_ptr].sc = 0; incr(fmemptr); incr(font_params[f]);
        } while (n!=font_params[f]);
        cur_val=fmem_ptr-1;
      }
    } else cur_val=n+param_base[f];
  }
  if (cur_val==fmem_ptr) {
    print_err("Font "); print_esc(font_id_text(f));
    print(" has only "); print_int(font_params[f]);
    print(" fontdimen parameters");
    help2("To increase the number of font parameters, you must",
      "use \fontdimen immediately after the \font is loaded.");
    error();
  }
}
