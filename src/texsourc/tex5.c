#ifdef _WINDOWS
  #define NOCOMM
  #define NOSOUND
  #define NODRIVERS
  #define STRICT
  #pragma warning(disable:4115) // kill rpcasync.h complaint
  #include <windows.h>
  #define MYLIBAPI __declspec(dllexport)
#endif

#include "texwin.h"

#pragma warning(disable:4131) // old style declarator
#pragma warning(disable:4135) // conversion between different integral types 
#pragma warning(disable:4127) // conditional expression is constant

#include <setjmp.h>

#define EXTERN extern

#include "texd.h"

#pragma warning(disable:4244)       /* 96/Jan/10 */


/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */

/* rebox_ used to be in tex4.c */

halfword rebox_(halfword b, scaled w)
{register halfword Result; 
  halfword p; 
  internal_font_number f; 
  scaled v; 
/* begin if (width(b)<>w)and(list_ptr(b)<>null) then l.14010 */
  if((mem[b + 1].cint != w)&&(mem[b + 5].hh.v.RH != 0)) 
  {
    if(mem[b].hh.b0 == 1)
    b = hpack(b, 0, 1); 
    p = mem[b + 5].hh.v.RH; 
/*  if (is_char_node(p))and(link(p)=null) then l.14013 */
    if(((p >= hi_mem_min)) &&(mem[p].hh.v.RH == 0)) 
    {
      f = mem[p].hh.b0; 
      v = font_info[width_base[f]+ font_info[char_base[f]+ mem[p]
      .hh.b1].qqqq.b0].cint; 
      if(v != mem[b + 1].cint)
      mem[p].hh.v.RH = new_kern(mem[b + 1].cint - v); 
    } 
    free_node(b, 7); 
    b = new_glue(12); 
    mem[b].hh.v.RH = p; 
/*   while link(p)<>null do p:=link(p); l.14019 */
    while(mem[p].hh.v.RH != 0)p = mem[p].hh.v.RH; 
    mem[p].hh.v.RH = new_glue(12); 
    Result = hpack(b, w, 0); 
  } 
  else {
      
    mem[b + 1].cint = w; 
    Result = b; 
  } 
  return Result; 
} 
/* This is to be the start of tex5.c */
halfword math_glue_(halfword g, scaled m)
{register halfword Result; 
  halfword p; 
  integer n; 
  scaled f; 
  n = x_over_n(m, 65536L); 
  f = tex_remainder; 
  if(f < 0)
  {
    decr(n); 
    f = f + 65536L; 
  } 
  p = get_node(4); 
  mem[p + 1].cint = mult_and_add(n, mem[g + 1].cint, xn_over_d(mem[
  g + 1].cint, f, 65536L), 1073741823L);  /* 2^30 - 1 */
  mem[p].hh.b0 = mem[g].hh.b0; 
  if(mem[p].hh.b0 == 0)
  mem[p + 2].cint = mult_and_add(n, mem[g + 2].cint, xn_over_d(mem[
  g + 2].cint, f, 65536L), 1073741823L);  /* 2^30 - 1 */
  else mem[p + 2].cint = mem[g + 2].cint; 
  mem[p].hh.b1 = mem[g].hh.b1; 
  if(mem[p].hh.b1 == 0)
  mem[p + 3].cint = mult_and_add(n, mem[g + 3].cint, xn_over_d(mem[
  g + 3].cint, f, 65536L), 1073741823L);  /* 2^30 - 1 */
  else mem[p + 3].cint = mem[g + 3].cint; 
  Result = p; 
  return Result; 
} 
void math_kern_ (halfword p, scaled m)
{ 
  integer n; 
  scaled f; 
  if(mem[p].hh.b1 == 99)
  {
    n = x_over_n(m, 65536L); 
    f = tex_remainder; 
    if(f < 0)
    {
      decr(n); 
      f = f + 65536L; 
    } 
    mem[p + 1].cint = mult_and_add(n, mem[p + 1].cint, xn_over_d(mem 
   [p + 1].cint, f, 65536L), 1073741823L);  /* 2^30 - 1 */
/*    mem[p].hh.b1 = 0;  */
    mem[p].hh.b1 = 1; /* changed in 3.14159 */
  } 
} 
void flush_math (void) 
{ 
  flush_node_list(mem[head].hh.v.RH); 
  flush_node_list(cur_list.aux_field.cint); 
  mem[head].hh.v.RH = 0; 
  tail = head; 
  cur_list.aux_field.cint = 0; 
} 
halfword clean_box_(halfword p, small_number s)
{/* 40 */ register halfword Result; 
  halfword q; 
  small_number savestyle; 
  halfword x; 
  halfword r; 
  switch(mem[p].hh.v.RH)
  {case 1 : 
    {
      cur_mlist = new_noad (); 
      mem[cur_mlist + 1]= mem[p]; 
    } 
    break; 
  case 2 : 
    {
      q = mem[p].hh.v.LH; 
      goto lab40; 
    } 
    break; 
  case 3 : 
    cur_mlist = mem[p].hh.v.LH; 
    break; 
    default: 
    {
      q = new_null_box (); 
      goto lab40; 
    } 
    break; 
  } 
  savestyle = cur_style; 
  cur_style = s; 
  mlist_penalties = false; 
  mlist_to_hlist (); 
  q = mem[mem_top - 3].hh.v.RH; 
  cur_style = savestyle; 
  {
    if(cur_style < 4)
    cur_size = 0; 
    else cur_size = 16 *((cur_style - 2)/ 2); 
    cur_mu = x_over_n(font_info[6 +
    param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint, 18); 
  } 
  lab40: if((q >= hi_mem_min)||(q == 0)) 
  x = hpack(q, 0, 1); 
  else if((mem[q].hh.v.RH == 0)&&(mem[q].hh.b0 <= 1)&&(mem 
 [q + 4].cint == 0)) 
  x = q; 
  else x = hpack(q, 0, 1); 
  q = mem[x + 5].hh.v.RH; 
  if((q >= hi_mem_min)) 
  {
    r = mem[q].hh.v.RH; 
/*   if r<>null then if link(r)=null then l.14140 */
    if(r != 0)
    if(mem[r].hh.v.RH == 0)
    if(!(r >= hi_mem_min)) 
    if(mem[r].hh.b0 == 11)
    {
      free_node(r, 2); 
      mem[q].hh.v.RH = 0;   /* link(q):=null; */
    } 
  } 
  Result = x; 
  return Result; 
} 
void fetch_(halfword a)
{
  cur_c = mem[a].hh.b1; 
  cur_f = eqtb[(hash_size + 1835) + mem[a].hh.b0 + cur_size].hh.v.RH; 
  if(cur_f == 0)
  {
	  print_err("");
    print_size(cur_size); 
    print_char(32);   /*   */
    print_int(mem[a].hh.b0); 
    print(878);     /* is undefined (character  */
    print(cur_c); 
    print_char(41);   /*)*/
	help4("Somewhere in the math formula just ended, you used the",
		"stated character from an undefined font family. For example,",
		"plain TeX doesn't allow \\it or \\sl in subscripts. Proceed,",
		"and I'll try to forget that I needed that character.");
    error (); 
    cur_i = null_character; 
    mem[a].hh.v.RH = 0; 
  } 
  else {
      
    if((cur_c >= font_bc[cur_f])&&(cur_c <= font_ec[cur_f])) 
    cur_i = font_info[char_base[cur_f]+ cur_c].qqqq; 
    else cur_i = null_character; 
    if(!((cur_i.b0 > 0)))
    {
      char_warning(cur_f, cur_c); 
      mem[a].hh.v.RH = 0; 
    } 
  } 
} 
void make_over_(halfword q)
{ 
  mem[q + 1].hh.v.LH = overbar(clean_box(q + 1, 2 *(cur_style / 2)
  + 1), 3 * font_info[8 + param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]
].cint, font_info[8 + param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]] 
 .cint); 
  mem[q + 1].hh.v.RH = 2; 
} 
void make_under_(halfword q)
{
  halfword p, x, y; 
  scaled delta; 
  x = clean_box(q + 1, cur_style); 
  p = new_kern(3 * font_info[8 +
    param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]].cint); 
  mem[x].hh.v.RH = p; 
  mem[p].hh.v.RH = fraction_rule(font_info[8 +
    param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]].cint); 
  y = vpackage(x, 0, 1, 1073741823L); /* 2^30 - 1 */
  delta = mem[y + 3].cint + mem[y + 2].cint + font_info[8 +
    param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]].cint; 
  mem[y + 3].cint = mem[x + 3].cint; 
  mem[y + 2].cint = delta - mem[y + 3].cint; 
  mem[q + 1].hh.v.LH = y; 
  mem[q + 1].hh.v.RH = 2; 
} 
void make_vcenter_(halfword q)
{ 
  halfword v; 
  scaled delta; 
  v = mem[q + 1].hh.v.LH; 
  if(mem[v].hh.b0 != 1){
    confusion(536);   /* vcenter */
    return;         // abort_flag set
  }
  delta = mem[v + 3].cint + mem[v + 2].cint; 
  mem[v + 3].cint = font_info[22 +
    param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint + half(delta); 
  mem[v + 2].cint = delta - mem[v + 3].cint; 
} 
void make_radical_(halfword q)
{ 
  halfword x, y; 
  scaled delta, clr; 
  x = clean_box(q + 1, 2 *(cur_style / 2)+ 1); 
  if(cur_style < 2)
  clr = font_info[8 +
  param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]].cint +
    (abs(font_info[5 + param_base[eqtb[(hash_size + 1837) +
        cur_size].hh.v.RH]].cint)/ 4); 
  else {
      
    clr = font_info[8 +
      param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]].cint; 
    clr = clr +(abs(clr)/ 4); 
  } 
  y = var_delimiter(q + 4, cur_size, mem[x + 3].cint + mem[x + 2]
 .cint + clr + font_info[8 +
    param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]].cint); 
  delta = mem[y + 2].cint -(mem[x + 3].cint + mem[x + 2].cint + 
  clr); 
  if(delta > 0)
  clr = clr + half(delta); 
  mem[y + 4].cint = - (integer)(mem[x + 3].cint + clr); 
  mem[y].hh.v.RH = overbar(x, clr, mem[y + 3].cint); 
  mem[q + 1].hh.v.LH = hpack(y, 0, 1); 
  mem[q + 1].hh.v.RH = 2; 
} 
void make_math_accent_(halfword q)
{/* 30 31 */ 
  halfword p, x, y; 
  integer a; 
  quarterword c; 
  internal_font_number f; 
  ffourquarters i; 
  scaled s; 
  scaled h; 
  scaled delta; 
  scaled w; 
  fetch(q + 4); 
  if((cur_i.b0 > 0)) 
  {
    i = cur_i; 
    c = cur_c; 
    f = cur_f; 
    s = 0; 
    if(mem[q + 1].hh.v.RH == 1)
    {
      fetch(q + 1); 
      if(((cur_i.b2)% 4)== 1)
      {
  a = lig_kern_base[cur_f]+ cur_i.b3; 
  cur_i = font_info[a].qqqq; 
  if(cur_i.b0 > 128)
  {
    a = lig_kern_base[cur_f]+ 256 * cur_i.b2 + cur_i.b3 + 32768L - 256 
    *(128); 
    cur_i = font_info[a].qqqq; 
  } 
  while(true){
    if(cur_i.b1 == skew_char[cur_f])
    {
      if(cur_i.b2 >= 128)
      if(cur_i.b0 <= 128)
      s = font_info[kern_base[cur_f]+ 256 * cur_i.b2 + cur_i.b3]
     .cint; 
      goto lab31; 
    } 
    if(cur_i.b0 >= 128)
    goto lab31; 
    a = a + cur_i.b0 + 1; 
    cur_i = font_info[a].qqqq; 
  } 
      } 
    } 
    lab31:; 
    x = clean_box(q + 1, 2 *(cur_style / 2)+ 1); 
    w = mem[x + 1].cint; 
    h = mem[x + 3].cint; 
    while(true){
      if(((i.b2)% 4)!= 2)
      goto lab30; 
      y = i.b3; 
      i = font_info[char_base[f]+ y].qqqq; 
      if(!(i.b0 > 0)) 
      goto lab30; 
      if(font_info[width_base[f]+ i.b0].cint > w)
      goto lab30; 
/*    long to unsigned short ... */
      c = y; 
    } 
    lab30:; 
    if(h < font_info[5 + param_base[f]].cint)
    delta = h; 
    else delta = font_info[5 + param_base[f]].cint; 
    if((mem[q + 2].hh.v.RH != 0)||(mem[q + 3].hh.v.RH != 0)
  )
    if(mem[q + 1].hh.v.RH == 1)
    {
      flush_node_list(x); 
      x = new_noad (); 
      mem[x + 1]= mem[q + 1]; 
      mem[x + 2]= mem[q + 2]; 
      mem[x + 3]= mem[q + 3]; 
      mem[q + 2].hh = empty_field; 
      mem[q + 3].hh = empty_field; 
      mem[q + 1].hh.v.RH = 3; 
      mem[q + 1].hh.v.LH = x; 
      x = clean_box(q + 1, cur_style); 
      delta = delta + mem[x + 3].cint - h; 
      h = mem[x + 3].cint; 
    } 
    y = char_box(f, c); 
    mem[y + 4].cint = s + half(w - mem[y + 1].cint); 
    mem[y + 1].cint = 0; 
    p = new_kern(- (integer) delta); 
    mem[p].hh.v.RH = x; 
    mem[y].hh.v.RH = p; 
    y = vpackage(y, 0, 1, 1073741823L);  /* 2^30 - 1 */
    mem[y + 1].cint = mem[x + 1].cint; 
    if(mem[y + 3].cint < h)
    {
      p = new_kern(h - mem[y + 3].cint); 
      mem[p].hh.v.RH = mem[y + 5].hh.v.RH; 
      mem[y + 5].hh.v.RH = p; 
      mem[y + 3].cint = h; 
    } 
    mem[q + 1].hh.v.LH = y; 
    mem[q + 1].hh.v.RH = 2; 
  } 
} 
void make_fraction_(halfword q) 
{
  halfword p, v, x, y, z; 
  scaled delta, delta1, delta2, shiftup, shiftdown, clr; 
  if(mem[q + 1].cint == 1073741824L) /* 2^30 */
  mem[q + 1].cint = font_info[8 +
    param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]].cint; 
  x = clean_box(q + 2, cur_style + 2 - 2 *(cur_style / 6)); 
  z = clean_box(q + 3, 2 *(cur_style / 2)+ 3 - 2 *(cur_style / 6)); 
  if(mem[x + 1].cint < mem[z + 1].cint)
  x = rebox(x, mem[z + 1].cint); 
  else z = rebox(z, mem[x + 1].cint); 
  if(cur_style < 2)
  {
    shiftup = font_info[8 +
      param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint; 
    shiftdown = font_info[11 +
    param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint; 
  } 
  else {
      
    shiftdown = font_info[12 +
    param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint; 
    if(mem[q + 1].cint != 0)
    shiftup = font_info[9 +
    param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint; 
    else shiftup = font_info[10 +
    param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint;
  } 
  if(mem[q + 1].cint == 0)
  {
    if(cur_style < 2)
    clr = 7 * font_info[8 + param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH]
  ].cint; 
    else clr = 3 * font_info[8 + param_base[eqtb[(hash_size + 1838) + cur_size].hh 
   .v.RH]].cint; 
    delta = half(clr -((shiftup - mem[x + 2].cint)-(mem[z + 3]
   .cint - shiftdown))); 
    if(delta > 0)
    {
      shiftup = shiftup + delta; 
      shiftdown = shiftdown + delta; 
    } 
  } 
  else {
      
    if(cur_style < 2)
    clr = 3 * mem[q + 1].cint; 
    else clr = mem[q + 1].cint; 
    delta = half(mem[q + 1].cint); 
    delta1 = clr -((shiftup - mem[x + 2].cint)-(font_info[22 + 
    param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint + delta)); 
    delta2 = clr -((font_info[22 + param_base[eqtb[(hash_size + 1837) + cur_size]
   .hh.v.RH]].cint - delta)-(mem[z + 3].cint - shiftdown)); 
    if(delta1 > 0)
    shiftup = shiftup + delta1; 
    if(delta2 > 0)
    shiftdown = shiftdown + delta2; 
  } 
  v = new_null_box (); 
  mem[v].hh.b0 = 1; 
  mem[v + 3].cint = shiftup + mem[x + 3].cint; 
  mem[v + 2].cint = mem[z + 2].cint + shiftdown; 
  mem[v + 1].cint = mem[x + 1].cint; 
  if(mem[q + 1].cint == 0)
  {
    p = new_kern((shiftup - mem[x + 2].cint)-(mem[z + 3].cint - 
    shiftdown)); 
    mem[p].hh.v.RH = z; 
  } 
  else {
      
    y = fraction_rule(mem[q + 1].cint); 
    p = new_kern((font_info[22 + param_base[eqtb[(hash_size + 1837) + cur_size].hh 
   .v.RH]].cint - delta)-(mem[z + 3].cint - shiftdown)); 
    mem[y].hh.v.RH = p; 
    mem[p].hh.v.RH = z; 
    p = new_kern((shiftup - mem[x + 2].cint)-(font_info[22 + 
    param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint + delta)); 
    mem[p].hh.v.RH = y; 
  } 
  mem[x].hh.v.RH = p; 
  mem[v + 5].hh.v.RH = x; 
  if(cur_style < 2)
  delta = font_info[20 + param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]] 
 .cint; 
  else delta = font_info[21 + param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH 
]].cint; 
  x = var_delimiter(q + 4, cur_size, delta); 
  mem[x].hh.v.RH = v; 
  z = var_delimiter(q + 5, cur_size, delta); 
  mem[v].hh.v.RH = z; 
  mem[q + 1].cint = hpack(x, 0, 1); 
} 
/***************************************************************************/
/* moved to end to avoid questions about pragma optimize 96/Sep/12 */
scaled make_op_ (halfword);
/***************************************************************************/
void make_ord_(halfword q)
{/* 20 10 */ 
  integer a; 
  halfword p, r; 
  lab20: if(mem[q + 3].hh.v.RH == 0)
  if(mem[q + 2].hh.v.RH == 0)
  if(mem[q + 1].hh.v.RH == 1)
  {
    p = mem[q].hh.v.RH; 
    if(p != 0)
    if((mem[p].hh.b0 >= 16)&&(mem[p].hh.b0 <= 22)) 
    if(mem[p + 1].hh.v.RH == 1)
    if(mem[p + 1].hh.b0 == mem[q + 1].hh.b0)
    {
      mem[q + 1].hh.v.RH = 4; 
      fetch(q + 1); 
      if(((cur_i.b2)% 4)== 1)
      {
  a = lig_kern_base[cur_f]+ cur_i.b3; 
  cur_c = mem[p + 1].hh.b1; 
  cur_i = font_info[a].qqqq; 
  if(cur_i.b0 > 128)
  {
    a = lig_kern_base[cur_f]+ 256 * cur_i.b2 + cur_i.b3 + 32768L - 256 
    *(128); 
    cur_i = font_info[a].qqqq; 
  } 
  while(true){
    if(cur_i.b1 == cur_c)
    if(cur_i.b0 <= 128)
    if(cur_i.b2 >= 128)
    {
      p = new_kern(font_info[kern_base[cur_f]+ 256 * cur_i.b2 + cur_i 
     .b3].cint); 
      mem[p].hh.v.RH = mem[q].hh.v.RH; 
      mem[q].hh.v.RH = p; 
      return; 
    } 
    else {
        
      {
        if(interrupt != 0){
        pause_for_instructions ();
      }
      } 
      switch(cur_i.b2)
      {case 1 : 
      case 5 : 
        mem[q + 1].hh.b1 = cur_i.b3; 
        break; 
      case 2 : 
      case 6 : 
        mem[p + 1].hh.b1 = cur_i.b3; 
        break; 
      case 3 : 
      case 7 : 
      case 11 : 
        {
    r = new_noad (); 
    mem[r + 1].hh.b1 = cur_i.b3; 
    mem[r + 1].hh.b0 = mem[q + 1].hh.b0; 
    mem[q].hh.v.RH = r; 
    mem[r].hh.v.RH = p; 
    if(cur_i.b2 < 11)
    mem[r + 1].hh.v.RH = 1; 
    else mem[r + 1].hh.v.RH = 4; 
        } 
        break; 
        default: 
        {
    mem[q].hh.v.RH = mem[p].hh.v.RH; 
    mem[q + 1].hh.b1 = cur_i.b3; 
    mem[q + 3]= mem[p + 3]; 
    mem[q + 2]= mem[p + 2]; 
    free_node(p, 4); 
        } 
        break; 
      } 
      if(cur_i.b2 > 3)
      return; 
      mem[q + 1].hh.v.RH = 1; 
      goto lab20; 
    } 
    if(cur_i.b0 >= 128)
    return; 
    a = a + cur_i.b0 + 1; 
    cur_i = font_info[a].qqqq; 
  } 
      } 
    } 
  } 
} 
/***************************************************************************/
/* moved to end to avoid questions about pragma optimize 96/Sep/12 */
void make_scripts_ (halfword, scaled);
/***************************************************************************/
small_number make_left_right_(halfword q, small_number style, scaled maxd, scaled max_h)
{register small_number Result; 
  scaled delta, delta1, delta2; 
  if(style < 4)
  cur_size = 0; 
  else cur_size = 16 *((style - 2)/ 2); 
  delta2 = maxd + font_info[22 + param_base[eqtb[(hash_size + 1837) + cur_size].hh 
 .v.RH]].cint; 
  delta1 = max_h + maxd - delta2; 
  if(delta2 > delta1)
  delta1 = delta2; 
  delta =(delta1 / 500)* eqtb[(hash_size + 3181)].cint; 
  delta2 = delta1 + delta1 - eqtb[(hash_size + 3740)].cint; 
  if(delta < delta2)
  delta = delta2; 
  mem[q + 1].cint = var_delimiter(q + 1, cur_size, delta); 
  Result = mem[q].hh.b0 -(10); 
  return Result; 
} 
void mlist_to_hlist (void) 
{/* 21 82 80 81 83 30 */
  halfword mlist; 
  bool penalties; 
  small_number style; 
  small_number savestyle; 
  halfword q; 
  halfword r; 
/*  small_number rtype;  */
  int rtype;            /* 95/Jan/7 */
/*  small_number t; */
  int t;              /* 95/Jan/7 */
  halfword p, x, y, z; 
  integer pen; 
  small_number s; 
  scaled max_h, maxd; 
  scaled delta; 
  mlist = cur_mlist; 
  penalties = mlist_penalties; 
  style = cur_style; 
  q = mlist; 
  r = 0; 
  rtype = 17; 
  max_h = 0; 
  maxd = 0; 
  {
    if(cur_style < 4)
    cur_size = 0; 
    else cur_size = 16 *((cur_style - 2)/ 2); 
    cur_mu = x_over_n(font_info[6 + param_base[eqtb[(hash_size + 1837) + cur_size].hh 
   .v.RH]].cint, 18); 
  } 
  while(q != 0){
      
    lab21: delta = 0; 
    switch(mem[q].hh.b0){
    case 18 : 
      switch(rtype)
      {case 18 : 
      case 17 : 
      case 19 : 
      case 20 : 
      case 22 : 
      case 30 : 
  {
    mem[q].hh.b0 = 16; 
    goto lab21; 
  } 
  break; 
  default: 
  ; 
  break; 
      } 
      break; 
    case 19 : 
    case 21 : 
    case 22 : 
    case 31 : 
      {
  if(rtype == 18) mem[r].hh.b0 = 16; 
  if(mem[q].hh.b0 == 31)  goto lab80; 
      } 
      break; 
    case 30 : 
      goto lab80; 
      break; 
    case 25 : 
      {
  make_fraction(q); 
  goto lab82; 
      } 
      break; 
    case 17 : 
      {
  delta = make_op(q); 
  if(mem[q].hh.b1 == 1)
  goto lab82; 
      } 
      break; 
    case 16 : 
      make_ord(q); 
      break; 
    case 20 : 
    case 23 : 
   ; 
      break; 
    case 24 : 
      make_radical(q); 
      break; 
    case 27 : 
      make_over(q); 
      break; 
    case 26 : 
      make_under(q); 
      break; 
    case 28 : 
      make_math_accent(q); 
      break; 
    case 29 : 
      make_vcenter(q); 
      break; 
    case 14 : 
      {
  cur_style = mem[q].hh.b1; 
  {
    if(cur_style < 4)
    cur_size = 0; 
    else cur_size = 16 *((cur_style - 2)/ 2); 
    cur_mu = x_over_n(font_info[6 + param_base[eqtb[(hash_size + 1837) + cur_size]
   .hh.v.RH]].cint, 18); 
  } 
  goto lab81; 
      } 
      break; 
    case 15 : 
      {
  switch(cur_style / 2)
  {case 0 : 
    {
      p = mem[q + 1].hh.v.LH; 
      mem[q + 1].hh.v.LH = 0; 
    } 
    break; 
  case 1 : 
    {
      p = mem[q + 1].hh.v.RH; 
      mem[q + 1].hh.v.RH = 0; 
    } 
    break; 
  case 2 : 
    {
      p = mem[q + 2].hh.v.LH; 
      mem[q + 2].hh.v.LH = 0; 
    } 
    break; 
  case 3 : 
    {
      p = mem[q + 2].hh.v.RH; 
      mem[q + 2].hh.v.RH = 0; 
    } 
    break; 
  } 
  flush_node_list(mem[q + 1].hh.v.LH); 
  flush_node_list(mem[q + 1].hh.v.RH); 
  flush_node_list(mem[q + 2].hh.v.LH); 
  flush_node_list(mem[q + 2].hh.v.RH); 
  mem[q].hh.b0 = 14; 
  mem[q].hh.b1 = cur_style; 
  mem[q + 1].cint = 0; 
  mem[q + 2].cint = 0; 
  if(p != 0)/* if p<>null then l.14317 */
  {
    z = mem[q].hh.v.RH; 
    mem[q].hh.v.RH = p; 
/*   while link(p)<>null do p:=link(p); */
    while(mem[p].hh.v.RH != 0)p = mem[p].hh.v.RH; 
    mem[p].hh.v.RH = z; 
  } 
  goto lab81; 
      } 
      break; 
    case 3 : 
    case 4 : 
    case 5 : 
    case 8 : 
    case 12 : 
    case 7 : 
      goto lab81; 
      break; 
    case 2 : 
      {
  if(mem[q + 3].cint > max_h)
  max_h = mem[q + 3].cint; 
  if(mem[q + 2].cint > maxd)
  maxd = mem[q + 2].cint; 
  goto lab81; 
      } 
      break; 
    case 10 : 
      {
  if(mem[q].hh.b1 == 99)
  {
    x = mem[q + 1].hh.v.LH; 
    y = math_glue(x, cur_mu); 
    delete_glue_ref(x); 
    mem[q + 1].hh.v.LH = y; 
    mem[q].hh.b1 = 0; 
  } 
  else if((cur_size != 0)&&(mem[q].hh.b1 == 98)) 
  {
    p = mem[q].hh.v.RH; 
/*   if p<>null then if (type(p)=glue_node)or(type(p)=kern_node) then */
    if(p != 0)
    if((mem[p].hh.b0 == 10)||(mem[p].hh.b0 == 11)) 
    {
      mem[q].hh.v.RH = mem[p].hh.v.RH; 
      mem[p].hh.v.RH = 0; 
      flush_node_list(p); 
    } 
  } 
  goto lab81; 
      } 
      break; 
    case 11 : 
      {
  math_kern(q, cur_mu); 
  goto lab81; 
      } 
      break; 
      default: 
      {
        confusion(883); /* mlist1 */
        return;       // abort_flag set
      }
      break; 
  } /* end of switch */

    switch(mem[q + 1].hh.v.RH){
  case 1 : 
    case 4 : 
      {
  fetch(q + 1); 
  if((cur_i.b0 > 0)) 
  {
    delta = font_info[italic_base[cur_f]+(cur_i.b2)/ 4].cint; 
    p = new_character(cur_f, cur_c); 
    if((mem[q + 1].hh.v.RH == 4)&&(font_info[2 + param_base 
   [cur_f]].cint != 0)) 
    delta = 0; 
    if((mem[q + 3].hh.v.RH == 0)&&(delta != 0)) 
    {
      mem[p].hh.v.RH = new_kern(delta); 
      delta = 0; 
    } 
  } 
  else p = 0; 
      } 
      break; 
    case 0 : 
      p = 0; 
      break; 
    case 2 : 
      p = mem[q + 1].hh.v.LH; 
      break; 
    case 3 : 
      {
  cur_mlist = mem[q + 1].hh.v.LH; 
  savestyle = cur_style; 
  mlist_penalties = false; 
  mlist_to_hlist (); 
  cur_style = savestyle; 
  {
    if(cur_style < 4)
    cur_size = 0; 
    else cur_size = 16 *((cur_style - 2)/ 2); 
    cur_mu = x_over_n(font_info[6 + param_base[eqtb[(hash_size + 1837) + cur_size]
   .hh.v.RH]].cint, 18); 
  } 
  p = hpack(mem[mem_top - 3].hh.v.RH, 0, 1); 
      } 
      break; 
      default: 
      {
        confusion(884); /* mlist2 */
        return;       // abort_flag set
      }
      break; 
  } /* end of switch */
  
    mem[q + 1].cint = p;  /* p may be used without ... */
    if((mem[q + 3].hh.v.RH == 0)&&(mem[q + 2].hh.v.RH == 0)
  )
    goto lab82; 
    make_scripts(q, delta); 
    lab82: z = hpack(mem[q + 1].cint, 0, 1); 
    if(mem[z + 3].cint > max_h)
    max_h = mem[z + 3].cint; 
    if(mem[z + 2].cint > maxd)
    maxd = mem[z + 2].cint; 
    free_node(z, 7); 
    lab80: r = q; 
    rtype = mem[r].hh.b0; 
    lab81: q = mem[q].hh.v.RH; 
  }
  if(rtype == 18)
  mem[r].hh.b0 = 16; 
  p = mem_top - 3; 
  mem[p].hh.v.RH = 0; 
  q = mlist; 
  rtype = 0; 
  cur_style = style; 
  {
    if(cur_style < 4)
    cur_size = 0; 
    else cur_size = 16 *((cur_style - 2)/ 2); 
    cur_mu = x_over_n(font_info[6 + param_base[eqtb[(hash_size + 1837) + cur_size].hh 
   .v.RH]].cint, 18); 
  } 
  while(q != 0){
      
    t = 16; 
    s = 4; 
    pen = 10000; 
    switch(mem[q].hh.b0)
    {case 17 : 
    case 20 : 
    case 21 : 
    case 22 : 
    case 23 : 
      t = mem[q].hh.b0; 
      break; 
    case 18 : 
      {
  t = 18; 
  pen = eqtb[(hash_size + 3172)].cint; 
      } 
      break; 
    case 19 : 
      {
  t = 19; 
  pen = eqtb[(hash_size + 3173)].cint; 
      } 
      break; 
    case 16 : 
    case 29 : 
    case 27 : 
    case 26 : 
   ; 
      break; 
    case 24 : 
      s = 5; 
      break; 
    case 28 : 
      s = 5; 
      break; 
    case 25 : 
      {
  t = 23; 
  s = 6; 
      } 
      break; 
    case 30 : 
    case 31 : 
      t = make_left_right(q, style, maxd, max_h); 
      break; 
    case 14 : 
      {
  cur_style = mem[q].hh.b1; 
  s = 3; 
  {
    if(cur_style < 4)
    cur_size = 0; 
    else cur_size = 16 *((cur_style - 2)/ 2); 
    cur_mu = x_over_n(font_info[6 + param_base[eqtb[(hash_size + 1837) + cur_size]
   .hh.v.RH]].cint, 18); 
  } 
  goto lab83; 
      } 
      break; 
    case 8 : 
    case 12 : 
    case 2 : 
    case 7 : 
    case 5 : 
    case 3 : 
    case 4 : 
    case 10 : 
    case 11 : 
      {
  mem[p].hh.v.RH = q; 
  p = q; 
  q = mem[q].hh.v.RH; 
  mem[p].hh.v.RH = 0; 
  goto lab30; 
      } 
      break; 
      default: 
      {
        confusion(885); /* mlist3 */
        return;       // abort_flag set
      }
      break; 
    } 
    if(rtype > 0)
    {
      switch(str_pool[rtype * 8 + t + magic_offset])
      {case 48 : 
  x = 0; 
  break; 
      case 49 : 
  if(cur_style < 4)
  x = 15; 
  else x = 0; 
  break; 
      case 50 : 
  x = 15; 
  break; 
      case 51 : 
  if(cur_style < 4)
  x = 16; 
  else x = 0; 
  break; 
      case 52 : 
  if(cur_style < 4)
  x = 17; 
  else x = 0; 
  break; 
  default: 
    {
      confusion(887);   /* mlist4 */
      return;       // abort_flag set
    }
    break; 
      } 
      if(x != 0)
      {
  y = math_glue(eqtb[(hash_size + 782) + x].hh.v.RH, cur_mu); /* gluebase + x */
  z = new_glue(y); 
  mem[y].hh.v.RH = 0; 
  mem[p].hh.v.RH = z; 
  p = z; 
  mem[z].hh.b1 = x + 1;   /* x may be used without ... */
      } 
    } 
    if(mem[q + 1].cint != 0)
    {
      mem[p].hh.v.RH = mem[q + 1].cint; 
      do {
    p = mem[p].hh.v.RH; 
      } while(!(mem[p].hh.v.RH == 0)); 
    } 
    if(penalties)
    if(mem[q].hh.v.RH != 0)
    if(pen < 10000)
    {
      rtype = mem[mem[q].hh.v.RH].hh.b0; 
      if(rtype != 12)
      if(rtype != 19)
      {
  z = new_penalty(pen); 
  mem[p].hh.v.RH = z; 
  p = z; 
      } 
    } 
    rtype = t; 
    lab83: r = q; 
    q = mem[q].hh.v.RH; 
    free_node(r, s); 
    lab30:; 
  } 
} 
void push_alignment (void) 
{
  halfword p; 
  p = get_node(5); 
  mem[p].hh.v.RH = align_ptr; 
  mem[p].hh.v.LH = cur_align; 
  mem[p + 1].hh.v.LH = mem[mem_top - 8].hh.v.RH; 
  mem[p + 1].hh.v.RH = cur_span; 
  mem[p + 2].cint = cur_loop; 
  mem[p + 3].cint = align_state; 
  mem[p + 4].hh.v.LH = cur_head; 
  mem[p + 4].hh.v.RH = cur_tail; 
  align_ptr = p; 
  cur_head = get_avail (); 
} 
void pop_alignment (void) 
{ 
  halfword p; 
  {
    mem[cur_head].hh.v.RH = avail; 
    avail = cur_head; 
  ;
#ifdef STAT
    decr(dyn_used); 
#endif /* STAT */
  } 
  p = align_ptr; 
  cur_tail = mem[p + 4].hh.v.RH; 
  cur_head = mem[p + 4].hh.v.LH; 
  align_state = mem[p + 3].cint; 
  cur_loop = mem[p + 2].cint; 
  cur_span = mem[p + 1].hh.v.RH; 
  mem[mem_top - 8].hh.v.RH = mem[p + 1].hh.v.LH; 
  cur_align = mem[p].hh.v.LH; 
  align_ptr = mem[p].hh.v.RH; 
  free_node(p, 5); 
} 
void get_preamble_token (void) 
{/* 20 */ 
  lab20: get_token (); 
  while((cur_chr == 256)&&(cur_cmd == 4)) {
      
    get_token (); 
    if(cur_cmd > 100)
    {
      expand (); 
      get_token (); 
    } 
  } 
  if(cur_cmd == 9){
    fatal_error("(interwoven alignment preambles are not allowed)"); /*  */
    return;     // abort_flag set
  }
  if((cur_cmd == 75)&&(cur_chr == (hash_size + 793))) 
  {
    scan_optional_equals (); 
    scan_glue(2); 
    if(eqtb[(hash_size + 3206)].cint > 0)
    geq_define((hash_size + 793), 117, cur_val); 
    else eq_define((hash_size + 793), 117, cur_val); 
    goto lab20; 
  } 
} 
void init_align (void) 
{/* 30 31 32 22 */ 
  halfword savecsptr; 
  halfword p; 
  savecsptr = cur_cs; 
  push_alignment (); 
  align_state = -1000000L; 
  if((mode == 203)&&((tail != cur_list 
 .head_field)||(cur_list.aux_field.cint != 0)))
  {
	  print_err("Improper ");
    print_esc("halign");
    print(888);     /* inside $$ */
	help3("Displays can use special alignments (like \\eqalignno)",
		"only if nothing but the alignment itself is between $$'s.",
		"So I've deleted the formulas that preceded this alignment.");
    error (); 
    flush_math (); 
  } 
  push_nest (); 
  if(mode == 203)
  {
    mode = -1; 
    cur_list.aux_field.cint = nest[nest_ptr - 2].aux_field.cint; 
  } 
  else if(mode > 0)
/*    long to short ... */
  mode = - (integer) mode; 
  scan_spec(6, false); 
  mem[mem_top - 8].hh.v.RH = 0; 
  cur_align = mem_top - 8; 
  cur_loop = 0; 
  scanner_status = 4; 
  warning_index = savecsptr; 
  align_state = -1000000L; 
  while(true){
    mem[cur_align].hh.v.RH = new_param_glue(11); 
    cur_align = mem[cur_align].hh.v.RH; 
    if(cur_cmd == 5)
    goto lab30; 
    p = mem_top - 4; 
    mem[p].hh.v.RH = 0; 
    while(true){
      get_preamble_token(); 
      if(cur_cmd == 6)
      goto lab31; 
      if((cur_cmd <= 5)&&(cur_cmd >= 4)&&(align_state == -1000000L)) 
      if((p == mem_top - 4)&&(cur_loop == 0)&&(cur_cmd == 4)) 
      cur_loop = cur_align; 
      else {
    print_err("Missing # inserted in alignment preamble");
	help3("There should be exactly one # between &'s, when an",
		"\\halign or \\valign is being set up. In this case you had",
		"none, so I've put one in; maybe that will work.");
  back_error (); 
  goto lab31; 
      } 
      else if((cur_cmd != 10)||(p != mem_top - 4)) 
      {
  mem[p].hh.v.RH = get_avail (); 
  p = mem[p].hh.v.RH; 
  mem[p].hh.v.LH = cur_tok; 
      } 
    } 
    lab31:; 
    mem[cur_align].hh.v.RH = new_null_box (); 
    cur_align = mem[cur_align].hh.v.RH; 
    mem[cur_align].hh.v.LH = mem_top - 9; 
    mem[cur_align + 1].cint = -1073741824L;  /* - 2^30 */
    mem[cur_align + 3].cint = mem[mem_top - 4].hh.v.RH; 
    p = mem_top - 4; 
    mem[p].hh.v.RH = 0; 
    while(true){
lab22:
    get_preamble_token(); 
      if((cur_cmd <= 5)&&(cur_cmd >= 4)&&(align_state == -1000000L)) 
      goto lab32; 
      if(cur_cmd == 6)
      {
		  print_err("Only one # is allowed per tab");
		  help3("There should be exactly one # between &'s, when an",
			  "\\halign or \\valign is being set up. In this case you had",
			  "more than one, so I'm ignoring all but the first."); 
  error (); 
  goto lab22; 
      } 
      mem[p].hh.v.RH = get_avail (); 
      p = mem[p].hh.v.RH; 
      mem[p].hh.v.LH = cur_tok; 
    } 
    lab32: mem[p].hh.v.RH = get_avail (); 
    p = mem[p].hh.v.RH; 
/*    mem[p].hh.v.LH = (hash_size + 4614);  */
/*    mem[p].hh.v.LH = (hash_size + 4095 + 519);  */
    mem[p].hh.v.LH = (hash_size + hash_extra + 4095 + 519); /* 96/Jan/10 */
    mem[cur_align + 2].cint = mem[mem_top - 4].hh.v.RH; 
  } 
  lab30: scanner_status = 0; 
  new_save_level(6); 
/* if every_cr<>null then begin_token_list(every_cr,every_cr_text); l.15665 */
  if(eqtb[(hash_size + 1320)].hh.v.RH != 0)/* everycr */
    begin_token_list(eqtb[(hash_size + 1320)].hh.v.RH, 13); 
  align_peek (); 
} 
void init_span_ (halfword p)
{
  push_nest (); 
  if(mode == -102)
  space_factor = 1000; 
  else {
      
    cur_list.aux_field.cint = ignore_depth; 
    normal_paragraph (); 
  } 
  cur_span = p; 
} 
void init_row (void) 
{
  push_nest (); 
  mode =(-103)- mode; 
  if(mode == -102)
  space_factor = 0; 
  else cur_list.aux_field.cint = 0; 
  {
    mem[tail].hh.v.RH = new_glue(mem[mem[mem_top - 8]
   .hh.v.RH + 1].hh.v.LH); 
    tail = mem[tail].hh.v.RH; 
  } 
  mem[tail].hh.b1 = 12; 
  cur_align = mem[mem[mem_top - 8].hh.v.RH].hh.v.RH; 
  cur_tail = cur_head; 
  init_span(cur_align); 
} 
void init_col (void) 
{ 
  mem[cur_align + 5].hh.v.LH = cur_cmd; 
  if(cur_cmd == 63)
  align_state = 0; 
  else {
      
    back_input (); 
    begin_token_list(mem[cur_align + 3].cint, 1); 
  } 
} 
/* fin_col() moved to end to hide in pragma (g, "off") */
/* may need to move more ? everything calling new_null_box() ? */
void fin_row (void) 
{
  halfword p; 
  if(mode == -102)
  {
    p = hpack(mem[head].hh.v.RH, 0, 1); 
    pop_nest (); 
    append_to_vlist(p); 
    if(cur_head != cur_tail)
    {
      mem[tail].hh.v.RH = mem[cur_head].hh.v.RH; 
      tail = cur_tail; 
    } 
  } 
  else {
      
    p = vpackage(mem[head].hh.v.RH, 0, 1,
    1073741823L);   /* 2^30 - 1 */
    pop_nest (); 
    mem[tail].hh.v.RH = p; 
    tail = p; 
    space_factor = 1000; 
  } 
  mem[p].hh.b0 = 13; 
  mem[p + 6].cint = 0; 
  if(eqtb[(hash_size + 1320)].hh.v.RH != 0)/* everycr */
    begin_token_list(eqtb[(hash_size + 1320)].hh.v.RH, 13); 
  align_peek (); 
} 
void fin_align (void) 
{ 
  halfword p, q, r, s, u, v; 
  scaled t, w; 
  scaled o; 
  halfword n; 
  scaled rulesave; 
  memory_word auxsave; 

  if(cur_group != 6){
    confusion(909); /* align1 */
    return;       // abort_flag set
  }
  unsave (); 
  if(cur_group != 6){
    confusion(910); /* align0 */
    return;       // abort_flag set
  }
  unsave (); 
  if(nest[nest_ptr - 1].mode_field == 203)
  o = eqtb[(hash_size + 3745)].cint; 
  else o = 0; 
  q = mem[mem[mem_top - 8].hh.v.RH].hh.v.RH; 
  do {
      flush_list(mem[q + 3].cint); 
    flush_list(mem[q + 2].cint); 
    p = mem[mem[q].hh.v.RH].hh.v.RH; 
    if(mem[q + 1].cint == -1073741824L) /* - 2^30 */
    {
      mem[q + 1].cint = 0; 
      r = mem[q].hh.v.RH; 
      s = mem[r + 1].hh.v.LH; 
      if(s != 0)
      {
  incr(mem[0].hh.v.RH); /* mem[mem_bot]? mem[null]? */
  delete_glue_ref(s); 
  mem[r + 1].hh.v.LH = 0; 
      } 
    } 
    if(mem[q].hh.v.LH != mem_top - 9)
    {
      t = mem[q + 1].cint + mem[mem[mem[q].hh.v.RH + 1].hh 
     .v.LH + 1].cint; 
      r = mem[q].hh.v.LH; 
      s = mem_top - 9; 
      mem[s].hh.v.LH = p; 
      n = 1; 
      do {
    mem[r + 1].cint = mem[r + 1].cint - t; 
  u = mem[r].hh.v.LH; 
  while(mem[r].hh.v.RH > n){
      
    s = mem[s].hh.v.LH; 
    n = mem[mem[s].hh.v.LH].hh.v.RH + 1; 
  } 
  if(mem[r].hh.v.RH < n)
  {
    mem[r].hh.v.LH = mem[s].hh.v.LH; 
    mem[s].hh.v.LH = r; 
    decr(mem[r].hh.v.RH); 
    s = r; 
  } 
  else {
      
    if(mem[r + 1].cint > mem[mem[s].hh.v.LH + 1].cint)
    mem[mem[s].hh.v.LH + 1].cint = mem[r + 1].cint; 
    free_node(r, 2); 
  } 
  r = u; 
      } while(!(r == mem_top - 9)); 
    } 
    mem[q].hh.b0 = 13; 
    mem[q].hh.b1 = 0; 
    mem[q + 3].cint = 0; 
    mem[q + 2].cint = 0; 
    mem[q + 5].hh.b1 = 0; 
    mem[q + 5].hh.b0 = 0; 
    mem[q + 6].cint = 0; 
    mem[q + 4].cint = 0; 
    q = p; 
  } while(!(q == 0)); 
  save_ptr = save_ptr - 2; 
  pack_begin_line = - (integer) mode_line; 
  if(mode == -1)
  {
    rulesave = eqtb[(hash_size + 3746)].cint; 
    eqtb[(hash_size + 3746)].cint = 0; 
    p = hpack(mem[mem_top - 8].hh.v.RH, save_stack[save_ptr + 1].cint 
   , save_stack[save_ptr + 0].cint); 
    eqtb[(hash_size + 3746)].cint = rulesave; 
  } 
  else {
      
    q = mem[mem[mem_top - 8].hh.v.RH].hh.v.RH; 
    do {
  mem[q + 3].cint = mem[q + 1].cint; 
      mem[q + 1].cint = 0; 
      q = mem[mem[q].hh.v.RH].hh.v.RH; 
    } while(!(q == 0)); 
    p = vpackage(mem[mem_top - 8].hh.v.RH, save_stack[save_ptr + 1]
   .cint, save_stack[save_ptr + 0].cint, 1073741823L);  /* 2^30 - 1 */
    q = mem[mem[mem_top - 8].hh.v.RH].hh.v.RH; 
    do {
  mem[q + 1].cint = mem[q + 3].cint; 
      mem[q + 3].cint = 0; 
      q = mem[mem[q].hh.v.RH].hh.v.RH; 
    } while(!(q == 0)); 
  } 
  pack_begin_line = 0; 
  q = mem[head].hh.v.RH; 
  s = head; 
  while(q != 0){    /* while q<>null l.15794 OK */
      
    if(!(q >= hi_mem_min))  /*   begin if not is_char_node(q) then */
    if(mem[q].hh.b0 == 13)
    {
      if(mode == -1)
      {
  mem[q].hh.b0 = 0; 
  mem[q + 1].cint = mem[p + 1].cint; 
      } 
      else {
    
  mem[q].hh.b0 = 1; 
  mem[q + 3].cint = mem[p + 3].cint; 
      } 
      mem[q + 5].hh.b1 = mem[p + 5].hh.b1; 
      mem[q + 5].hh.b0 = mem[p + 5].hh.b0; 
      mem[q + 6].gr = mem[p + 6].gr; 
      mem[q + 4].cint = o; 
      r = mem[mem[q + 5].hh.v.RH].hh.v.RH; 
      s = mem[mem[p + 5].hh.v.RH].hh.v.RH; 
      do {
    n = mem[r].hh.b1; 
  t = mem[s + 1].cint; 
  w = t; 
  u = mem_top - 4; 
  while(n > 0){
      
    decr(n); 
    s = mem[s].hh.v.RH; 
    v = mem[s + 1].hh.v.LH; 
    mem[u].hh.v.RH = new_glue(v); 
    u = mem[u].hh.v.RH; 
    mem[u].hh.b1 = 12; 
    t = t + mem[v + 1].cint; 
    if(mem[p + 5].hh.b0 == 1)
    {
      if(mem[v].hh.b0 == mem[p + 5].hh.b1)
      t = t + round(mem[p + 6].gr * mem[v + 2].cint); 
    } 
    else if(mem[p + 5].hh.b0 == 2)
    {
      if(mem[v].hh.b1 == mem[p + 5].hh.b1)
      t = t - round(mem[p + 6].gr * mem[v + 3].cint); 
    } 
    s = mem[s].hh.v.RH; 
    mem[u].hh.v.RH = new_null_box (); 
    u = mem[u].hh.v.RH; 
    t = t + mem[s + 1].cint; 
    if(mode == -1)
    mem[u + 1].cint = mem[s + 1].cint; 
    else {
        
      mem[u].hh.b0 = 1; 
      mem[u + 3].cint = mem[s + 1].cint; 
    } 
  } 
  if(mode == -1)
  {
    mem[r + 3].cint = mem[q + 3].cint; 
    mem[r + 2].cint = mem[q + 2].cint; 
    if(t == mem[r + 1].cint)
    {
      mem[r + 5].hh.b0 = 0; 
      mem[r + 5].hh.b1 = 0; 
      mem[r + 6].gr = 0.0; 
    } 
    else if(t > mem[r + 1].cint)
    {
      mem[r + 5].hh.b0 = 1; 
      if(mem[r + 6].cint == 0)
      mem[r + 6].gr = 0.0; 
      else mem[r + 6].gr =(t - mem[r + 1].cint)/ ((double) 
      mem[r + 6].cint); 
    } 
    else {
        
      mem[r + 5].hh.b1 = mem[r + 5].hh.b0; 
      mem[r + 5].hh.b0 = 2; 
      if(mem[r + 4].cint == 0)
      mem[r + 6].gr = 0.0; 
      else if((mem[r + 5].hh.b1 == 0)&&(mem[r + 1].cint - 
      t > mem[r + 4].cint)) 
      mem[r + 6].gr = 1.0; 
      else mem[r + 6].gr =(mem[r + 1].cint - t)/ ((double) 
      mem[r + 4].cint); 
    } 
    mem[r + 1].cint = w; 
    mem[r].hh.b0 = 0; 
  } 
  else {
      
    mem[r + 1].cint = mem[q + 1].cint; 
    if(t == mem[r + 3].cint)
    {
      mem[r + 5].hh.b0 = 0; 
      mem[r + 5].hh.b1 = 0; 
      mem[r + 6].gr = 0.0; 
    } 
    else if(t > mem[r + 3].cint)
    {
      mem[r + 5].hh.b0 = 1; 
      if(mem[r + 6].cint == 0)
      mem[r + 6].gr = 0.0; 
      else mem[r + 6].gr =(t - mem[r + 3].cint)/ ((double) 
      mem[r + 6].cint); 
    } 
    else {
        
      mem[r + 5].hh.b1 = mem[r + 5].hh.b0; 
      mem[r + 5].hh.b0 = 2; 
      if(mem[r + 4].cint == 0)
      mem[r + 6].gr = 0.0; 
      else if((mem[r + 5].hh.b1 == 0)&&(mem[r + 3].cint - 
      t > mem[r + 4].cint)) 
      mem[r + 6].gr = 1.0; 
      else mem[r + 6].gr =(mem[r + 3].cint - t)/ ((double) 
      mem[r + 4].cint); 
    } 
    mem[r + 3].cint = w; 
    mem[r].hh.b0 = 1; 
  } 
  mem[r + 4].cint = 0; 
  if(u != mem_top - 4)
  {
    mem[u].hh.v.RH = mem[r].hh.v.RH; 
    mem[r].hh.v.RH = mem[mem_top - 4].hh.v.RH; 
    r = u; 
  } 
  r = mem[mem[r].hh.v.RH].hh.v.RH; 
  s = mem[mem[s].hh.v.RH].hh.v.RH; 
      } while(!(r == 0)); 
    } 
    else if(mem[q].hh.b0 == 2)
    {
      if((mem[q + 1].cint == -1073741824L))  /* 2^30  */
      mem[q + 1].cint = mem[p + 1].cint; 
      if((mem[q + 3].cint == -1073741824L))  /* 2^30  */
      mem[q + 3].cint = mem[p + 3].cint; 
      if((mem[q + 2].cint == -1073741824L))  /* 2^30  */
      mem[q + 2].cint = mem[p + 2].cint; 
      if(o != 0)
      {
  r = mem[q].hh.v.RH; 
  mem[q].hh.v.RH = 0; 
  q = hpack(q, 0, 1); 
  mem[q + 4].cint = o; 
  mem[q].hh.v.RH = r; 
  mem[s].hh.v.RH = q; 
      } 
    } 
    s = q; 
    q = mem[q].hh.v.RH; 
  } 
  flush_node_list(p); 
  pop_alignment (); 
  auxsave = cur_list.aux_field; 
  p = mem[head].hh.v.RH; 
  q = tail; 
  pop_nest (); 
  if(mode == 203)
  {
    do_assignments (); 
    if(cur_cmd != 3) {
		print_err("Missing $$ inserted");
		help2("Displays can use special alignments (like \\eqalignno)",
			"only if nothing but the alignment itself is between $$'s.");
      back_error (); 
    } 
    else {
  
      get_x_token (); 
      if(cur_cmd != 3)
      {
		  print_err("Display math should end with $$");
		  help2("The `$' that I just saw supposedly matches a previous `$$'.",
			  "So I shall assume that you typed `$$' both times.");
  back_error (); 
      } 
  }
    pop_nest (); 
    {
      mem[tail].hh.v.RH = new_penalty(eqtb[(hash_size + 3174)].cint 
    ); 
      tail = mem[tail].hh.v.RH; 
    } 
    {
      mem[tail].hh.v.RH = new_param_glue(3); 
      tail = mem[tail].hh.v.RH; 
    } 
    mem[tail].hh.v.RH = p; 
    if(p != 0)
    tail = q; 
    {
      mem[tail].hh.v.RH = new_penalty(eqtb[(hash_size + 3175)].cint 
    ); 
      tail = mem[tail].hh.v.RH; 
    } 
    {
      mem[tail].hh.v.RH = new_param_glue(4); 
      tail = mem[tail].hh.v.RH; 
    } 
    cur_list.aux_field.cint = auxsave.cint; 
    resume_after_display (); 
  } 
  else {
      
    cur_list.aux_field = auxsave; 
    mem[tail].hh.v.RH = p; 
    if(p != 0)    /*   if p<>null then tail:=q; l.15926 */
    tail = q; 
    if(mode == 1){
    build_page ();
  }
  } 
} 
/* used to be align_peek, zfintieshrink, etc in old tex5.c */
/************************************************************************/
/* moved down here to avoid questions about pragma optimize */
#pragma optimize("g", off)          /* for MC VS compiler */
/*  Moved down here 96/Oct/12 in response to problem with texerror.tex */
/*  pragma optimize("a", off) not strong enough - this may slow things */
bool fin_col (void) 
{/* 10 */ register bool Result;
  halfword p; 
  halfword q, r; 
  halfword s; 
  halfword u; 
  scaled w; 
  glue_ord o; 
  halfword n; 
  if(cur_align == 0){
    confusion(903); /* endv */
    return 0;       // abort_flag set
  }
  q = mem[cur_align].hh.v.RH; 
  if(q == 0){
    confusion(903); /* endv */
    return 0;       // abort_flag set
  }
  if(align_state < 500000L) {     /* ??? */
    fatal_error("(interwoven alignment preambles are not allowed)"); /*  */
    return 0;     // abort_flag set
  }
  p = mem[q].hh.v.RH;         /* p <- link(q) p.791 */
/* if (p = null) ^ (extra_info(cur_align) < cr_code) then p.792 */
  if((p == 0)&&(mem[cur_align + 5].hh.v.LH < 257)) 
  if(cur_loop != 0)
  {
/*  potential problem here if new_null_box causes memory reallocation ??? */
/*  compiler optimization does not refresh `mem' loaded in registers ? */
    mem[q].hh.v.RH = new_null_box (); 
    p = mem[q].hh.v.RH; 
    mem[p].hh.v.LH = mem_top - 9; 
    mem[p + 1].cint = -1073741824L;  /* - 2^30 */
    cur_loop = mem[cur_loop].hh.v.RH; 
    q = mem_top - 4; 
    r = mem[cur_loop + 3].cint; 
    while(r != 0){
  
      mem[q].hh.v.RH = get_avail (); 
      q = mem[q].hh.v.RH; 
      mem[q].hh.v.LH = mem[r].hh.v.LH; 
      r = mem[r].hh.v.RH; 
    } 
    mem[q].hh.v.RH = 0; 
    mem[p + 3].cint = mem[mem_top - 4].hh.v.RH; 
    q = mem_top - 4; 
    r = mem[cur_loop + 2].cint; 
    while(r != 0){
  
      mem[q].hh.v.RH = get_avail (); 
      q = mem[q].hh.v.RH; 
      mem[q].hh.v.LH = mem[r].hh.v.LH; 
      r = mem[r].hh.v.RH; 
    } 
    mem[q].hh.v.RH = 0; 
    mem[p + 2].cint = mem[mem_top - 4].hh.v.RH; 
    cur_loop = mem[cur_loop].hh.v.RH; 
    mem[p].hh.v.RH = new_glue(mem[cur_loop + 1].hh.v.LH); 
  } 
  else {
      print_err("Extra alignment tab has been changed to ");
    print_esc("cr");
	help3("You have given more \\span or & marks than there were",
		"in the preamble to the \\halign or \\valign now in progress.",
		"So I'll assume that you meant to type \\cr instead.");
/* extra_info(cur_align) < cr_code) ? */
    mem[cur_align + 5].hh.v.LH = 257; 
    error (); 
  } 
  if(mem[cur_align + 5].hh.v.LH != 256)
  {
    unsave (); 
    new_save_level(6); 
    {
      if(mode == -102)
      {
  adjust_tail = cur_tail; 
  u = hpack(mem[head].hh.v.RH, 0, 1); 
  w = mem[u + 1].cint; 
  cur_tail = adjust_tail; 
  adjust_tail = 0; 
      } 
      else {
    
  u = vpackage(mem[head].hh.v.RH, 0, 1, 0); 
  w = mem[u + 3].cint; 
      } 
      n = 0; 
      if(cur_span != cur_align)
      {
  q = cur_span; 
  do {
      incr(n); 
    q = mem[mem[q].hh.v.RH].hh.v.RH; 
  } while(!(q == cur_align)); 
/*  if n > max_quarterword then confusion("256 spans"); p.798 */
/*  if(n > 255) */            /* 94/Apr/4 ? */
  if(n > max_quarterword) {       /* 96/Oct/12 ??? */
    confusion(908);   /* 256 spans --- message wrong now, but ... */
    return 0;       // abort_flag set
  }
  q = cur_span; 
  while(mem[mem[q].hh.v.LH].hh.v.RH < n)q = mem[q].hh 
  .v.LH; 
  if(mem[mem[q].hh.v.LH].hh.v.RH > n)
  {
    s = get_node(2); 
    mem[s].hh.v.LH = mem[q].hh.v.LH; 
    mem[s].hh.v.RH = n; 
    mem[q].hh.v.LH = s; 
    mem[s + 1].cint = w; 
  } 
  else if(mem[mem[q].hh.v.LH + 1].cint < w)
  mem[mem[q].hh.v.LH + 1].cint = w; 
      } 
      else if(w > mem[cur_align + 1].cint)
      mem[cur_align + 1].cint = w; 
      mem[u].hh.b0 = 13; 
      mem[u].hh.b1 = n; 
      if(totalstretch[3]!= 0)
      o = 3; 
      else if(totalstretch[2]!= 0)
      o = 2; 
      else if(totalstretch[1]!= 0)
      o = 1; 
      else o = 0; 
      mem[u + 5].hh.b1 = o; 
      mem[u + 6].cint = totalstretch[o]; 
      if(totalshrink[3]!= 0)
      o = 3; 
      else if(totalshrink[2]!= 0)
      o = 2; 
      else if(totalshrink[1]!= 0)
      o = 1; 
      else o = 0; 
      mem[u + 5].hh.b0 = o; 
      mem[u + 4].cint = totalshrink[o]; 
      pop_nest (); 
      mem[tail].hh.v.RH = u; 
      tail = u; 
    } 
    {
      mem[tail].hh.v.RH = new_glue(mem[mem[cur_align]
     .hh.v.RH + 1].hh.v.LH); 
      tail = mem[tail].hh.v.RH; 
    } 
    mem[tail].hh.b1 = 12; 
/* if (extra_info(cur_align) >= cr_code) then p.792 */
    if(mem[cur_align + 5].hh.v.LH >= 257)
    {
      Result = true; 
      return(Result); 
    } 
    init_span(p); 
  } 
  align_state = 1000000L; 
  do {
      get_x_token (); 
  } while(!(cur_cmd != 10)); 
  cur_align = p; 
  init_col (); 
  Result = false; 
  return Result; 
} 
/* #pragma optimize ("g", on) */        /* for MC VS compiler */
/* #pragma optimize ("g",) */         /* 94/Jan/25 */
/* #pragma optimize ("", on) */           /* 96/Sep/12 */
/* #pragma optimize("g", off) */          /* for MC VS compiler */

scaled make_op_(halfword q)
{register scaled Result; 
  scaled delta; 
  halfword p, v, x, y, z; 
  quarterword c; 
  ffourquarters i; 
  scaled shiftup, shiftdown; 
  if((mem[q].hh.b1 == 0)&&(cur_style < 2)) 
  mem[q].hh.b1 = 1; 
  if(mem[q + 1].hh.v.RH == 1)
  {
    fetch(q + 1); 
    if((cur_style < 2)&&(((cur_i.b2)% 4)== 2)) 
    {
      c = cur_i.b3; 
      i = font_info[char_base[cur_f]+ c].qqqq; 
      if((i.b0 > 0)) 
      {
  cur_c = c; 
  cur_i = i; 
  mem[q + 1].hh.b1 = c; 
      } 
    } 
    delta = font_info[italic_base[cur_f]+(cur_i.b2)/ 4].cint; 
    x = clean_box(q + 1, cur_style); 
    if((mem[q + 3].hh.v.RH != 0)&&(mem[q].hh.b1 != 1)) 
    mem[x + 1].cint = mem[x + 1].cint - delta; 
    mem[x + 4].cint = half(mem[x + 3].cint - mem[x + 2].cint)- 
    font_info[22 + param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint; 
    mem[q + 1].hh.v.RH = 2; 
    mem[q + 1].hh.v.LH = x; 
  } 
  else delta = 0; 
  if(mem[q].hh.b1 == 1)
  {
    x = clean_box(q + 2, 2 *(cur_style / 4)+ 4 +(cur_style % 2)); 
    y = clean_box(q + 1, cur_style); 
    z = clean_box(q + 3, 2 *(cur_style / 4)+ 5); 
    v = new_null_box (); 
    mem[v].hh.b0 = 1; 
    mem[v + 1].cint = mem[y + 1].cint; 
    if(mem[x + 1].cint > mem[v + 1].cint)
    mem[v + 1].cint = mem[x + 1].cint; 
    if(mem[z + 1].cint > mem[v + 1].cint)
    mem[v + 1].cint = mem[z + 1].cint; 
    x = rebox(x, mem[v + 1].cint); 
    y = rebox(y, mem[v + 1].cint); 
    z = rebox(z, mem[v + 1].cint); 
    mem[x + 4].cint = half(delta); 
    mem[z + 4].cint = - (integer) mem[x + 4].cint; 
    mem[v + 3].cint = mem[y + 3].cint; 
    mem[v + 2].cint = mem[y + 2].cint; 
    if(mem[q + 2].hh.v.RH == 0)
    {
      free_node(x, 7); 
      mem[v + 5].hh.v.RH = y; 
    } 
    else {
  
      shiftup = font_info[11 + param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH 
    ]].cint - mem[x + 2].cint; 
      if(shiftup < font_info[9 + param_base[eqtb[(hash_size + 1838) + cur_size].hh 
     .v.RH]].cint)
      shiftup = font_info[9 + param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH 
    ]].cint; 
      p = new_kern(shiftup); 
      mem[p].hh.v.RH = y; 
      mem[x].hh.v.RH = p; 
      p = new_kern(font_info[13 + param_base[eqtb[(hash_size + 1838) + cur_size].hh 
     .v.RH]].cint); 
      mem[p].hh.v.RH = x; 
      mem[v + 5].hh.v.RH = p; 
      mem[v + 3].cint = mem[v + 3].cint + font_info[13 + param_base[
      eqtb[(hash_size + 1838) + cur_size].hh.v.RH]].cint + mem[x + 3].cint + mem 
     [x + 2].cint + shiftup; 
    } 
    if(mem[q + 3].hh.v.RH == 0)
    free_node(z, 7); 
    else {
  
      shiftdown = font_info[12 + param_base[eqtb[(hash_size + 1838) + cur_size].hh 
     .v.RH]].cint - mem[z + 3].cint; 
      if(shiftdown < font_info[10 + param_base[eqtb[(hash_size + 1838) + cur_size]
     .hh.v.RH]].cint)
      shiftdown = font_info[10 + param_base[eqtb[(hash_size + 1838) + cur_size].hh 
     .v.RH]].cint; 
      p = new_kern(shiftdown); 
      mem[y].hh.v.RH = p; 
      mem[p].hh.v.RH = z; 
      p = new_kern(font_info[13 + param_base[eqtb[(hash_size + 1838) + cur_size].hh 
     .v.RH]].cint); 
      mem[z].hh.v.RH = p; 
      mem[v + 2].cint = mem[v + 2].cint + font_info[13 + param_base[
      eqtb[(hash_size + 1838) + cur_size].hh.v.RH]].cint + mem[z + 3].cint + mem 
     [z + 2].cint + shiftdown; 
    } 
    mem[q + 1].cint = v; 
  } 
  Result = delta; 
  return Result; 
} 
/* #pragma optimize ("g", on) */        /* for MC VS compiler */
/* #pragma optimize ("g",) */         /* 94/Jan/25 */
/* #pragma optimize ("", on) */           /* 96/Sep/12 */
/* #pragma optimize ("g", off) */

void make_scripts_(halfword q, scaled delta) 
{ 
  halfword p, x, y, z; 
  scaled shiftup, shiftdown, clr; 
  small_number t; 
  p = mem[q + 1].cint; 
  if((p >= hi_mem_min)) 
  {
    shiftup = 0; 
    shiftdown = 0; 
  } 
  else {
      
    z = hpack(p, 0, 1); 
    if(cur_style < 4)
    t = 16; 
    else t = 32; 
    shiftup = mem[z + 3].cint - font_info[18 + param_base[eqtb[(hash_size + 1837) + 
    t].hh.v.RH]].cint; 
    shiftdown = mem[z + 2].cint + font_info[19 + param_base[eqtb[(hash_size + 1837) 
    + t].hh.v.RH]].cint; 
    free_node(z, 7); 
  } 
  if(mem[q + 2].hh.v.RH == 0)
  {
    x = clean_box(q + 3, 2 *(cur_style / 4)+ 5); 
    mem[x + 1].cint = mem[x + 1].cint + eqtb[(hash_size + 3742)].cint; 
    if(shiftdown < font_info[16 + param_base[eqtb[(hash_size + 1837) + cur_size].hh 
   .v.RH]].cint)
    shiftdown = font_info[16 + param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH 
  ]].cint; 
    clr = mem[x + 3].cint -(abs(font_info[5 + param_base[eqtb[
    (hash_size + 1837) + cur_size].hh.v.RH]].cint * 4)/ 5); 
    if(shiftdown < clr)
    shiftdown = clr; 
    mem[x + 4].cint = shiftdown; 
  } 
  else {
      
    {
      x = clean_box(q + 2, 2 *(cur_style / 4)+ 4 +(cur_style % 2)); 
      mem[x + 1].cint = mem[x + 1].cint + eqtb[(hash_size + 3742)].cint; 
      if(odd(cur_style)) 
      clr = font_info[15 + param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]] 
     .cint; 
      else if(cur_style < 2)
      clr = font_info[13 + param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]] 
     .cint; 
      else clr = font_info[14 + param_base[eqtb[(hash_size + 1837) + cur_size].hh 
     .v.RH]].cint; 
      if(shiftup < clr)
      shiftup = clr; 
      clr = mem[x + 2].cint +(abs(font_info[5 +
    param_base[eqtb[(hash_size + 1837) + cur_size].hh.v.RH]].cint)/ 4); 
      if(shiftup < clr)
      shiftup = clr; 
    } 
    if(mem[q + 3].hh.v.RH == 0)
    mem[x + 4].cint = - (integer) shiftup; 
    else {
  
      y = clean_box(q + 3, 2 *(cur_style / 4)+ 5); 
      mem[y + 1].cint = mem[y + 1].cint + eqtb[(hash_size + 3742)].cint; 
      if(shiftdown < font_info[17 + param_base[eqtb[(hash_size + 1837) + cur_size]
     .hh.v.RH]].cint)
      shiftdown = font_info[17 + param_base[eqtb[(hash_size + 1837) + cur_size].hh 
     .v.RH]].cint; 
      clr = 4 * font_info[8 + param_base[eqtb[(hash_size + 1838) + cur_size].hh.v.RH 
    ]].cint -((shiftup - mem[x + 2].cint)-(mem[y + 3].cint 
      - shiftdown)); 
      if(clr > 0)
      {
  shiftdown = shiftdown + clr; 
  clr =(abs(font_info[5 + param_base[eqtb[(hash_size + 1837) + cur_size].hh 
  .v.RH]].cint * 4)/ 5)-(shiftup - mem[x + 2].cint); 
  if(clr > 0)
  {
    shiftup = shiftup + clr; 
    shiftdown = shiftdown - clr; 
  } 
      } 
      mem[x + 4].cint = delta; 
      p = new_kern((shiftup - mem[x + 2].cint)-(mem[y + 3].cint 
      - shiftdown)); 
      mem[x].hh.v.RH = p; 
      mem[p].hh.v.RH = y; 
      x = vpackage(x, 0, 1, 1073741823L);  /* 2^30 - 1 */
      mem[x + 4].cint = shiftdown; 
    } 
  } 
  if(mem[q + 1].cint == 0)
  mem[q + 1].cint = x; 
  else {
      
    p = mem[q + 1].cint; 
    while(mem[p].hh.v.RH != 0)p = mem[p].hh.v.RH; 
    mem[p].hh.v.RH = x; 
  } 
} 
/* #pragma optimize ("g", on) */        /* 96/Sep/12 */
/* #pragma optimize ("g") */          /* 94/Jan/25 */
#pragma optimize ("", on)           /* 96/Sep/12 */
/***************************************************************************/
