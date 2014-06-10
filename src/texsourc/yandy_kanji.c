/* Copyright 1992 Karl Berry
   Copyright 2007 TeX Users Group
   Copyright 2014 Clerk Ma

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA.  */

#define EXTERN extern

#include "texd.h"

boolean check_kanji(integer c)
{
  return is_char_kanji(c);
}

boolean is_char_ascii(integer c)
{
  return (0 <= c && c < 0x100);
}

boolean is_char_kanji(integer c)
{
  return (iskanji1(Hi(c)) && iskanji2(Lo(c)));
}

boolean ismultiprn(integer c)
{
  if (iskanji1(c) || iskanji2(c))
    return true;
  
  return false;
}

integer calc_pos(integer c)
{
  unsigned char c1, c2;
  
  if(c>=0 && c<=255)
    return(c);
  
  c1 = (c >> 8) & 0xff;
  c2 = c & 0xff;
  
  if (iskanji1(c1))
  {
    if (is_internalSJIS())
    {
      c1 = ((c1 - 0x81) % 4) * 64;  /* c1 = 0, 64, 128, 192 */
      c2 = c2 % 64;                 /* c2 = 0..63 */
    }
    else
    {
      c1 = ((c1 - 0xa1) % 4) * 64;  /* c1 = 0, 64, 128, 192 */
      c2 = c2 % 64;                 /* c2 = 0..63 */
    }
    
    return (c1 + c2);              /* ret = 0..255 */
  }
  else
    return (c2);
}

integer kcatcodekey(integer c)
{
  return Hi(toDVI(c));
}

void init_default_kanji (const_string file_str, const_string internal_str)
{
  char *p;
  
  enable_UPTEX (false); /* disable */
  
  if (!set_enc_string (file_str, internal_str))
  {
    fprintf (stderr, "Bad kanji encoding \"%s\" or \"%s\".\n",
      file_str ? file_str  : "NULL",
      internal_str ? internal_str : "NULL");
    uexit(1);
  }

  p = getenv ("PTEX_KANJI_ENC");
  
  if (p)
  {
    if (!set_enc_string (p, NULL))
      fprintf (stderr, "Ignoring bad kanji encoding \"%s\".\n", p);
  }

#ifdef WIN32
  p = kpse_var_value ("guess_input_kanji_encoding");
  
  if (p)
  {
    if (*p == '1' || *p == 'y' || *p == 't')
      infile_enc_auto = 1;
    
    free(p);
  }
#endif
}
/* Routines needed by pTeX */

void print_kanji(KANJI_code s)
{
  if (s > 255)
  {
    print_char(Hi(s));
    print_char(Lo(s));
  }
  else
    print_char(s);
}

void print_kansuji(integer n)
{
  int k;
  KANJI_code cx;

  k = 0;
  
  if (n < 0)
    return;

  do
    {
      dig[k] = n % 10;
      n = n / 10;
      incr(k);
    }
  while (!(n == 0));

  while (k > 0)
  {
    decr(k);
    cx = kansuji_char(dig[k]);
    print_kanji(fromDVI(cx));
  }
}

void print_dir(eight_bits dir)
{
  if (dir == dir_yoko)
    print_char('Y');
  else if (dir == dir_tate)
    print_char('T');
  else if (dir == dir_dtou)
    print_char('D');
}

void print_direction(integer d)
{
  switch (d)
  {
    case dir_yoko:
      print_string("yoko");
      break;

    case dir_tate:
      print_string("tate");
      break;

    case dir_dtou:
      print_string("dtou");
      break;
  }

  if (d < 0)
    print_string("(math)");
  else
    print_string(" direction");
}

eight_bits get_jfm_pos(KANJI_code kcode, internal_font_number f)
{
  KANJI_code jc;
  pointer sp, mp, ep;

  if (f == null_font)
  {
    return kchar_type(null_font, 0);
  }

  jc = toDVI(kcode);
  sp = 1;
  ep = font_num_ext[f] - 1;

  if (ep >= 1 && kchar_code(f, sp) <= jc && jc <= kchar_code(f, ep))
  {
    while (sp <= ep)
    {
      mp = sp + (ep - sp) / 2;

      if (jc < kchar_code(f, mp))
        ep = mp - 1;
      else if (jc > kchar_code(f, mp))
        sp = mp + 1;
      else
      {
        return kchar_type(f, mp);
      }
    }
  }

  return kchar_type(f, 0);
}
pointer get_kinsoku_pos(KANJI_code c, small_number n)
{
  pointer p,s;

  s = calc_pos(c);
  p = s;

#ifdef DEBUG
  print_ln();
  print_string("c:=");
  print_int(c);
  print_string(", p:=");
  print_int(s);

  if (p + kinsoku_base < 0)
  {
    print_string("p is negative value");
    print_ln();
  }
#endif

  if (n == new_pos)
  {
    do
      {
        if ((kinsoku_type(p) == 0) || (kinsoku_code(p) == c))
          goto done;

        incr(p);
        
        if (p > 255)
          p = 0;
      }
    while (!(s == p));

    p = no_entry;
  }
  else
  {
    do
      {
        if (kinsoku_type(p) == 0)
          goto done1;

        if (kinsoku_code(p) == c)
          goto done;

        incr(p);

        if (p > 255)
          p = 0;
      }
    while (!(s == p));
done1:
    p = no_entry;
  }
done:
  return p;
}
pointer get_inhibit_pos(KANJI_code c, small_number n)
{
  pointer p, s;

  s = calc_pos(c);
  p = s;
  
  if (n == new_pos)
  {
    do
      {
        if ((inhibit_xsp_code(p) == 0) || (inhibit_xsp_code(p) == c))
          goto done;

        incr(p);

        if (p > 255)
          p = 0;
      }
    while (!(s == p));
    
    p = no_entry;
  }
  else
  {
    do
    {
      if (inhibit_xsp_code(p) == 0)
        goto done1;
      
      if (inhibit_xsp_code(p) == c)
        goto done;

      incr(p);
      
      if (p > 255)
        p = 0;
    }
    while (!(s == p));
done1:
    p = no_entry;
  }
done:
  return p;
}


void set_math_kchar(integer c)
{
  pointer p;

  p = new_noad();
  math_type(nucleus(p)) = math_jchar;
  inhibit_glue_flag = false;
  character(nucleus(p)) = 0;
  math_kcode(p) = c;
  fam(nucleus(p)) = cur_jfam;

  if (font_dir[fam_fnt(fam(nucleus(p)) + cur_size)] == dir_default)
  {
    print_err("Not two-byte family");
    help1("IGNORE.");
    error();
  }

  type(p) = ord_noad;
  link(tail) = p;
  tail = p;
}

void synch_dir(void)
{
  scaled tmp;

  switch (cur_dir_hv)
  {
    case dir_yoko:
      if (dvi_dir != cur_dir_hv)
      {
        synch_h();
        synch_v();
        dvi_out(dirchg);
        dvi_out(dvi_yoko);
        dir_used = true;

        switch (dvi_dir)
        {
          case dir_tate:
            tmp = cur_h;
            cur_h = -cur_v;
            cur_v = tmp;
            break;

          case dir_dtou:
            tmp = cur_h;
            cur_h = cur_v;
            cur_v = -tmp;
            break;
        }

        dvi_h = cur_h;
        dvi_v = cur_v;
        dvi_dir = cur_dir_hv;
      }
      break;

    case dir_tate:
      if (dvi_dir != cur_dir_hv)
      {
        synch_h();
        synch_v();
        dvi_out(dirchg);
        dvi_out(dvi_tate);
        dir_used = true;

        switch (dvi_dir)
        {
          case dir_yoko:
            tmp = cur_h;
            cur_h = cur_v;
            cur_v = -tmp;
            break;

          case dir_dtou:
            cur_v = -cur_v;
            cur_h = -cur_h;
            break;
        }

        dvi_h = cur_h;
        dvi_v = cur_v;
        dvi_dir = cur_dir_hv;
      }
      break;

    case dir_dtou:
      if (dvi_dir != cur_dir_hv)
      {
        synch_h();
        synch_v();
        dvi_out(dirchg);
        dvi_out(dvi_dtou);
        dir_used = true;

        switch (dvi_dir)
        {
          case dir_yoko:
            tmp = cur_h;
            cur_h = -cur_v;
            cur_v = tmp;
            break;

          case dir_tate:
            cur_v = -cur_v;
            cur_h = -cur_h;
            break;
        }

        dvi_h = cur_h;
        dvi_v = cur_v;
        dvi_dir = cur_dir_hv;
      }
      break;

    default:
      confusion("synch_dir");
      break;
  }
}

void dir_out(void)
{
  pointer this_box;

  this_box = temp_ptr;
  temp_ptr = list_ptr(this_box);

  if (type(temp_ptr) != hlist_node && type(temp_ptr) != vlist_node)
  {
    confusion("dir_out");
    return;
  }

  switch (box_dir(this_box))
  {
    case dir_yoko:
      switch (box_dir(temp_ptr))
      {
        case dir_tate:
          cur_v = cur_v - height(this_box);
          cur_h = cur_h + depth(temp_ptr);
          break;

        case dir_dtou:
          cur_v = cur_v + depth(this_box);
          cur_h = cur_h + height(temp_ptr);
          break;
      }
      break;

    case dir_tate:
      switch (box_dir(temp_ptr))
      {
        case dir_yoko:
          cur_v = cur_v + depth(this_box);
          cur_h = cur_h + height(temp_ptr);
          break;

        case dir_dtou:
          cur_v = cur_v + depth(this_box) - height(temp_ptr);
          cur_h = cur_h + width(temp_ptr);
          break;
      }
      break;

    case dir_dtou:
      switch (box_dir(temp_ptr))
      {
        case dir_yoko:
          cur_v = cur_v - height(this_box);
          cur_h = cur_h + depth(temp_ptr);
          break;

        case dir_tate:
          cur_v = cur_v + depth(this_box) - height(temp_ptr);
          cur_h = cur_h + width(temp_ptr);
          break;
      }
      break;
  }

  cur_dir_hv = box_dir(temp_ptr);

  if (type(temp_ptr) == vlist_node)
    vlist_out();
  else
    hlist_out();
}
boolean check_box(pointer box_p)
{
  pointer p; // {run through the current box}
  boolean flag; // {found any printable character?}

  flag = false;
  p = box_p;

  while (p != 0)
  {
    if (is_char_node(p))
      do
        {
          if (find_first_char)
          {
            first_char = p;
            find_first_char = false;
          }
          
          last_char = p;
          flag = true;
          
          if (font_dir[font(p)] != dir_default)
            p = link(p);
          
          p = link(p);
          
          if (p == 0)
            goto done;
        }
      while (is_char_node(p));


    switch (type(p))
    {
      case hlist_node:
        {
          flag = true;
        
          if (shift_amount(p) == 0)
          {
            if (check_box(list_ptr(p)))
              flag = true;
          }
          else if (find_first_char)
            find_first_char = false;
          else
            last_char = 0;
        }
        break;

      case ligature_node:
        if (check_box(lig_ptr(p)))
          flag = true;
      
      case ins_node:
      case disp_node:
      case mark_node:
      case adjust_node:
      case whatsit_node:
      case penalty_node:
        break;
      
      case math_node:
        if ((subtype(p) == before) || (subtype(p) == after))
        {
          if (find_first_char)
          {
            find_first_char = false;
            first_char = p; 
          }

          last_char = p;
          flag = true;
        }
        break;

      default:
        {
          flag = true;
        
          if (find_first_char)
            find_first_char = false;
          else
            last_char = 0;
        }
    }

    p = link(p);
  }
done:
  return flag;
}

void adjust_hlist(pointer p, boolean pf)
{
  pointer q, s, t, u, v, x, z;
  halfword i,k;
  pointer a; // { temporary pointer for accent }
  unsigned char insert_skip;
  KANJI_code cx; // {temporaly register for KANJI character}
  ASCII_code ax; // {temporaly register for ASCII character}
  boolean do_ins; // {for inserting |xkanji_skip| into prevous (or after) KANJI}

  if (link(p) == 0)
    goto labexit;
  
  if (auto_spacing > 0)
  {
    delete_glue_ref(space_ptr(p));
    space_ptr(p) = kanji_skip;
    add_glue_ref(kanji_skip);
  }

  if (auto_xspacing > 0)
  {
    delete_glue_ref(xspace_ptr(p));
    xspace_ptr(p) = xkanji_skip;
    add_glue_ref(xkanji_skip);
  }

  u = space_ptr(p);
  add_glue_ref(u);
  s = xspace_ptr(p);
  add_glue_ref(s);
  
  if (!is_char_node(link(p)) && (type(link(p)) == glue_node) &&
    (subtype(link(p)) == jfm_skip + 1))
  {
    v = link(p);
    link(p) = link(v);
    delete_glue_ref(glue_ptr(v));
    free_node(v, small_node_size);
  }

  i = 0;
  insert_skip = no_skip;
  p = link(p);
  v = p;
  q = p;
  
  while (p != 0)
  {
    if (is_char_node(p))
    {
      do 
        {
          if (font_dir[font(p)] != dir_default)
          {
            cx = info(link(p));
            
            if (insert_skip == after_schar)
            {
              x = get_inhibit_pos(cx, cur_pos);
              
              if (x != no_entry)
                if ((inhibit_xsp_type(x) == inhibit_both) ||
                  (inhibit_xsp_type(x) == inhibit_previous))
                  do_ins = false;
                else
                  do_ins = true;
              else
                do_ins = true;
            }
            
            if (do_ins)
            {
              z = new_glue(s);
              subtype(z) = xkanji_skip_code + 1;
              link(z) = link(q);
              link(q) = z;
              q = z;
            }

            p = link(p);
            insert_skip = after_wchar;
          }
          else
          {
            ax = character(p);
            
            if (insert_skip == after_wchar)
            {
              if ((auto_xsp_code(ax) % 2) == 1)
              {
                x = get_inhibit_pos(cx, cur_pos);
                
                if (x != no_entry)
                  if ((inhibit_xsp_type(x) == inhibit_both) ||
                    (inhibit_xsp_type(x) == inhibit_after))
                    do_ins = false;
                  else
                    do_ins = true;
                else
                  do_ins = true;
              }
              else
                do_ins = false;
              
              if (do_ins)
              {
                z = new_glue(s);
                subtype(z) = xkanji_skip_code + 1;
                link(z) = link(q);
                link(q) = z;
                q = z;
              }
            }

            if (auto_xsp_code(ax) >= 2)
              insert_skip = after_schar;
            else
              insert_skip = no_skip;
          }

          q = p;
          p = link(p);
          incr(i);
          
          if ((i > 5) && pf)
          {
            if (is_char_node(v))
              if (font_dir[font(v)] != dir_default)
                v=link(v);
            
            v=link(v);
          }
        }
      while (is_char_node(p));
    }
    else
    {
      switch (type(p))
      {
        case hlist_node:
          {
            find_first_char = true;
            first_char = 0;
            last_char = 0;
            
            if (shift_amount(p) == 0)
            {
              if (check_box(list_ptr(p)))
              {
                if (first_char != 0)
                  if (type(first_char) == math_node)
                  {
                    ax = '0';
                    
                    if (insert_skip == after_wchar)
                    {
                      if ((auto_xsp_code(ax) % 2) == 1)
                      {
                        x = get_inhibit_pos(cx, cur_pos);
                        
                        if (x != no_entry)
                          if ((inhibit_xsp_type(x) == inhibit_both) || (inhibit_xsp_type(x) == inhibit_after))
                            do_ins = false;
                          else
                            do_ins = true;
                        else
                          do_ins = true;
                      }
                      else
                        do_ins = false;
                      
                      if (do_ins)
                      {
                        z = new_glue(s);
                        subtype(z) = xkanji_skip_code + 1;
                        link(z) = link(q);
                        link(q) = z;
                        q = z;
                      }
                    }
                  }
                  else if (font_dir[font(first_char)] != dir_default)
                  {
                    cx = info(link(first_char));
                    
                    if (insert_skip == after_schar)
                    {
                      x = get_inhibit_pos(cx, cur_pos);

                      if (x != no_entry)
                        if ((inhibit_xsp_type(x) == inhibit_both) ||
                          (inhibit_xsp_type(x) == inhibit_previous))
                          do_ins = false;
                        else
                          do_ins = true;
                      else
                        do_ins = true;
                      
                      if (do_ins)
                      {
                        z = new_glue(s);
                        subtype(z) = xkanji_skip_code + 1;
                        link(z) = link(q);
                        link(q) = z;
                        q = z;
                      }
                    }
                    else if (insert_skip == after_wchar)
                    {
                      z = new_glue(u);
                      subtype(z) = kanji_skip_code + 1;
                      link(z) = link(q);
                      link(q) = z;
                      q = z;
                    }
                  }
                  else
                  {
                    ax = character(first_char);

                    if (insert_skip == after_wchar)
                    {
                      if ((auto_xsp_code(ax) % 2) == 1)
                      {
                        x = get_inhibit_pos(cx, cur_pos);
                        
                        if (x != no_entry)
                          if ((inhibit_xsp_type(x) == inhibit_both) ||
                            (inhibit_xsp_type(x) == inhibit_after))
                            do_ins = false;
                          else
                            do_ins = true;
                        else
                          do_ins = true;
                      }
                      else
                        do_ins = false;
                      
                      if (do_ins)
                      {
                        z = new_glue(s);
                        subtype(z) = xkanji_skip_code + 1;
                        link(z) = link(q);
                        link(q) = z;
                        q = z;
                      }
                    }
                  }

                if (last_char != 0)
                {
                  if (type(last_char) == math_node)
                  {
                    ax = '0';
                    
                    if (auto_xsp_code(ax) >= 2)
                      insert_skip = after_schar;
                    else
                      insert_skip = no_skip;
                  }
                  else if (font_dir[font(last_char)] != dir_default)
                  {
                    insert_skip = after_wchar;
                    cx = info(link(last_char));
                    
                    if ((is_char_node(link(p)) && (font_dir[font(link(p))] != dir_default)))
                    {
                      z = new_glue(u);
                      subtype(z) = kanji_skip_code + 1;
                      link(z) = link(p);
                      link(p) = z;
                      p = link(z);
                      q = z;
                      p = link(p);
                    }
                  }
                  else
                  {
                    ax = character(last_char);
                    
                    if (auto_xsp_code(ax) >= 2)
                      insert_skip = after_schar;
                    else
                      insert_skip = no_skip;
                  }
                }
                else
                  insert_skip = no_skip;
              }
              else
                insert_skip = no_skip;
            }
            else
              insert_skip = no_skip;
          }
          break;
          
        case ligature_node:
          {
            t = lig_ptr(p);
            
            if (is_char_node(t))
            {
              ax = character(t);
              
              if (insert_skip == after_wchar)
              {
                if ((auto_xsp_code(ax) % 2) == 1)
                {
                  x = get_inhibit_pos(cx, cur_pos);
                
                  if (x != no_entry)
                    if ((inhibit_xsp_type(x) == inhibit_both) || (inhibit_xsp_type(x) == inhibit_after))
                      do_ins = false;
                    else
                      do_ins = true;
                  else
                    do_ins = true;
                }
                else
                  do_ins = false;
              
                if (do_ins)
                {
                  z = new_glue(s);
                  subtype(z) = xkanji_skip_code + 1;
                  link(z) = link(q);
                  link(q) = z;
                  q = z;
                }
              }
              
              while (link(t) != 0)
                t = link(t);
              
              if (is_char_node(t))
              {
                ax = character(t);
                
                if (auto_xsp_code(ax) >= 2)
                  insert_skip = after_schar;
                else
                  insert_skip = no_skip;
              }
            }
          }
          break;
          
        case penalty_node:
        case disp_node:
          {
            if (is_char_node(link(p)))
            {
              q = p;
              p = link(p);
              
              if (font_dir[font(p)] != dir_default)
              {
                cx = info(link(p));
                
                if (insert_skip == after_schar)
                {
                  x = get_inhibit_pos(cx, cur_pos);

                  if (x != no_entry)
                    if ((inhibit_xsp_type(x) == inhibit_both) || (inhibit_xsp_type(x) == inhibit_previous))
                      do_ins = false;
                    else
                      do_ins = true;
                  else
                    do_ins = true;

                  if (do_ins)
                  {
                    z = new_glue(s);
                    subtype(z) = xkanji_skip_code + 1;
                    link(z) = link(q);
                    link(q) = z;
                    q = z;
                  }
                }
                else if (insert_skip == after_wchar)
                {
                  z = new_glue(u);
                  subtype(z) = kanji_skip_code + 1;
                  link(z) = link(q);
                  link(q) = z;
                  q = z;
                }

                p = link(p);
                insert_skip = after_wchar;
              }
              else
              {
                ax = character(p);
                
                if (insert_skip == after_wchar)
                {
                  if ((auto_xsp_code(ax) % 2) == 1)
                  {
                    x = get_inhibit_pos(cx, cur_pos);
                    if (x != no_entry)
                      if ((inhibit_xsp_type(x) == inhibit_both) || (inhibit_xsp_type(x) == inhibit_after))
                        do_ins = false;
                      else
                        do_ins = true;
                    else
                      do_ins = true;
                  }
                  else
                    do_ins = false;

                  if (do_ins)
                  {
                    z = new_glue(s);
                    subtype(z) = xkanji_skip_code + 1;
                    link(z) = link(q);
                    link(q) = z;
                    q = z;
                  }
                }

                if (auto_xsp_code(ax) >= 2)
                  insert_skip = after_schar;
                else
                  insert_skip = no_skip;
              }
            }
          }
          break;
        
        case kern_node:
          if (subtype(p) == explicit)
            insert_skip = no_skip;
          else if (subtype(p) == acc_kern)
          {
            if (q == p)
            {
              t = link(p);
              
              if (is_char_node(t))
                if (font_dir[font(t)] != dir_default)
                  t = link(t);
              
              p = link(link(t));
              
              if (font_dir[font(p)] != dir_default)
              {
                p = link(p);
                insert_skip = after_wchar;
              }
              else
                insert_skip = after_schar;
            }
            else
            {
              a = p;
              t = link(p);
              
              if (is_char_node(t))
                if (font_dir[font(t)] != dir_default)
                  t = link(t);
              
              t = link(link(t));
              link(q) = t;
              p = t;
              if (font_dir[font(p)] != dir_default)
              {
                cx = info(link(p));
            
                if (insert_skip == after_schar)
                {
                  x = get_inhibit_pos(cx, cur_pos);
              
                  if (x != no_entry)
                    if ((inhibit_xsp_type(x) == inhibit_both) ||
                      (inhibit_xsp_type(x) == inhibit_previous))
                      do_ins = false;
                    else
                      do_ins = true;
                  else
                    do_ins = true;
                }
            
                if (do_ins)
                {
                  z = new_glue(s);
                  subtype(z) = xkanji_skip_code + 1;
                  link(z) = link(q);
                  link(q) = z;
                  q = z;
                }

                p = link(p);
                insert_skip = after_wchar;
              }
              else
              {
                ax = character(p);
            
                if (insert_skip == after_wchar)
                {
                  if ((auto_xsp_code(ax) % 2) == 1)
                  {
                    x = get_inhibit_pos(cx, cur_pos);
                
                    if (x != no_entry)
                      if ((inhibit_xsp_type(x) == inhibit_both) ||
                        (inhibit_xsp_type(x) == inhibit_after))
                        do_ins = false;
                      else
                        do_ins = true;
                    else
                      do_ins = true;
                  }
                  else
                    do_ins = false;
              
                  if (do_ins)
                  {
                    z = new_glue(s);
                    subtype(z) = xkanji_skip_code + 1;
                    link(z) = link(q);
                    link(q) = z;
                    q = z;
                  }
                }

                if (auto_xsp_code(ax) >= 2)
                  insert_skip = after_schar;
                else
                  insert_skip = no_skip;
              }
              incr(i);
              
              if ((i > 5) && pf)
              {
                if (is_char_node(v))
                  if (font_dir[font(v)] != dir_default)
                    v = link(v);
                
                v = link(v);
              }
              
              if (link(q) != t)
                link(link(q)) = a;
              else
                link(q) = a;
            }
          }
          break;
        
        case math_node:
          {
            if ((subtype(p) == before) && (insert_skip == after_wchar))
            {
              ax = '0';

              {
                if ((auto_xsp_code(ax) % 2) == 1)
                {
                  x = get_inhibit_pos(cx, cur_pos);
                  if (x != no_entry)
                    if ((inhibit_xsp_type(x) == inhibit_both) || (inhibit_xsp_type(x) == inhibit_after))
                      do_ins = false;
                    else
                      do_ins = true;
                  else
                    do_ins = true;
                }
                else
                  do_ins = false;
                
                if (do_ins)
                {
                  z = new_glue(s);
                  subtype(z) = xkanji_skip_code + 1;
                  link(z) = link(q);
                  link(q) = z;
                  q = z;
                }
              }

              insert_skip = no_skip;
            }
            else if (subtype(p) == after)
            {
              ax = '0'; 
              
              if (auto_xsp_code(ax) >= 2)
                insert_skip = after_schar;
              else
                insert_skip = no_skip;
            }
            else
              insert_skip = no_skip;
          }
          break;
          
        case mark_node:
        case adjust_node:
        case ins_node:
        case whatsit_node:
          break;
          
        default:
          insert_skip = no_skip;
          break;
      }
      q = p;
      p = link(p);
    }
  }
  
  if (!is_char_node(q) && (type(q) == glue_node) && (subtype(q) == jfm_skip + 1))
  {
    delete_glue_ref(glue_ptr(q));
    glue_ptr(q) = zero_glue;
    add_glue_ref(zero_glue);
  }
  
  delete_glue_ref(u);
  delete_glue_ref(s);
  
  if ((v != 0) && pf && (i > 5))
  {
    q = v;
    p = link(v);
    
    if (is_char_node(v) && (font_dir[font(v)] !=dir_default))
    {
      q = p;
      p = link(p);
    }

    t = q;
    s = 0;
    
    while (p != 0)
    {
      if (is_char_node(p))
      {
        if (font_dir[font(p)] != dir_default)
        {
          cx = info(link(p));
          i = kcat_code(kcatcodekey(cx));
          k = 0;
          
          if ((i == kanji) || (i == kana))
          {
            t = q;
            s = p;
          }
          
          p = link(p);
          q = p;
        }
        else
        {
          k = k + 1;
          
          if (k > 1)
          {
            q = p;
            s = 0;
          }
        }
      }
      else
      {
        switch (type(p))
        {
          case penalty_node:
          case mark_node:
          case adjust_node:
          case whatsit_node:
          case glue_node:
          case kern_node:
          case math_node:
          case disp_node:
            break;
          default:
            {
              q = p;
              s = 0;
            }
        }
      }
      
      p = link(p);
    }
    
    if (s != 0)
    {
      s = link(t);
      
      if (!is_char_node(s) && (type(s) == penalty_node))
        penalty(s) = penalty(s) + jchr_widow_penalty;
      else if (jchr_widow_penalty != 0)
      {
        s = new_penalty(jchr_widow_penalty);
        subtype(s) = widow_pena;
        link(s) = link(t);
        link(t) = s;
        t = link(s);
        
        while (!is_char_node(t))
        {
          if ((type(t) == glue_node) || (type(t) == kern_node))
            goto labexit;
          
          t = link(t);
        }

        z = new_glue(u);
        subtype(z) = kanji_skip_code + 1;
        link(z) = link(s);
        link(s) = z;
      }
    }
  }
labexit:;
}
