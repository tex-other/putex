void new_font(small_number a)
{
  pointer u; /* user's font identifier */
  scaled s; /* stated ``at'' size, or negative of scaled magnification */
  internal_font_number f; /* runs through existing fonts */
  str_number t; /* name for the frozen font identifier */
  char old_setting; /* holds |selector| setting */
/* XXXX  strnumber flushablestring; */

  if (jobname==0) open_log_file();
    /* avoid confusing \.{texput} with the font name */
  get_r_token(); u=cur_cs;
  if (u>=hash_base) t=text(u);
  else if (u>=single_base) {
    if (u==null_cs) t="FONT"; else t=u-single_base;
  } else {
    old_setting=selector; selector=new_string;
    print("FONT"); print(u-active_base);  selector=old_setting;
    str_room(1); t=make_string();
  }
  define(u,set_font,null_font); scan_optional_equals(); scan_file_name();
  name_in_progress=true; /* this keeps |cur_name| from being changed */
  if (scan_keyword("at")) {
    scan_normal_dimen(); s=cur_val;
    if (s<=0||s>=134217728L) {
      print_err("Improper `at' size (");
      print_scaled(s); print("pt), replaced by 10pt");
      help2("I can only handle fonts at positive sizes that are",
        "less than 2048pt, so I've changed what you said to 10pt.");
      error(); s=10*unity;
    }
  }
  else if (scankeyword("scaled")) {
    scan_int(); s=-cur_val;
    if (cur_val<=0 || cur_val>32768L) {
      print_err("Illegal magnification has been changed to 1000");
      help1("The magnification ratio must be between 1 and 32768.");
      int_error(cur_val); s=-1000;
    }
  } else  s=-1000;
  name_in_progress=false;   
#if 0
  flushablestring = strptr - 1;
#endif
  for (f=1; f<=font_ptr; f++) {
    if (str_eq_str(get_fontname(f),cur_name) /* &
	streqstr(fontarea[f ], curarea) */ )
    {   /*:1260*/
#if 0
      if (curname == flushablestring) {
	flushstring();
	curname = fontname[f ];
      }
#endif
      if (s>0) {
	    if (s==get_fontsize(f)) goto PUcommonending;
      } else if (get_fontsize(f)==xn_over_d(get_font_dsize(f),-s,1000))
        goto PUcommonending;
    }
  }
  f=readfontinfo(u,cur_name,cur_area,s);
PUcommonending:
  equiv(u)=f; eqtb[font_id_base+f-activebase]=eqtb[u-activebase]; set_fontidtext(f,t);
}
