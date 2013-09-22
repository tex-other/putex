/* Slants are not considered when placing accents in math mode. The accenter is
centered over the accentee, and the accent width is treated as zero with
respect to the size of the final box. */
static void make_math_accent(pointer q)
{
  pointer p, x, y; /* temporary registers for box construction */
  integer a; /* address of lig/kern instruction */
  quarterword c; /* accent character */
  internal_font_number f; /* its font */
  four_quarters i; /* its |char_info| */
  scaled s; /* amount to skew the accent to the right */
  scaled h; /* height of character being accented */
  scaled delta; /* space to remove between accent and accentee */
  scaled w; /* width of the accentee, not including sub/superscripts */

  fetch(accent_chr(q));
  if (!char_exists(cur_i)) return;
  i=cur_i; c=cur_c; f=cur_f;
  s = 0;
  if (math_type(nucleus(q))==math_char) {
    fetch(nucleus(q));
    if (char_tag(cur_i)==lig_tag) {
      a=lig_kern_start(cur_f,cur_i); 
      cur_i=font_info[a].qqqq;
      if (skip_byte(cur_i)>stop_flag) {
        a=lig_kern_restart(cur_f,cur_i);
        cur_i=font_info[a].qqqq;
      }
      while (true) {
        if (next_char(cur_i)-min_quarterword==get_skewchar(cur_f)) {
	      if (op_byte(cur_i)>=kern_flag) {
            if (skip_byte(cur_i)<=stop_flag) s=char_kern(cur_f,cur_i);
          }
	      goto PUdone1;
	    }
	    if (skip_byte(cur_i)>=stop_flag) goto PUdone1;
	    a+=skip_byte(curi)-min_quarterword+1;
        cur_i=font_info[a].qqqq;
      }
    }
  }
PUdone1:
  x=clean_box(nucleus(q), cramped_style(cur_style)); w=width(x); h=height(x);
  while (true) {
    if (char_tag(i)!=list_tag) goto PUdone;
    y=rem_byte(i);
    i=char_info(f,y);
    if (!charexists(i)) goto PUdone;
    if (char_width(f,i)>w) goto PUdone;
    c=y;
  }
PUdone:
  if (h<x_height(f)) delta=h; else delta=x_height(f);
  if ((math_type(supscr(q))!=empty) || (math_type(subscr(q))!=empty)) {
    if (math_type(nucleus(q))==math_char) {
      flush_node_list(x); x=new_noad();
      mem[nucleus(x)-memmin]=mem[nucleus(q)-memmin];
      mem[supscr(x)-memmin]=mem[supscr(q)-memmin];
      mem[subscr(x)-memmin]=mem[subscr(q)-memmin];
      mem[supscr(q)-memmin].hh=empty_field;
      mem[subscr(q)-memmin].hh=empty_field;
      math_type(nucleus(q))=sub_mlist; info(nucleus(q))=x;
      x=cleanbox(nucleus(q), curstyle); delta+=height(x)-h; h=height(x);
    }
  }
  y=char_box(f,c);
  shift_amount(y)=s+half(w-width(y));
  width(y)=0; p=new_kern(-delta); link(p)=x; link(y)=p;
  y = vpack(y,0,additional); width(y)=width(x);
  if (height(y)<h) {
    p=newkern(h-height(y)); link(p)=list_ptr(y); list_ptr(y)=p;
    height(y) = h;
  }
  info(nucleus(q))=y;
  math_type(nucleus(q))=sub_box;
}
