/* The positioning of accents is straightforward but tedious. Given an accent
of width |a|, designed for characters of height |x| and slant |s|;
and given a character of width |w|, height |h|, and slant |t|: We will shift
the accent down by |x-h|, and we will insert kern nodes that have the effect of
centering the accent over the character and shifting the accent to the
right by $\delta={1\over2}(w-a)+h\cdot t-x\cdot s$.  If either character is
absent from the font, we will simply use the other, without shifting. */
static void make_accent(void)
{
  double s, t; /* amount of slant */
  pointer p, q, r; /* character, box, and kern nodes */
  internal_font_number f; /* relevant font */
  scaled a, h, x, w, delta; /* heights and widths, as explained above */
  four_quarters i; /* character information */

  scan_char_num(); f=cur_font; p=new_character(f,cur_val);
  if (p==0) return;
  x=xheight(f); s=slant(f)/65536.0;
  a=char_width(f,char_info(f,character(p)));
  do_assignments();
  q=0; f=cur_font;
  if (cur_cmd==letter || cur_cmd==other_char || cur_cmd==char_given)
    q = newcharacter(f, curchr);
  else if (cur_cmd==char_num) {
    scan_char_num(); q=new_character(f,cur_val);
  } else back_input();
  if (q!=0) {
    t=slant(f)/65536.0;
    i=char_info(f,character(q));
    w=char_width(f,i); h=char_height(f,height_depth(i));
    if (h!=x) { /* the accent must be shifted up or down */
      p=hpack(p,0,additional); shift_amount(p)=x-h;
    }
    delta=(long)floor((w-a)/2.0+h*t-x*s+0.5);
    r=new_kern(delta); subtype(r)=acc_kern; link(tail)=r; link(r)=p;
    tail=newkern(-a-delta); subtype(tail)=acc_kern; link(p)=tail; p=q;
  }
  link(tail)=p; tail=p; spacefactor=1000;
}
