
/*
 * Common TeX:align.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX align.c 3.2@(#)";
#endif

#include "tex.h"

align_stack_t *align_stack;

ptr	align_head;

ptr	cur_align;
ptr	cur_span;
ptr	cur_loop;
ptr	cur_head;
ptr	cur_tail;

ptr	end_span;

  void
push_alignment ()
{
  align_stack_t *p;

  p = (align_stack_t *) new_node(ALIGN_STACK_NODE_SIZE);
  p->align_stack_field = align_stack;
  p->preamble_field = preamble;
  p->cur_align_field = cur_align;
  p->cur_span_field = cur_span;
  p->cur_loop_field = cur_loop;
  p->cur_head_field = cur_head;
  p->cur_tail_field = cur_tail;
  p->align_state_field = align_state;
  align_stack = p;
  cur_head = new_avail();
}

  void
pop_alignment ()
{
  align_stack_t *p;

  free_avail(cur_head);
  p = align_stack;
  align_stack = p->align_stack_field;
  preamble = p->preamble_field;
  cur_align = p->cur_align_field;
  cur_span = p->cur_span_field;
  cur_loop = p->cur_loop_field;
  cur_head = p->cur_head_field;
  cur_tail = p->cur_tail_field;
  align_state = p->align_state_field;
  free_node((ptr)p, ALIGN_STACK_NODE_SIZE);
}

  void
init_align ()
{
  ptr	p;
  sym	save_cs_ptr;

#define MISSING_MAC_PARAM "Missing # inserted in alignment preamble"
#define EXTRA_MAC_PARAM "Only one # is allowed per tab"

  save_cs_ptr = cur_cs;
  push_alignment();
  align_state = -1000000;
  if (mode == MMODE && (tail != head || incompleat_noad != null)) {
    print_err("Improper ");
    print_esc("halign");
    print(" inside $$'s");
    help_display_align();
    error();
    flush_math();
  }
  push_nest();
  if (mode == MMODE) {
    mode = -VMODE;
    prev_depth = nest_ptr[-2].aux_field;
  } else if (mode > 0) {
    mode = -mode;
  }
  scan_spec(ALIGN_GROUP, FALSE);
  preamble = null;
  cur_align = align_head;
  cur_loop = null;
  scanner_status = ALIGNING;
  warning_cs = save_cs_ptr;
  align_state = -1000000;
  loop {
    link(cur_align) = new_param_glue(TAB_SKIP_CODE);
    cur_align = link(cur_align);
    if (cur_cmd == CAR_RET)
      break;
    p = align_toks;
    token_link(p) = null;
    loop {
      get_preamble_token();
      if (cur_cmd == MAC_PARAM)
        break;
      if (cur_cmd <= CAR_RET &&
          cur_cmd >= TAB_MARK &&
          align_state == -1000000) {
        if (p == align_toks &&
            cur_loop == null &&
            cur_cmd == TAB_MARK) {
          cur_loop = cur_align;
        } else {
          print_err(MISSING_MAC_PARAM);
          help_preamble_missing();
          back_error();
          break;
        }
      } else if (cur_cmd != SPACER || p != align_toks) {
        p = token_link(p) = new_token();
        token(p) = cur_tok;
      }
    }
    link(cur_align) = new_null_box();
    cur_align = link(cur_align);
    end_link(cur_align) = end_span;
    unset_width(cur_align) = NULL_FLAG;
    u_part(cur_align) = token_link(align_toks);
    p = align_toks;
    token_link(p) = null;
    loop {
      get_preamble_token();
      if (cur_cmd <= CAR_RET &&
          cur_cmd >= TAB_MARK &&
          align_state == -1000000)
        break;
      if (cur_cmd == MAC_PARAM) {
        print_err(EXTRA_MAC_PARAM);
        help_preamble_many();
        error();
        continue;
      }
      p = token_link(p) = new_token();
      token(p) = cur_tok;
    }
    p = token_link(p) = new_token();
    token(p) = END_TEMPLATE_TOKEN;
    v_part(cur_align) = token_link(align_toks);
  }
  scanner_status = NORMAL;
  new_save_level(ALIGN_GROUP);
  if (every_cr != null)
    begin_token_list(every_cr, EVERY_CR_TEXT);
  align_peek();
}

  void
get_preamble_token ()
{
restart:
  get_token();
  while (cur_chr == SPAN_CODE && cur_cmd == TAB_MARK) {
    get_token();
    if (cur_cmd > MAX_COMMAND) {
      expand();
      get_token();
    }
  }
  if (cur_cmd == ASSIGN_GLUE && cur_chr == TAB_SKIP_CODE) {
    scan_optional_equals();
    scan_glue(GLUE_VAL);
    if (global_defs > 0)
      reg_gdefine(skip_reg[TAB_SKIP_CODE], SKIP_REG, cur_val);
    else reg_define(skip_reg[TAB_SKIP_CODE], SKIP_REG, cur_val);
    goto restart;
  }
}

  void
align_peek ()
{
restart:
  align_state = 1000000;
  get_nbx_token();
  if (cur_cmd == NO_ALIGN) {
    scan_left_brace();
    new_save_level(NO_ALIGN_GROUP);
    if (mode == -VMODE)
      normal_paragraph();
  } else if (cur_cmd == RIGHT_BRACE) {
    fin_align();
  } else if (cur_cmd == CAR_RET && cur_chr == CR_CR_CODE) {
    goto restart;
  } else {
    init_row();
    init_col();
  }
}

  void
init_row ()
{
  push_nest();
  mode = (-HMODE - VMODE) - mode;
  if (mode == -HMODE)
    space_factor = 0;
  else prev_depth = 0;
  tail_append(new_glue(glue_ptr(preamble)));
  subtype(tail) = TAB_SKIP_CODE + 1;
  cur_align = link(preamble);
  cur_tail = cur_head;
  init_span(cur_align);
}

  void
init_span (p)
  ptr	p;
{
  push_nest();
  if (mode == -HMODE) {
    space_factor = 1000;
  } else {
    prev_depth = IGNORE_DEPTH;
    normal_paragraph();
  }
  cur_span = p;
}

  void
init_col ()
{
  unset_info(cur_align) = cur_cmd;
  if (cur_cmd == OMIT) {
    align_state = 0;
  } else {
    back_input();
    begin_token_list(u_part(cur_align), U_TEMPLATE);
  }
}

  bool
fin_col ()
{
  int	n;
  int	o;
  ptr	p;
  ptr	q;
  ptr	r;
  ptr	s;
  ptr	u;
  scal	w;

#define INTERWOVEN_ALIGNS "(interwoven alignment preambles are not allowed)"

  if (cur_align == null)
    confusion("endv");
  q = link(cur_align);
  if (q == null)
    confusion("endv");
  if (align_state < 500000)
    fatal_error(INTERWOVEN_ALIGNS);
  p = link(q);
  if (p == null && unset_info(cur_align) < CR_CODE) {
    if (cur_loop != null) {
      p = link(q) = new_null_box();
      end_link(p) = end_span;
      unset_width(p) = NULL_FLAG;
      cur_loop = link(cur_loop);
      q = align_toks;
      r = u_part(cur_loop);
      while (r != null) {
        q = token_link(q) = new_token();
        token(q) = token(r);
        r = token_link(r);
      }
      token_link(q) = null;
      u_part(p) = token_link(align_toks);
      q = align_toks;
      r = v_part(cur_loop);
      while (r != null) {
        q = token_link(q) = new_token();
        token(q) = token(r);
        r = token_link(r);
      }
      token_link(q) = null;
      v_part(p) = token_link(align_toks);
      cur_loop = link(cur_loop);
      link(p) = new_glue(glue_ptr(cur_loop));
    } else {
      print_err("Extra alignment tab has been changed to ");
      print_esc("cr");
      help_align_apply();
      unset_info(cur_align) = CR_CODE;
      error();
    }
  }
  if (unset_info(cur_align) != SPAN_CODE) {
    unsave();
    new_save_level(ALIGN_GROUP);
    if (mode == -HMODE) {
      adjust_tail = cur_tail;
      u = hpack(link(head), NATURAL);
      w = box_width(u);
      cur_tail = adjust_tail;
      adjust_tail = null;
    } else {
      u = vpackage(link(head), NATURAL, 0);
      w = box_height(u);
    }
    n = MIN_QUARTERWORD;
    if (cur_span != cur_align) {
      q = cur_span;
      do	{
        incr(n);
        q = link(link(q));
      } while (q != cur_align);
      if (n > MAX_QUARTERWORD)
        confusion("256 spans");
      q = cur_span;
      while (span_count(span_link(q)) < n)
        q = span_link(q);
      if (span_count(span_link(q)) > n) {
        s = new_node(SPAN_NODE_SIZE);
        span_link(s) = span_link(q);
        span_link(q) = s;
        span_count(s) = n;
        span_width(s) = w;
      } else if (span_width(span_link(q)) < w) {
        span_width(span_link(q)) = w;
      }
    } else if (w > unset_width(cur_align)) {
      unset_width(cur_align) = w;
    }
    type(u) = UNSET_NODE;
    unset_span_count(u) = n;
    o = get_stretch_order();
    glue_order(u) = o;
    unset_stretch(u) = total_stretch[o];
    o = get_shrink_order();
    glue_sign(u) = o;
    unset_shrink(u) = total_shrink[o];
    pop_nest();
    tail_append(u);
    tail_append(new_glue(glue_ptr(link(cur_align))));
    subtype(tail) = TAB_SKIP_CODE + 1;
    if (unset_info(cur_align) >= CR_CODE)
      return TRUE;
    init_span(p);
  }
  align_state = 1000000;
  get_nbx_token();
  cur_align = p;
  init_col();
  return FALSE;
}

  void
fin_row ()
{
  ptr	p;

  if (mode == -HMODE) {
    p = hpack(link(head), NATURAL);
    pop_nest();
    append_to_vlist(p);
    if(cur_head != cur_tail) {
      link(tail) = link(cur_head);
      tail = cur_tail;
    }
  } else {
    p = vpack(link(head), NATURAL);
    pop_nest();
    tail = link(tail) = p;
    space_factor = 1000;
  }
  type(p) = UNSET_NODE;
  unset_stretch(p) = 0;
  if (every_cr != null)
    begin_token_list(every_cr, EVERY_CR_TEXT);
  align_peek();
}

  void
fin_align ()
{
  int	n;
  scal	o;
  ptr	p;
  ptr	q;
  ptr	r;
  ptr	s;
  scal	t;
  ptr	u;
  ptr	v;
  scal	w;
  int	aux_save;
  scal	rule_save;

  if (cur_group != ALIGN_GROUP)
    confusion("align1");
  unsave();
  if (cur_group != ALIGN_GROUP)
    confusion("align0");
  unsave();
  if (nest_ptr[-1].mode_field == MMODE) {
    o = display_indent;
  } else {
    o = 0;
  }
  q = link(preamble);
  do {
    flush_list(u_part(q));
    flush_list(v_part(q));
    p = link(link(q));
    if (unset_width(q) == NULL_FLAG) {
      unset_width(q) = 0;
      r = link(q);
      s = glue_ptr(r);
      if (s != zero_glue) {
        add_glue_ref(zero_glue);
        delete_glue_ref(s);
        glue_ptr(r) = zero_glue;
      }
    }
    if (unset_span_list(q) != end_span) {
      t = unset_width(q) + glue_width(glue_ptr(link(q)));
      r = unset_span_list(q);
      s = end_span;
      span_link(s) = p;
      n = MIN_QUARTERWORD + 1;
      do {	
        span_width(r) -= t;
        u = span_link(r);
        while (span_count(r) > n) {
          s = span_link(s);
          n = span_count(span_link(s)) + 1;
        }
        if (span_count(r) < n) {
          span_link(r) = span_link(s);
          span_link(s) = r;
          decr(span_count(r));
          s = r;
        } else {
          if (span_width(r) > span_width(span_link(s)))
            span_width(span_link(s)) =
              span_width(r);
          free_node(r, SPAN_NODE_SIZE);
        }
        r = u;
      } while (r != end_span);
    }
    type(q) = UNSET_NODE;
    unset_span_count(q) = MIN_QUARTERWORD;
    box_height(q) = 0;
    box_depth(q) = 0;
    glue_order(q) = NORMAL;
    glue_sign(q) = NORMAL;
    unset_stretch(q) = 0;
    unset_shrink(q) = 0;
    q = p;
  } while (q != null);
  save_ptr -= 2;
  pack_begin_line = -mode_line;
  if (mode == -VMODE) {
    rule_save = overfull_rule;
    overfull_rule = 0;
    p = hpack(preamble, saved(1), saved(0));
    overfull_rule = rule_save;
  } else {
    q = link(preamble);
    do {
      box_height(q) = box_width(q);
      box_width(q) = 0;
      q = link(link(q));
    } while (q != null);
    p = vpackage(preamble, saved(1), saved(0), MAX_DIMEN);
    q = link(preamble);
    do {
      box_width(q) = box_height(q);
      box_height(q) = 0;
      q = link(link(q));
    } while (q != null);
  }
  pack_begin_line = 0;
  for (q = link(head); q != null; q = link(q)) {
    if (type(q) == UNSET_NODE) {
      if (mode == -VMODE) {
        type(q) = HLIST_NODE;
        box_width(q) = box_width(p);
      } else {
        type(q) = VLIST_NODE;
        box_height(q) = box_height(p);
      }
      glue_order(q) = glue_order(p);
      glue_sign(q) = glue_sign(p);
      glue_set(q) = glue_set(p);
      shift_amount(q) = o;
      r = link(list_ptr(q));
      s = link(list_ptr(p)); 
      do	{
        n = unset_span_count(r); 
        w = t = unset_width(s);
        u = hold_head;
        while (n > MIN_QUARTERWORD) {
          decr(n);
          s = link(s);
          v = glue_ptr(s);
          link(u) = new_glue(v);
          u = link(u);
          subtype(u) = TAB_SKIP_CODE + 1;
          t += glue_width(v);
          if (glue_sign(p) == STRETCHING) {
            if (stretch_order(v) == glue_order(p))
              t += round(glue_set(p) *
                  stretch(v));
          } else if (glue_sign(p) == SHRINKING) {
            if (shrink_order(v) == glue_order(p))
              t -= round(glue_set(p) *
                  shrink(v));
          }
          s = link(s);
          u = link(u) = new_null_box();
          t += box_width(s);
          if (mode == -VMODE) {
            box_width(u) = box_width(s);
          } else {
            type(u) = VLIST_NODE;
            box_height(u) = box_width(s);
          }
        }
        if (mode == -VMODE) {
          box_height(r) = box_height(q);
          box_depth(r) = box_depth(q);
          if (t == box_width(r)) {
            glue_sign(r) = NORMAL;
            glue_order(r) = NORMAL;
            glue_set(r) = 0.0;
          } else if (t > box_width(r)) {
            glue_sign(r) = STRETCHING;
            glue_set(r) = (unset_stretch(r) == 0) ? 0.0 : 
              (float) (t - glue_width(r)) / unset_stretch(r);
          } else {
            glue_order(r) = glue_sign(r);
            glue_sign(r) = SHRINKING;
            if (unset_shrink(r) == 0)
              glue_set(r) = 0.0;
            else if (glue_order(r) == NORMAL &&
                box_width(r) - t > unset_shrink(r))
              glue_set(r) = 1.0;
            else glue_set(r) = (float) (box_width(r) - t) /
              unset_shrink(r);
          }
          box_width(r) = w;
          type(r) = HLIST_NODE;
        } else {
          box_width(r) = box_width(q);
          if (t == box_height(r)) {
            glue_sign(r) = NORMAL;
            glue_order(r) = NORMAL;
            glue_set(r) = 0.0;
          } else if (t > box_height(r)) {
            glue_sign(r) = STRETCHING;
            glue_set(r) = (unset_stretch(r) == 0) ? 0.0 :
              (float) (t - box_height(r)) / unset_stretch(r);
          } else {
            glue_order(r) = glue_sign(r);
            glue_sign(r) = SHRINKING;
            if (unset_shrink(r) == 0)
              glue_set(r) = 0.0;
            else if (glue_order(r) == NORMAL &&
                box_height(r) - t > unset_shrink(r))
              glue_set(r) = 1.0;
            else glue_set(r) = (float) (box_height(r) - t) /
              unset_shrink(r);
          }
          box_height(r) = w;
          type(r) = VLIST_NODE;
        } 
        shift_amount(r) = 0; 
        if (u != hold_head) {
          link(u) = link(r);
          link(r) = link(hold_head);
          r = u;
        }
        r = link(link(r));
        s = link(link(s));
      } while (r != null);
    } else if (type(q) == RULE_NODE) {
      if (is_running(rule_width(q)))
        rule_width(q) = rule_width(p);
      if (is_running(rule_height(q)))
        rule_height(q) = rule_height(p);
      if (is_running(rule_depth(q)))
        rule_depth(q) = rule_depth(p);
      if (o != 0) {
        r = link(q);
        link(q) = null;
        q = hpack(q, NATURAL);
        shift_amount(q) = o;
        link(q) = r;
        link(s) = q;
      }
    }
  }
  flush_node_list(p);
  pop_alignment();
  aux_save = aux;
  p = link(head);
  q = tail;
  pop_nest();
  if (mode == MMODE) {
    do_assignments();
    if (cur_cmd != MATH_SHIFT) {
      print_err("Missing $$ inserted");
      help_fin_display_align();
      back_error();
    } else {	
      get_x_token();
      if (cur_cmd != MATH_SHIFT) {
        print_err("Display math should end with $$");
        help_fin_display();
        back_error();
      }
    }
    pop_nest();
    tail_append(new_penalty(pre_display_penalty));
    tail_append(new_param_glue(ABOVE_DISPLAY_SKIP_CODE));
    link(tail) = p;
    if (p != null)
      tail = q;
    tail_append(new_penalty(post_display_penalty));
    tail_append(new_param_glue(BELOW_DISPLAY_SKIP_CODE));
    prev_depth = aux_save;
    resume_after_display();
  } else {
    aux = aux_save;
    link(tail) = p;
    if (p != null)
      tail = q;
    if (mode == VMODE)
      build_page();
  }
}

  void
_align_init ()
{
}

  void
_align_init_once ()
{
  align_head = new_avail(); 
  end_span = new_node(SPAN_NODE_SIZE);
  span_count(end_span) = MAX_QUARTERWORD + 1;
  span_link(end_span) = null;
}

/*
 **	Help text
 */

help_display_align ()
{
  help3("Displays can use special alignments (like \\eqalignno)",
      "only if nothing but the alignment itself is between $$'s.",
      "So I've deleted the formulas that preceded this alignment.");
}

help_fin_display_align ()
{
  help2("Displays can use special alignments (like \\eqalignno)",
      "only if nothing but the alignment itself is between $$'s.");
}

help_preamble_missing ()
{
  help3("There should be exactly one # between &'s, when an",
      "\\halign or \\valign is being set up. In this case you had",
      "none, so I've put one in; maybe that will work.");
}

help_preamble_many ()
{
  help3("There should be exactly one # between &'s, when an",
      "\\halign or \\valign is being set up. In this case you had",
      "more than one, so I'm ignoring all but the first.");
}

help_align_apply ()
{
  help3("You have given more \\span or & marks than there were",
      "in the preamble to the \\halign or \\valign now in progress.",
      "So I'll assume that you meant to type \\cr instead.");
}

help_fin_display ()
{
  help2("The `$' that I just saw supposedly matches a previous `$$'.",
      "So I shall assume that you typed `$$' both times.");
}

/*
 * Common TeX:alloc.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX alloc.c 3.2@(#)";
#endif

#include "tex.h"

ptr     avail;
ptr	avail_ptr;
ptr	avail_end;

struct blk_t {
  int	size_field;
  ptr	free_field;
  ptr	rblk_field;
  ptr	lblk_field;
};
typedef struct blk_t blk_t;

#define size(B)		((blk_t *) B)->size_field
#define free(B) 	((blk_t *) B)->free_field
#define rblk(B)		((blk_t *) B)->rblk_field
#define lblk(B)		((blk_t *) B)->lblk_field

#define is_free(B)	(free(B) == nil)

ptr	rover;
ptr	nil;

#ifdef STAT
int     dyn_used;
int	var_used;
#endif

#define BLOCK_SIZE	(8*8192)

  ptr
new_avail ()
{
  ptr	p;

  p = avail;
  if (p != null) {
    avail = link(avail);
  } else if (avail_ptr < avail_end) {
    p = avail_ptr;
    avail_ptr += sizeof(mcell);
  } else {
    p = avail_ptr = (ptr) malloc(BLOCK_SIZE);
    if (avail_ptr == null)
      overflow("new avail", BLOCK_SIZE);
    avail_end = avail_ptr + BLOCK_SIZE;
    avail_ptr += sizeof(mcell);
  }
  link(p) = null;
  type(p) = -1;
#ifdef STAT
  incr(dyn_used);
#endif
  return p;
}

  ptr
new_node (s)
  int     s;
{
  ptr	p;
  ptr	q;
  ptr	r;
  ptr	t;

restart:
  p = rover;
  do {
    q = p + size(p);
    while (is_free(q)) {
      t = rblk(q);
      if (q == rover)
        rover = t;
      lblk(t) = lblk(q);
      rblk(lblk(q)) = t;
      q += size(q);
    }
    r = q - s;
    if (r - p > sizeof(blk_t))  {
      size(p) = r - p;
      rover = p;
      goto found;
    }
    if (r == p && rblk(p) != p) {
      rover = rblk(p);
      t = lblk(p);
      lblk(rover) = t;
      rblk(t) = rover;
      goto found;
    }
    size(p) = q - p;
    p = rblk(p);
  } while (p != rover);

  q = (ptr)malloc(BLOCK_SIZE);
  if (q == null)
    overflow("get node", BLOCK_SIZE);
  p = lblk(rover);
  rblk(p) = q;
  lblk(rover) = q;
  rblk(q) = rover;
  lblk(q) = p;
  free(q) = nil;
  size(q) = BLOCK_SIZE - sizeof(blk_t);
  memset(q + size(q), 0, sizeof(blk_t));
  rover = q;
  goto restart;

found:
#ifdef STAT
  var_used += s;
#endif
  link(r) = null;
  return r;
}

  void
free_node (p, s)
  ptr	p;
  int	s;
{
  ptr     q;

  size(p) = s;
  free(p) = nil;
  q = lblk(rover);
  lblk(p) = q;
  rblk(p) = rover;
  lblk(rover) = p;
  rblk(q) = p;
#ifdef STAT
  var_used -= s;
#endif
}

  void
_alloc_init ()
{
}

  void
_alloc_init_once ()
{
  nil = (ptr)&nil;
  avail = null;
  rover = (ptr)malloc(BLOCK_SIZE);
  if (rover == null)
    overflow("init alloc", BLOCK_SIZE);
  size(rover) = BLOCK_SIZE - sizeof(blk_t);
  free(rover) = nil;
  lblk(rover) = rover;
  rblk(rover) = rover;
  memset(rover + size(rover), 0, sizeof(blk_t));
#ifdef STAT
  dyn_used = 0;
  var_used = 0;
#endif
}

/*
 * Common TeX:arith.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX arith.c 3.2@(#)";
#endif

#include "tex.h"

bool	arith_error;
scal	remainder;

  int 
half (x)
  int	x;
{
  return (odd(x) ? (x + 1) / 2 : x / 2);
}


  scal
round_decimals (k)
  int	k;
{
  int	a;

  a = 0;
  while (k > 0) {
    decr(k);
    a = (a + dig[k] * TWO) / 10;
  }
  return ((a + 1) / 2);
}

  void
print_scaled (s)
  scal	s;
{
  scal	delta;

  if (s < 0) {
    print("-");
    s = -s;
  }
  print_int(s / UNITY);
  print(".");
  s = 10 * (s % UNITY) + 5;
  delta = 10;
  do {
    if (delta > UNITY)
      s += 0100000 - 50000;
    print_char('0' + s / UNITY);
    s = 10 * (s % UNITY);
    delta *= 10;
  } while (s > delta);
}

  scal
mult_and_add (n, x, y, max_answer)
  int	n;
  scal	x;
  scal	y;
  scal	max_answer;
{

  if (n < 0) {
    x = -x;
    n = -n;
  }
  if (n == 0)
    return y;
  if (x <= (max_answer - y) / n
      && -x <= (max_answer + y) / n)
    return (n * x + y);
  arith_error = TRUE;
  return 0;
}

  scal
x_over_n (x, n)
  scal	x;
  int	n;
{
  bool	negative;
  scal	quotient;

  negative = FALSE;
  if (n == 0) {
    arith_error = TRUE;
    remainder = x;
    return 0;
  }
  if (n < 0) {
    x = -x;
    n = -n;
    negative = TRUE;
  }
  if (x >= 0) {
    quotient = x / n;
    remainder = x % n;
  } else {
    quotient = -(-x / n);
    remainder = -(-x % n);
  }
  if (negative)
    negate(remainder);
  return quotient;
}

  scal
xn_over_d (x, n, d)
  scal	x;
  int	n;
  int	d;
{
  int	t;
  int	u;
  int	v;
  bool	positive;

  if (x >= 0) {
    positive = TRUE;
  } else {
    x = -x;
    positive = FALSE;
  }
  t = x % 0100000 * n;
  u = x / 0100000 * n + t / 0100000;
  v = u % d * 0100000 + t % 0100000;
  if (u / d >= 0100000) {
    arith_error = TRUE;
  } else {
    u = 0100000 * (u / d) + v / d;
  }
  if (positive) {
    remainder = v % d;
    return u;
  } else {
    remainder = - (v % d);
    return -u;
  }
}

  int
badness (t, s)
  scal	t;
  scal	s;
{
  int	r;

  if (t == 0) {
    return 0;
  } else if (s <= 0) {
    return INF_BAD;
  } else {
    if (t <= 7230584)
      r = t * 297 / s;
    else if (s >= 1663497)
      r = t / (s / 297);
    else
      r = t;
    if (r > 1290)
      return INF_BAD;
    else
      return ((r * r * r + 0400000) / 01000000);
  }
}

  void
_arith_init ()
{
}

  void
_arith_init_once ()
{
}

/*
 * Common TeX:box.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX box.c 3.2@(#)";
#endif

#include "tex.h"

int	depth_threshold;
int	breadth_max;
fnt	font_in_short_display;

  ptr
new_null_box ()
{
  ptr	p;

  p = new_node(BOX_NODE_SIZE);
  type(p) = HLIST_NODE;
  subtype(p) = MIN_QUARTERWORD;
  box_width(p) = 0;
  box_depth(p) = 0;
  box_height(p) = 0;
  shift_amount(p) = 0;
  list_ptr(p) = null;
  glue_sign(p) = NORMAL;
  glue_order(p) = NORMAL;
  glue_set(p) = 0.0;

  return p;
}

  ptr
new_rule ()
{
  ptr	p;

  p = new_node(RULE_NODE_SIZE);
  type(p) = RULE_NODE;
  subtype(p) = 0;
  rule_width(p) = NULL_FLAG;
  rule_depth(p) = NULL_FLAG;
  rule_height(p) = NULL_FLAG;

  return p;
}

  ptr
new_ligature (f, c, q)
  int	f;
  int	c;
  ptr	q;
{
  ptr 	p;

  p = new_node(SMALL_NODE_SIZE);
  type(p) = LIGATURE_NODE;
  subtype(p) = 0;
  font(lig_char(p)) = f;
  character(lig_char(p)) = c;
  lig_ptr(p) = q;

  return p;
}

  ptr
new_lig_item (c)
  int	c;
{
  ptr	p;

  p = new_node(SMALL_NODE_SIZE);
  type(p) = LIGATURE_NODE;
  subtype(p) = c;
  lig_ptr(p) = null;

  return p;
}

  ptr
make_char_from_lig (p) 
  ptr	p;
{
  static mcell m;
  mcopy(&m, lig_char(p));
  link((ptr)&m) = link(p);
  return ((ptr)&m);
}

  ptr
new_disc ()
{
  ptr	p;

  p = new_node(SMALL_NODE_SIZE);
  type(p) = DISC_NODE;
  replace_count(p) = 0;
  pre_break(p) = null;
  post_break(p) = null;

  return p;
}

  ptr
new_math (w, s)
  scal	w;
  int	s;
{
  ptr	p;

  p = new_node(SMALL_NODE_SIZE);
  type(p) = MATH_NODE;
  subtype(p) = s;
  math_width(p) = w;

  return p;
}

  ptr
new_spec (p)
  ptr	p;
{
  ptr	q;

  q = new_node(GLUE_SPEC_SIZE);
  stretch_order(q) = stretch_order(p);
  shrink_order(q) = shrink_order(p);
  glue_ref_count(q) = 0;
  glue_width(q) = glue_width(p);
  stretch(q) = stretch(p);
  shrink(q) = shrink(p);

  return q;
}

  ptr
new_param_glue (n)
  int	n;
{
  ptr	p;
  ptr	q;

  p = new_node(SMALL_NODE_SIZE);
  type(p) = GLUE_NODE;
  subtype(p) = n + 1;
  leader_ptr(p) = null;
  q = glue_par(n);
  glue_ptr(p) = q;
  incr(glue_ref_count(q));

  return p;
}

  ptr
new_glue (q)
  ptr	q;
{
  ptr	p;

  p = new_node(SMALL_NODE_SIZE);
  type(p) = GLUE_NODE;
  subtype(p) = NORMAL;
  leader_ptr(p) = null;
  glue_ptr(p) = q;
  incr(glue_ref_count(q));

  return p;
}

  ptr
new_skip_param (n)
  int	n;
{
  ptr	p;
  ptr	q;

  q = new_spec(glue_par(n));
  p = new_glue(q);
  glue_ref_count(q) = 0;
  subtype(p) = n + 1;

  return p;
}

  ptr
new_kern (w)
  scal	w;
{
  ptr	p;

  p = new_node(SMALL_NODE_SIZE);
  type(p) = KERN_NODE;
  subtype(p) = NORMAL;
  kern_width(p) = w;

  return p;
}

  ptr
new_penalty (i)
  int	i;
{
  ptr	p;

  p = new_node(SMALL_NODE_SIZE);
  type(p) = PENALTY_NODE;
  subtype(p) = 0;
  penalty(p) = i;

  return p;
}

  void
print_font_and_char (p)
  ptr	p;
{
  if (font(p) < FONT_BASE || font(p) > FONT_MAX) {
    print("* ");
  } else {
    print_cs(font_id(font(p)));
    print_ASCII(character(p));
  }
}

  void
print_mark (p)
  ptr	p;
{
  print("{");
  show_token_list(token_link(p), null, MAX_PRINT_LINE - 10);
  print("}");
}

  void
print_rule_dimen (d)
  scal	d;
{
  if (is_running(d))
    print("*");
  else print_scaled(d);
}

  void
print_glue (d, o, s)
  scal	d;
  int	o;
  str	s;
{
  print_scaled(d);
  if (o < NORMAL || o > FILLL) {
    print("foul");
  } else if (o > NORMAL) {
    print("fil");
    while  (o >  FIL) {
      print("l");
      decr(o);
    }
  } else if (s) {
    print(s);
  }
}

  void
print_spec (p, s)
  ptr	p;
  str	s;
{
  print_scaled(glue_width(p));
  print(s);
  if (stretch(p) != 0) {
    print(" plus ");
    print_glue(stretch(p), stretch_order(p), s);
  }
  if (shrink(p) != 0) {
    print(" minus ");
    print_glue(shrink(p), shrink_order(p), s);
  }
}

  void
short_display (p)
  ptr	p;
{
  int	n;

  for (; p != null; p = link(p)) {
    if (is_char_node(p)) {
      if (font(p) != font_in_short_display) {
        if (font(p) < FONT_BASE || font(p) > FONT_MAX) {
          print("*");
        } else {
          print_esc(null_str);
          print(font_id_text(font(p)));
        }
        print(" ");
        font_in_short_display = font(p);
      }
      print_ASCII(character(p));
    } else {
      switch (type(p))
      {
        case HLIST_NODE:
        case VLIST_NODE: 
        case INS_NODE:
        case WHATSIT_NODE:
        case MARK_NODE:
        case ADJUST_NODE:
        case UNSET_NODE:
          print("[]");
          break;

        case RULE_NODE:
          print("|");
          break;

        case GLUE_NODE:
          if (glue_ptr(p) != zero_glue)
            print(" ");
          break;

        case MATH_NODE:
          print("$");
          break;

        case LIGATURE_NODE:
          short_display(lig_ptr(p));
          break;

        case DISC_NODE:
          short_display(pre_break(p));
          short_display(post_break(p));
          n = replace_count(p);
          while (n > 0) {
            if (link(p) != null)
              p = link(p);
            decr(n);
          }
          break;

        default:
          break;
      }
    }
  }
}

  void
show_box (p)
  ptr	p;
{
  depth_threshold = show_box_depth;
  breadth_max = show_box_breadth;
  if (breadth_max <= 0)
    breadth_max = 5;
  flush_str();
  if (cur_str_ptr + depth_threshold >= cur_str_end)
    depth_threshold = cur_str_end - cur_str_ptr - 1;
  show_node_list(p);
}

  void
show_glue_set (p)
  ptr	p;
{
  if (glue_set(p) != 0 && glue_sign(p) != NORMAL) {
    print(", glue set ");
    if (glue_sign(p) == SHRINKING)
      print("- ");
    if (abs(glue_set(p)) > 20000.0) {
      if (glue_set(p) > 0)
        print(">");
      else print("< -");
      print_glue(20000 * UNITY, glue_order(p), null_str);
    } else {
      print_glue(round(glue_set(p) * UNITY),
          glue_order(p), null_str);
    }
  }
}

  void
show_box1 (p)
  ptr	p;
{
  if (type(p) == HLIST_NODE)
    print_esc("h");
  else if (type(p) == VLIST_NODE)
    print_esc("v");
  else print_esc("unset");
  print("box(");
  print_scaled(box_height(p));
  print("+")	;
  print_scaled(box_depth(p));
  print(")x")	;
  print_scaled(box_width(p));
  if (type(p) == UNSET_NODE) {
    if (unset_span_count(p) != MIN_QUARTERWORD) {
      print(" (");
      print_int(unset_span_count(p)+1);
      print(" columns)");
    }
    if (unset_stretch(p) != 0) {
      print(", stretch ");
      print_glue(unset_stretch(p), glue_order(p), null_str);
    }
    if (unset_shrink(p) != 0) {
      print(", shrink ");
      print_glue(unset_shrink(p), glue_sign(p), null_str);
    }
  } else {
    show_glue_set(p);
    if (shift_amount(p) != 0) {
      print(", shifted ");
      print_scaled(shift_amount(p));
    }
  }
  node_list_display(list_ptr(p));
}

  void
show_rule (p)
  ptr	p;
{
  print_esc("rule(");
  print_rule_dimen(rule_height(p));
  print("+");
  print_rule_dimen(rule_depth(p));
  print(")x");
  print_rule_dimen(rule_width(p));
}

  void
show_insertion (p)
  ptr	p;
{
  print_esc("insert");
  print_int(subtype(p));
  print(", natural size ");
  print_scaled(ins_height(p));
  print("; split(");
  print_spec(split_top_ptr(p), null_str);
  print(",");
  print_scaled(ins_depth(p));
  print("); float cost ");
  print_int(float_cost(p));
  node_list_display(ins_ptr(p));
}

  void
show_leaders (p)
  ptr	p;
{
  print_esc(null_str);
  if (subtype(p) == C_LEADERS)
    print("c");
  else if (subtype(p) == X_LEADERS)
    print("x");
  print("leaders ");
  print_spec(glue_ptr(p), null_str);
  node_list_display(leader_ptr(p));
}

  void
show_glue_type (n)
  int	n;
{
  print("(");
  if (n < COND_MATH_GLUE) {
    if (n <= GLUE_PARS) {
      print_skip_param(n - 1);
    } else {
      print_mu_skip_param(n - 1 - GLUE_PARS);
    }
  } else if (n == COND_MATH_GLUE) {
    print_esc("nonscript");
  } else {
    print_esc("mskip");
  }
  print(")");
}

  void
show_glue (p)
  ptr	p;
{
  if (subtype(p) >= A_LEADERS) {
    show_leaders(p);
  } else {
    print_esc("glue");
    if (subtype(p) != NORMAL)
      show_glue_type(subtype(p));
    if (subtype(p) != COND_MATH_GLUE) {
      print(" ");
      if (subtype(p) < COND_MATH_GLUE)
        print_spec(glue_ptr(p), null_str);
      else print_spec(glue_ptr(p), "mu");
    }
  }
}

  void
show_kern (p)
  ptr	p;
{
  if (subtype(p) != MU_GLUE) {
    print_esc("kern");
    if (subtype(p) != NORMAL)
      print(" ");
    print_scaled(kern_width(p));
    if (subtype(p) == ACC_KERN)
      print(" (for accent)");
  } else {
    print_esc("mkern");
    print_scaled(kern_width(p));
    print("mu");
  }
}

  void
show_math (p)
  ptr	p;
{
  print_esc("math");
  if (subtype(p) == BEFORE) {
    print("on");
  } else {
    print("off");
  }
  if (math_width(p) != 0) {
    print(", surrounded ");
    print_scaled(math_width(p));
  }
}

  void
show_ligature (p)
  ptr	p;
{
  print_font_and_char(lig_char(p));
  print(" (ligature ");
  if (subtype(p) > 1)
    print("|");
  font_in_short_display = font(lig_char(p));
  short_display(lig_ptr(p));
  if (odd(subtype(p)))
    print("|");
  print(")");
}

  void
show_discretionary (p)
  ptr	p;
{
  print_esc("discretionary");
  if (replace_count(p) > 0) {
    print(" replacing ");
    print_int(replace_count(p));
  }
  node_list_display(pre_break(p));
  append_char('|');
  show_node_list(post_break(p));
  flush_char();
}

  void
show_penalty (p)
  ptr	p;
{
  print_esc("penalty ");
  print_int(penalty(p));
}

  void
show_mark (p)
  ptr	p;
{
  print_esc("mark");
  print_mark(mark_ptr(p));
}

  void
show_adjust (p)
  ptr	p;
{
  print_esc("vadjust");
  node_list_display(adjust_ptr(p));
}

  void
show_node_list (p)
  ptr 	p;
{
  int	n;

  if (cur_length() > depth_threshold) {
    if (p > null)
      print(" []");
    return;
  }
  n = 0;
  while (p > null) {
    print_ln();
    print_str();
    incr(n);
    if (n > breadth_max) {
      print("etc.");
      return;
    }
    if (is_char_node(p)) {
      print_font_and_char(p);
    } else {
      switch (type(p))
      {
        case HLIST_NODE:
        case VLIST_NODE:
        case UNSET_NODE:
          show_box1(p);
          break;

        case RULE_NODE:
          show_rule(p);
          break;

        case INS_NODE:
          show_insertion(p);
          break;

        case WHATSIT_NODE:
          show_whatsit(p);
          break;

        case GLUE_NODE:
          show_glue(p);
          break;

        case KERN_NODE:
          show_kern(p);
          break;

        case MATH_NODE:
          show_math(p);
          break;

        case LIGATURE_NODE:
          show_ligature(p);
          break;

        case PENALTY_NODE:
          show_penalty(p);
          break;

        case DISC_NODE:
          show_discretionary(p);
          break;

        case MARK_NODE:
          show_mark(p);
          break;

        case ADJUST_NODE:
          show_adjust(p);
          break;

        case STYLE_NODE:
          print_style(subtype(p));
          break;

        case CHOICE_NODE:
          show_choice_node(p);
          break;

        case INNER_NOAD:
        case ORD_NOAD:
        case OP_NOAD:
        case BIN_NOAD:
        case REL_NOAD:
        case OPEN_NOAD:
        case CLOSE_NOAD:
        case PUNCT_NOAD:
        case RADICAL_NOAD:
        case OVER_NOAD:
        case UNDER_NOAD:
        case VCENTER_NOAD:
        case ACCENT_NOAD:
        case LEFT_NOAD:
        case RIGHT_NOAD:
          show_normal_noad(p);
          break;

        case FRACTION_NOAD:
          show_fraction_noad(p);
          break;

        default:
          print("Unknown node type!");
          break;
      }
    }
    p = link(p);
  }
}

  void
delete_glue_ref (p)
  ptr	p;
{
  if (glue_ref_count(p) == 0) {
    free_node(p, GLUE_SPEC_SIZE);
  } else {
    decr(glue_ref_count(p));
  }
}

  void
flush_node_list (p)
  ptr	p;
{
  ptr	q;

  while (p != null) {
    q = link(p);
    if (is_char_node(p)) {
      free_avail(p);
    } else {
      switch (type(p))
      {
        case HLIST_NODE:
        case VLIST_NODE:
        case UNSET_NODE:
          flush_node_list(list_ptr(p));
          free_node(p, BOX_NODE_SIZE);
          goto done;

        case RULE_NODE:
          free_node(p, RULE_NODE_SIZE);
          goto done;

        case INS_NODE:
          flush_node_list(ins_ptr(p));
          delete_glue_ref(split_top_ptr(p));
          free_node(p, INS_NODE_SIZE);
          goto done;

        case WHATSIT_NODE:
          free_whatsit(p);
          goto done;

        case GLUE_NODE:
          fast_delete_glue_ref(glue_ptr(p));
          if (leader_ptr(p) != null)
            flush_node_list(leader_ptr(p));
          break;

        case KERN_NODE:
        case MATH_NODE:
        case PENALTY_NODE:
          break;

        case LIGATURE_NODE:
          flush_node_list(lig_ptr(p));
          break;

        case MARK_NODE:
          delete_token_ref(mark_ptr(p));
          break;

        case DISC_NODE:
          flush_node_list(pre_break(p));
          flush_node_list(post_break(p));
          break;

        case ADJUST_NODE:
          flush_node_list(adjust_ptr(p));
          break;

        case STYLE_NODE:
          free_node(p, STYLE_NODE_SIZE);
          goto done;

        case CHOICE_NODE:
          flush_node_list(display_mlist(p));
          flush_node_list(text_mlist(p));
          flush_node_list(script_mlist(p));
          flush_node_list(script_script_mlist(p));
          free_node(p, STYLE_NODE_SIZE);
          goto done;

        case ORD_NOAD:
        case OP_NOAD:
        case BIN_NOAD:
        case REL_NOAD:
        case OPEN_NOAD:
        case CLOSE_NOAD:
        case PUNCT_NOAD:
        case INNER_NOAD:
        case RADICAL_NOAD:
        case OVER_NOAD:
        case UNDER_NOAD:
        case VCENTER_NOAD:
        case ACCENT_NOAD:
          if (math_type(nucleus(p)) >= SUB_BOX)
            flush_node_list(math_link(nucleus(p)));
          if (math_type(supscr(p)) >= SUB_BOX)
            flush_node_list(math_link(supscr(p)));
          if (math_type(subscr(p)) >= SUB_BOX)
            flush_node_list(math_link(subscr(p)));
          if (type(p) == RADICAL_NOAD)
            free_node(p, RADICAL_NOAD_SIZE);
          else if (type(p) == ACCENT_NOAD)
            free_node(p, ACCENT_NOAD_SIZE);
          else free_node(p, NOAD_SIZE);
          goto done;

        case LEFT_NOAD:
        case RIGHT_NOAD:
          free_node(p, NOAD_SIZE);
          goto done;

        case FRACTION_NOAD:
          flush_node_list(math_link(numerator(p)));
          flush_node_list(math_link(denominator(p)));
          free_node(p, FRACTION_NOAD_SIZE);
          goto done;

        default:
          confusion("flushing");
          break;
      }
      free_node(p, SMALL_NODE_SIZE);
done:;	
    }
    p = q;
  }
}

  ptr
copy_node_list (p)
  ptr	p;
{
  ptr	h;
  ptr	q;
  ptr	r;

  h = q = new_avail();
  while (p != null) {
    if (is_char_node(p)) {
      r = new_avail();
      font(r) = font(p);
      character(r) = character(p);
    } else {
      switch (type(p))
      {
        case HLIST_NODE:
        case VLIST_NODE:
        case UNSET_NODE:
          r = new_node(BOX_NODE_SIZE);
          memcpy((void *)r, (void *)p, BOX_NODE_SIZE);
          list_ptr(r) = copy_node_list(list_ptr(p));
          break;

        case RULE_NODE:
          r = new_node(RULE_NODE_SIZE);
          memcpy((void *)r, (void *)p, RULE_NODE_SIZE);
          break;

        case INS_NODE:
          r = new_node(INS_NODE_SIZE);
          memcpy((void *)r, (void *)p, INS_NODE_SIZE);
          add_glue_ref(split_top_ptr(p));
          ins_ptr(r) = copy_node_list(ins_ptr(p));
          break;

        case WHATSIT_NODE:
          r = copy_whatsit(p);
          break;

        case GLUE_NODE:
          r = new_node(SMALL_NODE_SIZE);
          memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
          add_glue_ref(glue_ptr(p));
          glue_ptr(r) = glue_ptr(p);
          leader_ptr(r) = copy_node_list(leader_ptr(p));
          break;

        case KERN_NODE:
        case MATH_NODE:
        case PENALTY_NODE:
          r = new_node(SMALL_NODE_SIZE);
          memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
          break;

        case LIGATURE_NODE:
          r = new_node(SMALL_NODE_SIZE);
          memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
          lig_ptr(r) = copy_node_list(lig_ptr(p));
          break;

        case DISC_NODE:
          r = new_node(SMALL_NODE_SIZE);
          memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
          pre_break(r) = copy_node_list(pre_break(p));
          post_break(r) = copy_node_list(post_break(p));
          break;

        case MARK_NODE:
          r = new_node(SMALL_NODE_SIZE);
          memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
          add_token_ref(mark_ptr(p));
          break;

        case ADJUST_NODE:
          r = new_node(SMALL_NODE_SIZE);
          memcpy((void *)r, (void *)p, SMALL_NODE_SIZE);
          adjust_ptr(r) = copy_node_list(adjust_ptr(p));
          break;

        default:
          confusion("copying");
          break;
      }
    }
    q = link(q) = r;
    p = link(p);
  }
  link(q) = null;
  q = link(h);
  free_avail(h);
  return q;
}

  void
_box_init ()
{
}

  void
_box_init_once ()
{
}

/*
 * Common TeX:boxlist.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX boxlist.c 3.2@(#)";
#endif

#include "tex.h"

ptr	cur_box;

  void
handle_right_brace ()
{
  scal	d;
  int	f;
  ptr	p;
  ptr	q;

  switch (cur_group)
  {
    case SIMPLE_GROUP:
      unsave();
      break;

    case BOTTOM_LEVEL:
      print_err("Too many }'s");
      help_close_group();
      error();
      break;

    case SEMI_SIMPLE_GROUP:
    case MATH_SHIFT_GROUP:
    case MATH_LEFT_GROUP:
      extra_right_brace();
      break;

    case HBOX_GROUP:
      package(0);
      break;

    case ADJUSTED_HBOX_GROUP:
      adjust_tail = adjust_head;
      package(0);
      break;

    case VBOX_GROUP:
      end_graf();
      package(0);
      break;

    case VTOP_GROUP:
      end_graf();
      package(VTOP_CODE);
      break;

    case INSERT_GROUP:
      end_graf();
      q = split_top_skip;
      add_glue_ref(q);
      d = split_max_depth;
      f = floating_penalty;
      unsave();
      decr(save_ptr);
      p = vpack(link(head), NATURAL);
      pop_nest();
      if (saved(0) < 255) {
        tail_append(new_node(INS_NODE_SIZE));
        type(tail) = INS_NODE;
        subtype(tail) = saved(0);
        ins_height(tail) = box_height(p) + box_depth(p);
        ins_ptr(tail) = list_ptr(p);
        split_top_ptr(tail) = q;
        ins_depth(tail) = d;
        float_cost(tail) = f;
      } else {
        tail_append(new_node(SMALL_NODE_SIZE));
        type(tail) = ADJUST_NODE;
        subtype(tail) = 0;
        adjust_ptr(tail) = list_ptr(p);
        delete_glue_ref(q);
      }
      free_node(p, BOX_NODE_SIZE);
      if (nest_ptr == nest) 
        build_page();
      break;

    case OUTPUT_GROUP:
      if (loc != null
          || (token_type != OUTPUT_TEXT && token_type != BACKED_UP)) {
        print_err("Unbalanced output routine");
        help_output_balance();
        error();
        do get_token();
        while (loc != null);
      }
      end_token_list();
      end_graf();
      unsave();
      output_active = FALSE;
      insert_penalties = 0;
      if (box(255) != null) {
        print_err("Output routine didn't use all of ");
        print_esc("box255");
        help_output();
        box_error(255);
      }
      if (tail != head) {
        link(page_tail) = link(head);
        page_tail = tail;
      }
      if (link(page_head) != null) {
        if (link(contrib_head) == null)
          contrib_tail = page_tail;
        link(page_tail) = link(contrib_head);
        link(contrib_head) = link(page_head);
        link(page_head) = null;
        page_tail = page_head;
      }
      pop_nest();
      build_page();
      break;

    case DISC_GROUP:
      build_discretionary();
      break;

    case ALIGN_GROUP:
      back_input();
      cur_tok = sym2tok(FROZEN_CR);
      print_err("Missing ");
      print_esc("cr");
      print(" inserted");
      help_align_cr();
      ins_error(); 
      break;

    case NO_ALIGN_GROUP:
      end_graf();
      unsave();
      align_peek();
      break;

    case VCENTER_GROUP:
      end_graf();
      unsave();
      save_ptr -= 2;
      p = vpackage(link(head), saved(1), saved(0), MAX_DIMEN);
      pop_nest();
      tail_append(new_noad());
      type(tail) = VCENTER_NOAD;
      math_type(nucleus(tail)) = SUB_BOX;
      info(nucleus(tail)) = p;
      break;

    case MATH_CHOICE_GROUP:
      build_choices();
      break;

    case MATH_GROUP:
      unsave();
      decr(save_ptr);
      math_type(saved(0)) = SUB_MLIST;
      p = fin_mlist(null);
      math_link(saved(0)) = p;
      if (p != null) {
        if (link(p) == null) {
          if (type(p) == ORD_NOAD) {
            if (math_type(subscr(p)) == EMPTY
                && math_type(supscr(p)) == EMPTY) {
              mcopy(saved(0), nucleus(p));
              free_node(p, NOAD_SIZE);
            }
          } else if (type(p) == ACCENT_NOAD
              && saved(0) == nucleus(tail)
              && type(tail) == ORD_NOAD) {
            q = head;
            while (link(q) != tail)
              q = link(q);
            link(q) = p;
            free_node(tail, NOAD_SIZE);
            tail = p;
          }
        }
      }
      break;

    default:
      confusion("rightbrace");
      break;
  }
}

  void
extra_right_brace ()
{
  print_err("Extra }, or forgotten ");
  switch (cur_group) {
    case SEMI_SIMPLE_GROUP: print_esc("endgroup"); break;
    case MATH_SHIFT_GROUP: print("$"); break;
    case MATH_LEFT_GROUP: print_esc("right"); break;
  }
  help_group_close();
  error();
  incr(align_state);
}

  void
append_glue ()
{
  int	s;

  s = cur_chr;
  switch (s) {
    case FIL_CODE: cur_val = fil_glue; break;
    case FILL_CODE: cur_val = fill_glue; break;
    case SS_CODE: cur_val = ss_glue; break;
    case FIL_NEG_CODE: cur_val = fil_neg_glue; break;
    case SKIP_CODE: scan_glue(GLUE_VAL); break;
    case MSKIP_CODE: scan_glue(MU_VAL); break;
    default: break;
  }
  tail_append(new_glue(cur_val));
  if (s >= SKIP_CODE) {
    decr(glue_ref_count(cur_val));
    if (s > SKIP_CODE)
      subtype(tail) = MU_GLUE;
  }
}

  void
append_kern ()
{
  int	s;

  s = cur_chr;
  scan_dimen(s == MU_GLUE, FALSE, FALSE);
  tail_append(new_kern(cur_val));
  subtype(tail) = s;
}

  void
scan_box (box_context)
  int	box_context;
{
  get_nbrx_token();
  if (cur_cmd == MAKE_BOX) {
    begin_box(box_context);
  } else if (box_context >= LEADER_FLAG
      && (cur_cmd == HRULE || cur_cmd == VRULE)) {
    cur_box = scan_rule_spec();
    box_end(box_context);
  } else {
    print_err("A <box> was supposed to be here");
    help_box();
    back_error();
  }
}

  void
begin_box (box_context)
  int	box_context;
{
  int	k, m, n;
  ptr	p, q;

  switch (cur_chr)
  {
    case BOX_CODE:
      scan_eight_bit_int();
      cur_box = box(cur_val);
      box(cur_val) = null;
      break;

    case COPY_CODE:
      scan_eight_bit_int();
      cur_box = copy_node_list(box(cur_val));
      break;

    case LAST_BOX_CODE:
      cur_box = null;
      if (abs(mode) == MMODE) {
        you_cant();
        help_lastbox_m();
        error();
      } else if (mode == VMODE && head == tail) {
        you_cant();
        help_lastbox_v();
        error();
      } else if (type(tail) == HLIST_NODE
          || type(tail) == VLIST_NODE) {
        for (q = head; q != tail; q = link(p)) {
          p = q;
          if (type(q) == DISC_NODE) {
            m = 1;
            while (m <= replace_count(q)) {
              p = link(p);
              incr(m);
            }
            if (p == tail) {
              break;
            }
          }
        }
        cur_box = tail;
        shift_amount(cur_box) = 0;
        tail = p;
        link(p) = null;
      }
      break;

    case VSPLIT_CODE:
      scan_eight_bit_int();
      n = cur_val;
      if (!scan_keyword("to")) {
        print_err("Missing `to' inserted");
        help_vsplit();
        error();
      }
      scan_normal_dimen();
      cur_box = vsplit(n, cur_val);
      break;

    default:
      k = cur_chr - VTOP_CODE;
      saved(0) = box_context;
      if (k == HMODE) {
        if (box_context < BOX_FLAG && abs(mode) == VMODE) {
          scan_spec(ADJUSTED_HBOX_GROUP, TRUE);
        } else {
          scan_spec(HBOX_GROUP, TRUE);
        }
      } else {
        if (k == VMODE) {
          scan_spec(VBOX_GROUP, TRUE);
        } else {
          scan_spec(VTOP_GROUP, TRUE);
          k = VMODE;
        }
        normal_paragraph();
      }
      push_nest();
      mode = -k;
      if (k == VMODE) {
        prev_depth = IGNORE_DEPTH;
        if (every_vbox != null)
          begin_token_list(every_vbox, EVERY_VBOX_TEXT);
      } else {
        space_factor = 1000;
        if (every_hbox != null) 
          begin_token_list(every_hbox, EVERY_HBOX_TEXT);
      }
      return;
  }
  box_end(box_context);
}

  void
box_end (box_context)
  int	box_context;
{
  ptr	p;

#define LEADERS "Leaders not followed by proper glue"

  if (box_context < BOX_FLAG) {
    if (cur_box != null) {
      shift_amount(cur_box) = box_context;
      if (abs(mode) == VMODE) {
        append_to_vlist(cur_box);
        if (adjust_tail != null) {
          if (adjust_head != adjust_tail) {
            link(tail) = link(adjust_head);
            tail = adjust_tail;
          }
          adjust_tail = null;
        }
        if (mode > 0) 
          build_page();
      } else {
        if (abs(mode) == HMODE) {
          space_factor = 1000;
        } else {
          p = new_noad();
          math_type(nucleus(p)) = SUB_BOX;
          info(nucleus(p)) = cur_box;
          cur_box = p;
        }
        tail_append(cur_box);
      }
    }
  } else if (box_context < SHIP_OUT_FLAG) {
    if (box_context < BOX_FLAG + 256) {
      reg_define(box_reg[box_context - BOX_FLAG],
          BOX_REG, cur_box);
    } else {
      reg_gdefine(box_reg[box_context - BOX_FLAG - 256],
          BOX_REG, cur_box);
    }
  } else if (cur_box != null) {
    if (box_context > SHIP_OUT_FLAG) {
      get_nbrx_token();
      if (cur_cmd == HSKIP && abs(mode) != VMODE
          || cur_cmd == VSKIP && abs(mode) == VMODE
          || cur_cmd == MSKIP && abs(mode) == MMODE) {
        append_glue();
        leader_ptr(tail) = cur_box;
        subtype(tail) = box_context -
          (LEADER_FLAG - A_LEADERS);
      } else {
        print_err(LEADERS);
        help_leaders();
        back_error();
        flush_node_list(cur_box);
      }
    } else {
      ship_out(cur_box);
    }
  }
}

  void
package (c)
  int	c;
{
  scal	d;
  scal	h;
  ptr	p;

  d = box_max_depth;
  unsave();
  save_ptr -= 3;
  if (mode == -HMODE) {
    cur_box = hpack(link(head), saved(2), saved(1));
  } else {
    cur_box = vpackage(link(head), saved(2), saved(1), d);
    if (c == VTOP_CODE) {
      h = 0;
      p = list_ptr(cur_box);
      if (p != null && type(p) <= RULE_NODE)
        h = box_height(p);
      box_depth(cur_box) += box_height(cur_box) - h;
      box_height(cur_box) = h;
    }
  }
  pop_nest();
  box_end(saved(0));
}

  void
normal_paragraph ()
{
  if (looseness != 0)
    reg_define(int_reg[LOOSENESS_CODE], INT_REG, 0);
  if (hang_indent != 0)
    reg_define(dimen_reg[HANG_INDENT_CODE], DIMEN_REG, 0);
  if (hang_after != 1)
    reg_define(int_reg[HANG_AFTER_CODE], INT_REG, 1);
  if (par_shape_ptr != null)
    eq_define(par_shape_cs, SET_SHAPE, null);
}

  int
norm_min (h)
  int	h;
{
  return (h <= 0 ? 1 : h >= 63 ? 63 : h);
}

  void
new_graf (indented)
  bool	indented;
{
  prev_graf = 0;
  if (mode == VMODE || head != tail)
    tail_append(new_param_glue(PAR_SKIP_CODE));
  lhmin = norm_min(left_hyphen_min);
  rhmin = norm_min(right_hyphen_min);
  push_nest();
  mode = HMODE;
  space_factor = 1000;
  clang = 0;	
  if (indented) {
    link(head) = tail = new_null_box();
    box_width(tail) = par_indent;
  }
  if (every_par != null)
    begin_token_list(every_par, EVERY_PAR_TEXT);
  if (nest_ptr == nest + 1) 
    build_page();
}

  void
indent_in_hmode ()
{
  ptr	p, q;

  if (cur_chr > 0) {
    p = new_null_box();
    box_width(p) = par_indent;
    if (abs(mode) == HMODE) {
      space_factor = 1000;
    } else {
      q = new_noad();
      math_type(nucleus(q)) = SUB_BOX;
      math_link(nucleus(q)) = p;
      p = q;
    }
    tail_append(p);
  }
}

  void
end_graf ()
{
  if (mode == HMODE) {
    if (head == tail)
      pop_nest();
    else line_break(widow_penalty);
    normal_paragraph();
    error_cnt = 0;
  }
}

  void
head_for_vmode ()
{
  if (mode < 0) {
    if (cur_cmd != HRULE) {
      off_save();
    } else {
      print_err("You can't use `");
      print_esc("hrule");
      print("' here except with leaders");
      help_head_for_vmode();
      error();
    }
  } else {
    back_input();
    cur_tok = par_tok;
    back_input();
    token_type = INSERTED;
  }
}

  void
append_to_vlist (b)
  ptr	b;
{
  scal	d;
  ptr	p;

  if (prev_depth > IGNORE_DEPTH) {
    d = glue_width(baseline_skip) - prev_depth - box_height(b);
    if (d < line_skip_limit) {
      p = new_param_glue(LINE_SKIP_CODE);
    } else {
      p = new_skip_param(BASELINE_SKIP_CODE);
      glue_width(glue_ptr(p)) = d;
    }
    tail_append(p);
  }
  tail_append(b);
  prev_depth = box_depth(b);
}

  void
begin_insert_or_adjust ()
{
  if (cur_cmd == VADJUST) {
    cur_val = 255;
  } else {
    scan_eight_bit_int();
    if (cur_val == 255) {
      print_err("You can't ");
      print_esc("insert");
      print_int(255);
      help_insert_255();
      error();
      cur_val = 0;
    }
  }
  saved(0) = cur_val;
  incr(save_ptr);
  new_save_level(INSERT_GROUP);
  scan_left_brace();
  normal_paragraph();
  push_nest();
  mode = -VMODE;
  prev_depth = IGNORE_DEPTH;
}

  void
make_mark ()
{
  ptr	p;

  scan_toks(FALSE, TRUE);
  p = new_node(SMALL_NODE_SIZE);
  type(p) = MARK_NODE;
  subtype(p) = 0;
  mark_ptr(p) = def_ref;
  tail_append(p);
}

  void
append_penalty ()
{
  scan_int();
  tail_append(new_penalty(cur_val));
  if (mode == VMODE) 
    build_page();
}

  void
delete_last ()
{
  int	m;
  ptr	p, q;

  if (mode == VMODE && tail == head) {
    if (cur_chr != GLUE_NODE
        || last_glue != null) {
      you_cant();
      if (cur_chr == KERN_NODE) {
        help_delete_last_kern();
      } else if (cur_chr == GLUE_NODE) {
        help_delete_last_skip();
      } else {
        help_delete_last_pen(); 
      }
      error();
    }
  } else if (!is_char_node(tail) && type(tail) == cur_chr) {
    for (q = head; q != tail; q = link(p)) {
      p = q;
      if (!is_char_node(q) && type(q) == DISC_NODE) {
        for (m = 1; m <= replace_count(q); incr(m))
          p = link(p);
        if (p == tail)
          return;
      }
    }
    link(p) = null;
    flush_node_list(tail);
    tail = p;
  }
}

  void
unpackage ()
{	
  int	c;
  ptr	p;

  c = cur_chr;
  scan_eight_bit_int();
  p = box(cur_val);
  if (p == null)
    return;
  if (abs(mode) == MMODE
      || abs(mode) == VMODE && type(p) != VLIST_NODE
      || abs(mode) == HMODE && type(p) != HLIST_NODE) {
    print_err("Incompatible list can't be unboxed");
    help_pandora();
    error();
    return;
  }
  if (c == COPY_CODE) {
    link(tail) = copy_node_list(list_ptr(p));
  } else {
    link(tail) = list_ptr(p);
    box(cur_val) = null;
    free_node(p, BOX_NODE_SIZE);
  }
  while (link(tail) != null)
    tail = link(tail);
}

  void
append_italic_correction ()
{
  ptr	p;
  fnt	f;

  if (tail != head) {
    if (is_char_node(tail))
      p = tail;
    else if (type(tail) == LIGATURE_NODE)
      p = lig_char(tail);
    else return;
    f = font(p);
    tail = link(tail) =
      new_kern(char_italic(f, char_info(f, character(p))));
    subtype(tail) = EXPLICIT;
  }
}

  void
append_discretionary ()
{
  int	c;

  tail_append(new_disc());
  if (cur_chr == 1) {
    c = hyphen_char(cur_font);
    if (c >= 0 && c < 256)
      pre_break(tail) = new_character(cur_font, c);
  } else {
    incr(save_ptr);
    saved(-1) = 0;
    scan_left_brace();
    new_save_level(DISC_GROUP);
    push_nest();
    mode = -HMODE;
    space_factor = 1000;
  }
}

  void
build_discretionary ()
{
  int	n;
  ptr	p, q;

  unsave();
  q = head;
  p = link(q);
  n = 0;
  while (p != null) {
    if (!is_char_node(p)
        && type(p) > RULE_NODE
        && type(p) != KERN_NODE
        && type(p) != LIGATURE_NODE) {
      print_err("Improper discretionary list");
      help_discretionary();
      error();
      flush_discretionary(p);
      link(q) = null;
      break;
    }
    q = p;
    p = link(q);
    incr(n);
  }
  p = link(head);
  pop_nest();
  switch (saved(-1))
  {
    case 0:
      pre_break(tail) = p;
      break;

    case 1:
      post_break(tail) = p;
      break;

    case 2:
      if (n > 0 && abs(mode) == MMODE) {
        print_err("Illegal math ");
        print_esc("discretionary");
        help_math_disc();
        flush_node_list(p);
        n = 0;
        error();
      } else {
        link(tail) = p;
      }
      if (n <= MAX_QUARTERWORD) {
        replace_count(tail) = n;
      } else {
        print_err("Discretionary list is too long");
        help_disc();
        error();
      }
      if (n > 0)
        tail = q;
      decr(save_ptr);
      return;
  }
  incr(saved(-1));
  scan_left_brace();
  new_save_level(DISC_GROUP);
  push_nest();
  mode = -HMODE;
  space_factor = 1000;
}

  void
flush_discretionary (p)
  ptr	p;
{
  begin_diagnostic();
  print_nl("The following discretionary sublist has been deleted:");
  show_box(p);
  end_diagnostic(TRUE);
  flush_node_list(p);
}

  void
make_accent ()
{
  fnt	f;
  qcell	i;
  ptr	p, q, r;
  scal	a, h, w, x;
  float	s, t;
  scal	delta;

  scan_char_num();
  f = cur_font;
  p = new_character(f, cur_val);
  if (p != null) {
    x = x_height(f);
    s = (float) slant(f) / 65536.0;
    a = char_width(f, char_info(f, character(p)));
    do_assignments();
    q = null;
    f = cur_font;
    if (cur_cmd == LETTER
        || cur_cmd == OTHER_CHAR
        || cur_cmd ==  CHAR_GIVEN) {
      q = new_character(f, cur_chr);
    } else if (cur_cmd ==  CHAR_NUM) {
      scan_char_num();
      q = new_character(f, cur_val);
    } else {
      back_input();
    }
    if (q != null) {
      t = (float) slant(f) / 65536.0;
      i = char_info(f, character(q));
      w = char_width(f, i); 
      h = char_height(f, height_depth(i));
      if (h != x) {
        p = hpack(p, NATURAL);
        shift_amount(p) = x - h;
      }
      delta = round((float) (w - a) / 2.0 + h * t - x * s);
      r = new_kern(delta);
      subtype(r) = ACC_KERN;
      link(tail) = r;
      link(r) = p;
      tail = new_kern(-a - delta);
      subtype(tail) = ACC_KERN;
      link(p) = tail;
      p = q;
    }
    tail_append(p);
    space_factor = 1000;
  }
}

  void
align_error ()
{
  if (abs(align_state) > 2) {
    print_err("Misplaced ");
    print_cmd_chr(cur_cmd, cur_chr);
    if (cur_tok == TAB_TOKEN + '&') 
      help_tab();
    else help_align_error();
    error();
  } else {
    back_input();
    if (align_state < 0) {
      print_err("Missing { inserted");
      incr(align_state);
      cur_tok = LEFT_BRACE_TOKEN + '{';
    } else {
      print_err("Missing } inserted");
      decr(align_state);
      cur_tok = RIGHT_BRACE_TOKEN + '}';
    }
    help_fix_alignment();
    ins_error();
  }
}

  void
no_align_error ()
{
  print_err("Misplaced ");
  print_esc("noalign");
  help_noalign();
  error();
}

  void
omit_error ()
{
  print_err("Misplaced ");
  print_esc("omit");
  help_omit();
  error();
}

  void
do_endv ()
{
  if (cur_group == ALIGN_GROUP) {
    end_graf();
    if (fin_col())
      fin_row();
  } else {
    off_save();
  }
}

  void
cs_error ()
{
  print_err("Extra ");
  print_esc("endcsname");
  help_csname();
  error();
}

  void
_boxlist_init ()
{
}

  void
_boxlist_init_once ()
{
}

/*
 **	Help text
 */

help_head_for_vmode ()
{
  help2("To put a horizontal rule in an hbox or an alignment,",
      "you should use \\leaders or \\hrulefill (see The TeXbook).");
}

help_close_group ()
{
  help2("You've closed more groups than you opened.",
      "Such booboos are generally harmless, so keep going.");
}

help_output_balance ()
{
  help2("Your sneaky output routine has problematic {'s and/or }'s.",
      "I can't handle that very well; good luck.");
}

help_output ()
{
  help3("Your \\output commands should empty \\box255,",
      "e.g., by saying `\\shipout\\box255'.",
      "Proceed; I'll discard its present contents.");
}

help_group_close ()
{
  help5("I've deleted a group-closing symbol because it seems to be",
      "spurious, as in `$x}$'. But perhaps the } is legitimate and",
      "you forgot something else, as in `\\hbox{$x}'. In such cases",
      "the way to recover is to insert both the forgotten and the",
      "deleted material, e.g., by typing `I$}'.");
}

help_leaders ()
{
  help3("You should say `\\leaders <box or rule><hskip or vskip>'.",
      "I found the <box or rule>, but there's no suitable",
      "<hskip or vskip>, so I'm ignoring these leaders.");
}

help_lastbox_m ()
{
  help1("Sorry; this \\lastbox will be void.");
}

help_lastbox_v ()
{
  help2("Sorry...I usually can't take things from the current page.",
      "This \\lastbox will therefore be void.");
}

help_vsplit ()
{
  help2("I'm working on `\\vsplit<box number> to <dimen>';",
      "will look for the <dimen> next.");
}

help_box ()
{
  help3("I was expecting to see \\hbox or \\vbox or \\copy or \\box or",
      "something like that. So you might find something missing in",
      "your output. But keep trying; you can fix this later.");
}

help_insert_255 ()
{
  help1("I'm changing to \\insert0; box 255 is special.");
}

help_space_fac ()
{
  help1("I allow only values in the range 1..65536 here.");
}

help_delete_last_pen ()
{
  help2("Sorry...I usually can't take things from the current page.",
      "Perhaps you can make the output routine do it.");
}

help_delete_last_skip ()
{
  help2("Sorry...I usually can't take things from the current page.",
      "Try `I\\vskip-\\lastskip' instead.");
}

help_delete_last_kern ()
{
  help2("Sorry...I usually can't take things from the current page.",
      "Try `I\\kern-\\lastkern' instead.");
}

help_pandora ()
{
  help3("Sorry, Pandora. (You sneaky devil.)",
      "I refuse to unbox an \\hbox in vertical mode or vice versa.",
      "And I can't open any boxes in math mode.");
}

help_math_disc ()
{
  help2("Sorry: The third part of a discretionary break must be",
      "empty, in math formulas. I had to delete your third part.");
}

help_discretionary ()
{
  help1("Discretionary lists must contain only boxes and kerns.");
}

help_disc ()
{
  help2("Wow---I never thought anybody would tweak me here.",
      "You can't seriously need such a huge discretionary list?");
}

help_tab ()
{
  help6("I can't figure out why you would want to use a tab mark",
      "here. If you just want an ampersand, the remedy is",
      "simple: Just type `I\\&' now. But if some right brace",
      "up above has ended a previous alignment prematurely,",
      "you're probably due for more error messages, and you",
      "might try typing `S' now just to see what is salvageable.");
}

help_align_error ()
{
  help5("I can't figure out why you would want to use a tab mark",
      "or \\cr or \\span just now. If something like a right brace",
      "up above has ended a previous alignment prematurely,",
      "you're probably due for more error messages, and you",
      "might try typing `S' now just to see what is salvageable.");
}

help_align_cr ()
{
  help1("I'm guessing that you meant to end an alignment here.");
}

help_fix_alignment ()
{
  help3("I've put in what seems to be necessary to fix",
      "the current column of the current alignment.",
      "Try to go on, since this might almost work.");
}

help_noalign ()
{
  help2("I expect to see \\noalign only after the \\cr of",
      "an alignment. Proceed, and I'll ignore this case.");
}

help_omit ()
{
  help2("I expect to see \\omit only after tab marks or the \\cr of",
      "an alignment. Proceed, and I'll ignore this case.");
}

help_csname ()
{
  help1("I'm ignoring this, since I wasn't doing a \\csname.");
}

/*
 * Common TeX:chr.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX chr.c 3.2@(#)";
#endif

#include "tex.h"

byte	xord[256];
byte	xchr[256];

  void
_chr_init ()
{
  int	i;

  for (i = 0; i <= 0377; incr(i))
    xchr[i] = i;
  for (i = 0; i <= 0377; incr(i))
    xord[xchr[i]] = i;
}

  void
_chr_init_once ()
{
}

/*
 * Common TeX:cmd.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX cmd.c 3.2@(#)";
#endif

#include "tex.h"

  void
show_cur_cmd_chr()
{
  begin_diagnostic();
  print_nl("{");
  if (mode != shown_mode) {
    print_mode(mode);
    print(": ");
    shown_mode = mode;
  }
  print_cmd_chr(cur_cmd, cur_chr);
  print("}");
  end_diagnostic(FALSE);
}

  void
print_cmd_chr (cmd, chr)
  int	cmd;
  int	chr;
{

#define chr_cmd(S) {print(S); print_ASCII(chr);}

  switch (cmd)
  {
    case RELAX:
      print_esc("relax");
      break;

    case LEFT_BRACE:
      chr_cmd("begin-group character ");
      break;

    case RIGHT_BRACE:
      chr_cmd("end-group character ");
      break;

    case MATH_SHIFT:
      chr_cmd("math shift character ");
      break;

    case TAB_MARK:
      if (chr == SPAN_CODE)
        print_esc("span");
      else chr_cmd("alignment tab character ");
      break;

    case CAR_RET:
      if (chr == CR_CODE)
        print_esc("cr");
      else print_esc("crcr");
      break;

    case MAC_PARAM:
      chr_cmd("macro parameter character ");
      break;

    case SUP_MARK:
      chr_cmd("superscript character ");
      break;

    case SUB_MARK:
      chr_cmd("subscript character ");
      break;

    case ENDV:
      print("end of alignment template");
      break;

    case SPACER:
      chr_cmd("blank space ");
      break;

    case LETTER:
      chr_cmd("the letter ");
      break;

    case OTHER_CHAR:
      chr_cmd("the character ");
      break;

    case ASSIGN_GLUE:
      if (chr < GLUE_PARS) {
        print_skip_param(chr);
      } else {
        print_esc("skip");
        print_int(chr - GLUE_PARS);
      }
      break;

    case ASSIGN_MU_GLUE:
      if (chr < MU_GLUE_PARS) {
        print_mu_skip_param(chr);
      } else {
        print_esc("muskip");
        print_int(chr - MU_GLUE_PARS);
      }
      break;

    case ASSIGN_TOKS:
      if (chr < TOK_PARS) {
        print_toks_param(chr);
      } else {
        print_esc("toks");
        print_int(chr - TOK_PARS);
      }
      break;

    case ASSIGN_INT:
      if (chr < INT_PARS) {
        print_param(chr);
      } else {
        print_esc("count");
        print_int(chr - INT_PARS);
      } 
      break;

    case ASSIGN_DIMEN:
      if (chr < DIMEN_PARS) {
        print_length_param(chr);
      } else {
        print_esc("dimen");
        print_int(chr - DIMEN_PARS);
      } 
      break;

    case ACCENT:
      print_esc("accent");
      break;

    case ADVANCE:
      print_esc("advance");
      break;

    case AFTER_ASSIGNMENT:
      print_esc("afterassignment");
      break;

    case AFTER_GROUP:
      print_esc("aftergroup");
      break;

    case ASSIGN_FONT_DIMEN:
      print_esc("fontdimen");
      break;

    case BEGIN_GROUP:
      print_esc("begingroup");
      break;

    case BREAK_PENALTY:
      print_esc("penalty");
      break;

    case CHAR_NUM:
      print_esc("char");
      break;

    case CS_NAME:
      print_esc("csname");
      break;

    case DEF_FONT:
      print_esc("font");
      break;

    case DELIM_NUM:
      print_esc("delimiter");
      break;

    case DIVIDE:
      print_esc("divide");
      break;

    case END_CS_NAME:
      print_esc("endcsname");
      break;

    case END_GROUP:
      print_esc("endgroup");
      break;

    case EX_SPACE:
      print_esc(" ");
      break;

    case EXPAND_AFTER:
      print_esc("expandafter");
      break;

    case INPUT:
      if (chr == 0)
        print_esc("input");
      else print_esc("endinput");
      break;

    case HALIGN:
      print_esc("halign");
      break;

    case HRULE:
      print_esc("hrule");
      break;

    case IGNORE_SPACES:
      print_esc("ignorespaces");
      break;

    case INSERT:
      print_esc("insert");
      break;

    case ITAL_CORR:
      print_esc("/");
      break;

    case MARK:
      print_esc("mark");
      break;

    case TOP_BOT_MARK:
      switch (chr) {
        case TOP_MARK_CODE: print_esc("topmark");break; 
        case FIRST_MARK_CODE: print_esc("firstmark");break; 
        case BOT_MARK_CODE: print_esc("botmark");break; 
        case SPLIT_FIRST_MARK_CODE: print_esc("splitfirstmark");break; 
        case SPLIT_BOT_MARK_CODE: print_esc("splitbotmark");break;
      }
      break;

    case MATH_ACCENT:
      print_esc("mathaccent");
      break;

    case MATH_CHAR_NUM:
      print_esc("mathchar");
      break;

    case MATH_CHOICE:
      print_esc("mathchoice");
      break;

    case MULTIPLY:
      print_esc("multiply");
      break;

    case NO_ALIGN:
      print_esc("noalign");
      break;

    case NO_BOUNDARY:
      print_esc("noboundary");
      break;

    case NO_EXPAND:
      print_esc("noexpand");
      break;

    case NON_SCRIPT:
      print_esc("nonscript");
      break;

    case OMIT:
      print_esc("omit");
      break;

    case RADICAL:
      print_esc("radical");
      break;

    case READ_TO_CS:
      print_esc("read");
      break;

    case SET_BOX:
      print_esc("setbox");
      break;

    case SET_PREV_GRAF:
      print_esc("prevgraf");
      break;

    case SET_SHAPE:
      print_esc("parshape");
      break;

    case THE:
      print_esc("the");
      break;

    case TOKS_REGISTER:
      print_esc("toks");
      break;

    case VADJUST:
      print_esc("vadjust");
      break;

    case VALIGN:
      print_esc("valign");
      break;

    case VCENTER:
      print_esc("vcenter");
      break;

    case VRULE:
      print_esc("vrule");
      break;

    case PAR_END:
      print_esc("par");
      break;

    case SET_AUX:
      if (chr == VMODE)
        print_esc("prevdepth");
      else print_esc("spacefactor");
      break;

    case SET_PAGE_INT:
      if (chr == 0)
        print_esc("deadcycles");
      else print_esc("insertpenalties");
      break;

    case SET_BOX_DIMEN:
      if (chr == WD_CODE)
        print_esc("wd");
      else if (chr == HT_CODE)
        print_esc("ht");
      else print_esc("dp");
      break;

    case SET_PAGE_DIMEN:
      switch (chr) {
        case 0: print_esc("pagegoal");break; 
        case 1: print_esc("pagetotal");break; 
        case 2: print_esc("pagestretch");break; 
        case 3: print_esc("pagefilstretch");break; 
        case 4: print_esc("pagefillstretch");break; 
        case 5: print_esc("pagefilllstretch");break; 
        case 6: print_esc("pageshrink");break;
      }
      break;

    case LAST_ITEM:
      switch (chr) {
        case INT_VAL: print_esc("lastpenalty");break;
        case DIMEN_VAL: print_esc("lastkern");break;
        case GLUE_VAL: print_esc("lastskip");break;
        case INPUT_LINE_NO_CODE: print_esc("inputlineno");break;
        default: print_esc("badness");break;
      }
      break;

    case REGISTER:
      if (chr == INT_VAL)
        print_esc("count");
      else if (chr == DIMEN_VAL)
        print_esc("dimen");
      else if (chr == GLUE_VAL)
        print_esc("skip");
      else print_esc("muskip");
      break;

    case CONVERT:
      switch (chr) {
        case NUMBER_CODE: print_esc("number");break; 
        case ROMAN_NUMERAL_CODE: print_esc("romannumeral");break; 
        case STRING_CODE: print_esc("string");break; 
        case MEANING_CODE: print_esc("meaning");break; 
        case FONT_NAME_CODE: print_esc("fontname");break; 
        default: print_esc("jobname");break;
      }
      break;

    case IF_TEST:
      switch (chr) {
        case IF_CHAR_CODE: print_esc("if");break; 
        case IF_CAT_CODE: print_esc("ifcat");break; 
        case IF_INT_CODE: print_esc("ifnum");break; 
        case IF_DIM_CODE: print_esc("ifdim");break; 
        case IF_ODD_CODE: print_esc("ifodd");break; 
        case IF_VMODE_CODE: print_esc("ifvmode");break; 
        case IF_HMODE_CODE: print_esc("ifhmode");break; 
        case IF_MMODE_CODE: print_esc("ifmmode");break; 
        case IF_INNER_CODE: print_esc("ifinner");break; 
        case IF_VOID_CODE: print_esc("ifvoid");break; 
        case IF_HBOX_CODE: print_esc("ifhbox");break; 
        case IF_VBOX_CODE: print_esc("ifvbox");break; 
        case IFX_CODE: print_esc("ifx");break; 
        case IF_EOF_CODE: print_esc("ifeof");break; 
        case IF_TRUE_CODE: print_esc("iftrue");break; 
        case IF_FALSE_CODE: print_esc("iffalse");break; 
        case IF_CASE_CODE: print_esc("ifcase");break;
      }
      break;

    case FI_OR_ELSE:
      if (chr == FI_CODE)
        print_esc("fi");
      else if (chr == OR_CODE)
        print_esc("or");
      else print_esc("else");
      break;

    case PREFIX:
      if (chr == 1)
        print_esc("long");
      else if (chr == 2)
        print_esc("outer");
      else print_esc("global");
      break;

    case DEF:
      if (chr == 0)
        print_esc("def");
      else if (chr == 1)
        print_esc("gdef");
      else if (chr == 2)
        print_esc("edef");
      else print_esc("xdef");
      break;

    case LET:
      if (chr != NORMAL)
        print_esc("futurelet");
      else print_esc("let");
      break;

    case SHORTHAND_DEF:
      switch (chr) {
        case CHAR_DEF_CODE: print_esc("chardef");break; 
        case MATH_CHAR_DEF_CODE: print_esc("mathchardef");break; 
        case COUNT_DEF_CODE: print_esc("countdef");break; 
        case DIMEN_DEF_CODE: print_esc("dimendef");break; 
        case SKIP_DEF_CODE: print_esc("skipdef");break; 
        case MU_SKIP_DEF_CODE: print_esc("muskipdef");break; 
        default: print_esc("toksdef");break;
      }
      break;

    case CHAR_GIVEN:
      print_esc("char");
      print_hex(chr);
      break;

    case MATH_GIVEN:
      print_esc("mathchar");
      print_hex(chr);
      break;

    case DEF_CODE:
      if (chr == CAT_CODE)
        print_esc("catcode");
      else if (chr == MATH_CODE)
        print_esc("mathcode");
      else if (chr == LC_CODE)
        print_esc("lccode");
      else if (chr == UC_CODE)
        print_esc("uccode");
      else if (chr == SF_CODE)
        print_esc("sfcode");
      else print_esc("delcode");
      break;

    case DEF_FAMILY:
      print_size(chr);
      break;

    case SET_FONT:
      print("select font ");
      print(font_name(chr));
      if (font_size(chr) != font_dsize(chr)) {
        print(" at ");
        print_scaled(font_size(chr));
        print("pt");
      } 
      break;

    case ASSIGN_FONT_INT:
      if (chr == 1)
        print_esc("skewchar");
      else print_esc("hyphenchar");
      break;

    case HYPH_DATA:
      if (chr == 1)
        print_esc("patterns");
      else print_esc("hyphenation");
      break;

    case SET_INTERACTION:
      switch (chr) {
        case BATCH_MODE: print_esc("batchmode");break; 
        case NONSTOP_MODE: print_esc("nonstop");break; 
        case SCROLL_MODE: print_esc("scrollmode");break; 
        default: print_esc("errorstopmode");break;
      }
      break;

    case IN_STREAM:
      if (chr == 0)
        print_esc("closein");
      else print_esc("openin");
      break;

    case MESSAGE:
      if (chr == 0)
        print_esc("message");
      else print_esc("errmessage");
      break;

    case CASE_SHIFT:
      if (chr == LC_CODE)	
        print_esc("lowercase");
      else print_esc("uppercase");
      break;

    case XRAY:
      switch (chr) {
        case SHOW_BOX_CODE: print_esc("showbox");break; 
        case SHOW_THE_CODE: print_esc("showthe");break; 
        case SHOW_LISTS: print_esc("showlists");break; 
        default: print_esc("show");break;
      }
      break;

    case UNDEFINED_CS:
      print("undefined");
      break;

    case CALL:
      print("macro");
      break;

    case LONG_CALL:
      print_esc("long macro");
      break;

    case OUTER_CALL:
      print_esc("outer macro");
      break;

    case LONG_OUTER_CALL:
      print_esc("long");
      print_esc("outer macro");
      break;

    case END_TEMPLATE:
      print_esc("outer endtemplate");
      break;

    case STOP:
      if (chr == 1)
        print_esc("dump");
      else print_esc("end");
      break;

    case HSKIP:
      switch (chr) {
        case SKIP_CODE: print_esc("hskip");break; 
        case FIL_CODE: print_esc("hfil");break; 
        case FILL_CODE: print_esc("hfill");break; 
        case SS_CODE: print_esc("hss");break; 
        default: print_esc("hfilneg");break;
      }
      break;

    case VSKIP:
      switch (chr) {
        case SKIP_CODE: print_esc("vskip");break; 
        case FIL_CODE: print_esc("vfil");break; 
        case FILL_CODE: print_esc("vfill");break; 
        case SS_CODE: print_esc("vss");break; 
        default: print_esc("vfilneg");break;
      }
      break;

    case MSKIP:
      print_esc("mskip");
      break;

    case KERN:
      print_esc("kern");
      break;

    case MKERN:
      print_esc("mkern");
      break;

    case HMOVE:
      if (chr == 1)
        print_esc("moveleft");
      else print_esc("moveright");
      break;

    case VMOVE:
      if (chr == 1)
        print_esc("raise");
      else print_esc("lower");
      break;

    case MAKE_BOX:
      switch (chr) {
        case BOX_CODE: print_esc("box");break; 
        case COPY_CODE: print_esc("copy");break; 
        case LAST_BOX_CODE: print_esc("lastbox");break; 
        case VSPLIT_CODE: print_esc("vsplit");break; 
        case VTOP_CODE: print_esc("vtop");break; 
        case VTOP_CODE + VMODE: print_esc("vbox");break; 
        default: print_esc("hbox");break;
      } 
      break;

    case LEADER_SHIP:
      if (chr == A_LEADERS)
        print_esc("leaders");
      else if (chr == C_LEADERS)
        print_esc("cleaders");
      else if (chr == X_LEADERS)
        print_esc("xleaders");
      else print_esc("shipout");
      break;

    case START_PAR:
      if (chr == 0)
        print_esc("noindent");
      else print_esc("indent");
      break;

    case REMOVE_ITEM:
      if (chr == GLUE_NODE)
        print_esc("unskip");
      else if (chr == KERN_NODE)
        print_esc("unkern");
      else print_esc("unpenalty");
      break;

    case UN_HBOX:
      if (chr == COPY_CODE)
        print_esc("unhcopy");
      else print_esc("unhbox");
      break;

    case UN_VBOX:
      if (chr == COPY_CODE)
        print_esc("unvcopy");
      else print_esc("unvbox");
      break;

    case DISCRETIONARY:
      if (chr == 1)
        print_esc("-"); 
      else print_esc("discretionary");
      break;

    case EQ_NO:
      if (chr == 1)
        print_esc("leqno");
      else print_esc("eqno");
      break;

    case MATH_COMP:
      switch (chr) {
        case ORD_NOAD: print_esc("mathord");break; 
        case OP_NOAD: print_esc("mathop");break; 
        case BIN_NOAD: print_esc("mathbin");break; 
        case REL_NOAD: print_esc("mathrel");break; 
        case OPEN_NOAD: print_esc("mathopen");break; 
        case CLOSE_NOAD: print_esc("mathclose");break; 
        case PUNCT_NOAD: print_esc("mathpunct");break; 
        case INNER_NOAD: print_esc("mathinner");break; 
        case UNDER_NOAD: print_esc("underline");break; 
        default: print_esc("overline");break;
      }
      break;

    case LIMIT_SWITCH:
      if (chr == LIMITS)
        print_esc("limits");
      else if (chr == NO_LIMITS)
        print_esc("nolimits");
      else print_esc("displaylimits");
      break;

    case MATH_STYLE:
      print_style(chr);
      break;

    case ABOVE:
      switch (chr) {
        case OVER_CODE:
          print_esc("over");
          break;

        case ATOP_CODE:
          print_esc("atop");
          break;

        case DELIMITED_CODE + ABOVE_CODE:
          print_esc("abovewithdelims");
          break;

        case DELIMITED_CODE + OVER_CODE:
          print_esc("overwithdelims");
          break;

        case DELIMITED_CODE + ATOP_CODE:
          print_esc("atopwithdelims");
          break;

        default:
          print_esc("above");
          break;
      }
      break;

    case LEFT_RIGHT:
      if (chr == LEFT_NOAD)
        print_esc("left");
      else print_esc("right");
      break;

    case EXTENSION:
      switch (chr) {
        case OPEN_NODE: print_esc("openout");break; 
        case WRITE_NODE: print_esc("write");break; 
        case CLOSE_NODE: print_esc("closeout");break; 
        case SPECIAL_NODE: print_esc("special");break; 
        case IMMEDIATE_CODE: print_esc("immediate");break;
        case SET_LANGUAGE_CODE: print_esc("setlanguage");break;
      }
      break;
  }
}

  void
print_mu_skip_param (n)
  int     n;
{
  switch (n)
  {
    case THIN_MU_SKIP_CODE:
      print_esc("thinmuskip");
      break;

    case MED_MU_SKIP_CODE:
      print_esc("medmuskip");
      break;

    case THICK_MU_SKIP_CODE:
      print_esc("thickmuskip");
      break;

    default:
      print("[unknown muglue parameter!]");
      break;
  }
}

  void
print_skip_param (n)
  int     n;
{
  switch (n)
  {
    case LINE_SKIP_CODE:
      print_esc("lineskip");
      break;

    case BASELINE_SKIP_CODE:
      print_esc("baselineskip");
      break;

    case PAR_SKIP_CODE:
      print_esc("parskip");
      break;

    case ABOVE_DISPLAY_SKIP_CODE:
      print_esc("abovedisplayskip");
      break;

    case BELOW_DISPLAY_SKIP_CODE:
      print_esc("belowdisplayskip");
      break;

    case ABOVE_DISPLAY_SHORT_SKIP_CODE:
      print_esc("abovedisplayshortskip");
      break;

    case BELOW_DISPLAY_SHORT_SKIP_CODE:
      print_esc("belowdisplayshortskip");
      break;

    case LEFT_SKIP_CODE:
      print_esc("leftskip");
      break;

    case RIGHT_SKIP_CODE:
      print_esc("rightskip");
      break;

    case TOP_SKIP_CODE:
      print_esc("topskip");
      break;

    case SPLIT_TOP_SKIP_CODE:
      print_esc("splittopskip");
      break;

    case TAB_SKIP_CODE:
      print_esc("tabskip");
      break;

    case SPACE_SKIP_CODE:
      print_esc("spaceskip");
      break;

    case XSPACE_SKIP_CODE:
      print_esc("xspaceskip");
      break;

    case PAR_FILL_SKIP_CODE:
      print_esc("parfillskip");
      break;

    default:
      print("[unknown glue parameter!]");
      break;
  }
}

  void
print_length_param (n)
  int     n;
{
  switch (n)
  {
    case PAR_INDENT_CODE:
      print_esc("parindent");
      break;

    case MATH_SURROUND_CODE:
      print_esc("mathsurround");
      break;

    case LINE_SKIP_LIMIT_CODE:
      print_esc("lineskiplimit");
      break;

    case HSIZE_CODE:
      print_esc("hsize");
      break;

    case VSIZE_CODE:
      print_esc("vsize");
      break;

    case MAX_DEPTH_CODE:
      print_esc("maxdepth");
      break;

    case SPLIT_MAX_DEPTH_CODE:
      print_esc("splitmaxdepth");
      break;

    case BOX_MAX_DEPTH_CODE:
      print_esc("boxmaxdepth");
      break;

    case HFUZZ_CODE:
      print_esc("hfuzz");
      break;

    case VFUZZ_CODE:
      print_esc("vfuzz");
      break;

    case DELIMITER_SHORTFALL_CODE:
      print_esc("delimitershortfall");
      break;

    case NULL_DELIMITER_SPACE_CODE:
      print_esc("nulldelimiterspace");
      break;

    case SCRIPT_SPACE_CODE:
      print_esc("scriptspace");
      break;

    case PRE_DISPLAY_SIZE_CODE:
      print_esc("predisplaysize");
      break;

    case DISPLAY_INDENT_CODE:
      print_esc("displayindent");
      break;

    case DISPLAY_WIDTH_CODE:
      print_esc("displaywidth");
      break;

    case OVERFULL_RULE_CODE:
      print_esc("overfullrule");
      break;

    case HANG_INDENT_CODE:
      print_esc("hangindent");
      break;

    case H_OFFSET_CODE:
      print_esc("hoffset");
      break;

    case V_OFFSET_CODE:
      print_esc("voffset");
      break;

    case EMERGENCY_STRETCH_CODE:
      print_esc("emergency_stretch");
      break;

    default:
      print("[unknown dimen parameter!]");
      break;
  }
}

  void
print_param (n)
  int     n;
{
  switch (n)
  {
    case PRETOLERANCE_CODE:
      print_esc("pretolerance");
      break;

    case TOLERANCE_CODE:
      print_esc("tolerance");
      break;

    case LINE_PENALTY_CODE:
      print_esc("linepenalty");
      break;

    case HYPHEN_PENALTY_CODE:
      print_esc("hyphenpenalty");
      break;

    case EX_HYPHEN_PENALTY_CODE:
      print_esc("exhyphenpenalty");
      break;

    case CLUB_PENALTY_CODE:
      print_esc("clubpenalty");
      break;

    case WIDOW_PENALTY_CODE:
      print_esc("widowpenalty");
      break;

    case DISPLAY_WIDOW_PENALTY_CODE:
      print_esc("displaywidowpenalty");
      break;

    case BROKEN_PENALTY_CODE:
      print_esc("brokenpenalty");
      break;

    case BIN_OP_PENALTY_CODE:
      print_esc("binoppenalty");
      break;

    case REL_PENALTY_CODE:
      print_esc("relpenalty");
      break;

    case PRE_DISPLAY_PENALTY_CODE:
      print_esc("predisplaypenalty");
      break;

    case POST_DISPLAY_PENALTY_CODE:
      print_esc("postdisplaypenalty");
      break;

    case INTER_LINE_PENALTY_CODE:
      print_esc("interlinepenalty");
      break;

    case DOUBLE_HYPHEN_DEMERITS_CODE:
      print_esc("doublehyphendemerits");
      break;

    case FINAL_HYPHEN_DEMERITS_CODE:
      print_esc("finalhyphendemerits");
      break;

    case ADJ_DEMERITS_CODE:
      print_esc("adjdemerits");
      break;

    case MAG_CODE:
      print_esc("mag");
      break;

    case DELIMITER_FACTOR_CODE:
      print_esc("delimiterfactor");
      break;

    case LOOSENESS_CODE:
      print_esc("looseness");
      break;

    case TIME_CODE:
      print_esc("time");
      break;

    case DAY_CODE:
      print_esc("day");
      break;

    case MONTH_CODE:
      print_esc("month");
      break;

    case YEAR_CODE:
      print_esc("year");
      break;

    case SHOW_BOX_BREADTH_CODE:
      print_esc("showboxbreadth");
      break;

    case SHOW_BOX_DEPTH_CODE:
      print_esc("showboxdepth");
      break;

    case HBADNESS_CODE:
      print_esc("hbadness");
      break;

    case VBADNESS_CODE:
      print_esc("vbadness");
      break;

    case PAUSING_CODE:
      print_esc("pause");
      break;

    case TRACING_ONLINE_CODE:
      print_esc("tracingonline");
      break;

    case TRACING_MACROS_CODE:
      print_esc("tracingmacros");
      break;

    case TRACING_STATS_CODE:
      print_esc("tracingstats");
      break;

    case TRACING_PARAGRAPHS_CODE:
      print_esc("tracingparagraphs");
      break;

    case TRACING_PAGES_CODE:
      print_esc("tracingpages");
      break;

    case TRACING_OUTPUT_CODE:
      print_esc("tracingoutput");
      break;

    case TRACING_LOST_CHARS_CODE:
      print_esc("tracinglostchars");
      break;

    case TRACING_COMMANDS_CODE:
      print_esc("tracingcommands");
      break;

    case TRACING_RESTORES_CODE:
      print_esc("tracingrestores");
      break;

    case UC_HYPH_CODE:
      print_esc("uchyph");
      break;

    case OUTPUT_PENALTY_CODE:
      print_esc("outputpenalty");
      break;

    case MAX_DEAD_CYCLES_CODE:
      print_esc("maxdeadcycles");
      break;

    case HANG_AFTER_CODE:
      print_esc("hangafter");
      break;

    case FLOATING_PENALTY_CODE:
      print_esc("floatingpenalty");
      break;

    case GLOBAL_DEFS_CODE:
      print_esc("globaldefs");
      break;

    case CUR_FAM_CODE:
      print_esc("fam");
      break;

    case ESCAPE_CHAR_CODE:
      print_esc("escapechar");
      break;

    case DEFAULT_HYPHEN_CHAR_CODE:
      print_esc("defaulthyphenchar");
      break;

    case DEFAULT_SKEW_CHAR_CODE:
      print_esc("defaultskewchar");
      break;

    case END_LINE_CHAR_CODE:
      print_esc("endlinechar");
      break;

    case NEW_LINE_CHAR_CODE:
      print_esc("newlinechar");
      break;

    case LANGUAGE_CODE:
      print_esc("language");
      break;

    case LEFT_HYPHEN_MIN_CODE:
      print_esc("lefthyphenmin");
      break;

    case RIGHT_HYPHEN_MIN_CODE:
      print_esc("righthyphenmin");
      break;

    case HOLDING_INSERTS_CODE:
      print_esc("holdinginserts");
      break;

    case ERROR_CONTEXT_LINES_CODE:
      print_esc("errorcontextlines");
      break;

    default:
      print("[unknown integer parameter!]");
      break;
  }
}

  void
print_toks_param (n)
  int	n;
{
  switch (n)
  {
    case OUTPUT_ROUTINE_CODE:
      print_esc("output");
      break;

    case EVERY_PAR_CODE:
      print_esc("everypar");
      break;

    case EVERY_MATH_CODE:
      print_esc("everymath");
      break;

    case EVERY_DISPLAY_CODE:
      print_esc("everydisplay");
      break;

    case EVERY_HBOX_CODE:
      print_esc("everyhbox");
      break;

    case EVERY_VBOX_CODE:
      print_esc("everyvbox");
      break;

    case EVERY_JOB_CODE:
      print_esc("everyjob");
      break;

    case EVERY_CR_CODE:
      print_esc("everycr");
      break;

    case ERR_HELP_CODE:
      print_esc("errhelp");
      break;

    default:
      print("[unknown toks parameter!]");
      break;
  }
}

  void
_cmd_init ()
{
  no_new_control_sequence = TRUE;
}

  void
_cmd_init_once ()
{
  no_new_control_sequence = FALSE;
  primitive("thinmuskip", ASSIGN_MU_GLUE, THIN_MU_SKIP_CODE);
  primitive("medmuskip", ASSIGN_MU_GLUE, MED_MU_SKIP_CODE);
  primitive("thickmuskip", ASSIGN_MU_GLUE, THICK_MU_SKIP_CODE);
  primitive("lineskip", ASSIGN_GLUE, LINE_SKIP_CODE);
  primitive("baselineskip", ASSIGN_GLUE, BASELINE_SKIP_CODE);
  primitive("parskip", ASSIGN_GLUE, PAR_SKIP_CODE);
  primitive("abovedisplayskip", ASSIGN_GLUE, ABOVE_DISPLAY_SKIP_CODE);
  primitive("abovedisplayshortskip", ASSIGN_GLUE, ABOVE_DISPLAY_SHORT_SKIP_CODE);
  primitive("belowdisplayskip", ASSIGN_GLUE, BELOW_DISPLAY_SKIP_CODE);
  primitive("belowdisplayshortskip", ASSIGN_GLUE, BELOW_DISPLAY_SHORT_SKIP_CODE);
  primitive("parfillskip", ASSIGN_GLUE, PAR_FILL_SKIP_CODE);
  primitive("leftskip", ASSIGN_GLUE, LEFT_SKIP_CODE);
  primitive("rightskip", ASSIGN_GLUE, RIGHT_SKIP_CODE);
  primitive("topskip", ASSIGN_GLUE, TOP_SKIP_CODE);
  primitive("splittopskip", ASSIGN_GLUE, SPLIT_TOP_SKIP_CODE);
  primitive("tabskip", ASSIGN_GLUE, TAB_SKIP_CODE);
  primitive("spaceskip", ASSIGN_GLUE, SPACE_SKIP_CODE);
  primitive("xspaceskip", ASSIGN_GLUE, XSPACE_SKIP_CODE);
  primitive("output", ASSIGN_TOKS, OUTPUT_ROUTINE_CODE);
  primitive("everycr", ASSIGN_TOKS, EVERY_CR_CODE);
  primitive("everypar", ASSIGN_TOKS, EVERY_PAR_CODE);
  primitive("everymath", ASSIGN_TOKS, EVERY_MATH_CODE);
  primitive("everydisplay", ASSIGN_TOKS, EVERY_DISPLAY_CODE);
  primitive("everyhbox", ASSIGN_TOKS, EVERY_HBOX_CODE);
  primitive("everyvbox", ASSIGN_TOKS, EVERY_VBOX_CODE);
  primitive("everyjob", ASSIGN_TOKS, EVERY_JOB_CODE);
  primitive("errhelp", ASSIGN_TOKS, ERR_HELP_CODE);
  primitive("pretolerance", ASSIGN_INT, PRETOLERANCE_CODE);
  primitive("tolerance", ASSIGN_INT, TOLERANCE_CODE);
  primitive("linepenalty", ASSIGN_INT, LINE_PENALTY_CODE);
  primitive("hyphenpenalty", ASSIGN_INT, HYPHEN_PENALTY_CODE);
  primitive("exhyphenpenalty", ASSIGN_INT, EX_HYPHEN_PENALTY_CODE);
  primitive("clubpenalty", ASSIGN_INT, CLUB_PENALTY_CODE);
  primitive("widowpenalty", ASSIGN_INT, WIDOW_PENALTY_CODE);
  primitive("displaywidowpenalty", ASSIGN_INT, DISPLAY_WIDOW_PENALTY_CODE);
  primitive("brokenpenalty", ASSIGN_INT, BROKEN_PENALTY_CODE);
  primitive("binoppenalty", ASSIGN_INT, BIN_OP_PENALTY_CODE);
  primitive("relpenalty", ASSIGN_INT, REL_PENALTY_CODE);
  primitive("predisplaypenalty", ASSIGN_INT, PRE_DISPLAY_PENALTY_CODE);
  primitive("postdisplaypenalty", ASSIGN_INT, POST_DISPLAY_PENALTY_CODE);
  primitive("interlinepenalty", ASSIGN_INT, INTER_LINE_PENALTY_CODE);
  primitive("doublehyphendemerits", ASSIGN_INT, DOUBLE_HYPHEN_DEMERITS_CODE);
  primitive("finalhyphendemerits", ASSIGN_INT, FINAL_HYPHEN_DEMERITS_CODE);
  primitive("adjdemerits", ASSIGN_INT, ADJ_DEMERITS_CODE);
  primitive("mag", ASSIGN_INT, MAG_CODE);
  primitive("delimiterfactor", ASSIGN_INT, DELIMITER_FACTOR_CODE);
  primitive("looseness", ASSIGN_INT, LOOSENESS_CODE);
  primitive("time", ASSIGN_INT, TIME_CODE);
  primitive("day", ASSIGN_INT, DAY_CODE);
  primitive("month", ASSIGN_INT, MONTH_CODE);
  primitive("year", ASSIGN_INT, YEAR_CODE);
  primitive("showboxbreadth", ASSIGN_INT, SHOW_BOX_BREADTH_CODE);
  primitive("showboxdepth", ASSIGN_INT, SHOW_BOX_DEPTH_CODE);
  primitive("hbadness", ASSIGN_INT, HBADNESS_CODE);
  primitive("vbadness", ASSIGN_INT, VBADNESS_CODE);
  primitive("pausing", ASSIGN_INT, PAUSING_CODE);
  primitive("tracingonline", ASSIGN_INT, TRACING_ONLINE_CODE);
  primitive("tracingmacros", ASSIGN_INT, TRACING_MACROS_CODE);
  primitive("tracingstats", ASSIGN_INT, TRACING_STATS_CODE);
  primitive("tracingoutput", ASSIGN_INT, TRACING_OUTPUT_CODE);
  primitive("tracingparagraphs", ASSIGN_INT, TRACING_PARAGRAPHS_CODE);
  primitive("tracingpages", ASSIGN_INT, TRACING_PAGES_CODE);
  primitive("tracinglostchars", ASSIGN_INT, TRACING_LOST_CHARS_CODE);
  primitive("tracingcommands", ASSIGN_INT, TRACING_COMMANDS_CODE);
  primitive("tracingrestores", ASSIGN_INT, TRACING_RESTORES_CODE);
  primitive("uchyph", ASSIGN_INT, UC_HYPH_CODE);
  primitive("outputpenalty", ASSIGN_INT, OUTPUT_PENALTY_CODE);
  primitive("maxdeadcycles", ASSIGN_INT, MAX_DEAD_CYCLES_CODE);
  primitive("floatingpenalty", ASSIGN_INT, FLOATING_PENALTY_CODE);
  primitive("globaldefs", ASSIGN_INT, GLOBAL_DEFS_CODE);
  primitive("fam", ASSIGN_INT, CUR_FAM_CODE);
  primitive("escapechar", ASSIGN_INT, ESCAPE_CHAR_CODE);
  primitive("defaulthyphenchar", ASSIGN_INT, DEFAULT_HYPHEN_CHAR_CODE);
  primitive("defaultskewchar", ASSIGN_INT, DEFAULT_SKEW_CHAR_CODE);
  primitive("endlinechar", ASSIGN_INT, END_LINE_CHAR_CODE);
  primitive("newlinechar", ASSIGN_INT, NEW_LINE_CHAR_CODE);
  primitive("language", ASSIGN_INT, LANGUAGE_CODE);
  primitive("lefthyphenmin", ASSIGN_INT, LEFT_HYPHEN_MIN_CODE);
  primitive("righthyphenmin", ASSIGN_INT, RIGHT_HYPHEN_MIN_CODE);
  primitive("holdinginserts", ASSIGN_INT, HOLDING_INSERTS_CODE);
  primitive("errorcontextlines", ASSIGN_INT, ERROR_CONTEXT_LINES_CODE);
  primitive("parindent", ASSIGN_DIMEN, PAR_INDENT_CODE);
  primitive("mathsurround", ASSIGN_DIMEN, MATH_SURROUND_CODE);
  primitive("lineskiplimit", ASSIGN_DIMEN, LINE_SKIP_LIMIT_CODE);
  primitive("hsize", ASSIGN_DIMEN, HSIZE_CODE);
  primitive("vsize", ASSIGN_DIMEN, VSIZE_CODE);
  primitive("maxdepth", ASSIGN_DIMEN, MAX_DEPTH_CODE);
  primitive("splitmaxdepth", ASSIGN_DIMEN, SPLIT_MAX_DEPTH_CODE);
  primitive("boxmaxdepth", ASSIGN_DIMEN, BOX_MAX_DEPTH_CODE);
  primitive("hfuzz", ASSIGN_DIMEN, HFUZZ_CODE);
  primitive("vfuzz", ASSIGN_DIMEN, VFUZZ_CODE);
  primitive("delimitershortfall", ASSIGN_DIMEN, DELIMITER_SHORTFALL_CODE);
  primitive("nulldelimiterspace", ASSIGN_DIMEN, NULL_DELIMITER_SPACE_CODE);
  primitive("scriptspace", ASSIGN_DIMEN, SCRIPT_SPACE_CODE);
  primitive("predisplaysize", ASSIGN_DIMEN, PRE_DISPLAY_SIZE_CODE);
  primitive("displaywidth", ASSIGN_DIMEN, DISPLAY_WIDTH_CODE);
  primitive("displayindent", ASSIGN_DIMEN, DISPLAY_INDENT_CODE);
  primitive("overfullrule", ASSIGN_DIMEN, OVERFULL_RULE_CODE);
  primitive("hangafter", ASSIGN_INT, HANG_AFTER_CODE);
  primitive("hangindent", ASSIGN_DIMEN, HANG_INDENT_CODE);
  primitive("hoffset", ASSIGN_DIMEN, H_OFFSET_CODE);
  primitive("voffset", ASSIGN_DIMEN, V_OFFSET_CODE);
  primitive("emergencystretch", ASSIGN_DIMEN, EMERGENCY_STRETCH_CODE);
  primitive(" ", EX_SPACE, 0);
  primitive("/", ITAL_CORR, 0);
  primitive("accent", ACCENT, 0);
  primitive("advance", ADVANCE, 0);
  primitive("afterassignment", AFTER_ASSIGNMENT, 0);
  primitive("aftergroup", AFTER_GROUP, 0);
  primitive("begingroup", BEGIN_GROUP, 0);
  primitive("char", CHAR_NUM, 0);
  primitive("csname", CS_NAME, 0);
  primitive("font", DEF_FONT, 0);
  primitive("fontdimen", ASSIGN_FONT_DIMEN, 0);
  FROZEN_NULL_FONT=freeze(primitive("nullfont", SET_FONT, null_font));
  font_id_base[null_font] = FROZEN_NULL_FONT;
  primitive("delimiter", DELIM_NUM, 0);
  primitive("divide", DIVIDE, 0);
  primitive("endcsname", END_CS_NAME, 0);
  FROZEN_END_GROUP=freeze(primitive("endgroup", END_GROUP, 0));
  primitive("expandafter", EXPAND_AFTER, 0);
  primitive("halign", HALIGN, 0);
  primitive("hrule", HRULE, 0);
  primitive("ignorespaces", IGNORE_SPACES, 0);
  primitive("insert", INSERT, 0);
  primitive("mark", MARK, 0);
  primitive("topmark", TOP_BOT_MARK, TOP_MARK_CODE);
  primitive("firstmark", TOP_BOT_MARK, FIRST_MARK_CODE);
  primitive("botmark", TOP_BOT_MARK, BOT_MARK_CODE);
  primitive("splitfirstmark", TOP_BOT_MARK, SPLIT_FIRST_MARK_CODE);
  primitive("splitbotmark", TOP_BOT_MARK, SPLIT_BOT_MARK_CODE);
  primitive("mathaccent", MATH_ACCENT, 0);
  primitive("mathchar", MATH_CHAR_NUM, 0);
  primitive("mathchoice", MATH_CHOICE, 0);
  primitive("multiply", MULTIPLY, 0);
  primitive("noalign", NO_ALIGN, 0);
  primitive("noboundary", NO_BOUNDARY, 0);
  FROZEN_DONT_EXPAND=freeze(primitive("noexpand", NO_EXPAND, 0));
  eq_type(FROZEN_DONT_EXPAND) = DONT_EXPAND;
  text(FROZEN_DONT_EXPAND) = "notexpanded:";
  length(FROZEN_DONT_EXPAND) = 12;
  primitive("nonscript", NON_SCRIPT, 0);
  primitive("omit", OMIT, 0);
  par_shape_cs=primitive("parshape", SET_SHAPE, 0);
  primitive("penalty", BREAK_PENALTY, 0);
  primitive("prevgraf", SET_PREV_GRAF, 0);
  primitive("radical", RADICAL, 0);
  primitive("read", READ_TO_CS, 0);
  FROZEN_RELAX=freeze(primitive("relax", RELAX, 256));
  primitive("setbox", SET_BOX, 0);
  primitive("the", THE, 0);
  primitive("toks", TOKS_REGISTER, 0);
  primitive("vadjust", VADJUST, 0);
  primitive("valign", VALIGN, 0);
  primitive("vcenter", VCENTER, 0);
  primitive("vrule", VRULE, 0);
  par_cs=primitive("par", PAR_END, 0);
  par_tok=sym2tok(par_cs);
  primitive("count", REGISTER, INT_VAL);
  primitive("dimen", REGISTER, DIMEN_VAL);
  primitive("skip", REGISTER, GLUE_VAL);
  primitive("muskip", REGISTER, MU_VAL);
  primitive("spacefactor", SET_AUX, HMODE);
  primitive("prevdepth", SET_AUX, VMODE);
  primitive("deadcycles", SET_PAGE_INT, 0);
  primitive("insertpenalties", SET_PAGE_INT, 1);
  primitive("wd", SET_BOX_DIMEN, WD_CODE);
  primitive("ht", SET_BOX_DIMEN, HT_CODE);
  primitive("dp", SET_BOX_DIMEN, DP_CODE);
  primitive("pagegoal", SET_PAGE_DIMEN, 0);
  primitive("pagetotal", SET_PAGE_DIMEN, 1);
  primitive("pagestretch", SET_PAGE_DIMEN, 2);
  primitive("pagefilstretch", SET_PAGE_DIMEN, 3);
  primitive("pagefillstretch", SET_PAGE_DIMEN, 4);
  primitive("pagefilllstretch", SET_PAGE_DIMEN, 5);
  primitive("pageshrink", SET_PAGE_DIMEN, 6);
  primitive("pagedepth", SET_PAGE_DIMEN, 7);
  primitive("lastpenalty", LAST_ITEM, INT_VAL);
  primitive("lastkern", LAST_ITEM, DIMEN_VAL);
  primitive("lastskip", LAST_ITEM, GLUE_VAL);
  primitive("inputlineno", LAST_ITEM, INPUT_LINE_NO_CODE);
  primitive("badness", LAST_ITEM, BADNESS_CODE);
  primitive("input", INPUT, 0);
  primitive("endinput", INPUT, 1);
  primitive("number", CONVERT, NUMBER_CODE);
  primitive("romannumeral", CONVERT, ROMAN_NUMERAL_CODE);
  primitive("string", CONVERT, STRING_CODE);
  primitive("meaning", CONVERT, MEANING_CODE);
  primitive("fontname", CONVERT, FONT_NAME_CODE);
  primitive("jobname", CONVERT, JOB_NAME_CODE);
  primitive("if", IF_TEST, IF_CHAR_CODE);
  primitive("ifcat", IF_TEST, IF_CAT_CODE);
  primitive("ifnum", IF_TEST, IF_INT_CODE);
  primitive("ifdim", IF_TEST, IF_DIM_CODE);
  primitive("ifodd", IF_TEST, IF_ODD_CODE);
  primitive("ifvmode", IF_TEST, IF_VMODE_CODE);
  primitive("ifhmode", IF_TEST, IF_HMODE_CODE);
  primitive("ifmmode", IF_TEST, IF_MMODE_CODE);
  primitive("ifinner", IF_TEST, IF_INNER_CODE);
  primitive("ifvoid", IF_TEST, IF_VOID_CODE);
  primitive("ifhbox", IF_TEST, IF_HBOX_CODE);
  primitive("ifvbox", IF_TEST, IF_VBOX_CODE);
  primitive("ifx", IF_TEST, IFX_CODE);
  primitive("ifeof", IF_TEST, IF_EOF_CODE);
  primitive("iftrue", IF_TEST, IF_TRUE_CODE);
  primitive("iffalse", IF_TEST, IF_FALSE_CODE);
  primitive("ifcase", IF_TEST, IF_CASE_CODE);
  FROZEN_FI=freeze(primitive("fi", FI_OR_ELSE, FI_CODE));
  primitive("or", FI_OR_ELSE, OR_CODE);
  primitive("else", FI_OR_ELSE, ELSE_CODE);
  primitive("hskip", HSKIP, SKIP_CODE);
  primitive("hfil", HSKIP, FIL_CODE);
  primitive("hfill", HSKIP, FILL_CODE);
  primitive("hss", HSKIP, SS_CODE);
  primitive("hfilneg",  HSKIP, FIL_NEG_CODE);
  primitive("vskip", VSKIP, SKIP_CODE);
  primitive("vfil", VSKIP, FIL_CODE);
  primitive("vfill", VSKIP, FILL_CODE);
  primitive("vss", VSKIP, SS_CODE);
  primitive("vfilneg", VSKIP, FIL_NEG_CODE);
  primitive("mskip", MSKIP, MSKIP_CODE);
  primitive("kern", KERN, EXPLICIT);
  primitive("mkern", MKERN, MU_GLUE);
  primitive("moveleft", HMOVE, 1);
  primitive("moveright", HMOVE, 0);
  primitive("raise", VMOVE, 1);
  primitive("lower", VMOVE, 0);
  primitive("box", MAKE_BOX, BOX_CODE);
  primitive("copy", MAKE_BOX, COPY_CODE);
  primitive("lastbox", MAKE_BOX, LAST_BOX_CODE);
  primitive("vsplit", MAKE_BOX, VSPLIT_CODE);
  primitive("vtop", MAKE_BOX, VTOP_CODE);
  primitive("vbox", MAKE_BOX, VTOP_CODE + VMODE);
  primitive("hbox", MAKE_BOX, VTOP_CODE + HMODE);
  primitive("indent", START_PAR, 1);
  primitive("noindent", START_PAR, 0);
  primitive("shipout", LEADER_SHIP, A_LEADERS - 1);
  primitive("leaders", LEADER_SHIP, A_LEADERS);
  primitive("cleaders", LEADER_SHIP, C_LEADERS);
  primitive("xleaders", LEADER_SHIP, X_LEADERS);
  primitive("unpenalty", REMOVE_ITEM, PENALTY_NODE);
  primitive("unkern", REMOVE_ITEM, KERN_NODE);
  primitive("unskip", REMOVE_ITEM, GLUE_NODE);
  primitive("unhbox", UN_HBOX, BOX_CODE);
  primitive("unhcopy", UN_HBOX, COPY_CODE);
  primitive("unvbox", UN_VBOX, BOX_CODE);
  primitive("unvcopy", UN_VBOX, COPY_CODE);
  primitive("discretionary", DISCRETIONARY, 0);
  primitive("-", DISCRETIONARY, 1);
  primitive("eqno", EQ_NO, 0);
  primitive("leqno", EQ_NO, 1);
  primitive("mathord", MATH_COMP, ORD_NOAD);
  primitive("mathop", MATH_COMP, OP_NOAD);
  primitive("mathbin", MATH_COMP, BIN_NOAD);
  primitive("mathrel", MATH_COMP, REL_NOAD);
  primitive("mathopen", MATH_COMP, OPEN_NOAD);
  primitive("mathclose", MATH_COMP, CLOSE_NOAD);
  primitive("mathpunct", MATH_COMP, PUNCT_NOAD);
  primitive("mathinner", MATH_COMP, INNER_NOAD);
  primitive("underline", MATH_COMP, UNDER_NOAD);
  primitive("overline", MATH_COMP, OVER_NOAD);
  primitive("displaylimits", LIMIT_SWITCH, NORMAL);
  primitive("limits", LIMIT_SWITCH, LIMITS);
  primitive("nolimits", LIMIT_SWITCH, NO_LIMITS);
  primitive("displaystyle", MATH_STYLE, DISPLAY_STYLE);
  primitive("textstyle", MATH_STYLE, TEXT_STYLE);
  primitive("scriptstyle", MATH_STYLE, SCRIPT_STYLE);
  primitive("scriptscriptstyle", MATH_STYLE, SCRIPT_SCRIPT_STYLE);
  primitive("above", ABOVE, ABOVE_CODE);
  primitive("over", ABOVE, OVER_CODE);
  primitive("atop", ABOVE, ATOP_CODE);
  primitive("abovewithdelims", ABOVE, DELIMITED_CODE + ABOVE_CODE);
  primitive("overwithdelims", ABOVE, DELIMITED_CODE + OVER_CODE);
  primitive("atopwithdelims", ABOVE, DELIMITED_CODE + ATOP_CODE);
  primitive("left", LEFT_RIGHT, LEFT_NOAD);
  FROZEN_RIGHT=freeze(primitive("right", LEFT_RIGHT, RIGHT_NOAD));
  primitive("span", TAB_MARK, SPAN_CODE);
  FROZEN_CR=freeze(primitive("cr", CAR_RET, CR_CODE));
  primitive("crcr", CAR_RET, CR_CR_CODE);
  FROZEN_ENDV=new_sym("endtemplate", 11);
  eq_type(FROZEN_ENDV) = ENDV;
  eq_level(FROZEN_ENDV) = LEVEL_ONE;
  equiv(FROZEN_ENDV) = null_list;
  FROZEN_END_TEMPLATE=freeze(FROZEN_ENDV);
  eq_type(FROZEN_END_TEMPLATE) = END_TEMPLATE;
  primitive("long", PREFIX, 1);
  primitive("outer", PREFIX, 2);
  primitive("global", PREFIX, 4);
  primitive("def", DEF, 0);
  primitive("gdef", DEF, 1);
  primitive("edef", DEF, 2);
  primitive("xdef", DEF, 3);
  primitive("let", LET, NORMAL);
  primitive("futurelet", LET, NORMAL + 1);
  primitive("chardef", SHORTHAND_DEF, CHAR_DEF_CODE);
  primitive("mathchardef", SHORTHAND_DEF, MATH_CHAR_DEF_CODE);
  primitive("countdef", SHORTHAND_DEF, COUNT_DEF_CODE);
  primitive("dimendef", SHORTHAND_DEF, DIMEN_DEF_CODE);
  primitive("skipdef", SHORTHAND_DEF, SKIP_DEF_CODE);
  primitive("muskipdef", SHORTHAND_DEF, MU_SKIP_DEF_CODE);
  primitive("toksdef", SHORTHAND_DEF, TOKS_DEF_CODE);
  primitive("catcode", DEF_CODE, CAT_CODE);
  primitive("mathcode", DEF_CODE, MATH_CODE);
  primitive("lccode", DEF_CODE, LC_CODE);
  primitive("uccode", DEF_CODE, UC_CODE);
  primitive("sfcode", DEF_CODE, SF_CODE);
  primitive("delcode", DEF_CODE, DEL_CODE);
  primitive("textfont", DEF_FAMILY, TEXT_SIZE);
  primitive("scriptfont", DEF_FAMILY, SCRIPT_SIZE);
  primitive("scriptscriptfont", DEF_FAMILY, SCRIPT_SCRIPT_SIZE);
  primitive("hyphenation", HYPH_DATA, 0);
  primitive("patterns", HYPH_DATA, 1);
  primitive("hyphenchar", ASSIGN_FONT_INT, 0);
  primitive("skewchar", ASSIGN_FONT_INT, 1);
  primitive("batchmode", SET_INTERACTION,  BATCH_MODE);
  primitive("nonstopmode", SET_INTERACTION, NONSTOP_MODE);
  primitive("scrollmode", SET_INTERACTION, SCROLL_MODE);
  primitive("errorstopmode", SET_INTERACTION, ERROR_STOP_MODE);
  primitive("closein", IN_STREAM, 0);
  primitive("openin", IN_STREAM, 1);
  primitive("message", MESSAGE, 0);
  primitive("errmessage", MESSAGE, 1);
  primitive("lowercase", CASE_SHIFT, LC_CODE);
  primitive("uppercase", CASE_SHIFT, UC_CODE);
  primitive("show", XRAY, SHOW_CODE);
  primitive("showbox", XRAY, SHOW_BOX_CODE);
  primitive("showthe", XRAY, SHOW_THE_CODE);
  primitive("showlists", XRAY, SHOW_LISTS);
  primitive("openout", EXTENSION, OPEN_NODE);
  write_cs=primitive("write", EXTENSION, WRITE_NODE);
  FROZEN_END_WRITE=freeze(primitive("endwrite", OUTER_CALL, 0));
  eq_level(FROZEN_END_WRITE) = LEVEL_ONE;
  primitive("closeout", EXTENSION, CLOSE_NODE);
  primitive("special", EXTENSION, SPECIAL_NODE);
  primitive("immediate", EXTENSION, IMMEDIATE_CODE);
  primitive("setlanguage", EXTENSION, SET_LANGUAGE_CODE);
  primitive("end", STOP, 0);
  primitive("dump", STOP, 1);
  FROZEN_PROTECTION = new_sym("inaccessible", 12);
}

/*
 * Common TeX:cond.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX cond.c 3.2@(#)";
#endif

#include "tex.h"

ptr	cond_ptr;

int	cur_if;
int	if_limit;
int	if_line;
int	skip_line;

  void
conditional ()
{
  bool	b;
  sym	s;
  int	m, n;
  ptr	p, q, r;
  int	this_if;
  ptr	save_cond_ptr;
  int	save_scanner_status;

  push_cond();
  save_cond_ptr = cond_ptr;
  this_if = cur_chr;
  switch (this_if)
  {
    case IF_CHAR_CODE:
    case IF_CAT_CODE:

#define get_x_token_or_active_char() {get_x_token(); \
  if (cur_cmd == RELAX && cur_chr == NO_EXPAND_FLAG) { \
    cur_cmd = ACTIVE_CHAR; \
    cur_chr = tok2sym(cur_tok) - active_base[0]; }}

      get_x_token_or_active_char();
      if (cur_cmd > ACTIVE_CHAR || cur_chr > 255) {
        m = RELAX;
        n = 256;
      } else {
        m = cur_cmd;
        n = cur_chr;
      }
      get_x_token_or_active_char();
      if (cur_cmd > ACTIVE_CHAR || cur_chr > 255) {
        cur_cmd = RELAX;
        cur_chr = 256;
      }
      if (this_if == IF_CHAR_CODE) {
        b = n == cur_chr;
      } else {
        b = m == cur_cmd;
      }
      break;

    case IF_INT_CODE:
    case IF_DIM_CODE:
      if (this_if == IF_INT_CODE) {
        scan_int();
      } else {
        scan_normal_dimen();
      }
      n = cur_val;
      get_nbx_token(); 
      if (cur_tok >= OTHER_TOKEN + '<'
          && cur_tok <= OTHER_TOKEN + '>') {
        r = cur_tok - OTHER_TOKEN;
      } else {
        print_err("Missing = inserted for ");
        print_cmd_chr(IF_TEST, this_if);
        help_relation();
        back_error();
        r = '=';
      }
      if (this_if == IF_INT_CODE) {
        scan_int();
      } else {
        scan_normal_dimen();
      }
      switch (r) {
        case '<': b = n < cur_val; break;
        case '=': b = n == cur_val; break; 
        case '>': b = n > cur_val; break;
      }
      break;

    case IF_ODD_CODE:
      scan_int();
      b = odd(cur_val);
      break;

    case IF_VMODE_CODE:
      b = abs(mode) == VMODE;
      break;

    case IF_HMODE_CODE:
      b = abs(mode) == HMODE;
      break;

    case IF_MMODE_CODE:
      b = abs(mode) == MMODE;
      break;

    case IF_INNER_CODE:
      b = mode < 0;
      break;

    case IF_VOID_CODE:
    case IF_HBOX_CODE:
    case IF_VBOX_CODE:
      scan_eight_bit_int();
      p = box(cur_val);
      if (this_if == IF_VOID_CODE) {
        b = p == null;
      } else if (p == null) {
        b = FALSE;
      } else if (this_if == IF_HBOX_CODE) {
        b = type(p) == HLIST_NODE;
      } else {
        b = type(p) == VLIST_NODE;
      }
      break;

    case IFX_CODE:
      save_scanner_status = scanner_status;
      scanner_status = NORMAL;
      get_next();
      s = cur_cs;
      p = cur_cmd;
      q = cur_chr;
      get_next(); 
      if (cur_cmd != p) {
        b = FALSE;
      } else if (cur_cmd < CALL) {
        b = cur_chr == q;
      } else {
        p = token_link(cur_chr);
        q = token_link(equiv(s));
        if (p == q) {
          b = TRUE;
        } else {
          while (p != null && q != null) {
            if (token(p) != token(q)) {
              p = null;
            } else {
              p = token_link(p);
              q = token_link(q);
            }
          }
          b = p == null && q == null;
        }
      }
      scanner_status = save_scanner_status;
      break;

    case IF_EOF_CODE:
      scan_four_bit_int();
      b = read_open[cur_val] == CLOSED;
      break;

    case IF_TRUE_CODE:
      b = TRUE;
      break;

    case IF_FALSE_CODE:
      b = FALSE;
      break;

    case IF_CASE_CODE: 
      scan_int();
      n = cur_val;
      if (tracing_commands > 1) {
        begin_diagnostic();
        print("{case ");
        print_int(n);
        print("}");
        end_diagnostic(FALSE);
      }
      while (n != 0) {
        pass_text();
        if (cond_ptr == save_cond_ptr) {
          if (cur_chr == OR_CODE) {
            decr(n);
          } else {
            goto common_end;
          }
        } else if (cur_chr == FI_CODE) {
          pop_cond();
        }
      }
      change_if_limit(OR_CODE, save_cond_ptr);
      return;

    default:
      break;
  }

  if (tracing_commands > 1) {
    begin_diagnostic();
    print(b ? "{true}" : "{false}");
    end_diagnostic(FALSE);
  }

  if (b) {
    change_if_limit(ELSE_CODE, save_cond_ptr);
    return;
  }

  loop {
    pass_text(); 
    if (cond_ptr == save_cond_ptr) {
      if (cur_chr != OR_CODE)
        goto common_end;
      print_err("Extra ");
      print_esc("or");
      help_or();
      error();
    } else if (cur_chr == FI_CODE) {
      pop_cond();
    }
  }

common_end:
  if (cur_chr == FI_CODE) {
    pop_cond();
  } else {
    if_limit = FI_CODE;
  }
}

  void
push_cond ()
{
  ptr	p;

  p = new_node(IF_NODE_SIZE);
  link(p) = cond_ptr;
  type(p) = if_limit;
  subtype(p) = cur_if;
  if_line_field(p) = if_line;
  cond_ptr = p;
  cur_if = cur_chr;
  if_limit = IF_CODE;
  if_line = line;
}

  void
pop_cond ()
{
  ptr	p;

  p = cond_ptr;
  if_line = if_line_field(p);
  cur_if = subtype(p);
  if_limit = type(p);
  cond_ptr = link(p);
  free_node(p, IF_NODE_SIZE);
}

  void
pass_text ()
{
  int	l;
  int	save_scanner_status;

  l = 0;
  save_scanner_status = scanner_status;
  scanner_status = SKIPPING;
  skip_line = line;
  loop {
    get_next();
    if (cur_cmd == FI_OR_ELSE) {
      if (l == 0)
        break;
      if (cur_chr == FI_CODE)
        decr(l);
    } else if (cur_cmd == IF_TEST) {
      incr(l);
    }
  }
  scanner_status = save_scanner_status;
}

  void
change_if_limit (l, p)
  int	l;
  ptr	p;
{
  ptr	q;

  if (p == cond_ptr) {
    if_limit = l;
  } else {
    q = cond_ptr; 
    loop {
      if (q == null)
        confusion("if");
      if (link(q) == p) {
        type(q) = l;
        return;
      }
      q = link(q);
    }
  }
}

  void
_cond_init ()
{
  cond_ptr = null;
  if_limit = NORMAL;
  if_line = 0;
  cur_if = 0;
}

  void
_cond_init_once ()
{
}

/*
 **	Help text
 */

help_or ()
{
  help1("I'm ignoring this; it doesn't match any \\if.");
}

help_relation ()
{
  help1("I was expecting to see `<', `=', or `>'. Didn't.");
}

/*
 * Common TeX:def.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX def.c 3.2@(#)";
#endif

#include "tex.h"

tok	after_token;
int	mag_set;
bool	long_help_seen;

#define glob	(a >= 4)

#define def(eq, type, value) \
  if (glob) \
eq_gdefine(eq, type, value); \
else eq_define(eq, type, value)

#define reg_def(reg, type, value) \
  if (glob) \
reg_gdefine(reg, type, value); \
else reg_define(reg, type, value)

  void
get_r_token ()
{
restart:
  do get_token();
  while (cur_tok == SPACE_TOKEN);
  if (cur_cs == null_sym || isfrozen(cur_cs)) {
    print_err("Missing control sequence inserted");
    help_missing_cs();
    if (cur_cs == null_sym)
      back_input();
    cur_tok = sym2tok(FROZEN_PROTECTION);
    ins_error();
    goto restart;
  }
}

  void
prefixed_command ()
{
  int	a = 0;

  while (cur_cmd == PREFIX) {
    if (!odd(a / cur_chr))
      a += cur_chr;
    get_nbrx_token();
    if (cur_cmd <= MAX_NON_PREFIXED_COMMAND) {
      print_err("You can't use a prefix with `");
      print_cmd_chr(cur_cmd, cur_chr);
      print("'");
      help_prefix();
      back_error();
      return;
    }
  }
  if (cur_cmd != DEF && a % 4 != 0) {
    print_err("You can't use `");
    print_esc("long");
    print("' or `");
    print_esc("outer");
    print("' with `");
    print_cmd_chr(cur_cmd, cur_chr);
    print("'");
    help_pref();
    error();
  }
  if (global_defs != 0) {
    if (global_defs < 0) {
      if (glob) a -= 4;
    } else {
      if (!glob) a += 4;
    }
  }
  switch (cur_cmd) 
  {
    case SET_FONT:
      reg_def(cur_font_ptr, FNT_REG, cur_chr);
      break;

    case DEF: {
                bool	e;
                sym	s;

                if (odd(cur_chr) && !glob && global_defs >= 0)
                  a += 4;
                e = cur_chr >= 2;
                get_r_token();
                s = cur_cs;
                scan_toks(TRUE, e);
                def(s, CALL + (a % 4), def_ref);
                break;
              }
    case LET: {
                int	n;
                sym	s;
                tok	t;

                n = cur_chr;
                get_r_token();
                s = cur_cs;
                if (n == NORMAL) {
                  do get_token();
                  while (cur_cmd == SPACER);
                  if (cur_tok == OTHER_TOKEN + '=') {
                    get_token();
                    if (cur_cmd == SPACER)
                      get_token();
                  }
                } else {
                  get_token();
                  t = cur_tok;
                  get_token();
                  back_input();
                  cur_tok = t;
                  back_input();
                }
                if (cur_cmd >= CALL)
                  add_token_ref(cur_chr);
                def(s, cur_cmd, cur_chr);
                break;
              }
    case SHORTHAND_DEF: {
                          int	n;
                          sym	s;

                          n = cur_chr;
                          get_r_token();
                          s = cur_cs; 
                          def(s, RELAX, 256);
                          scan_optional_equals();
                          switch (n)
                          {
                            case CHAR_DEF_CODE:
                              scan_char_num();
                              def(s, CHAR_GIVEN, cur_val);
                              break;

                            case MATH_CHAR_DEF_CODE:
                              scan_fifteen_bit_int();
                              def(s, MATH_GIVEN, cur_val);
                              break;

                            default:
                              scan_eight_bit_int();
                              switch (n) {
                                case MU_SKIP_DEF_CODE:
                                  def(s, ASSIGN_MU_GLUE, MU_GLUE_PARS + cur_val);
                                  break;
                                case SKIP_DEF_CODE:
                                  def(s, ASSIGN_GLUE, GLUE_PARS + cur_val);
                                  break;
                                case DIMEN_DEF_CODE:
                                  def(s, ASSIGN_DIMEN, DIMEN_PARS + cur_val);
                                  break;
                                case COUNT_DEF_CODE:
                                  def(s, ASSIGN_INT, INT_PARS + cur_val);
                                  break;
                                case TOKS_DEF_CODE:
                                  def(s, ASSIGN_TOKS, TOK_PARS + cur_val);
                                  break;
                              }
                              break;
                          }
                          break;
                        }
    case READ_TO_CS: {
                       int	n;
                       sym	s;

                       scan_int();
                       n = cur_val;
                       if (!scan_keyword("to")) {
                         print_err("Missing `to' inserted");
                         help_read_to();
                         error();
                       }
                       get_r_token();
                       s = cur_cs;
                       read_toks(n, s);
                       def(s, CALL, cur_val);
                       break;
                     }
    case TOKS_REGISTER:
    case ASSIGN_TOKS: {
                        ptr	p;
                        reg	r;
                        sym	s;

                        s = cur_cs;
                        if (cur_cmd == TOKS_REGISTER) {
                          scan_eight_bit_int();
                          r = toks_reg[TOK_PARS + cur_val];
                        } else {
                          r = toks_reg[cur_chr];
                        }
                        scan_optional_equals();
                        get_nbrx_token();
                        if (cur_cmd != LEFT_BRACE) {
                          if (cur_cmd == TOKS_REGISTER) {
                            scan_eight_bit_int();
                            cur_cmd = ASSIGN_TOKS;
                            cur_chr = TOK_PARS + cur_val;
                          }
                          if (cur_cmd == ASSIGN_TOKS) {
                            p = reg_equiv(toks_reg[cur_chr]);
                            if (p == null) {
                              reg_def(r, TOKS_REG, null);
                            } else {    
                              add_token_ref(p);
                              reg_def(r, TOKS_REG, p);
                            }
                            break;
                          }
                        }
                        back_input();
                        cur_cs = s;
                        scan_toks(FALSE, FALSE);
                        if (token_link(def_ref) == null) {
                          reg_def(r, TOKS_REG, null);
                          free_token(def_ref);
                        } else {
                          if (r == output_routine_reg) {
                            p = token_link(def_ref);
                            while (token_link(p) != null)
                              p = token_link(p);
                            p = token_link(p) = new_token();
                            token(p) = RIGHT_BRACE_TOKEN + '}';
                            p = new_token();
                            token(p) = LEFT_BRACE_TOKEN + '{';
                            token_link(p) = token_link(def_ref);
                            token_link(def_ref) = p;
                          }
                          reg_def(r, TOKS_REG, def_ref);
                        }
                        break;
                      }
    case ASSIGN_INT: {
                       reg		r;

                       r = int_reg[cur_chr];
                       scan_optional_equals();
                       scan_int();
                       reg_def(r, INT_REG, cur_val); 
                       break;
                     }
    case ASSIGN_DIMEN: {
                         reg		r;

                         r = dimen_reg[cur_chr];
                         scan_optional_equals();
                         scan_normal_dimen();
                         reg_def(r, DIMEN_REG, cur_val); 
                         break;
                       }
    case ASSIGN_GLUE: {
                        reg		r;

                        r = skip_reg[cur_chr];
                        scan_optional_equals();
                        scan_glue(GLUE_VAL);
                        trap_zero_glue();
                        reg_def(r, SKIP_REG, cur_val); 
                        break;
                      }
    case ASSIGN_MU_GLUE: {
                           reg		r;

                           r = mu_skip_reg[cur_chr];
                           scan_optional_equals();
                           scan_glue(MU_VAL);
                           trap_zero_glue();
                           reg_def(r, MU_SKIP_REG, cur_val); 
                           break;
                         }
    case DEF_CODE: {
                     int	m;
                     int	n;
                     int	o;
                     reg		r;

                     m = cur_chr;
                     if (m == CAT_CODE) {
                       n = MAX_CHAR_CODE;
                       o = CAT_CODE_REG;
                     } else if (m == LC_CODE) {
                       n = 255;
                       o = LC_CODE_REG;
                     } else if (m == UC_CODE) {
                       n = 255;
                       o = UC_CODE_REG;
                     } else if (m == SF_CODE) {
                       n = 077777;
                       o = SF_CODE_REG;
                     } else if (m == MATH_CODE) {
                       n = 0100000;
                       o = MATH_CODE_REG;
                     } else if (m == DEL_CODE) {
                       n = 077777777;
                       o = DEL_CODE_REG;
                     } else {
                       confusion("confusing code");
                     }
                     scan_char_num();
                     switch (m) {
                       case CAT_CODE: r = cat_code_reg[cur_val]; break;
                       case UC_CODE: r = uc_code_reg[cur_val]; break;
                       case LC_CODE: r = lc_code_reg[cur_val]; break;
                       case SF_CODE: r = sf_code_reg[cur_val]; break;
                       case MATH_CODE: r = math_code_reg[cur_val]; break;
                       case DEL_CODE: r = del_code_reg[cur_val]; break;
                     }
                     scan_optional_equals();
                     scan_int(); 
                     if (cur_val < 0 && m != DEL_CODE || cur_val > n) {
                       print_err("Invalid code (");
                       print_int(cur_val);
                       if (m != DEL_CODE) {
                         print("), should be in the range 0..");
                       } else {
                         print("), should be at most ");
                       }
                       print_int(n);
                       help_code();
                       error();
                       cur_val = 0;
                     }
                     reg_def(r, o, cur_val);
                     break;
                   }
    case DEF_FAMILY: {
                       int	i;
                       reg	r;

                       i = cur_chr;
                       scan_four_bit_int();
                       i += cur_val;
                       scan_optional_equals();
                       scan_font_ident();
                       r = fnt_reg[1 + i];
                       reg_def(r, FNT_REG, cur_val);
                       break;
                     }
    case REGISTER:
    case ADVANCE:
    case MULTIPLY:
    case DIVIDE:
                     do_register_command(a);
                     break;

    case SET_BOX: {
                    int	n;

                    scan_eight_bit_int();
                    if (glob) {
                      n = 256 + cur_val;
                    } else {
                      n = cur_val;
                    }
                    scan_optional_equals();
                    scan_box(BOX_FLAG + n);
                    break;
                  }
    case SET_AUX:
                  alter_aux();
                  break;

    case SET_PREV_GRAF:
                  alter_prev_graf();
                  break;

    case SET_PAGE_DIMEN:
                  alter_page_so_far();
                  break;

    case SET_PAGE_INT:
                  alter_integer();
                  break;

    case SET_BOX_DIMEN:
                  alter_box_dimen();
                  break;

    case SET_SHAPE: {
                      int	i;
                      int	n;
                      ptr	p;

                      scan_optional_equals();
                      scan_int();
                      n = cur_val;
                      if (n <= 0) {
                        def(par_shape_cs, SET_SHAPE, null);
                      } else {
                        p = new_node(sizeof(mcell) + n * sizeof(shape_t));
                        def(par_shape_cs, SET_SHAPE, p);
                        info(p) = n;
                        for (i = 1; i <= n; incr(i)) {
                          scan_normal_dimen();
                          par_shape_indent(i) = cur_val;
                          scan_normal_dimen();
                          par_shape_width(i) = cur_val;
                        }
                      }
                      break;
                    }
    case HYPH_DATA:
                    if (cur_chr == 1) 
                      new_patterns();
                    else new_hyph_exceptions();
                    break;

    case ASSIGN_FONT_DIMEN: {
                              scal	*d;

                              d = find_font_dimen(TRUE);
                              scan_optional_equals();
                              scan_normal_dimen();
                              if (d != (scal *) 0)
                                *d = cur_val;
                              break;
                            }
    case ASSIGN_FONT_INT: {
                            int	f;
                            int	n;

                            n = cur_chr;
                            scan_font_ident();
                            f = cur_val;
                            scan_optional_equals();
                            scan_int(); 
                            if (n == 0)
                              hyphen_char(f) = cur_val;
                            else skew_char(f) = cur_val;
                            break;
                          }
    case DEF_FONT:
                          new_font(a);
                          break;

    case SET_INTERACTION:
                          new_interaction();
                          break;

    default:
                          confusion("prefix");
                          break;
  }
  if (after_token != 0) {
    cur_tok = after_token;
    back_input();
    after_token = 0;
  }
} 

  void
trap_zero_glue()
{
  if (glue_width(cur_val) == 0
      && stretch(cur_val) == 0
      && shrink(cur_val) == 0) {
    add_glue_ref(zero_glue);
    delete_glue_ref(cur_val);
    cur_val = zero_glue;
  }
}

  void
do_register_command (a)
  int	a;
{
  int	b;
  int	c;
  ptr	p;
  reg	r;
  int	v;

  c = cur_cmd;
  if (c != REGISTER) {
    get_x_token();
    if (cur_cmd >= ASSIGN_INT && cur_cmd <= ASSIGN_MU_GLUE) {
      b = cur_cmd - ASSIGN_INT;
      switch (b) {
        case INT_VAL: r = int_reg[cur_chr]; break;
        case DIMEN_VAL: r = dimen_reg[cur_chr]; break; 
        case GLUE_VAL: r = skip_reg[cur_chr]; break; 
        case MU_VAL: r = mu_skip_reg[cur_chr]; break;
      }
      goto found;
    }
    if (cur_cmd != REGISTER) {
      print_err("You can't use `");
      print_cmd_chr(cur_cmd, cur_chr);
      print("' after ");
      print_cmd_chr(c, 0);
      help_register();
      error();
      return;
    }
  }

  b = cur_chr;
  scan_eight_bit_int();
  switch (b) {
    case INT_VAL: r = int_reg[INT_PARS + cur_val]; break;
    case DIMEN_VAL: r = dimen_reg[DIMEN_PARS + cur_val]; break; 
    case GLUE_VAL: r = skip_reg[GLUE_PARS + cur_val]; break; 
    case MU_VAL: r = mu_skip_reg[MU_GLUE_PARS + cur_val]; break;
  }

found:
  v = reg_equiv(r);
  if (c == REGISTER)
    scan_optional_equals();
  else scan_keyword("by");
  arith_error = FALSE;
  if (c < MULTIPLY)  {
    if (b < GLUE_VAL) {
      if (b == INT_VAL)
        scan_int();
      else scan_normal_dimen();
      if (c == ADVANCE)
        cur_val += v;
    } else {
      scan_glue(b);
      if (c == ADVANCE) {
        p = new_spec(cur_val);
        delete_glue_ref(cur_val);
        glue_width(p) += glue_width(v);
        if (stretch(p) == 0)
          stretch_order(p) = NORMAL;
        if (stretch_order(p) == stretch_order(v)) {
          stretch(p) += stretch(v);
        } else if (stretch_order(p) < stretch_order(v)
            && stretch(v)) {
          stretch(p) = stretch(v);
          stretch_order(p) = stretch_order(v);
        }
        if (shrink(p) == 0)
          shrink_order(p) = NORMAL;
        if (shrink_order(p) == shrink_order(v)) {
          shrink(p) += shrink(v);
        } else if (shrink_order(p) < shrink_order(v)
            && shrink(v)) {
          shrink(p) = shrink(v); 
          shrink_order(p) = shrink_order(v);
        }
        cur_val = p;
      }
    }
  } else {
    scan_int();
    if (b < GLUE_VAL) {
      if (c == MULTIPLY) {
        if (b == INT_VAL) {
          cur_val = mult_integers(v, cur_val);
        } else {
          cur_val = nx_plus_y(v, cur_val, 0);
        }
      } else {
        cur_val = x_over_n(v, cur_val);
      }
    } else {
      p = new_spec(v);
      if (c == MULTIPLY) {
        glue_width(p) =
          nx_plus_y(glue_width(v), cur_val, 0);
        stretch(p) = nx_plus_y(stretch(v), cur_val, 0);
        shrink(p) = nx_plus_y(shrink(v), cur_val, 0);
      } else {
        glue_width(p) =
          x_over_n(glue_width(v), cur_val);
        stretch(p) = x_over_n(stretch(v), cur_val);
        shrink(p) = x_over_n(shrink(v), cur_val);
      }
      cur_val = p;
    }
  }
  if (arith_error) {
    print_err("Arithmetic overflow");
    help_overflow();
    error();
    return;
  }
  switch (b) {
    case INT_VAL: reg_def(r, INT_REG, cur_val); break;
    case DIMEN_VAL: reg_def(r, DIMEN_REG, cur_val); break;
    case GLUE_VAL: trap_zero_glue(); reg_def(r, SKIP_REG, cur_val); break;
    case MU_VAL: trap_zero_glue(); reg_def(r, MU_SKIP_REG, cur_val); break;
  }
}

  void
alter_aux ()
{
  int	c;

  if (cur_chr != abs(mode)) {
    report_illegal_case();
  } else {
    c = cur_chr;
    scan_optional_equals();
    if (c == VMODE)  {
      scan_normal_dimen();
      prev_depth = cur_val;
    } else {
      scan_int();
      if (cur_val <= 0 || cur_val > 32767) {
        print_err("Bad space factor");
        help_space_factor();
        int_error(cur_val);
      } else {
        space_factor = cur_val;
      }
    }
  }
}

  void
alter_prev_graf ()
{
  list	*p;

  *nest_ptr = cur_list;
  p = nest_ptr;
  while (abs(p->mode_field) != VMODE)
    decr(p);
  scan_optional_equals();
  scan_int();
  if (cur_val < 0) {
    print_err("Bad ");
    print_esc("prevgraf");
    help_prevgraf();
    int_error(cur_val);
  } else {
    p->pg_field = cur_val;
    cur_list = *nest_ptr;
  }
}

  void
alter_page_so_far ()
{
  int	c;

  c = cur_chr;
  scan_optional_equals(); 
  scan_normal_dimen();
  page_so_far[c] = cur_val;
}

  void
alter_integer ()
{
  int	c;

  c = cur_chr;
  scan_optional_equals();
  scan_int();
  if (c == 0) {
    dead_cycles = cur_val;
  } else {
    insert_penalties = cur_val;
  }
}

  void
alter_box_dimen ()
{
  int	b;
  int	c;

  c = cur_chr;
  scan_eight_bit_int();
  b = cur_val; 
  scan_optional_equals();
  scan_normal_dimen();
  if (box(b) != null) {
    switch (c) {
      case WD_CODE: box_width(box(b)) = cur_val; break;
      case HT_CODE: box_height(box(b)) = cur_val; break;
      case DP_CODE: box_depth(box(b)) = cur_val; break;
    }
  }
}

  void
new_font (a)
  int	a;
{
  fnt	f;
  scal	s;
  str	t;
  int	l;
  sym	u;

#define ILL_MAG "Illegal magnification has been changed to 1000"

  if (job_name == null_str)
    open_log_file();
  get_r_token();
  u = cur_cs;
  switch (length(u))
  {
    case 0:
      t = make_str_given("FONT");
      l = 0;
      break;

    case 1:
      if (isactive(u)) {
        int old_setting = selector;
        selector = NEW_STRING;
        print("FONT");
        print_ASCII(*text(u));
        selector = old_setting;
        l = cur_length();
        t = make_str();
      } else {
        l = 1;
        t = text(u);
      }
      break;

    default:
      l = length(u);
      t = text(u);
      break;
  }
  def(u, SET_FONT, null_font);
  scan_optional_equals();
  scan_file_name();
  name_in_progress = TRUE;
  if (scan_keyword("at")) {
    scan_normal_dimen();
    s = cur_val;
    if (s <= 0 || s >= 01000000000) {
      print_err("Improper `at' size (");
      print_scaled(s);
      print("pt), replaced by 10pt");
      help_font_at();
      error();
      s = 10 * UNITY;
    }
  } else if (scan_keyword("scaled")) {
    scan_int();
    s = -cur_val;
    if (cur_val <= 0 || cur_val > 32768) {
      print_err(ILL_MAG);
      help_font_magnification();
      int_error(cur_val);
      s = -1000;
    }
  } else {
    s = -1000;
  }
  name_in_progress = FALSE;
  for (f = null_font + 1; f < null_font + font_ptr - font_info; f++) {
    if (str_eq(font_name(f), cur_name) &&
        str_eq(font_area(f), cur_area)) {
      if (s > 0) {
        if (s == font_size(f))
          goto common_end;
      } else if (font_size(f) ==
          xn_over_d(font_dsize(f), -s, 1000)) {
        goto common_end;
      }
    }
  }
  f = read_font_info(u, cur_name, cur_area, s);

common_end:
  equiv(u) = f;
  *font_id_base[f] = *u;
  font_id_text(f) = t;
  font_id_length(f) = l;
}

  void
prepare_mag ()
{
  if (mag_set > 0 && mag != mag_set) {
    print_err("Incompatible magnification (");
    print_int(mag);
    print(");");
    print_nl(" the previous value will be retained");
    help_mag();
    int_error(mag_set);
    reg_gdefine(int_reg[MAG_CODE], INT_REG, mag_set);
  }
  if (mag <= 0 || mag > 32768) {
    print_err("Illegal magnification has been changed to 1000");
    help_ill_mag();
    int_error(mag);
    reg_gdefine(int_reg[MAG_CODE], INT_REG, 1000);
  }
  mag_set = mag;
}

  void
new_interaction ()
{
  print_ln();
  interaction = cur_chr;
  if (interaction == BATCH_MODE)
    selector = NO_PRINT;
  else selector = TERM_ONLY;
  if (job_name != null_str)
    selector += 2;
}

  void
do_assignments ()
{
  loop {
    get_nbrx_token();
    if (cur_cmd <= MAX_NON_PREFIXED_COMMAND)
      break;
    prefixed_command();
  }
}

  void
clopen_stream ()
{	
  int	c;
  int	n;

  c = cur_chr;
  scan_four_bit_int();
  n = cur_val; 
  if (read_open[n] != CLOSED) {
    a_close(read_file[n]);
    read_open[n] = CLOSED;
  }
  if (c != 0) {
    scan_optional_equals();
    scan_file_name();
    if (cur_ext == null_str)
      cur_ext = str_tex;
    pack_cur_name();
    if (read_file[n] = a_open_in())
      read_open[n] = OPENED;
  }
}

  void
issue_message ()
{
  int	c;
  str	s;
  int	old_setting;

  c = cur_chr;
  scan_toks(FALSE, TRUE);
  old_setting = selector;
  selector = NEW_STRING;
  token_show(def_ref);
  selector = old_setting;
  flush_list(def_ref);
  s = make_str();
  if (c == 0) {
    if (term_offset + str_length(s) > MAX_PRINT_LINE - 2) {
      print_ln();
    } else if (term_offset > 0 || file_offset > 0) {
      print(" ");
    }
    print(s);
    update_terminal();
  } else {
    print_err(null_str);
    print(s);
    if (err_help != null) {
      use_err_help = TRUE;
    } else if (long_help_seen) {
      help_err_msg();
    } else {
      if (interaction < ERROR_STOP_MODE)
        long_help_seen = TRUE;
      help_poirot();
    }
    error();
    use_err_help = FALSE;
  }
  flush_str();
}

  void
give_err_help ()
{
  token_show(err_help);
}

  void
shift_case ()
{
  reg	*r;
  int	c;
  int	i;
  ptr	p;
  sym	s;
  tok	t;

  r = cur_chr == LC_CODE ? lc_code_reg : uc_code_reg;
  scan_toks(FALSE, FALSE);
  for (p = token_link(def_ref); p != null; p = token_link(p)) {
    t = token(p);
    if (t >= CS_TOKEN_FLAG) {
      s = tok2sym(t);
      if (!issingle(s) && !isactive(s)) {
        continue;
      }
      c = *text(s);
      if (reg_equiv(r[c]) != 0) {
        i = reg_equiv(r[c]);
        if (cat_code(c) == ACTIVE_CHAR) {
          token(p) = sym2tok(active_base[i]);
        } else {
          token(p) = sym2tok(single_base[i]);
        }
      }
    } else {
      c = t % 0400;
      if (reg_equiv(r[c]) != 0) {
        token(p) = (t / 0400) * 256 + reg_equiv(r[c]);
      }
    }
  }
  back_list(token_link(def_ref));
  free_token(def_ref);
}

  void
show_whatever ()
{
  ptr	t;

  switch (cur_chr)
  {
    case SHOW_LISTS:
      begin_diagnostic();
      show_activities();
      break;

    case SHOW_BOX_CODE:
      scan_eight_bit_int();
      begin_diagnostic();
      print_nl("> \\box");
      print_int(cur_val);
      print("=");
      if (box(cur_val) == null) {
        print("void");
      } else {
        show_box(box(cur_val));
      }
      break;

    case SHOW_CODE:
      get_token();
      print_nl("> ");
      if (cur_cs != null_sym) {
        sprint_cs(cur_cs);
        print("=");
      }
      print_meaning();
      goto common_end;

    default:
      t = the_toks();
      print_nl("> ");
      token_show(t);
      flush_list(t);
      goto common_end;
  }
  end_diagnostic(TRUE);
  print_err("OK");
  if (selector == TERM_AND_LOG && tracing_online <= 0) {
    selector = TERM_ONLY;
    print(" (see the transcript file)");
    selector = TERM_AND_LOG;
  }

common_end:
  if (interaction < ERROR_STOP_MODE) {
    help0();
    decr(error_cnt);
  } else if (tracing_online > 0) {
    help_show_online();
  } else {
    help_show();
  }
  error();
}

  void
_def_init ()
{
  long_help_seen = FALSE;
  mag_set = 0;
}

  void
_def_init_once ()
{
}

/*
 **	Help text
 */

help_missing_cs ()
{
  help5("Please don't say `\\def cs{...}', say `\\def\\cs{...}'.",
      "I've inserted an inaccessible control sequence so that your",
      "definition will be completed without mixing me up too badly.",
      "You can recover graciously from this error, if you're",
      "careful; see exercise 27.2 in The TeXbook.");
}

help_prefix ()
{
  help1("I'll pretend you didn't say \\long or \\outer or \\global.");
}

help_pref ()
{
  help1("I'll pretend you didn't say \\long or \\outer here.");
}

help_read_to ()
{
  help2("You should have said `\\read<number> to \\cs'.",
      "I'm going to look for the \\cs now.");
}

help_code ()
{
  help1("I'm going to use 0 instead of that illegal code value.");
}

help_register ()
{
  help1("I'm forgetting what you said and not changing anything.");
}

help_space_factor ()
{
  help1("I allow only values in the range 1..32767 here.");
}

help_prevgraf ()
{
  help1("I allow only nonnegative values here.");
}

help_overflow ()
{
  help2("I can't carry out that multiplication or division,",
      "since the result is out of range.");
}

help_font_at ()
{
  help2("I can only handle fonts at positive sizes that are",
      "less than 2048pt, so I've changed what you said to 10pt.");
}

help_font_magnification ()
{
  help1("The magnification ratio must be between 1 and 32768.");
}

help_mag()
{
  help2("I can handle only one magnification ratio per job. So I've",
      "reverted to the magnification you used earlier on this run.");
}

help_ill_mag ()
{
  help1("The magnification ratio must be between 1 and 32768.");
}

help_err_msg ()
{
  help1("(That was another \\errmessage.)");
}

help_poirot ()
{
  help4("This error message was generated by an \\errmessage",
      "command, so I can't give any explicit help.",
      "Pretend that you're Hercule Poirot: Examine all clues,",
      "and deduce the truth by order and method.");
}

help_show_online ()
{
  help3("This isn't an error message; I'm just \\showing something.",
      "Type `I\\show...' to show more (e.g., \\show\\cs,",
      "\\showthe\\count10, \\showbox255, \\showlists).");
}

help_show ()
{
  help5("This isn't an error message; I'm just \\showing something.",
      "Type `I\\show...' to show more (e.g., \\show\\cs,",
      "\\showthe\\count10, \\showbox255, \\showlists).",
      "And type `I\\tracingonline=1\\show...' to show boxes and",
      "lists on your terminal as well as in the transcript file.");
}

/*
 * Common TeX:dvi.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX dvi.c 3.2@(#)";
#endif

#include "tex.h"

file	dvi_file;
str	dvi_name;

#define HALF_BUF	2048
#define DVI_BUF_SIZE	4096

byte	*dvi_buf;
int	dvi_ptr;
int	dvi_limit;
int	dvi_offset;
int	dvi_gone;

int	cur_s;
scal	cur_h;
scal	cur_v;
fnt	dvi_f;
scal	dvi_h;
scal	dvi_v;
scal	max_h;
scal	max_v;
scal	rule_dp;
scal	rule_ht;
scal	rule_wd;
int	max_push;
int	total_pages;
int	dead_cycles;
bool	doing_leaders;
int	last_bop;

ptr	down_ptr;
ptr	right_ptr;

  void
ship_out (p)
  ptr	p;
{
  int	page_loc;
  int	j, k;

  if (tracing_output > 0) {
    print_ln();
    print("Completed box being shipped out");
  }
  if (term_offset > MAX_PRINT_LINE - 9)
    print_ln();
  else if (term_offset > 0 || file_offset > 0)
    print(" ");
  print("["); 
  for (j = 9; j > 0 && count(j) == 0; decr(j))
    ;
  for (k = 0; k <= j; incr(k)) {
    print_int(count(k));
    if (k < j)
      print(".");
  }
  update_terminal();
  if (tracing_output > 0) {
    print("]");
    begin_diagnostic();
    show_box(p);
    end_diagnostic(TRUE);
  }
  if (box_height(p) > MAX_DIMEN || box_depth(p) > MAX_DIMEN ||
      box_height(p) + box_depth(p) + v_offset > MAX_DIMEN ||
      box_width(p) + h_offset > MAX_DIMEN) {
    print_err("Huge page cannot be shipped out");
    help_huge_page();
    error();
    if (tracing_output <= 0) {
      begin_diagnostic();
      print_nl("The following box has been deleted:");
      show_box(p);
      end_diagnostic(TRUE);
    }
    goto done;
  }
  if (box_height(p) + box_depth(p) + v_offset > max_v)
    max_v = box_height(p) + box_depth(p) + v_offset;
  if (box_width(p) + h_offset > max_h)
    max_h = box_width(p) + h_offset;
  dvi_h = dvi_v = 0;
  cur_h = h_offset;
  dvi_f = null_font;
  if (dvi_name == null_str) {
    if (job_name == null_str)
      job_name = str_texput;
    pack_job_name(str_dvi);
    while ((dvi_file = b_open_out()) == null_file)
      prompt_file_name("file name for output", str_dvi);
    dvi_name = make_name_str();
  }
  if (total_pages == 0)
    start_dvi();
  page_loc = dvi_offset + dvi_ptr;
  dvi_out(BOP);
  for (k = 0; k <= 9; incr(k))
    dvi_four(count(k));
  dvi_four(last_bop);
  last_bop = page_loc;
  cur_v = box_height(p) + v_offset;
  if (type(p) == VLIST_NODE) 
    vlist_out(p);
  else hlist_out(p);
  dvi_out(EOP);
  incr(total_pages);
  cur_s = -1;

done:
  if (tracing_output <= 0)
    print("]"); 
  dead_cycles = 0;
  update_terminal();
#ifdef STAT
  if (tracing_stats > 1) {
    print_nl("Memory usage before: ");
    print_int(var_used);
    print("&");
    print_int(dyn_used);
    print(";");
  }
#endif
  flush_node_list(p);
#ifdef STAT
  if (tracing_stats > 1) {
    print(" after: ");
    print_int(var_used);
    print("&");
    print_int(dyn_used);
    print_ln();
  }
#endif
}

  void
hlist_out (this_box)
  ptr	this_box;
{
  scal	left_edge, base_line;
  int	save_loc;
  ptr	p;

  p = list_ptr(this_box);
  incr(cur_s);
  if (cur_s > 0)
    dvi_out(PUSH);
  if (cur_s > max_push)
    max_push = cur_s;
  save_loc = dvi_offset + dvi_ptr;
  left_edge = cur_h;
  base_line = cur_v;
  while (p != null) {
reswitch:
    if (is_char_node(p)) {
      synch_h();
      synch_v();
      do {
        int f = font(p);
        int c = character(p);
        if (f != dvi_f) {
          fnt_out(f);
          dvi_f = f;
        }
        if (c >= 128)
          dvi_out(SET1);
        dvi_out(c);
        cur_h += char_width(f, char_info(f, c));
        p = link(p);
      } while (is_char_node(p));
      dvi_h = cur_h;
    } else {
      switch (type(p))
      {
        case HLIST_NODE:
        case VLIST_NODE:
          if (list_ptr(p) == null) {
            cur_h += box_width(p);
          } else {
            scal save_h = dvi_h;
            scal save_v = dvi_v;
            scal edge = cur_h;
            cur_v = base_line + shift_amount(p);
            if (type(p) == VLIST_NODE)
              vlist_out(p);
            else hlist_out(p);
            dvi_h = save_h;
            dvi_v = save_v;
            cur_h = edge + box_width(p);
            cur_v = base_line;
          }
          break;

        case RULE_NODE:
          rule_wd = rule_width(p);
          rule_ht = rule_height(p);
          rule_dp = rule_depth(p);
          hrule_out(this_box, base_line);
          break;

        case WHATSIT_NODE:
          out_whatsit(p);
          break;

        case GLUE_NODE:
          hskip_out(p, this_box, left_edge, base_line);
          break;

        case KERN_NODE:
          cur_h += kern_width(p);
          break;

        case MATH_NODE:
          cur_h += math_width(p);
          break;

        case LIGATURE_NODE:
          p = make_char_from_lig(p);
          goto reswitch;

        default:
          break;
      }
      p = link(p);
    }
  }
  prune_movements(save_loc);
  if (cur_s > 0)
    dvi_pop(save_loc);
  decr(cur_s);
}

  void
hskip_out (p, this_box, left_edge, base_line)
  ptr	p, this_box;
  scal	left_edge, base_line;
{
  ptr	g;
  int	g_sign, g_order;
  ptr	leader_box;
  scal	leader_wd;
  scal	edge, lx, lq, lr;
  bool	outer_doing_leaders;

  g = glue_ptr(p);
  rule_wd = glue_width(g);
  g_order = glue_order(this_box);
  g_sign = glue_sign(this_box);
  if (g_sign != NORMAL) {
    if (g_sign == STRETCHING) {
      if (stretch_order(g) == g_order)
        rule_wd += round(glue_set(this_box) *
            stretch(g));
    } else {
      if (shrink_order(g) == g_order)
        rule_wd -= round(glue_set(this_box) *
            shrink(g));
    }
  }
  if (subtype(p) >= A_LEADERS) {
    leader_box = leader_ptr(p);
    if (type(leader_box) == RULE_NODE) {
      rule_ht = rule_height(leader_box);
      rule_dp = rule_depth(leader_box);
      hrule_out(this_box, base_line);
      return;
    }
    leader_wd = box_width(leader_box);
    if (leader_wd > 0 && rule_wd > 0) {
      scal save_h, save_v;
      rule_wd += 10;
      edge = cur_h + rule_wd;
      lx = 0;
      if (subtype(p) == A_LEADERS) {
        save_h = cur_h;
        cur_h = left_edge;
        cur_h += leader_wd *
          ((save_h - left_edge) / leader_wd);
        if (cur_h < save_h)
          cur_h += leader_wd;
      } else {
        lq = rule_wd / leader_wd;
        lr = rule_wd % leader_wd;
        if (subtype(p) == C_LEADERS) {
          cur_h += lr / 2;
        } else {
          lx = (2 * lr + lq + 1) / (2 * lq + 2);
          cur_h += (lr - (lq - 1) * lx) / 2;
        }
      }
      while (cur_h + leader_wd <= edge) {
        cur_v = base_line + shift_amount(leader_box);
        synch_v();
        save_v = dvi_v;
        synch_h();
        save_h = dvi_h;
        outer_doing_leaders = doing_leaders;
        doing_leaders = TRUE;
        if (type(leader_box) == VLIST_NODE)
          vlist_out(leader_box);
        else hlist_out(leader_box);
        doing_leaders = outer_doing_leaders;
        dvi_v = save_v;
        dvi_h = save_h;
        cur_v = save_v;
        cur_h = save_h + lx + leader_wd;
      }
      cur_h = edge - 10;
      return;
    }
  }
  cur_h += rule_wd;
}

  void
hrule_out (this_box, base_line)
  ptr	this_box;
  int	base_line;
{
  if (is_running(rule_ht))
    rule_ht = box_height(this_box);
  if (is_running(rule_dp))
    rule_dp = box_depth(this_box);
  rule_ht = rule_ht + rule_dp;
  if (rule_ht > 0 && rule_wd > 0) {
    synch_h();
    cur_v = base_line + rule_dp;
    synch_v();
    dvi_out(SET_RULE);
    dvi_four(rule_ht);
    dvi_four(rule_wd);
    cur_v = base_line;
    dvi_h += rule_wd;
  }
  cur_h += rule_wd;
}

  void
vlist_out (this_box)
  ptr	this_box;
{
  scal	left_edge, top_edge;
  int	save_loc;
  ptr	p;

  p = list_ptr(this_box);
  incr(cur_s);
  if (cur_s > 0)
    dvi_out(PUSH);
  if (cur_s > max_push)
    max_push = cur_s;
  save_loc = dvi_offset + dvi_ptr;
  left_edge = cur_h;
  cur_v -= box_height(this_box);
  top_edge = cur_v;
  while (p != null) {
    if (is_char_node(p)) {
      confusion("vlistout");
    } else {
      switch (type(p))
      {
        case HLIST_NODE:
        case VLIST_NODE:
          if (list_ptr(p) == null) {
            cur_v += box_height(p) + box_depth(p);
          } else {
            scal save_h, save_v;
            cur_v += box_height(p);
            synch_v();
            save_h = dvi_h;
            save_v = dvi_v;
            cur_h = left_edge + shift_amount(p);
            if (type(p) == VLIST_NODE)
              vlist_out(p);
            else hlist_out(p);
            dvi_h = save_h;
            dvi_v = save_v;
            cur_v = save_v + box_depth(p);
            cur_h = left_edge;
          }
          break;

        case RULE_NODE:
          rule_ht = rule_height(p);
          rule_dp = rule_depth(p);
          rule_wd = rule_width(p);
          vrule_out(this_box);
          break;

        case WHATSIT_NODE:
          out_whatsit(p);
          break;

        case GLUE_NODE:
          vskip_out(p, this_box, left_edge, top_edge);
          break;

        case KERN_NODE:
          cur_v += kern_width(p);
          break;

        default:
          break;
      }
    }
    p = link(p);
  }
  prune_movements(save_loc);
  if (cur_s > 0)
    dvi_pop(save_loc);
  decr(cur_s);
}

  void
vskip_out (p, this_box, left_edge, top_edge)
  ptr	p, this_box;
  scal	left_edge, top_edge;
{
  ptr	g;
  int	g_sign, g_order;
  ptr	leader_box;
  scal	leader_ht;
  scal	edge, lq, lr, lx;
  bool	outer_doing_leaders;

  g = glue_ptr(p);
  rule_ht = glue_width(g);
  g_order = glue_order(this_box);
  g_sign = glue_sign(this_box);
  if (g_sign != NORMAL) {
    if (g_sign == STRETCHING) {
      if (stretch_order(g) == g_order)
        rule_ht += round(glue_set(this_box) *
            stretch(g));
    } else if (shrink_order(g) == g_order)
      rule_ht -= round(glue_set(this_box) *
          shrink(g));
  }
  if (subtype(p) >= A_LEADERS) {
    leader_box = leader_ptr(p);
    if (type(leader_box) == RULE_NODE) {
      rule_wd = box_width(leader_box);
      rule_dp = 0;
      vrule_out(this_box);
      return;
    }
    leader_ht = box_height(leader_box) + box_depth(leader_box);
    if (leader_ht > 0 && rule_ht > 0) {
      scal save_h, save_v;
      rule_ht += 10;
      edge = cur_v + rule_ht;
      lx = 0;
      if (subtype(p) == A_LEADERS) {
        save_v = cur_v;
        cur_v = top_edge;
        cur_v += leader_ht *
          ((cur_v - top_edge) / leader_ht);
        if (cur_v < save_v)
          cur_v += leader_ht;
      } else {
        lq = rule_ht / leader_ht;
        lr = rule_ht % leader_ht;
        if (subtype(p) == C_LEADERS) {
          cur_v += lr / 2;
        } else {
          lx = (2 * lr + lq + 1) / (2 * lq + 2);
          cur_v += (lr - (lq - 1) * lx) / 2;
        }
      }
      while (cur_v + leader_ht <= edge) {
        cur_h = left_edge + shift_amount(leader_box);
        synch_h();
        save_h = dvi_h;
        cur_v += box_height(leader_box);
        synch_v();
        save_v = dvi_v;
        outer_doing_leaders = doing_leaders;
        doing_leaders = TRUE;
        if (type(leader_box) == VLIST_NODE) 
          vlist_out(leader_box);
        else hlist_out(leader_box);
        doing_leaders = outer_doing_leaders;
        cur_v = dvi_v = save_v;
        cur_h = dvi_h = save_h;
        cur_v += lx + leader_ht -
          box_height(leader_box);
      }
      cur_v = edge - 10;
      return;
    }
  }
  cur_v += rule_ht;
}

  void
vrule_out (this_box)
  ptr	this_box;
{
  if (is_running(rule_wd))
    rule_wd = box_width(this_box);
  rule_ht += rule_dp;
  cur_v += rule_ht;
  if (rule_ht > 0 && rule_wd > 0) {
    synch_h();
    synch_v();
    dvi_out(PUT_RULE);
    dvi_four(rule_ht);
    dvi_four(rule_wd);
  }
}

  void
dvi_swap ()
{
  if (dvi_limit == DVI_BUF_SIZE) {
    write_dvi(0, HALF_BUF);
    dvi_limit = HALF_BUF;
    dvi_offset += DVI_BUF_SIZE;
    dvi_ptr = 0;
  } else {
    write_dvi(HALF_BUF, DVI_BUF_SIZE);
    dvi_limit = DVI_BUF_SIZE;
  }
  dvi_gone += HALF_BUF;
}

  void
dvi_four (x)
  int	x;
{
  if (x >= 0) {
    dvi_out(x / 0100000000);
  } else {
    x += 010000000000;
    x += 010000000000;
    dvi_out(x / 0100000000 + 128);
  }
  x %= 01000000000;
  dvi_out(x / 0200000);
  x %= 0200000;
  dvi_out(x / 0400);
  dvi_out(x % 0400);
}

  void
dvi_pop (l)
  int	l;
{
  if (l == dvi_offset + dvi_ptr && dvi_ptr > 0)
    decr(dvi_ptr);
  else dvi_out(POP);
}

  void
fnt_out (f)
  fnt	f;
{
  if (!font_used(f)) {
    dvi_font_def(f);
    font_used(f) = TRUE;
  }
  if (f <= 64 + null_font) {
    dvi_out(f - null_font - 1 + FNT_NUM_0);
  } else {
    dvi_out(FNT1);
    dvi_out(f - null_font - 1);
  }
}

  void
dvi_font_def (f)
  fnt	f;
{
  str	s;

  dvi_out(FNT_DEF1);
  dvi_out(f - null_font - 1);
  dvi_out(font_check(f).b0);
  dvi_out(font_check(f).b1);
  dvi_out(font_check(f).b2);
  dvi_out(font_check(f).b3);
  dvi_four(font_size(f));
  dvi_four(font_dsize(f));
  dvi_out(str_length(font_area(f)));
  dvi_out(str_length(font_name(f)));
  for (s = font_area(f); *s != NUL; incr(s))
    dvi_out(*s);
  for (s = font_name(f); *s != NUL; incr(s))
    dvi_out(*s);
}

#define Y_HERE		1
#define Z_HERE		2
#define YZ_OK		3
#define Y_OK		4
#define Z_OK		5
#define D_FIXED		6
#define NONE_SEEN	0
#define Y_SEEN		6
#define Z_SEEN		12

  void
movement (m, o)
  scal	m;
  int	o;
{
  int	k;
  ptr	p;
  ptr	q;
  int	mstate;

  q = new_node(MOVEMENT_NODE_SIZE);
  move_amount(q) = m;
  location(q) = dvi_offset + dvi_ptr;
  if (o == DOWN1) {
    link(q) = down_ptr;
    down_ptr = q;
  } else {
    link(q) = right_ptr;
    right_ptr = q;
  }
  mstate = NONE_SEEN;
  for (p = link(q); p != null; p = link(p)) {
    if (move_amount(p) == m) {
      switch (mstate + info(p))
      {
        case NONE_SEEN + YZ_OK:
        case NONE_SEEN + Y_OK:
        case Z_SEEN + YZ_OK:
        case Z_SEEN + Y_OK:
          if (location(p) < dvi_gone) {
            goto not_found;
          } else {
            k = location(p) - dvi_offset;
            if (k < 0)
              k += DVI_BUF_SIZE;
            dvi_buf[k] += Y1 - DOWN1;
            info(p) = Y_HERE;
            goto found;
          }

        case NONE_SEEN + Z_OK:
        case Y_SEEN + YZ_OK:
        case Y_SEEN + Z_OK:
          if (location(p) < dvi_gone) {
            goto not_found;
          } else {
            k = location(p) - dvi_offset;
            if (k < 0)
              k += DVI_BUF_SIZE;
            dvi_buf[k] += Z1 - DOWN1;
            info(p) = Z_HERE;
            goto found;
          }

        case NONE_SEEN + Y_HERE:
        case NONE_SEEN + Z_HERE:
        case Y_SEEN + Z_HERE:
        case Z_SEEN + Y_HERE:
          goto found;
      }
    } else {
      switch (mstate + info(p))
      {
        case NONE_SEEN + Y_HERE:
          mstate = Y_SEEN;
          break;

        case NONE_SEEN + Z_HERE:
          mstate = Z_SEEN;
          break;

        case Y_SEEN + Z_HERE:
        case Z_SEEN + Y_HERE:
          goto not_found;

        default:
          break;
      }
    }
  }

not_found:
  info(q) = YZ_OK;
  if (abs(m) >= 040000000) {
    dvi_out(o + 3);
    dvi_four(m);
    return;
  }
  if (abs(m) >= 0100000) {
    dvi_out(o + 2);
    if (m < 0)
      m += 0100000000;
    dvi_out(m / 0200000);
    m %= 0200000;
    goto two;
  }
  if (abs(m) >= 0200) {
    dvi_out(o + 1);
    if (m < 0)
      m += 0200000;
    goto two;
  }
  dvi_out(o);
  if (m < 0)
    m += 0400;
  goto one;

two: dvi_out(m / 0400);
one: dvi_out(m % 0400);
     return;

found:
     info(q) = info(p);
     if (info(q) == Y_HERE) {
       dvi_out(o + Y0 - DOWN1);
       while (link(q) != p) {
         q = link(q);
         switch (info(q))
         {
           case YZ_OK:
             info(q) = Z_OK;
             break;

           case Y_OK:
             info(q) = D_FIXED;
             break;
         }
       }
     } else {
       dvi_out(o + Z0 - DOWN1);
       while (link(q) != p) {
         q = link(q);
         switch (info(q))
         {
           case YZ_OK:
             info(q) = Y_OK;
             break;

           case Z_OK:
             info(q) = D_FIXED;
             break;
           default:
             break;
         }
       }
     }
}

  void
prune_movements (l)
  int	l;
{
  ptr	p;

  while (down_ptr != null) {
    if (location(down_ptr) < l)
      break;
    p = down_ptr;
    down_ptr = link(p);
    free_node(p, MOVEMENT_NODE_SIZE);
  }
  while (right_ptr != null) {
    if (location(right_ptr) < l)
      break;
    p = right_ptr;
    right_ptr = link(p);
    free_node(p, MOVEMENT_NODE_SIZE);
  }
}

  void
start_dvi ()
{
  str	s;
  int	old_setting;

  dvi_out(PRE);
  dvi_out(ID_BYTE); 
  dvi_four(25400000);
  dvi_four(473628672);
  prepare_mag();
  dvi_four(mag); 
  old_setting = selector;
  selector = NEW_STRING;
  print(" Common TeX output "); 
  print_int(year);
  print(".");
  print_two(month);
  print(".");
  print_two(day);
  print(":");
  print_two(time / 60);
  print_two(time % 60);
  selector = old_setting;
  dvi_out(cur_length());
  for (s = cur_str; s < cur_str_ptr; incr(s))
    dvi_out(*s);
  flush_str();
}

  void
fin_dvi ()
{
  int	f;

  while (cur_s > -1) {
    if (cur_s > 0) {
      dvi_out(POP);
    } else {
      dvi_out(EOP);
      incr(total_pages);
    }
    decr(cur_s);
  }
  if (total_pages == 0) {
    print_nl("No pages of output.");
    return;
  }
  dvi_out(POST);
  dvi_four(last_bop);
  last_bop = dvi_offset + dvi_ptr - 5;
  dvi_four(25400000);
  dvi_four(473628672);
  prepare_mag();
  dvi_four(mag);
  dvi_four(max_v);
  dvi_four(max_h);
  dvi_out(max_push / 256);
  dvi_out(max_push % 256);
  dvi_out(total_pages / 256 % 256);
  dvi_out(total_pages % 256);
  for (f = null_font + font_ptr - font_info - 1; f > null_font; decr(f))
    if (font_used(f))
      dvi_font_def(f);
  dvi_out(POST_POST);
  dvi_four(last_bop);
  dvi_out(ID_BYTE);
  for (f = 4 + (DVI_BUF_SIZE - dvi_ptr) % 4; f > 0; decr(f))
    dvi_out(223);
  if (dvi_limit == HALF_BUF)
    write_dvi(HALF_BUF, DVI_BUF_SIZE);
  if (dvi_ptr > 0)
    write_dvi(0, dvi_ptr);
  print_nl("Output written on ");
  print(dvi_name);
  print(" (");
  print_int(total_pages);
  print(" page");
  if (total_pages != 1)
    print("s");
  print(", ");
  print_int(dvi_offset + dvi_ptr);
  print(" bytes).");
  b_close(dvi_file);
}

  void
_dvi_init ()
{
  dvi_limit = DVI_BUF_SIZE;
  dvi_name = null_str;
  last_bop = -1;
  cur_s = -1;
}

  void
_dvi_init_once ()
{
  dvi_buf = (byte *)malloc(DVI_BUF_SIZE);
  if (dvi_buf == (byte *) 0) {
    overflow("dvi buffer", DVI_BUF_SIZE);
  }
}

/*
 **	Help text
 */

help_huge_page ()
{
  help2("The page just created is more than 18 feet tall or",
      "more than 18 feet wide, so I suspect something went wrong.");
}

/*
 * Common TeX:error.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX error.c 3.2@(#)";
#endif

#include "tex.h"

int	interaction;
int	history;
int	error_cnt;

bool	interrupted;
bool	OK_to_interrupt;
bool	deletions_allowed;
bool	use_err_help;

str	help_line[6];
int	help_cnt;
int	old_setting;

  void
begin_diagnostic ()
{
  old_setting = selector;
  if (tracing_online <= 0 && selector == TERM_AND_LOG) {
    decr(selector);
    if (history == SPOTLESS)
      history = WARNING_ISSUED;
  }
}

  void
end_diagnostic (blank_line)
  bool	blank_line;
{
  print_nl(null_str);
  if (blank_line)
    print_ln();
  selector = old_setting;
  fflush(stdout);
}

  void
print_err (msg)
  str	msg;
{
  if (interaction == ERROR_STOP_MODE)	
    wake_up_terminal(); 
  print_nl("! ");
  print(msg);
}

  void
error ()
{
  int	c;
  int	hx;
  int	s1, s2, s3, s4;

  if (history < ERROR_MESSAGE_ISSUED)
    history = ERROR_MESSAGE_ISSUED;
  print(".");
  show_context();
  if (interaction == ERROR_STOP_MODE) {
    loop {
      clear_for_error_prompt();
      prompt_input("? ");
      if (cur_length() == 0)
        return;
      c = *cur_str;
      if (c >= 'a')
        c -= 'a' - 'A';
      switch (c)
      {
        case '1': case '2': case '3':
        case '4': case '5': case '6':
        case '7': case '8': case '9':
          if (! deletions_allowed)
            continue;
          s1 = cur_tok;
          s2 = cur_cmd;
          s3 = cur_chr;
          s4 = align_state;
          align_state = 1000000;
          OK_to_interrupt = FALSE;
          if (cur_str_ptr > cur_str + 1 &&
              cur_str[1] >= '0' &&
              cur_str[1] <= '9')
            c = c * 10 + cur_str[1] - '0' * 11;
          else c -= '0';
          while (c-- > 0)
            get_token();
          cur_tok = s1;
          cur_cmd = s2;
          cur_chr = s3;
          align_state = s4;
          OK_to_interrupt = TRUE;
          help_delete_text();
          show_context();
          continue;

        case 'H':
          if (use_err_help)  {
            give_err_help();
            use_err_help = FALSE;
          } else {
            if (help_cnt == 0) {
              help_no_help();
            } else for (hx = 0;
                hx < help_cnt;
                incr(hx)) {
              print(help_line[hx]);
              print_ln();
            }
            help_help();
          }
          continue;

        case 'I':
          begin_file_reading();
          if (cur_length() > 1) {
            *cur_str = ' ';
          } else {
            prompt_input("insert>");
          }
          next = buffer;
          limit = buffer + cur_length() - 1;
          memcpy(buffer, cur_str, cur_length());
          flush_str();
          return;

        case 'Q':
        case 'R':
        case 'S':
          error_cnt = 0;
          interaction = BATCH_MODE + c - 'Q';
          print("OK, entering ");
          switch (c)
          {
            case 'Q':
              print_esc("batchmode");
              decr(selector);
              break;

            case 'R':
              print_esc("nonstopmode");
              break;

            case 'S':
              print_esc("scrollmode");
              break;
          }
          print("...");
          print_ln();
          update_terminal();
          return;

        case 'E':
          if (base_ptr > input_stack)
            close_files_and_terminate(TRUE, FALSE);
          break;

        case 'X':
          interaction = SCROLL_MODE;
          jump_out();
          break;

        default:
          print_menu();
          continue;
      }
    }
  }
  incr(error_cnt);
  if (error_cnt == 100) {
    print_nl("(That makes 100 errors; please try again.)");
    history = FATAL_ERROR_STOP;
    jump_out();
  }
  if (interaction > BATCH_MODE)
    decr(selector);
  if (use_err_help) {
    print_ln();
    give_err_help();
  } else for (hx = 0; hx < help_cnt; incr(hx)) {
    print_nl(help_line[hx]);
  }
  help_cnt = 0;
  print_ln();
  if (interaction > BATCH_MODE)
    incr(selector);
  print_ln();
}

print_menu ()
{
  print("Type <return> to proceed, S to scroll future error messages,");
  print_nl("R to run without stopping, Q to run quietly,");
  print_nl("I to insert something, ");
  if (base_ptr > input_stack)
    print("E to edit your file,");
  if (deletions_allowed)
    print_nl("1 or ... or 9 to ignore the next 1 to 9 tokens of input");
  print_nl("H for help, X to quit.");
}

  void
int_error (i)
  int	i;
{
  print(" (");
  print_int(i);
  print(")");
  error();
}

  void
normalize_selector ()
{
  if (log_opened)
    selector = TERM_AND_LOG;
  else selector = TERM_ONLY;
  if (job_name == null_str)
    open_log_file();
  if (interaction == BATCH_MODE)
    decr(selector);
}

  void
jump_out ()
{
  close_files_and_terminate(FALSE, FALSE);
}

  void
succumb()
{
  if (interaction == ERROR_STOP_MODE)
    interaction = SCROLL_MODE;
  if (log_opened)
    error();
  history = FATAL_ERROR_STOP;
  jump_out();
}

  void
fatal_error (s)
  str	s;
{
  normalize_selector();
  print_err("Emergency stop");
  help1(s);
  succumb();
}

  void
overflow (s, n)
  str	s;
  int	n;
{
  normalize_selector();
  print_err("TeX capacity exceeded, sorry [");
  print(s);
  print("=");
  print_int(n);
  print("]");
  help_capacity();
  succumb();
}

  void
confusion (s)
  str	s;
{
  normalize_selector();
  if (history < ERROR_MESSAGE_ISSUED) {
    print_err("This can't happen (");
    print(s);
    print(")");
    help_broken();
  } else {
    print_err("I can't go on meeting you like this");
    help_wounded();
  }
  succumb();
}

  void
pause_for_instructions ()
{
  if (OK_to_interrupt) {
    interaction = ERROR_STOP_MODE;
    if (selector == LOG_ONLY || selector == NO_PRINT)
      incr(selector);
    print_err("Interruption");
    help_interrupt();
    deletions_allowed = FALSE;
    error();
    deletions_allowed = TRUE;
    interrupted = 0;
  }
}

  void
_error_init ()
{
  interrupted = FALSE;
  OK_to_interrupt = TRUE;
  deletions_allowed = TRUE;
  history = FATAL_ERROR_STOP;
  use_err_help = FALSE;
  error_cnt = 0;
  help_cnt = 0;
}

  void
_error_init_once ()
{
  interrupted = FALSE;
  OK_to_interrupt = TRUE;
  deletions_allowed = TRUE;
  history = FATAL_ERROR_STOP;
  interaction = ERROR_STOP_MODE;
  use_err_help = FALSE;
  error_cnt = 0;
  help_cnt = 0;
}

/*
 **	Help text
 */

help_delete_text () 
{
  help2("I have just deleted some text, as you asked.", 
      "You can now delete more, or insert, or whatever.");
}

help_no_help () 
{
  help2("Sorry, I don't know how to help in this situation.",
      "Maybe you should try asking a human?");
}

help_help () 
{
  help4("Sorry, I already gave what help I could...",
      "Maybe you should try asking a human?", 
      "An error might have occurred before I noticed any problems.",
      "``If all else fails, read the instructions.''");
}

help_capacity () 
{
  help2("If you really absolutely need more capacity,",
      "you can ask a wizard to enlarge me.");
}

help_broken () 
{
  help1("I'm broken. Please show this to someone who can fix can fix");
}

help_wounded () 
{
  help2("One of your faux pas seems to have wounded me deeply...",
      "in fact, I'm barely conscious. Please fix it and try again.");
}

help_interrupt () 
{
  help3("You rang?",
      "Try to insert some instructions for me (e.g., `I\\showlists),",
      "unless you just want to quit by typing `X'.");
}

/*
 * Common TeX:eval.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX eval.c 3.2@(#)";
#endif

#include "tex.h"

#define NEST_SIZE	40
#define NEST_INC	20

list	cur_list;
int	nlists;
list	*nest;
list	*nest_end;
list	*nest_ptr;
list	*max_nest_stack;
int	shown_mode;

static fnt	main_f;
static qcell	main_i;
static qcell	main_j;
static qcell	*main_k;
static ptr	main_p;
static scal	*main_q;
static int	main_s;
static ptr	main_t;

int	cur_l;
int	cur_r;
ptr	cur_q;
ptr	lig_stack;
int	bchar;
int	false_bchar;
bool	lft_hit;
bool	rt_hit;
bool	ligature_present;
bool	cancel_boundary;
bool	ins_disc;

  void
push_nest ()
{
  if (nest_ptr > max_nest_stack) {
    max_nest_stack = nest_ptr;
    if (nest_ptr == nest_end && !realloc_nest()) {
      overflow("semantic nest size", nlists);
    }
  }
  *nest_ptr++ = cur_list;
  tail = head = new_avail();
  prev_graf = 0;
  mode_line = line;
}

  void
pop_nest ()
{
  free_avail(head);
  cur_list = *--nest_ptr;
}

  bool
realloc_nest ()
{
  list *tmp;

  nlists += NEST_INC;
  tmp = (list *)realloc(nest, nlists * sizeof(list));
  if (tmp == (list *) 0) {
    overflow("nest", nlists);
  }
  nest = tmp;
  nest_end = nest + nlists - 1;
  nest_ptr = nest + nlists - NEST_INC;
  return TRUE;
}

  void
print_mode (m)
  int	m;
{
  if (m > 0) {
    switch (m / (MAX_COMMAND + 1)) {
      case 0: print("vertical"); break;
      case 1: print("horizontal"); break;
      case 2: print("display math"); break;
    }
  } else if (m == 0) {
    print("no");
  } else {	
    switch (-m / (MAX_COMMAND + 1)) {
      case 0: print("internal vertical"); break;
      case 1: print("restricted horizontal"); break;
      case 2: print("math"); break;
    }
  }
  print(" mode");
}

#define vmode(CMD) \
  case VMODE + CMD

#define hmode(CMD) \
  case HMODE + CMD

#define non_math(CMD) \
  case VMODE + CMD: \
case HMODE + CMD

#define mmode(CMD) \
  case MMODE + CMD

#define any_mode(CMD) \
  case VMODE + CMD: \
case HMODE + CMD: \
case MMODE + CMD

  void
main_control ()
{
  if (every_job != null)
    begin_token_list(every_job, EVERY_JOB_TEXT);

big_switch:
  get_x_token();

reswitch:
  if (interrupted && OK_to_interrupt) {
    back_input();
    pause_for_instructions();
    goto big_switch;
  }

  if (tracing_commands > 0)
    show_cur_cmd_chr();

  switch (abs(mode) + cur_cmd)
  {
    hmode(LETTER):
      hmode(OTHER_CHAR):
        hmode(CHAR_GIVEN):
          goto main_loop;

    hmode(CHAR_NUM):
      scan_char_num();
    cur_chr = cur_val;
    goto main_loop;

    hmode(NO_BOUNDARY):
      get_x_token();
    if (cur_cmd == LETTER || cur_cmd == OTHER_CHAR
        || cur_cmd == CHAR_GIVEN || cur_cmd == CHAR_NUM) {
      cancel_boundary = TRUE;
    }
    goto reswitch;

    hmode(SPACER):
      if (space_factor == 1000) {
        goto append_normal_space;
      } else {
        app_space();
      }
    break;

    hmode(EX_SPACE):
      mmode(EX_SPACE):
        goto append_normal_space;

    any_mode(RELAX):
      vmode(SPACER):
        mmode(SPACER):
          mmode(NO_BOUNDARY):
            break;

    any_mode(IGNORE_SPACES):
      get_nbx_token();
    goto reswitch;

    vmode(STOP):
      if (its_all_over())
      return;
    break;

    any_mode(LAST_ITEM):
      any_mode(MAC_PARAM):
        non_math(EQ_NO):
          vmode(VMOVE):
            hmode(HMOVE):
              mmode(HMOVE):
                vmode(VADJUST):
                  vmode(ITAL_CORR):
                    report_illegal_case();
    break;

    non_math(SUP_MARK):
      non_math(SUB_MARK):
        non_math(MATH_CHAR_NUM):
          non_math(MATH_GIVEN):
            non_math(MATH_COMP):
              non_math(DELIM_NUM):
                non_math(LEFT_RIGHT):
                  non_math(ABOVE):
                    non_math(RADICAL):
                      non_math(MATH_STYLE):
                        non_math(MATH_CHOICE):
                          non_math(VCENTER):
                            non_math(NON_SCRIPT):
                              non_math(MKERN):
                                non_math(LIMIT_SWITCH):
                                  non_math(MSKIP):
                                    non_math(MATH_ACCENT):
                                      mmode(ENDV):
                                        mmode(PAR_END):
                                          mmode(STOP):
                                            mmode(VSKIP):
                                              mmode(UN_VBOX):
                                                mmode(VALIGN):
                                                  mmode(HRULE):
                                                    insert_dollar_sign();
    break;

    vmode(HRULE):
      hmode(VRULE):
        mmode(VRULE):
          tail_append(scan_rule_spec());
    if (abs(mode) == VMODE)
      prev_depth = IGNORE_DEPTH;
    else if (abs(mode) == HMODE)
      space_factor = 1000;
    break;

    vmode(VSKIP):
      hmode(HSKIP):
        mmode(HSKIP):
          mmode(MSKIP):
            append_glue();
    break;

    any_mode(KERN):
      mmode(MKERN):
        append_kern();
    break;

    non_math(LEFT_BRACE):
      new_save_level(SIMPLE_GROUP);
    break;

    any_mode(BEGIN_GROUP):
      new_save_level(SEMI_SIMPLE_GROUP);
    break;

    any_mode(END_GROUP):
      if (cur_group == SEMI_SIMPLE_GROUP) {
        unsave();
      } else {
        off_save();
      }
    break;

    any_mode(RIGHT_BRACE):
      handle_right_brace();
    break;

    vmode(HMOVE):
      hmode(VMOVE):
        mmode(VMOVE): {
          int	t;

          t = cur_chr;
          scan_normal_dimen();
          if (t == 0) {
            scan_box(cur_val);
          } else {
            scan_box(-cur_val);
          }
          break;
        }
    any_mode(LEADER_SHIP):
      scan_box(LEADER_FLAG + cur_chr - A_LEADERS);
    break;

    any_mode(MAKE_BOX):
      begin_box(0);
    break;

    vmode(START_PAR):
      new_graf(cur_chr > 0);
    break;

    vmode(LETTER):
      vmode(OTHER_CHAR):
        vmode(CHAR_NUM):
          vmode(CHAR_GIVEN):
            vmode(MATH_SHIFT):
              vmode(UN_HBOX):
                vmode(VRULE):
                  vmode(ACCENT):
                    vmode(DISCRETIONARY):
                      vmode(HSKIP):
                        vmode(VALIGN):
                          vmode(EX_SPACE):
                            vmode(NO_BOUNDARY):
                              back_input();
    new_graf(TRUE);
    break;

    hmode(START_PAR):
      mmode(START_PAR):
        indent_in_hmode();
    break;

    vmode(PAR_END):
      normal_paragraph();
    if (mode > 0)
      build_page();
    break;

    hmode(PAR_END):
      if (align_state < 0)
      off_save();
    end_graf();
    if (mode == VMODE)	
      build_page();
    break;

    hmode(STOP):
      hmode(VSKIP):
        hmode(HRULE):
          hmode(UN_VBOX):
            hmode(HALIGN):
              head_for_vmode();
    break;

    any_mode(INSERT):
      hmode(VADJUST):
        mmode(VADJUST):
          begin_insert_or_adjust();
    break;

    any_mode(MARK):
      make_mark();
    break;

    any_mode(BREAK_PENALTY):
      append_penalty();
    break;

    any_mode(REMOVE_ITEM):
      delete_last();
    break;

    vmode(UN_VBOX):
      hmode(UN_HBOX):
        mmode(UN_HBOX):
          unpackage();
    break;

    hmode(ITAL_CORR):
      append_italic_correction();
    break;

    mmode(ITAL_CORR):
      tail_append(new_kern(0));
    break;

    hmode(DISCRETIONARY):
      mmode(DISCRETIONARY):
        append_discretionary();
    break;

    hmode(ACCENT):
      make_accent();
    break;

    any_mode(CAR_RET):
      any_mode(TAB_MARK):
        align_error();
    break;

    any_mode(NO_ALIGN):
      no_align_error();
    break;

    any_mode(OMIT):
      omit_error();
    break;

    vmode(HALIGN):
      hmode(VALIGN):
        init_align();
    break;

    mmode(HALIGN):
      if (privileged()) {
        if (cur_group == MATH_SHIFT_GROUP) {
          init_align();
        } else {
          off_save();
        }
      }
    break;

    vmode(ENDV):
      hmode(ENDV):
        do_endv();
    break;

    any_mode(END_CS_NAME):
      cs_error();
    break;

    hmode(MATH_SHIFT):
      init_math();
    break;

    mmode(EQ_NO):
      if (privileged()) {
        if (cur_group == MATH_SHIFT_GROUP) {
          start_eq_no();
        } else {
          off_save();
        }
      }
    break;

    mmode(LEFT_BRACE):
      tail_append(new_noad());
    back_input();
    scan_math(nucleus(tail));
    break;

    mmode(LETTER):
      mmode(OTHER_CHAR):
        mmode(CHAR_GIVEN):
          if (cur_chr < 256)
          set_math_char(math_code(cur_chr));
          else set_math_char(cur_chr);
    break;

    mmode(CHAR_NUM):
      scan_char_num();
    cur_chr = cur_val;
    if (cur_chr < 256)
      set_math_char(math_code(cur_chr));
    else set_math_char(cur_chr);
    break;

    mmode(MATH_CHAR_NUM):
      scan_fifteen_bit_int();
    set_math_char(cur_val);
    break;

    mmode(MATH_GIVEN):
      set_math_char(cur_chr);
    break;

    mmode(DELIM_NUM):
      scan_twenty_seven_bit_int();
    set_math_char(cur_val / 010000);
    break;

    mmode(MATH_COMP):
      tail_append(new_noad());
    type(tail) = cur_chr;
    scan_math(nucleus(tail));
    break;

    mmode(LIMIT_SWITCH):
      math_limit_switch();
    break;

    mmode(RADICAL):
      math_radical();
    break;

    mmode(ACCENT):
      mmode(MATH_ACCENT):
        math_ac();
    break;

    mmode(VCENTER):
      scan_spec(VCENTER_GROUP, FALSE);
    normal_paragraph();
    push_nest();
    mode = -VMODE;
    prev_depth = IGNORE_DEPTH;
    if (every_vbox != null)
      begin_token_list(every_vbox, EVERY_VBOX_TEXT);
    break;

    mmode(MATH_STYLE):
      tail_append(new_style(cur_chr));
    break;

    mmode(NON_SCRIPT):
      tail_append(new_glue(zero_glue));
    subtype(tail) = COND_MATH_GLUE;
    break;

    mmode(MATH_CHOICE):
      append_choices();
    break;

    mmode(SUB_MARK):
      mmode(SUP_MARK):
        sub_sup();
    break;

    mmode(ABOVE):
      math_fraction();
    break;

    mmode(LEFT_RIGHT):
      math_left_right();
    break;

    mmode(MATH_SHIFT):
      if (cur_group == MATH_SHIFT_GROUP) {
        after_math();
      } else {
        off_save();
      }
    break;

    any_mode(AFTER_ASSIGNMENT):
      get_token();
    after_token = cur_tok;
    break;

    any_mode(AFTER_GROUP):
      get_token();
    save_for_after(cur_tok);
    break;

    any_mode(IN_STREAM):
      clopen_stream();
    break;

    any_mode(MESSAGE):
      issue_message();
    break;

    any_mode(CASE_SHIFT):
      shift_case();
    break;

    any_mode(XRAY):
      show_whatever();
    break;

    any_mode(EXTENSION):
      do_extension();
    break;

    default:
    prefixed_command();
    break;
  }
  goto big_switch;

#define adjust_space_factor() \
  { \
    main_s = sf_code(cur_chr); \
    if (main_s == 1000) { \
      space_factor = 1000; \
    } else if (main_s < 1000) { \
      if (main_s > 0) { \
        space_factor = main_s; \
      } \
    } else if (space_factor < 1000) { \
      space_factor = 1000; \
    } else { \
      space_factor = main_s; \
    } \
  }

main_loop:
  adjust_space_factor();
  main_f = cur_font;
  bchar = font_bchar(main_f);
  false_bchar = font_false_bchar(main_f);
  if (mode > 0 && language != clang)
    fix_language();
  fast_new_avail(lig_stack);
  font(lig_stack) = main_f;
  character(lig_stack) = cur_l = cur_chr;
  cur_q = tail;
  if (cancel_boundary) {
    cancel_boundary = FALSE;
    main_k = NON_ADDRESS;
  } else {
    main_k = bchar_label(main_f);
  }
  if (main_k == NON_ADDRESS)
    goto main_loop_move_2;
  cur_r = cur_l;
  cur_l = NON_CHAR;
  goto main_lig_loop_1;

#define pack_lig(RT_HIT) \
  { \
    main_p = new_ligature(main_f, cur_l, link(cur_q)); \
    if (lft_hit) { \
      subtype(main_p) = 2; \
      lft_hit = FALSE; \
    } \
    if (RT_HIT && lig_stack == null) { \
      incr(subtype(main_p)); \
      rt_hit = FALSE; \
    } \
    tail = link(cur_q) = main_p; \
    ligature_present = FALSE; \
  }

#define wrapup(RT_HIT) \
  { \
    if (cur_l < NON_CHAR) { \
      if (character(tail) == hyphen_char(main_f) \
          && link(cur_q) != null) \
      ins_disc = TRUE; \
      if (ligature_present) \
      pack_lig(RT_HIT); \
      if (ins_disc) { \
        ins_disc = FALSE; \
        if (mode > 0) \
        tail_append(new_disc()); \
      } \
    } \
  }

main_loop_wrapup:
  wrapup(rt_hit);

main_loop_move:
  if (lig_stack == null)
    goto reswitch;
  cur_q = tail;
  cur_l = cur_r;

main_loop_move_1:
  if (!is_char_node(lig_stack))
    goto main_loop_move_lig;

main_loop_move_2:
  if (cur_chr < font_bc(main_f) || cur_chr > font_ec(main_f)) {
    char_warning(main_f, cur_chr);
    free_avail(lig_stack);
    goto big_switch;
  }
  main_i = char_info(main_f, cur_l);
  if (!char_exists(main_i)) {
    char_warning(main_f, cur_chr);
    free_avail(lig_stack);
    goto big_switch;
  }
  tail_append(lig_stack);

main_loop_lookahead:
  get_next();
  if (cur_cmd == LETTER
      || cur_cmd == OTHER_CHAR
      || cur_cmd == CHAR_GIVEN) {
    goto main_loop_lookahead_1;
  }
  x_token();
  if (cur_cmd == LETTER
      || cur_cmd == OTHER_CHAR
      || cur_cmd == CHAR_GIVEN) {
    goto main_loop_lookahead_1;
  }
  if (cur_cmd == CHAR_NUM) {
    scan_char_num();
    cur_chr = cur_val;
    goto main_loop_lookahead_1;
  }
  if (cur_cmd == NO_BOUNDARY) {
    bchar = NON_CHAR;
  }
  cur_r = bchar;
  lig_stack = null;
  goto main_lig_loop;

main_loop_lookahead_1:
  adjust_space_factor();
  fast_new_avail(lig_stack);
  font(lig_stack) = main_f;
  character(lig_stack) = cur_r = cur_chr;
  if (cur_r == false_bchar) {
    cur_r = NON_CHAR;
  }

main_lig_loop:
  if (char_tag(main_i) != LIG_TAG) {
    goto main_loop_wrapup;
  }
  main_k = lig_kern_start(main_f, main_i);
  main_j = *main_k;
  if (skip_byte(main_j) <= STOP_FLAG) {
    goto main_lig_loop_2;
  }
  main_k = lig_kern_restart(main_f, main_j);

main_lig_loop_1:
  main_j = *main_k;

main_lig_loop_2:
  if (next_char(main_j) == cur_r
      && skip_byte(main_j) <= STOP_FLAG) {
    if (op_byte(main_j) >= KERN_FLAG) {
      wrapup(rt_hit);
      tail_append(new_kern(char_kern(main_f, main_j)));
      goto main_loop_move;
    }
    if (cur_l == NON_CHAR) {
      lft_hit = TRUE;
    } else if (lig_stack == null) {
      rt_hit = TRUE;
    }
    check_interrupt();
    switch (op_byte(main_j))
    {
      case 1: case 5:
        cur_l = rem_byte(main_j);
        main_i = char_info(main_f, cur_l);
        ligature_present = TRUE;
        break;

      case 2: case 6:
        cur_r = rem_byte(main_j);
        if (lig_stack == null) {
          lig_stack = new_lig_item(cur_r);
          bchar = NON_CHAR;
        } else if (is_char_node(lig_stack)) {
          main_p = lig_stack;
          lig_stack = new_lig_item(cur_r);
          lig_ptr(lig_stack) = main_p;
        } else {
          character(lig_stack) = cur_r;
        }
        break;

      case 3:
        cur_r = rem_byte(main_j);
        main_p = lig_stack;
        lig_stack = new_lig_item(cur_r);
        link(lig_stack) = main_p;
        break;

      case 7: case 11:
        wrapup(FALSE);
        cur_q = tail;
        cur_l = rem_byte(main_j);
        main_i = char_info(main_f, cur_l);
        ligature_present = TRUE;
        break;

      default:
        cur_l = rem_byte(main_j);
        ligature_present = TRUE;
        if (lig_stack == null) {
          goto main_loop_wrapup;
        } else {
          goto main_loop_move_1;
        }
        break;
    }
    if (op_byte(main_j) > 4 && op_byte(main_j) != 7)
      goto main_loop_wrapup;
    if (cur_l < NON_CHAR)
      goto main_lig_loop;
    main_k = bchar_label(main_f);
    goto main_lig_loop_1;
  }
  if (skip_byte(main_j) == 0) {
    incr(main_k);
  } else {
    if (skip_byte(main_j) >= STOP_FLAG)
      goto main_loop_wrapup;
    main_k += skip_byte(main_j) + 1;
  }
  goto main_lig_loop_1;

main_loop_move_lig:
  main_p = lig_ptr(lig_stack);
  if (main_p != null) {
    tail_append(main_p);
  }
  main_t = lig_stack;
  lig_stack = link(main_t);
  free_node(main_t, SMALL_NODE_SIZE);
  main_i = char_info(main_f, cur_l);
  ligature_present = TRUE;
  if (lig_stack == null) {
    if (main_p != null) {
      goto main_loop_lookahead;
    } else {
      cur_r = bchar;
    }
  } else {
    cur_r = character(lig_stack);
  }
  goto main_lig_loop;

append_normal_space:
  if (space_skip == zero_glue) {
    main_p = font_glue(cur_font);
    if (main_p == null)
      main_p = find_font_glue(cur_font);
    main_t = new_glue(main_p);
  } else {
    main_t = new_param_glue(SPACE_SKIP_CODE);
  }
  tail_append(main_t);
  goto big_switch;
}

  void
app_space ()
{
  ptr	p, q;

  if (space_factor >= 2000 && xspace_skip != zero_glue) {
    q = new_param_glue(XSPACE_SKIP_CODE);
  } else {
    if (space_skip != zero_glue) {
      p = space_skip;
    } else {
      p = find_font_glue(cur_font);
    }
    p = new_spec(p);
    if (space_factor >= 2000)
      glue_width(p) += extra_space(cur_font);
    stretch(p) = xn_over_d(stretch(p), space_factor, 1000);
    shrink(p) = xn_over_d(shrink(p), 1000L, space_factor);
    q = new_glue(p);
    glue_ref_count(p) = 0;
  }
  tail_append(q);
}

  void
insert_dollar_sign ()
{
  back_input();
  cur_tok = MATH_SHIFT_TOKEN + '$';
  print_err("Missing $ inserted");
  help_dollar();
  ins_error();
}

  void
you_cant ()
{
  print_err("You can't use `");
  print_cmd_chr(cur_cmd, cur_chr);
  print("' in ");
  print_mode(mode);
}

  void
report_illegal_case ()
{
  you_cant();
  help_illegal_case();
  error();
}

  bool
privileged ()
{
  if (mode > 0)
    return TRUE;
  report_illegal_case();
  return FALSE;
}

  bool
its_all_over ()
{
  if (privileged()) {
    if (page_head == page_tail
        && head == tail
        && dead_cycles == 0) {
      return TRUE;
    }
    back_input();
    tail_append(new_null_box());
    box_width(tail) = hsize;
    tail_append(new_glue(fill_glue));
    tail_append(new_penalty(-010000000000));
    build_page();
  }
  return FALSE;
}

  void
show_activities ()
{
  int	a;
  int	m;
  list	*p;

  *nest_ptr = cur_list;
  print_nl(null_str);
  print_ln();
  for (p = nest_ptr; p >= nest; decr(p)) {
    m = p->mode_field;
    a = p->aux_field;
    print_nl("### ");
    print_mode(m);
    print(" entered at line ");
    print_int(abs(p->ml_field));
    if (m == HMODE) {
      if (p->lhm_field != 2 || p->rhm_field != 3) {
        print(" (hyphenmin ");
        print_int(p->lhm_field);
        print(",");
        print_int(p->rhm_field);
        print(")");
      }
    }
    if (p->ml_field < 0) {
      print(" (\\output routine)");
    }
    if (p == nest) {
      show_cur_page();
      if (link(contrib_head) != null)
        print_nl("### recent contributions:");
    }
    show_box(link(p->head_field));
    switch (abs(m) / (MAX_COMMAND + 1))
    {
      case 0:
        print_nl("prevdepth ");
        if (a <= IGNORE_DEPTH) {
          print("ignored");
        } else {
          print_scaled(a);
        }
        if (p->pg_field != 0) {
          print(", prevgraf ");
          print_int(p->pg_field);
          print(" line");
          if (p->pg_field != 1)
            print("s");
        }
        break;

      case 1:
        print_nl("spacefactor ");
        print_int(a);
        if (m > 0 && p->clang_field > 0) {
          print(", current language ");
          print_int(p->clang_field);
        }
        break;

      case 2:
        if (a != null) {
          print_nl("this will be denominator of:");
          show_box(a);
        }
        break;
    }
  }
}

  void
_eval_init ()
{
  head = tail = contrib_head;
  mode = VMODE;
  prev_depth = IGNORE_DEPTH;
  mode_line = 0;
  prev_graf = 0;
  shown_mode = 0;
  lhmin = 0;
  rhmin = 0;
  ligature_present = FALSE;
  cancel_boundary = FALSE;
  ins_disc = FALSE;
  lft_hit = FALSE;
  rt_hit = FALSE;
  max_nest_stack = nest_ptr = nest;
}

  void
_eval_init_once ()
{
  nlists = NEST_SIZE;
  nest = (list *)malloc(nlists * sizeof(list));
  if (nest == (list *) 0)
    overflow("nest", nlists);
  nest_end = nest + nlists - 1;
}

/*
 ** Help text
 */

help_dollar ()
{
  help2("I've inserted a begin-math/end-math symbol since I think",
      "you left one out. Proceed, with fingers crossed.");
}

help_illegal_case ()
{
  help4("Sorry, but I'm not programmed to handle this case;",
      "I'll just pretend that you didn't ask for it.",
      "If you're in the wrong mode, you might be able to",
      "return to the right one by typing `I}' or `I$' or `I\\par'.");
}

/*
 * Common TeX:expand.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX expand.c 3.2@(#)";
#endif

#include "tex.h"

int	long_state;
ptr	cur_mark[5];
ptr	pstack[9];

  void
get_x_token ()
{
restart:
  get_next();
  if (cur_cmd <= MAX_COMMAND)
    goto done;
  if (cur_cmd >= CALL) {
    if (cur_cmd < END_TEMPLATE) {
      macro_call();
    } else {
      cur_cs = FROZEN_ENDV;
      cur_cmd = ENDV;
      goto done;
    }
  } else {
    expand();
  }
  goto restart;

done:
  if (cur_cs == null_sym)
    cur_tok = (tok) (cur_cmd * 0400 + cur_chr);
  else cur_tok = sym2tok(cur_cs);
}

  void
expand ()
{
  ptr	p;
  ptr	q;
  str	s;
  tok	t;
  int	co_backup;
  int	cv_backup;
  int	cvl_backup;
  int	radix_backup;
  int	save_scanner_status;

  radix_backup = radix;
  co_backup = cur_order;
  cv_backup = cur_val;
  cvl_backup = cur_val_level;
  if (cur_cmd < CALL) {
    if (tracing_commands > 1)
      show_cur_cmd_chr();
    switch (cur_cmd)
    {
      case TOP_BOT_MARK:
        if (cur_mark[cur_chr] != null)
          begin_token_list(cur_mark[cur_chr], MARK_TEXT);
        break;

      case EXPAND_AFTER: 
        get_token();
        t = cur_tok;
        get_token();
        if (cur_cmd > MAX_COMMAND)
          expand();
        else back_input();
        cur_tok = t;
        back_input();
        break;

      case NO_EXPAND:
        save_scanner_status = scanner_status;
        scanner_status = NORMAL;
        get_token(); 
        scanner_status = save_scanner_status;
        t = cur_tok;
        back_input();
        if (t >= CS_TOKEN_FLAG) {
          p = new_token();
          token(p) = sym2tok(FROZEN_DONT_EXPAND);
          token_link(p) = loc;
          loc = start = p;
        }
        break;

      case CS_NAME:
        p = q = new_token();
        do {
          get_x_token();
          if (cur_cs == null_sym) {
            p = token_link(p) = new_token();
            token(p) = cur_tok;
          }
        } while (cur_cs == null_sym);
        if (cur_cmd != END_CS_NAME) {
          print_err("Missing ");
          print_esc("endcsname");
          print(" inserted");
          help_cs();
          back_error();
        }
        s = cur_str;
        p = token_link(q);
        while (p != null) {
          if (s >= cur_str_end) {
            overflow("str size", STR_SIZE);
          }
          *s++ = token(p) % 0400;
          p = token_link(p);
        }
        if (s > cur_str + 1) {
          no_new_control_sequence = FALSE;
          cur_cs = id_lookup(cur_str, s - cur_str);
          no_new_control_sequence = TRUE;
        } else if (s == cur_str) {
          cur_cs = null_cs;
        } else {
          cur_cs = single_base[*cur_str];
        }
        flush_list(q);
        if (eq_type(cur_cs) == UNDEFINED_CS)
          eq_define(cur_cs, RELAX, 256);
        cur_tok = sym2tok(cur_cs);
        back_input();
        break;

      case CONVERT:
        conv_toks();
        break;

      case THE:
        ins_list(token_link(the_toks()));
        break;

      case IF_TEST:
        conditional();
        break;

      case FI_OR_ELSE:
        if (cur_chr > if_limit) {
          if (if_limit == IF_CODE) {
            insert_relax();
          } else {
            print_err("Extra ");
            print_cmd_chr(FI_OR_ELSE, cur_chr);
            help_extra_if();
            error();
          }
        } else {
          while (cur_chr != FI_CODE)
            pass_text();
          pop_cond();
        }
        break;

      case INPUT:
        if (cur_chr > 0)
          force_eof = TRUE;
        else if (name_in_progress)
          insert_relax();
        else start_input();
        break;

      default:
        print_err("Undefined control sequence");
        help_undefd();
        error();
        break;
    }
  } else if (cur_cmd < END_TEMPLATE) {
    macro_call();
  } else {
    cur_tok = sym2tok(FROZEN_ENDV);
    back_input();
  }
  cur_val = cv_backup;
  cur_val_level = cvl_backup;
  radix = radix_backup;
  cur_order = co_backup;
}

  void
insert_relax ()
{
  cur_tok = sym2tok(cur_cs);
  back_input();
  cur_tok = sym2tok(FROZEN_RELAX);
  back_input();
  token_type = INSERTED;
}

  void
macro_call ()
{
  int	m;
  int	n;
  ptr	p;
  ptr	q;
  ptr	r;
  ptr	s;
  ptr	t;
  ptr	u;
  ptr	v;
  int	match_chr;
  ptr	ref_count;
  ptr	rbrace_ptr;
  sym	save_warning_cs;
  int	save_scanner_status;
  int	unbalance;

  n = 0;
  ref_count = cur_chr;
  r = token_link(ref_count);
  save_scanner_status = scanner_status;
  save_warning_cs = warning_cs;
  warning_cs = cur_cs;
  if (tracing_macros > 0) {
    begin_diagnostic();
    print_ln();
    print_cs(warning_cs);
    token_show(ref_count);
    end_diagnostic(FALSE);
  }
  if (token(r) != END_MATCH_TOKEN) {
    scanner_status = MATCHING;
    unbalance = 0;
    long_state = eq_type(cur_cs);
    if (long_state >= OUTER_CALL)
      long_state -= 2;
    do {
      if (token(r) > MATCH_TOKEN + 255
          || token(r) < MATCH_TOKEN) {
        s = null;
      } else {
        match_chr = token(r) - MATCH_TOKEN;
        s = r = token_link(r);
        p = match_toks;
        token_link(p) = null;
        m = 0;
      }

contin:
      get_token();
      if (cur_tok == token(r)) {
        r = token_link(r);
        if (token(r) >= MATCH_TOKEN &&
            token(r) <= END_MATCH_TOKEN) {
          if (cur_tok < LEFT_BRACE_LIMIT)
            decr(align_state);
          goto found;
        } else {
          goto contin;
        }
      }
      if (s != r) {
        if (s == null) {
          print_err("Use of ");
          sprint_cs(warning_cs);
          print(" doesn't match its definition");
          help_match();
          error();
          goto local_exit;
        } else {
          t = s;
          do {
            store_new_token(token(t));
            incr(m);
            u = token_link(t);
            v = s;
            loop {
              if (u == r) {
                if (cur_tok != token(v)) {
                  break;
                } else {
                  r = token_link(v);
                  goto contin;
                }
              }
              if (token(u) != token(v))
                break;
              u = token_link(u);
              v = token_link(v);
            }
            t = token_link(t);
          } while (t != r);
          r = s;
        }
      }
      if (cur_tok == par_tok && long_state != LONG_CALL) {
        runaway_arg(n);
        align_state -= unbalance; 
        goto local_exit;
      }
      if (cur_tok < RIGHT_BRACE_LIMIT) {
        if (cur_tok < LEFT_BRACE_LIMIT) {
          unbalance = 1;
          loop {
            store_new_token(cur_tok);
            get_token();
            if (cur_tok == par_tok && 
                long_state != LONG_CALL) {
              runaway_arg(n);
              align_state -= unbalance; 
              goto local_exit;
            }
            if (cur_tok < RIGHT_BRACE_LIMIT) {
              if (cur_tok < LEFT_BRACE_LIMIT) {
                incr(unbalance);
              } else {
                decr(unbalance);
                if (unbalance == 0)
                  break;
              }
            }
          }
          rbrace_ptr = p;
          store_new_token(cur_tok);
        } else {
          back_input();
          print_err("Argument of ");
          sprint_cs(warning_cs);
          print(" has an extra }");
          help_match_xtra();
          incr(align_state);
          long_state = CALL;
          cur_tok = par_tok;
          ins_error();
        }
      } else {
        if (cur_tok == SPACE_TOKEN &&
            token(r) <= END_MATCH_TOKEN &&
            token(r) >= MATCH_TOKEN)
          goto contin;
        store_new_token(cur_tok);
      }
      incr(m);
      if (token(r) > END_MATCH_TOKEN || token(r) < MATCH_TOKEN)
        goto contin;

found:
      if (s != null) {
        if (m == 1 &&
            token(p) < RIGHT_BRACE_LIMIT &&
            p != match_toks) {
          token_link(rbrace_ptr) = null;
          free_token(p);
          p = token_link(match_toks);
          pstack[n] = token_link(p);
          free_token(p);
        } else {
          pstack[n] = token_link(match_toks);
        }
        incr(n);
        if (tracing_macros > 0) {
          begin_diagnostic();
          print_nl(null_str);
          print_char(match_chr);
          print_int(n);
          print("<-");
          show_token_list(pstack[n - 1], null, 1000);
          end_diagnostic(FALSE);
        }
      }
    } while (token(r) != END_MATCH_TOKEN);
  }
  token_link(match_toks) = null;
  while (state == TOKEN_LIST && loc == null)
    end_token_list();
  begin_token_list(ref_count, MACRO);
  in_cs = warning_cs;
  name = text(warning_cs);
  loc = token_link(r);
  if (n > 0) {
    if (param_ptr + n > max_param_stack) {
      if (param_ptr + n >= param_end)
        overflow("parameter stack size", nparams);
      max_param_stack = param_ptr + n;
    }
    for (m = 0; m < n; incr(m))
      param_ptr[m] = pstack[m];
    param_ptr += n;
  }

local_exit:
  scanner_status = save_scanner_status; 
  warning_cs = save_warning_cs; 
}

  void
runaway_arg (n) 
  int	n;
{
  int	m;

  if (long_state == CALL) { 
    runaway(); 
    print_err("Paragraph ended before "); 
    sprint_cs(warning_cs); 
    print(" was complete"); 
    help_runaway(); 
    back_error();
  }
  pstack[n] = token_link(match_toks); 
  for (m = 0; m <= n; incr(m))
    flush_list(pstack[m]);
}

  void
x_token ()
{
  while (cur_cmd > MAX_COMMAND) {
    expand();
    get_next();
  }
  if (cur_cs == 0)
    cur_tok = (tok) (cur_cmd * 0400 + cur_chr);
  else cur_tok = sym2tok(cur_cs);
}

  void
_expand_init ()
{
  top_mark = null;
  first_mark = null;
  bot_mark = null;
  split_first_mark = null;
  split_bot_mark = null;
}

  void
_expand_init_once ()
{
}

/*
 **	Help text
 */

help_runaway ()
{
  help3("I suspect you've forgotten a `}', causing me to apply this",
      "control sequence to too much text. How can we recover?",
      "My plan is to forget the whole thing and hope for the best.");
}

help_match ()
{
  help4("If you say, e.g., `\\def\\a1{...}', then you must always",
      "put `1' after `\\a', since control sequence names are",
      "made up of letters only. The macro here has not been",
      "followed by the required stuff, so I'm ignoring it.");
}

help_match_xtra ()
{
  help6("I've run across a `}' that doesn't seem to match anything.",
      "For example, `\\def\\a#1{...}' and `\\a}' would produce",
      "this error. If you simply proceed now, the `\\par' that",
      "I've just inserted will cause me to report a runaway",
      "argument that might be the root of the problem. But if",
      "your `}' was spurious, just type `2' and it will go away.");
}

help_undefd ()
{
  help5("The control sequence at the end of the top line",
      "of your error message was never \\def'ed. If you have",
      "misspelled it (e.g., `\\hobx'), type `I' and the correct",
      "spelling (e.g., `I\\hbox'). Otherwise just continue,",
      "and I'll forget about whatever was undefined.");
}

help_cs ()
{
  help2("The control sequence marked <to be read again> should",
      "not appear between \\csname and \\endcsname.");
}

help_extra_if ()
{
  help1("I'm ignoring this; it doesn't match any \\if.");
}

/*
 * Common TeX:fileio.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX fileio.c 3.2@(#)";
#endif

#include <sys/types.h>
#ifdef quad
#undef quad
#endif
#include <sys/stat.h>

#include "tex.h"

int	name_length;
str	name_of_file;
str	name_str;
str	area_str;
str	ext_str;

str	cur_name;
str	cur_area;
str	cur_ext;

str	job_area;
str	job_name;

bool    name_in_progress;
bool	log_opened;

int	area_delimiter;
int	ext_delimiter;
int	path_delimiter;

file	read_file[16];

int	read_open[17]; 

str	str_dvi;
str	str_log;
str	str_tex;
str	str_tfm;
str	str_texput;

  void
begin_name ()
{
  name_str = name_of_file;
  area_str = null_str;
  ext_str = null_str;
}

#define append_to_name(C) \
{if (name_str - name_of_file == STR_SIZE) \
  overflow("file name", STR_SIZE); \
  *name_str++ = C;}

  bool
more_name (c)
  int	c;
{
  if (c == ' ') {
    return FALSE;
  } else {
    append_to_name(c);
    if (c == area_delimiter) {
      area_str = name_str;
      ext_str = null_str;
    } else if (c == ext_delimiter && ext_str == null_str) {
      ext_str = name_str - 1;
    }
    return TRUE;
  }
}

  void
end_name ()
{
  int	n;

  if (area_str == null_str) {
    cur_area = null_str;
    area_str = name_of_file;
  } else {
    n = area_str - name_of_file;
    cur_area = new_str(n);
    strncpy(cur_area, name_of_file, n);
  }
  if (ext_str == null_str) {
    cur_ext = null_str;
    ext_str = name_str;
  } else {
    n = name_str - ext_str;
    cur_ext = new_str(n);
    strncpy(cur_ext, ext_str, n);
  }
  n = ext_str - area_str;
  if (n == 0) {
    cur_name = null_str;
  } else {
    cur_name = new_str(n);
    strncpy(cur_name, area_str, n);
  }	
}

  void
scan_file_name ()
{
  name_in_progress = TRUE;
  get_nbx_token();
  begin_name();
  loop {
    if (cur_cmd > OTHER_CHAR || cur_chr > 255) {
      back_input();
      break;
    }
    if (!more_name(cur_chr))
      break;
    get_x_token();
  }
  end_name();
  name_in_progress = FALSE;
}


  void
pack_file_name (n, a, e)
  str	n;
  str	a;
  str	e;
{
  str	s;

  name_str = name_of_file;
  for (s = a; *s; incr(s))
    append_to_name(*s);
  for (s = n; *s; incr(s))
    append_to_name(*s);
  for (s = e; *s; incr(s))
    append_to_name(*s);
  append_to_name(NUL);
  name_length = name_str - name_of_file;
}

  void
pack_job_name (s)
  str	s;
{
  cur_area = job_area;
  cur_name = job_name;
  cur_ext = s;
  pack_cur_name();
}

  void
print_file_name (n, a, e)
  str	n;
  str	a;
  str	e;
{
  print(a);
  print(n);
  print(e);
}

  str
make_name_str ()
{
  str	s;

  s = new_str(name_length);
  strcpy(s, name_of_file);

  return (s);
}

  void
prompt_file_name (s, e)
  str	s;
  str	e;
{
  str	t;

  if (s[0] == 'i' && s[1] == 'n')
    print_nl("! I can't find file `");
  else print_nl("! I can't write on file `");
  print_file_name(cur_name, cur_area, cur_ext);
  print("'.");
  if (e == str_tex)
    show_context();
  print_nl("Please type another ");
  print(s);
  if (interaction < SCROLL_MODE)
    fatal_error("*** (job aborted, file error in nonstop mode)");
  clear_terminal();
  prompt_input(": ");
  begin_name();
  t = cur_str;
  while (*t == ' ' && t < cur_str_ptr)
    incr(t);
  while (t < cur_str_ptr && more_name(*t))
    incr(t);
  end_name();
  if (cur_ext == null_str)
    cur_ext = e;
  pack_cur_name();
  flush_str();
}

  void
start_input ()
{
  int	save_selector;

  scan_file_name();
  if (cur_ext == null_str)
    cur_ext = str_tex;
  pack_cur_name();
  loop {
    begin_file_reading();
    if (cur_file = a_open_in())
      break;
    end_file_reading();
    if (cur_ext == str_tex) {
      cur_ext = null_str;
      pack_cur_name();
      begin_file_reading();
      if (cur_file = a_open_in())
        break;
      end_file_reading();
    }
    prompt_file_name("input file name", str_tex);
  }
  name = make_name_str();
  if (job_name == null_str) {
    job_area = cur_area;
    job_name = cur_name;
    open_log_file();
    save_selector = selector;
    selector = NEW_STRING;
    print(" (preloaded format=");
    print(job_name);
    print(" ");
    print_int(year % 100);
    print(".");
    print_int(month);
    print(".");
    print_int(day);
    print(")");
    format_ident = make_str();
    selector = save_selector;
  }
  if (term_offset + str_length(name) > MAX_PRINT_LINE - 2) {
    print_ln();
  } else if (term_offset > 0 || file_offset > 0) {
    print(" ");
  }
  incr(open_parens);
  print("(");
  print(name);
  update_terminal(); 
  state = NEW_LINE;
  line = 1;
  index = 18;
  input_ln(cur_file);
  firm_up_the_line();
  if (end_line_char_active)
    *++limit = end_line_char;
  next = buffer;
}

  FILE *
a_open_in ()
{
  if (test_access(READ_ACCESS, INPUT_FILE_PATH))
    return (fopen(name_of_file, "r"));
  return null_file;
}

  FILE *
a_open_out ()
{
  if (test_access(WRITE_ACCESS, NO_FILE_PATH))
    return (fopen(name_of_file, "w"));
  return null_file;
}

  FILE *
b_open_in ()
{
  if (test_access(READ_ACCESS, FONT_FILE_PATH))
    return (fopen(name_of_file, "rb"));
  return null_file;
}

  FILE *
b_open_out ()
{
  if (test_access(WRITE_ACCESS, NO_FILE_PATH))
    return (fopen(name_of_file, "wb"));
  return null_file;
}

  bool 
input_ln (f)
  file	f;
{
  int	c;

  next = buffer;
  limit = next - 1;
  c = getc(f);
  if (feof(f))
    return FALSE;
  loop {
    if (c == EOLN || c == EOF)
      break;
    if ((*next = xord[c]) != ' ')
      limit = next;
    if (++next >= buffer + BUF_SIZE)
      overflow("buffer size", BUF_SIZE);
    c = getc(f);
  }
  next = buffer;
  return TRUE;
}

  void
term_input ()
{
  int	c;
  str	s;

  update_terminal();
  flush_str();
  loop {
    c = getc(term_in);
    if (c == EOLN)
      break;
    if (c == EOF)
      fatal_error("! End of file on the terminal");
    append_char(xord[c]);
    if (cur_str_ptr >= cur_str_end)
      overflow("str size", STR_SIZE);
  }
  term_offset = 0;
  decr(selector);
  if (cur_str_ptr != cur_str)
    for (s = cur_str; s < cur_str_ptr; incr(s))
      print_char(*s);
  print_ln();
  incr(selector);
}

  bool
init_terminal ()
{
  loop {
    fputs("**", term_out);
    update_terminal();
    if (!input_ln(term_in)) {
      fputs("\n! End of file on the terminal...why?\n",
          term_out);
      return FALSE;
    }
    next = buffer;
    while (next <= limit && *next == ' ')
      incr(next);
    if (next <= limit)
      return TRUE;
    fputs("Please type the name of your input file.\n", term_out);
  }
}

  void
open_log_file ()
{
  int	k;
  byte	*s;
  byte	*t;
  char	*months; 
  int	old_setting;

  old_setting = selector;
  if (job_name == null_str) {
    job_area = null_str;
    job_name = str_texput;
  }
  pack_job_name(str_log);
  while ((log_file = a_open_out()) == null_file) {
    if (interaction < SCROLL_MODE) {
      print_err("I can't write on file `");
      print_file_name(cur_name, cur_area, cur_ext);
      print("'.");
      job_name = null_str;
      history = FATAL_ERROR_STOP;
      jump_out();
    }
    prompt_file_name("transcript file name", str_log);
  }
  log_name = make_name_str();
  selector = LOG_ONLY;
  log_opened = TRUE;
  fputs(banner, log_file);
  if (format_ident == null_str)
    print(" (no format preloaded)");
  else print(format_ident);
  print(" ");
  print_int(day);
  print(" ");
  months = "JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC";
  for (k = 3 * month - 3; k < 3 * month; incr(k))
    putc(months[k], log_file);
  print(" ");
  print_int(year);
  print(" ");
  print_two(time / 60);
  print(":");
  print_two(time % 60);
  *input_ptr = cur_input;
  print_nl("**");
  s = input_stack[0].obj_field.f.buf_field;
  t = input_stack[0].obj_field.f.limit_field;
  if (*t == end_line_char)
    decr(t);
  while (s <= t)
    print_char(*s++);
  print_ln();
  selector = old_setting + 2; 
}

str	cur_path;
str	input_path = default_input_path;
str 	font_path = default_font_path;

  void
set_paths ()
{
  str	env_path;
  str	getenv();

  if (env_path = getenv("TEXINPUTS"))
    input_path = env_path;
  if (env_path = getenv("TEXFONTS"))
    font_path = env_path;
}

/*
 **	test_access(amode, file_path)
 **
 **  Test whether or not the file whose name is in the global name_of_file
 **  can be opened for reading according to access mode.
 **
 **  If the filename given in name_of_file does not begin with '/', we try 
 **  prepending all the ':'-separated areanames in the appropriate path to the
 **  filename until access can be made.
 */

  bool
test_access (amode, file_path)
  int	amode;
  int	file_path;
{
  int	nl;
  bool	ok;
  char	original_name[STR_SIZE];

  strcpy(original_name, name_of_file);
  nl = name_length;
  switch (file_path)
  {
    case NO_FILE_PATH:
      cur_path = null_str;
      break;

    case INPUT_FILE_PATH: 
      cur_path = input_path;
      break;

    case FONT_FILE_PATH: 
      cur_path = font_path;
      break;

  }
  if (name_of_file[0] == '/' ||
      name_of_file[0] == '.' && name_of_file[1] == '/') {
    cur_path = null_str;
  }
  do {
    strcpy(name_of_file, original_name);
    name_length = nl;
    if (get_real_name()) {
      switch (amode)
      {
        case READ_ACCESS: {
                            FILE *fp = fopen(name_of_file, "r");
                            ok = fp != (FILE *) 0;
                            if (ok) {
                              struct stat st;
                              fstat(fileno(fp), &st);
                              fclose(fp);
                              ok = (st.st_mode & S_IFMT) == S_IFREG;
                            }
                            break;
                          }

        case WRITE_ACCESS: {
                             FILE *fp = fopen(name_of_file, "w");
                             ok = fp != (FILE *) 0;
                             if (ok)
                               fclose(fp);
                             break;
                           }
      }
    } else {
      ok = FALSE;
    }
  } while (!ok && cur_path != null_str);
  return ok;
}

#define append_to_real_name(C) \
{if (k == STR_SIZE) \
  overflow("real_name", STR_SIZE); \
  real_name[k++] = C;}

get_real_name ()
{
  int	k;
  str	s;
  char	real_name[STR_SIZE];

  real_name[k = 0] = NUL;
  s = cur_path;
  while (*s && *s != path_delimiter) {
    append_to_real_name(*s);
    incr(s);
  }
  if (*s == NUL)
    cur_path = null_str;
  else cur_path = ++s;
  if (k && real_name[k-1] != area_delimiter)
    append_to_real_name(area_delimiter);
  if (*(s = name_of_file)) {
    while (*s) {
      append_to_real_name(*s);
      incr(s);
    }
  }
  name_length = k;
  append_to_real_name(NUL);
  strcpy(name_of_file, real_name);
  return k;
}

  void
_fileio_init ()
{
  int	i;

  job_name = null_str;
  name_in_progress = FALSE;
  log_opened = FALSE;
  for (i = 0; i <= 16; incr(i)) 
    read_open[i] = CLOSED;
}

  void
_fileio_init_once ()
{
  name_of_file = new_str(STR_SIZE);
  area_delimiter='/';
  ext_delimiter='.';
  path_delimiter=':';
  str_tex = ".tex";
  str_dvi = ".dvi";
  str_log = ".log";
  str_tfm = ".tfm";
  str_texput = "texput";
}

/*
 * Common TeX:hyph.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX hyph.c 3.2@(#)";
#endif

#include "tex.h"

ptr	ha;
ptr	hb;
int	hc[66];
fnt	hf;
int	hn;
int	hu[64];
int	hyf[65];
int	hyf_char;

int	hyphen_passed;

int	l_hyf;
int	r_hyf;
int	cur_lang;

ptr	init_list;
bool	init_lig;
bool	init_lft;

#define HYPH_SIZE	307

int	hyph_count;
int	*hyph_len;
str	*hyph_word;
ptr	*hyph_list;

#define TRIE_SIZE	20000

bool	trie_not_ready;

trie_t	*trie;
int	trie_max;
int	trie_ptr;
int	*trie_hash;
bool	*trie_taken;
int	*trie_min;
int	*trie_c;
int	*trie_o;
int	*trie_l;
int	*trie_r;

#define TRIE_OP_SIZE		600
#define MAX_TRIE_OPS_PER_LANG	300

int	trie_op_ptr;
int	*trie_op_hash;	
int	*trie_op_lang;
int	*trie_op_used;
int	*trie_op_val;
int	*op_start;
int	*hyf_distance;
int	*hyf_next;
int	*hyf_num;

  void
init_hyph ()
{
  if (trie_not_ready)
    init_trie();
  l_hyf = lhmin;
  r_hyf = rhmin;
  cur_lang = 0;
}

  void
try_hyph ()
{
  int	c, j;
  ptr	prev_p, p, q;

  prev_p = cur_p;
  p = link(cur_p);
  if (p != null) {
    loop {
      if (is_char_node(p)) {
        c = character(p);
        hf = font(p);
      } else if (type(p) == LIGATURE_NODE) {
        if (lig_ptr(p) == null)
          goto contin;
        q = lig_ptr(p);
        c = character(q);
        hf = font(q);
      } else if (type(p) == KERN_NODE
          && subtype(p) == NORMAL) {
        goto contin;
      } else if (type(p) == WHATSIT_NODE) {
        try_hyph_whatsit(p);
        goto contin;
      } else {
        goto done1;
      }
      if (lc_code(c) != 0) {
        if (lc_code(c) == c || uc_hyph > 0) {
          goto done2;
        } else {
          goto done1;
        }
      }
contin:
      prev_p = p;
      p = link(p);
    }
done2:
    hyf_char = hyphen_char(hf);
    if (hyf_char < 0 || hyf_char > 255)
      goto done1;
    ha = prev_p;
    if (l_hyf + r_hyf > 63)
      goto done1;
    hn = 0;
    loop {
      if (is_char_node(p)) {
        if (font(p) != hf)
          goto done3;
        c = character(p);
        if (lc_code(c) == 0 || hn == 63)
          goto done3;
        hb = p;
        incr(hn);
        hu[hn] = c;
        hc[hn] = lc_code(c);
      } else if (type(p) == LIGATURE_NODE) {
        if (font(lig_char(p)) != hf)
          goto done3;
        j = hn;
        q = lig_ptr(p);
        while (q != null) {
          c = character(q);
          if (lc_code(c) == 0 || j == 63)
            goto done3;
          incr(j);
          hu[j] = c;
          hc[j] = lc_code(c);
          q = link(q);
        }
        hb = p;
        hn = j;
      } else if (type(p) != KERN_NODE
          || subtype(p) != NORMAL) {
        goto done3;
      }
      p = link(p);
    }
done3:
    if (hn < l_hyf + r_hyf)
      goto done1;
    loop {
      if (!is_char_node(p)) {
        switch (type(p))
        {
          case LIGATURE_NODE:
            break;

          case KERN_NODE:
            if (subtype(p) != NORMAL)
              goto done4;
            break;

          case WHATSIT_NODE:
          case GLUE_NODE:
          case PENALTY_NODE:
          case INS_NODE:
          case ADJUST_NODE:
          case MARK_NODE:
            goto done4;

          default: 
            goto done1;
        }
      }
      p = link(p);
    }

done4:
    hyphenate();
  }
done1:	return;
}

  void
hyphenate ()
{
  str	t, u;
  ptr	q, r, s;
  ptr	hyf_node;
  int	bchar;
  int	c_loc;
  int	r_count;
  ptr	major_tail;
  ptr	minor_tail;
  int	c, h, i, j, l, v, z;

  for (j = 0; j <= hn; incr(j))
    hyf[j] = 0;
  h = hc[1];
  incr(hn);
  hc[hn] = cur_lang;
  for (j = 2; j <= hn; incr(j))
    h = (h + h + hc[j]) % HYPH_SIZE;
  loop {
    t = hyph_word[h];
    if (t == null_str)
      goto not_found;
    l = hyph_len[h];
    if (l < hn)
      goto not_found;
    if (l == hn) {
      j = 1;
      u = t;
      while (j <= hn) {
        if (*u < hc[j])
          goto not_found;
        if (*u > hc[j])
          goto done;
        incr(u);
        incr(j);
      }
      for (s = hyph_list[h]; s != null; s = link(s))
        hyf[info(s)] = 1;
      decr(hn);
      goto found;
    }

done:
    if (h > 0) {
      decr(h);
    } else {
      h = HYPH_SIZE;
    }
  }

not_found:
  decr(hn);
  if (trie_char(cur_lang + 1) != cur_lang)
    return;
  hc[0] = 0;
  hc[hn + 1] = 0;
  hc[hn + 2] = 256;
  for (j = 0; j <= hn - r_hyf + 1; incr(j)) {
    z = trie_link(cur_lang + 1) + hc[j];
    l = j;
    while (hc[l] == trie_char(z)) {
      if (v = trie_op(z)) {
        while (v) {
          v += op_start[cur_lang];
          i = l - hyf_distance[v];
          if (hyf_num[v] > hyf[i])
            hyf[i] = hyf_num[v];
          v = hyf_next[v];
        }
      }
      incr(l);
      z = trie_link(z) + hc[l];
    }
  }

found:
  for (j = 0; j < l_hyf; incr(j)) {
    hyf[j] = 0;
  }
  for (j = 0; j < r_hyf; incr(j)) {
    hyf[hn - j] = 0;
  }
  for (j = l_hyf; j <= hn - r_hyf; incr(j))
    if (odd(hyf[j]))
      goto found1;
  return;

found1:
  q = link(hb);
  link(hb) = null;
  r = link(ha);
  link(ha) = null;
  bchar = NON_CHAR;
  if (type(hb) == LIGATURE_NODE && odd(subtype(hb))) {
    bchar = font_bchar(hf);
  }
  if (is_char_node(ha)) {
    if (font(ha) != hf) {
      goto found2;
    } else {
      init_list = ha;
      init_lig = FALSE;
      hu[0] = character(ha);
    }
  } else if (type(ha) == LIGATURE_NODE) {
    if (font(lig_char(ha)) != hf) {
      goto found2;
    } else {
      init_list = lig_ptr(ha);
      init_lig = TRUE;
      init_lft = subtype(ha) > 1;
      hu[0] = character(lig_char(ha));
      if (init_list == null && init_lft) {
        hu[0] = 256;
        init_lig = FALSE;
      }
      free_node(ha, SMALL_NODE_SIZE);
    }
  } else {
    if (type(r) == LIGATURE_NODE && subtype(r) > 1) {
      goto found2;
    }
    j = 1;
    s = ha;
    init_list = null;
    goto common_end;
  }
  s = cur_p;
  while (link(s) != ha)
    s = link(s);
  j = 0;
  goto common_end;

found2:
  s = ha;
  j = 0;
  hu[0] = 256;
  init_lig = FALSE;
  init_list = null;

common_end:
  flush_node_list(r);

#define advance_major_tail()						\
  {	major_tail = link(major_tail);					\
    incr(r_count);							\
  }

#define put_pre_break()							\
  {	minor_tail = null;						\
    pre_break(r) = null;						\
    hyf_node = new_character(hf, hyf_char);				\
    if (hyf_node != null) {						\
      incr(i);						\
      c = hu[i];						\
      hu[i] = hyf_char;					\
      free_avail(hyf_node);					\
    }								\
    while (l <= i) {						\
      l = reconstitute(l, i, font_bchar(hf), NON_CHAR) + 1;	\
      if (link(hold_head) == null) {				\
        continue;					\
      }							\
      if (minor_tail == null) {				\
        pre_break(r) = link(hold_head);			\
      } else {						\
        link(minor_tail) = link(hold_head);		\
      }							\
      minor_tail = link(hold_head);				\
      while (link(minor_tail) != null)			\
      minor_tail = link(minor_tail);			\
    }								\
    if (hyf_node != null) {						\
      hu[i] = c;						\
      l = i;							\
      decr(i);						\
    }								\
  }

#define put_post_break()						\
  {	minor_tail = null;						\
    post_break(r) = null;						\
    c_loc = 0;							\
    if (bchar_label(hf) != NON_ADDRESS) {				\
      decr(l);						\
      c = hu[l];						\
      c_loc = l;						\
      hu[l] = 256;						\
    }								\
    while (l < j) {							\
      do {							\
        l = reconstitute(l, hn, bchar, NON_CHAR) + 1;	\
        if (c_loc > 0) {				\
          hu[c_loc] = c;				\
          c_loc = 0;				\
        }						\
        if (link(hold_head) == null) {			\
          continue;				\
        }						\
        if (minor_tail == null) {			\
          post_break(r) = link(hold_head);	\
        } else {					\
          link(minor_tail) = link(hold_head);	\
        }						\
        minor_tail = link(hold_head);			\
        while (link(minor_tail) != null)		\
        minor_tail = link(minor_tail);		\
      } while (l < j);					\
      while (l > j) {						\
        j = reconstitute(j, hn, bchar, NON_CHAR) + 1;	\
        link(major_tail) = link(hold_head);		\
        while (link(major_tail) != null) {		\
          advance_major_tail();			\
        }						\
      }							\
    }								\
  }

  do {
    l = j;
    j = reconstitute(j, hn, bchar, hyf_char) + 1;
    if (hyphen_passed == 0) {
      link(s) = link(hold_head);
      while (link(s) != null)
        s = link(s);
      if (odd(hyf[j - 1])) {
        l = j;
        hyphen_passed = j - 1;
        link(hold_head) = null;
      }
    }
    if (hyphen_passed > 0) {
      do {
        r = new_node(SMALL_NODE_SIZE);
        link(r) = link(hold_head);
        type(r) = DISC_NODE;
        major_tail = r;
        r_count = 0;
        while (link(major_tail) != null) {
          advance_major_tail();
        }
        i = hyphen_passed;
        hyf[i] = 0;
        put_pre_break();
        put_post_break();
        if (r_count > 127) {
          link(s) = link(r);
          link(r) = null;
          flush_node_list(r);
        } else {
          link(s) = r;
          replace_count(r) = r_count;
        }
        s = major_tail;
        hyphen_passed = j - 1;
        link(hold_head) = null;
      } while (odd(hyf[j - 1]));
    }
  } while (j <= hn);
  link(s) = q;
  flush_list(init_list);
}

#define append_charnode_to_t(C)					\
{	t = link(t) = new_avail(); 				\
  font(t) = hf; character(t) = (C); 			\
}

#define set_cur_r()						\
{	cur_r = (j < n) ? hu[j + 1] : bchar;			\
  cur_rh = (odd(hyf[j])) ? hchar : NON_CHAR;		\
}

#define wrap_lig(RT_HIT)					\
{	if (ligature_present) {					\
  p = new_ligature(hf, cur_l, link(cur_q));	\
  if (lft_hit) {					\
    subtype(p) = 2;				\
    lft_hit = FALSE;			\
  }						\
  if (RT_HIT) {					\
    if (lig_stack == null) {		\
      incr(subtype(p));		\
      rt_hit = FALSE;			\
    }					\
  }						\
  t = link(cur_q) = p;				\
  ligature_present = FALSE;			\
                        }							\
}

#define pop_lig_stack()						\
{	if (lig_ptr(lig_stack) != null) {			\
  t = link(t) = lig_ptr(lig_stack);		\
  incr(j);					\
                                  }							\
  p = lig_stack;						\
  lig_stack = link(p);					\
  free_node(p, SMALL_NODE_SIZE);				\
  if (lig_stack == null) {				\
    set_cur_r();					\
  } else {						\
    cur_r = character(lig_stack);			\
  }							\
}

#define lig_replace() 						\
{	if (cur_l == NON_CHAR) 					\
  lft_hit = TRUE;					\
  if (j == n && lig_stack == null)			\
  rt_hit = TRUE;					\
  check_interrupt(); 					\
  switch (op_byte(q)) {					\
    case 1: case 5:						\
                              cur_l = rem_byte(q);				\
    ligature_present = TRUE;			\
    break;						\
    case 2: case 6:						\
                              cur_r = rem_byte(q);				\
    if (lig_stack != null) {			\
      character(lig_stack) = cur_r;		\
    } else {					\
      lig_stack = new_lig_item(cur_r);	\
      if (j == n) {				\
        bchar = NON_CHAR;		\
      } else {				\
        p = new_avail();		\
        lig_ptr(lig_stack) = p;		\
        character(p) = hu[j + 1];	\
        font(p) = hf;			\
      }					\
    }						\
    break;						\
    case 3:							\
                        cur_r = rem_byte(q);				\
    p = lig_stack;					\
    lig_stack = new_lig_item(cur_r);		\
    link(lig_stack) = p;				\
    break;						\
    case 7: case 11:					\
                              wrap_lig(FALSE);				\
    cur_q = t;					\
    cur_l = rem_byte(q);				\
    ligature_present = TRUE;			\
    break;						\
    default:						\
                        cur_l = rem_byte(q);				\
    ligature_present = TRUE;			\
    if (lig_stack != null) {			\
      pop_lig_stack();			\
    } else if (j == n) {				\
      goto done;				\
    } else {					\
      append_charnode_to_t(cur_r);		\
      incr(j);				\
      set_cur_r();				\
    }						\
    break;						\
  }							\
  if (op_byte(q) > 4 && op_byte(q) != 7) {		\
    goto done;					\
  }							\
  goto contin;						\
}


  int
reconstitute (j, n, bchar, hchar)
  int	j, n, bchar, hchar;
{
  scal	w;
  qcell	*k, q;
  ptr	p, t;
  int	cur_rh;
  int	test_char;

  hyphen_passed = 0;
  t = hold_head;
  link(hold_head) = null;
  w = 0;
  cur_l = hu[j];
  cur_q = t;
  if (j == 0) {
    ligature_present = init_lig;
    p = init_list;
    if (ligature_present)
      lft_hit = init_lft;
    while (p != null) {
      append_charnode_to_t(character(p));
      p = link(p);
    }
  } else if (cur_l < NON_CHAR) {
    append_charnode_to_t(cur_l);
  }
  lig_stack = null;
  set_cur_r();

contin:
  if (cur_l == NON_CHAR) {
    k = bchar_label(hf);
    if (k == NON_ADDRESS) {
      goto done;
    } else {
      q = *k;
    }
  } else {
    q = char_info(hf, cur_l);
    if (char_tag(q) != LIG_TAG)
      goto done;
    k = lig_kern_start(hf, q);
    q = *k;
    if (skip_byte(q) > STOP_FLAG) {
      k = lig_kern_restart(hf, q);
      q = *k;
    }
  }
  test_char = (cur_rh < NON_CHAR) ? cur_rh : cur_r;
  loop {
    if (next_char(q) == test_char
        && skip_byte(q) <= STOP_FLAG) {
      if (cur_rh < NON_CHAR) {
        hyphen_passed = j;
        hchar = NON_CHAR;
        cur_rh = NON_CHAR;
        goto contin;
      } else {
        if (hchar < NON_CHAR && odd(hyf[j])) {
          hyphen_passed = j;
          hchar = NON_CHAR;
        }
        if (op_byte(q) < KERN_FLAG) {
          lig_replace();
        }
        w = char_kern(hf, q);
        goto done;
      }
    }
    if (skip_byte(q) >= STOP_FLAG) {
      if (cur_rh == NON_CHAR) {
        goto done;
      } else {
        cur_rh = NON_CHAR;
        goto contin;
      }
    }
    k += skip_byte(q) + 1;
    q = *k;
  }
done:
  wrap_lig(rt_hit);
  if (w != 0) {
    t = link(t) = new_kern(w);
    w = 0;
  }
  if (lig_stack != null) {
    cur_q = t;
    cur_l = character(lig_stack);
    ligature_present = TRUE;
    pop_lig_stack();
    goto contin;
  }
  return j;
}

#define set_cur_lang() \
{cur_lang = (language <= 0 || language > 255) ? 0 : language;}

  void
new_hyph_exceptions ()
{
  ptr	p, q;

  scan_left_brace();
  set_cur_lang();
  hn = 0;
  p = null;
  loop {
    get_x_token();

reswitch:
    switch (cur_cmd)
    {
      case LETTER:
      case OTHER_CHAR:
      case CHAR_GIVEN:
        if (cur_chr == '-') {
          if (hn < 63) {
            q = new_avail();
            link(q) = p;
            info(q) = hn;
            p = q;
          }
        } else {
          if (lc_code(cur_chr) == 0) {
            print_err("Not a letter");
            help_hyph_lccode();
            error();
          } else if (hn < 63) {
            incr(hn);
            hc[hn] = lc_code(cur_chr);
          }
        }
        break;

      case CHAR_NUM:
        scan_char_num();
        cur_chr = cur_val;
        cur_cmd = CHAR_GIVEN;
        goto reswitch;

      case SPACER:
      case RIGHT_BRACE:
        if (hn > 1)
          enter_hyph_exception(p);
        if (cur_cmd == RIGHT_BRACE)
          return;
        hn = 0;
        p = null;
        break;

      default:
        print_err("Improper ");
        print_esc("hyphenation");
        print(" will be flushed");
        help_hyph();
        error();
        break;
    }
  }
}

  void
enter_hyph_exception (p)
  ptr	p;
{
  ptr	q;
  int	h, j, l, m;
  str 	s, t, u, v, w;

  incr(hn);
  hc[hn] = cur_lang;
  str_room(hn);
  h = 0;
  for (j = 1; j <= hn; incr(j)) {
    h = (h + h + hc[j]) % HYPH_SIZE;
    append_char(hc[j]);
  }
  l = cur_length();
  s = make_str();
  if (hyph_count == HYPH_SIZE)
    overflow("exception dictionary", HYPH_SIZE);
  incr(hyph_count);
  while (hyph_word[h] != null_str) {
    w = hyph_word[h];
    m = hyph_len[h];
    if (m < l)
      goto found;
    if (m > l)
      goto not_found;
    u = w;
    v = s;
    while (m--) {
      if (*u < *v)
        goto found;
      if (*u > *v)
        goto not_found;
      incr(u);
      incr(v);
    }

found:
    q = hyph_list[h];
    hyph_list[h] = p;
    p = q;
    t = hyph_word[h];
    hyph_word[h] = s;
    s = t;
    m = hyph_len[h];
    hyph_len[h] = l;
    l = m;

not_found:
    if (h > 0) {
      decr(h);
    } else {
      h = HYPH_SIZE;
    }
  }
  hyph_word[h] = s;
  hyph_len[h] = l;
  hyph_list[h] = p;
}

  void
new_patterns ()
{
  trie_t	z;
  int	c, k, l, p, q, r, s, v;
  bool	first_child;
  bool	digit_sensed;

  if (trie_not_ready == FALSE) {
    print_err("Too late for ");
    print_esc("patterns");
    help_patterns();
    error();
    scan_toks(FALSE, FALSE);
    flush_list(def_ref);
    return;
  }
  set_cur_lang();
  scan_left_brace();
  k = hyf[0] = 0;
  digit_sensed = FALSE;
  loop {
    get_x_token();
    switch (cur_cmd)
    {
      case LETTER:
      case OTHER_CHAR:
        if (digit_sensed || cur_chr < '0' || cur_chr > '9') {
          if (cur_chr == '.') {
            cur_chr = 0;
          } else {
            cur_chr = lc_code(cur_chr);
            if (cur_chr == 0) {
              print_err("Nonletter");
              help1("(See Appendix H.)");
              error();
            }
          }
          if (k < 63) {
            incr(k);
            hc[k] = cur_chr;
            hyf[k] = 0;
            digit_sensed = FALSE;
          }
        } else if (k < 63){
          hyf[k] = cur_chr - '0';
          digit_sensed = TRUE;
        }
        break;

      case SPACER:
      case RIGHT_BRACE:
        if (k == 0) {
          if (cur_cmd == RIGHT_BRACE)
            goto done;
          break;
        }
        if (hc[1] == 0)
          hyf[0] = 0;
        if (hc[k] == 0)
          hyf[k] = 0;
        v = 0;
        l = k;
        loop {
          if (hyf[l])
            v = new_trie_op(k - l, hyf[l], v);
          if (l > 0) {
            decr(l);
          } else {
            break;
          }
        }
        q = 0; 
        hc[0] = cur_lang;
        while (l <= k) {
          c = hc[l];
          incr(l);
          p = trie_l[q];
          first_child = TRUE;
          while (p > 0 && c > trie_c[p]) {
            q = p;
            p = trie_r[q];
            first_child = FALSE;
          }
          if (p == 0 || c < trie_c[p]) {
            check_trie_ptr();
            incr(trie_ptr);
            trie_r[trie_ptr] = p;
            p = trie_ptr;
            trie_l[p] = 0;
            if (first_child) {
              trie_l[q] = p;
            } else {
              trie_r[q] = p;
            }
            trie_c[p] = c;
            trie_o[p] = 0;
          }
          q = p;
        }
        if (trie_o[q]) {
          print_err("Duplicate pattern");
          help1("(See Appendix H.)");
          error();
        }
        trie_o[q] = v;
        if (cur_cmd == RIGHT_BRACE)
          goto done;
        k = hyf[0] = 0;
        digit_sensed = FALSE;
        if (cur_cmd == RIGHT_BRACE)
          goto done;
        break;

      default:
        print_err("Bad ");
        print_esc("patterns");
        help1("(See Appendix H.)");
        error();
        break;
    }
  }
done:
  return;
}

check_trie_ptr ()
{
  if (trie_ptr == TRIE_SIZE) {
    overflow("pattern memory", TRIE_SIZE);
  }
}

  void
init_trie ()
{
  int	j, k, p, r, s, t;
  trie_t	z;

  op_start[0] = 0;
  for (j = 1; j <= 255; incr(j))
    op_start[j] = op_start[j - 1] + trie_op_used[j - 1];
  for (j = 1; j <= trie_op_ptr; incr(j))
    trie_op_hash[j] = op_start[trie_op_lang[j]] + trie_op_val[j];
  for (j = 1; j <= trie_op_ptr; incr(j)) {
    while (trie_op_hash[j] > j) {
      k = trie_op_hash[j];
      t = hyf_distance[k];
      hyf_distance[k] = hyf_distance[j];
      hyf_distance[j] = t;
      t = hyf_num[k];
      hyf_num[k] = hyf_num[j];
      hyf_num[j] = t;
      t = hyf_next[k];
      hyf_next[k] = hyf_next[j];
      hyf_next[j] = t;
      trie_op_hash[j] = trie_op_hash[k];
      trie_op_hash[k] = k;
    }
  }
  for (p = 0; p <= TRIE_SIZE; incr(p))
    trie_hash[p] = 0;
  trie_root = trie_compress(trie_root);
  for (p = 0; p <= trie_ptr; incr(p))
    trie_ref[p] = 0;
  for (p = 0; p <= 255; incr(p))
    trie_min[p] = p + 1;
  trie_link(0) = 1;
  trie_max = 0;
  if (trie_root != 0) {
    first_fit(trie_root);
    trie_pack(trie_root);
  }
  z.s = 0;
  z.u_s.bb.b0 = z.u_s.bb.b1 = 0;
  if (trie_root == 0) {
    for (r = 0; r <= 256; incr(r))
      trie[r] = z;
    trie_max = 256;
  } else {
    trie_fix(trie_root);
    for (r = 0; r <= trie_max; r = s) {
      s = trie_link(r);
      trie[r] = z;
    }
  }
  trie_char(0) = '?';
  trie_not_ready = FALSE;
  free_pattern_memory();
}

  int
new_trie_op (d, n, v)
  int	d;
  int	n;
  int	v;
{
  int	h;
  int	u;
  int	l;

  h = abs(n + 313 * d + 361 * v + 1009 * cur_lang);
  h = h % (TRIE_OP_SIZE + TRIE_OP_SIZE) - TRIE_OP_SIZE;
  loop {
    l = trie_op_hash[h];
    if (l == 0) {
      if (trie_op_ptr == TRIE_OP_SIZE) {
        overflow("pattern memory ops", TRIE_OP_SIZE);
      }
      u = trie_op_used[cur_lang];
      if (u == MAX_TRIE_OPS_PER_LANG) {
        overflow("pattern memory ops per language",
            MAX_TRIE_OPS_PER_LANG);
      }
      incr(u);
      trie_op_used[cur_lang] = u;
      incr(trie_op_ptr);
      hyf_distance[trie_op_ptr] = d;
      hyf_num[trie_op_ptr] = n;
      hyf_next[trie_op_ptr] = v;
      trie_op_lang[trie_op_ptr] = cur_lang;
      trie_op_val[trie_op_ptr] = u;
      trie_op_hash[h] = trie_op_ptr;
      return u;
    }
    if (hyf_distance[l] == d
        && hyf_num[l] == n
        && hyf_next[l] == v
        && trie_op_lang[l] == cur_lang) {
      return (trie_op_val[l]);
    }
    if (h > -TRIE_OP_SIZE) {
      decr(h);
    } else {
      h = TRIE_OP_SIZE;
    }
  }
}

  int
trie_compress (p)
  int	p;
{
  if (p == 0)
    return 0;
  trie_l[p] = trie_compress(trie_l[p]);
  trie_r[p] = trie_compress(trie_r[p]);
  return (trie_node(p));
}

  int
trie_node (p)
  int	p;
{
  int	h;
  int	q;

  h = trie_c[p] + 1009 * trie_o[p] +
    2718 * trie_l[p] + 3142 * trie_r[p];
  h = abs(h) % TRIE_SIZE;
  loop {
    q = trie_hash[h];
    if (q == 0) {
      trie_hash[h] = p; 
      return p;
    }
    if (trie_c[q] == trie_c[p]
        && trie_o[q] == trie_o[p]
        && trie_l[q] == trie_l[p]
        && trie_r[q] == trie_r[p])
      return q;
    if (h > 0) {
      decr(h);
    } else {
      h = TRIE_SIZE;
    }
  }
}

  void
trie_pack (p)
  int	p;
{
  int	q;

  do {
    q = trie_l[p];
    if (q > 0 && trie_ref[q] == 0) {
      first_fit(q);
      trie_pack(q);
    }
    p = trie_r[p];
  } while (p);
}

  void
first_fit (p)
  int	p;
{
  int	c, h, l, r, ll, q, z;

  c = trie_c[p];
  z = trie_min[c];
  loop {
    h = z - c;
    if (trie_max < h + 256) {
      if (TRIE_SIZE <= h + 256)
        overflow("pattern memory", TRIE_SIZE);
      while (trie_max != h + 256) {
        incr(trie_max); 
        trie_taken[trie_max] = FALSE;
        trie_link(trie_max) = trie_max + 1;
        trie_back(trie_max) = trie_max - 1;
      }
    }
    if (trie_taken[h])
      goto not_found;
    for (q = trie_r[p]; q > 0; q = trie_r[q])
      if (trie_link(h + trie_c[q]) == 0)
        goto not_found;
    goto found;

not_found:
    z = trie_link(z);
  }

found:
  trie_taken[h] = TRUE;
  trie_ref[p] = h;
  for (q = p; q > 0; q = trie_r[q]) {
    z = h + trie_c[q];
    l = trie_back(z);
    r = trie_link(z);
    trie_back(r) = l;
    trie_link(l) = r;
    trie_link(z) = 0;
    if (l < 256) {
      ll = (z < 256) ? z : 256;
      while (l != ll) {
        trie_min[l] = r;
        incr(l);
      }
    }
  }
}

  void
trie_fix (p)
  int	p;
{
  int	c;
  int	q;
  int	z;

  z = trie_ref[p];
  while (p) {
    q = trie_l[p];
    c = trie_c[p];
    trie_link(z + c) = trie_ref[q];
    trie_char(z + c) = c;
    trie_op(z + c) = trie_o[p];
    if (q > 0)
      trie_fix(q);
    p = trie_r[p];
  }
}

  void
init_pattern_memory ()
{
  int	l;
  int	p;

  l = TRIE_OP_SIZE + 1;
  trie_op_hash = (int *) malloc ((l+l)*sizeof(int));
  trie_op_hash += l;
  trie_op_val = (int *) malloc (l*sizeof(int));
  trie_op_lang = (int *) malloc (l*sizeof(int));
  trie_op_used = (int *) malloc (256*sizeof(int));
  for (p = -TRIE_OP_SIZE; p <= TRIE_OP_SIZE; incr(p))
    trie_op_hash[p] = 0;
  for (p = 0; p <= 255; incr(p))
    trie_op_used[p] = 0;
  trie_op_ptr = 0;

  l = TRIE_SIZE + 1;
  trie_hash = (int *) malloc (l*sizeof(int));
  trie_taken = (bool *) malloc (l*sizeof(bool));
  trie_c = (int *) malloc (l*sizeof(int));
  trie_o = (int *) malloc (l*sizeof(int));
  trie_l = (int *) malloc (l*sizeof(int));
  trie_r = (int *) malloc (l*sizeof(int));
  trie_min = (int *) malloc(256*sizeof(int));
  trie_root = 0;
  trie_c[0] = 0;
  trie_ptr = 0;
}

  void
free_pattern_memory ()
{
  free(trie_op_hash - TRIE_OP_SIZE - 1);
  free(trie_op_used);
  free(trie_op_val);
  free(trie_op_lang);
  free(trie_hash);
  free(trie_taken);
  free(trie_min);
  free(trie_c);
  free(trie_o);
  free(trie_l);
  free(trie_r);
}

  void
_hyph_init ()
{
}

  void
_hyph_init_once ()
{
  int	k;

  init_pattern_memory();
  trie = (trie_t *)malloc((TRIE_SIZE+1)*sizeof(trie_t));
  op_start = (int *)malloc(256*sizeof(int));
  hyf_distance = (int *)malloc((TRIE_OP_SIZE+1)*sizeof(int));
  hyf_num = (int *)malloc((TRIE_OP_SIZE+1)*sizeof(int));
  hyf_next = (int *)malloc((TRIE_OP_SIZE+1)*sizeof(int));
  hyph_word = (str *)malloc((HYPH_SIZE+1)*sizeof(str));
  hyph_len = (int *)malloc((HYPH_SIZE+1)*sizeof(int));
  hyph_list = (ptr *)malloc((HYPH_SIZE+1)*sizeof(ptr));
  for (k = 0; k <= HYPH_SIZE; incr(k)) {
    hyph_word[k] = null_str;
    hyph_list[k] = null;
  }
  hyph_count = 0;
  trie_not_ready = TRUE;
}

/*
 **	Help text
 */

help_patterns ()
{
  help1("All patterns must be given before typesetting begins.");
}

help_hyph_lccode ()
{
  help2("Letters in \\hyphenation words must have \\lccode > 0",
      "Proceed; I'll ignore the character I just read.");
}

help_hyph ()
{
  help2("Hyphenation exceptions must contain only letters",
      "and hyphens. But continue; I'll forgive and forget.");
}


/*
 * Common TeX:math.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX math.c 3.2@(#)";
#endif

#include "tex.h"

  ptr
new_style (s)
  int	s;
{
  ptr	p;

  p = new_node(STYLE_NODE_SIZE);
  type(p) = STYLE_NODE;
  subtype(p) = s;
  display_mlist(p) = null;
  text_mlist(p) = null;
  script_mlist(p) = null;
  script_script_mlist(p) = null;

  return p;
}

  ptr
new_choice ()
{
  ptr	p;

  p = new_node(STYLE_NODE_SIZE);
  type(p) = CHOICE_NODE;
  subtype(p) = 0;
  display_mlist(p) = null;
  text_mlist(p) = null;
  script_mlist(p) = null;
  script_script_mlist(p) = null;

  return p;
}

  ptr
new_noad ()
{
  ptr	p;

  p = new_node(NOAD_SIZE);
  type(p) = ORD_NOAD;
  subtype(p) = NORMAL;
  mzero(nucleus(p));
  mzero(subscr(p));
  mzero(supscr(p));

  return p;
}

  void
print_fam_and_char (p)
  ptr 	p;
{
  print_esc("fam");
  print_int(fam(p));
  print(" ");
  print_ASCII(character(p));
}

  void
print_delimiter (p)
  ptr	p;
{
  int	a;

  a = small_fam(p) * 256 + small_char(p);
  a = a * 0x1000 + large_fam(p) * 256 + large_char(p);
  if (a < 0)
    print_int(a);
  else print_hex(a);
}

  void
print_subsidiary_data (p, c)
  ptr	p;
  int	c;
{
  if (cur_length() >= depth_threshold) {
    if (math_type(p) != EMPTY)
      print(" []");
    return;
  }
  append_char(c);
  switch (math_type(p))
  {
    case MATH_CHAR:
      print_ln();
      print_str();
      print_fam_and_char(p);
      break;

    case SUB_BOX:
      show_node_list(math_link(p));
      break;

    case SUB_MLIST:
      if (math_link(p) == null) {
        print_ln();
        print_str();
        print("{}");
      } else {
        show_node_list(math_link(p));
      }
      break;
  }
  flush_char();
}

  void
print_style (c)
  int	c;
{
  switch (c / 2) {
    case 0: print_esc("displaystyle"); break;
    case 1: print_esc("textstyle"); break;
    case 2: print_esc("scriptstyle"); break;
    case 3: print_esc("scriptscriptstyle"); break;
    default: print("Unknown style!"); break;
  }
}

  void
print_size (s)
  int	s;
{
  if (s == TEXT_SIZE)
    print_esc("textfont");
  else if (s == SCRIPT_SIZE)
    print_esc("scriptfont");
  else print_esc("scriptscriptfont");
}

  void
show_normal_noad (p)
  ptr	p;
{
  switch (type(p)) 
  {
    case ORD_NOAD:
      print_esc("mathord");
      break;

    case OP_NOAD:
      print_esc("mathop");
      break;

    case BIN_NOAD:
      print_esc("mathbin");
      break;

    case REL_NOAD:
      print_esc("mathrel");
      break;

    case OPEN_NOAD:
      print_esc("mathopen");
      break;

    case CLOSE_NOAD:	
      print_esc("mathclose");
      break;

    case PUNCT_NOAD:
      print_esc("mathpunct");
      break;

    case INNER_NOAD:
      print_esc("mathinner");
      break;

    case OVER_NOAD:
      print_esc("overline");
      break;

    case UNDER_NOAD:
      print_esc("underline");
      break;

    case VCENTER_NOAD:
      print_esc("vcenter");
      break;

    case RADICAL_NOAD:
      print_esc("radical");
      print_delimiter(left_delimiter(p));
      break;

    case ACCENT_NOAD:
      print_esc("accent");
      print_fam_and_char(accent_chr(p));
      break;

    case LEFT_NOAD:
      print_esc("left");
      print_delimiter(nucleus(p));
      break;

    case RIGHT_NOAD:
      print_esc("right");
      print_delimiter(nucleus(p));
      break;

  }
  if (subtype(p) != NORMAL)
    if (subtype(p) == LIMITS)
      print_esc("limits");
    else print_esc("nolimits");
  if (type(p) < LEFT_NOAD)
    print_subsidiary_data(nucleus(p), '.');
  print_subsidiary_data(supscr(p), '^');
  print_subsidiary_data(subscr(p), '_');
}

  void
show_fraction_noad (p)
  ptr	p;
{
  print_esc("fraction, thickness ");
  if (thickness(p) == DEFAULT_CODE)
    print("= default");
  else print_scaled(thickness(p));
  if (small_fam(left_delimiter(p)) != 0 ||
      small_char(left_delimiter(p)) != MIN_QUARTERWORD ||
      large_fam(left_delimiter(p)) != 0 ||
      large_char(left_delimiter(p)) != MIN_QUARTERWORD) {
    print(", left-delimiter ");
    print_delimiter(left_delimiter(p));
  }
  if (small_fam(right_delimiter(p)) != 0 ||
      small_char(right_delimiter(p)) != MIN_QUARTERWORD ||
      large_fam(right_delimiter(p)) != 0 ||
      large_char(right_delimiter(p)) != MIN_QUARTERWORD) {
    print(", right-delimiter ");
    print_delimiter(right_delimiter(p));
  }
  print_subsidiary_data(numerator(p), '\\');
  print_subsidiary_data(denominator(p), '/');
}

  void
show_choice_node (p)
  ptr	p;
{
  print_esc("mathchoice");
  append_char('D');
  show_node_list(display_mlist(p));
  flush_char();
  append_char('T');
  show_node_list(text_mlist(p));
  flush_char();
  append_char('S');
  show_node_list(script_mlist(p));
  flush_char();
  append_char('s');
  show_node_list(script_script_mlist(p));
  flush_char();
}

  ptr
fraction_rule (t)
  scal	t;
{
  ptr	p;

  p = new_rule();
  rule_height(p) = t;
  rule_depth(p) = 0;

  return p;
}

  ptr
overbar (b, k, t)
  ptr	b;
  scal	k;
  scal	t;
{
  ptr	p;
  ptr	q;

  p = new_kern(k);
  link(p) = b;
  q = fraction_rule(t);
  link(q) = p;
  p = new_kern(t);
  link(p) = q;
  return (vpack(p, NATURAL));
}

#define check_variants()						\
  y = x; 								\
if (y >= font_bc(g) && y <= font_ec(g)) {			\
  contin:		q = char_info(g, y); 					\
  if (char_exists(q)) { 					\
    if (char_tag(q) == EXT_TAG)  { 			\
      f = g; 					\
      c = y; 					\
      goto found; 				\
    }						\
    hd = height_depth(q);				\
    u = char_height(g, hd) + char_depth(g, hd);	\
    if (u > w) {					\
      f = g;					\
      c = y;					\
      w = u;					\
      if (u >= v)				\
      goto found;			\
    }						\
    if (char_tag(q) == LIST_TAG) {			\
      y = rem_byte(q);			\
      goto contin;				\
    }						\
  }							\
}								\


  ptr
var_delimiter (d, s, v)
  ptr	d;
  int	s;
  scal	v;
{
  ptr	b;
  fnt	f, g;
  int	m, n;
  qcell	q, r;
  scal	u, w;
  int	c, x, y, z;
  int	hd;
  bool	large_attempt;

  f = null_font;
  w = 0;
  large_attempt = FALSE;
  z = small_fam(d);
  x = small_char(d);
  loop {
    if (z != 0 || x != 0) {
      z += s + 16;
      do {
        z -= 16;
        g = fam_fnt(z);
        if (g != null_font) {
          check_variants();
        }
      } while (z >= 16);
    }
    if (large_attempt)
      goto found;
    large_attempt = TRUE;
    z = large_fam(d);
    x = large_char(d);
  }

found:
  if (f != null_font) {
    if (char_tag(q) == EXT_TAG) {
      b = new_null_box();
      type(b) = VLIST_NODE;
      r = exten_base(f)[rem_byte(q)];
      c = ext_rep(r);
      u = height_plus_depth(f, c);
      w = 0;
      q = char_info(f, c);
      box_width(b) = char_width(f, q) + char_italic(f, q);
      c = ext_bot(r); 
      if (c != MIN_QUARTERWORD)
        w += height_plus_depth(f, c);
      c = ext_mid(r);
      if (c != MIN_QUARTERWORD)
        w += height_plus_depth(f, c);
      c = ext_top(r);
      if (c != MIN_QUARTERWORD)
        w += height_plus_depth(f, c);
      n = 0;
      if (u > 0) {
        while (w < v) {
          w += u;
          incr(n);
          if (ext_mid(r) != MIN_QUARTERWORD)
            w += u;
        }
      }
      c = ext_bot(r);
      if (c != MIN_QUARTERWORD)
        stack_into_box(b, f, c);
      c = ext_rep(r);
      for (m = 1; m <= n; incr(m)) 
        stack_into_box(b, f, c);
      c = ext_mid(r);
      if (c != MIN_QUARTERWORD) {
        stack_into_box(b, f, c);
        c = ext_rep(r);
        for (m = 1; m <= n; incr(m))
          stack_into_box(b, f, c);
      }
      c = ext_top(r);
      if (c != MIN_QUARTERWORD)
        stack_into_box(b, f, c);
      box_depth(b) = w - box_height(b);
    } else {
      b = char_box(f, c);
    }
  } else {
    b = new_null_box();
    box_width(b) = null_delimiter_space;
  }
  shift_amount(b) = half(box_height(b) - box_depth(b)) - axis_height(s);
  return b;
}

  ptr
char_box (f, c)
  fnt	f;
  int	c;
{
  ptr	b;
  ptr	p;
  qcell	q;
  int	hd;

  q = char_info(f, c);
  hd = height_depth(q);
  b = new_null_box();
  box_width(b) = char_width(f, q) + char_italic(f, q);
  box_height(b) = char_height(f, hd);
  box_depth(b) = char_depth(f, hd);
  p = new_avail();
  character(p) = c;
  font(p) = f;
  list_ptr(b) = p;

  return b;
}

  void
stack_into_box (b, f, c)
  ptr	b;
  fnt	f;
  int	c;
{
  ptr	p;

  p = char_box(f, c);
  link(p) = list_ptr(b);
  list_ptr(b) = p;
  box_height(b) = box_height(p);
}

  scal
height_plus_depth (f, c)
  fnt	f;
  int	c;
{
  qcell	q;
  int	hd;

  q = char_info(f, c);
  hd = height_depth(q);
  return (char_height(f, hd) + char_depth(f, hd));
}

  ptr
rebox (b, w)
  ptr	b;
  scal	w;
{
  fnt	f;
  ptr	p;
  scal	v;

  if (box_width(b) != w && list_ptr(b) != null) {
    if (type(b) == VLIST_NODE)
      b = hpack(b, NATURAL);
    p = list_ptr(b);
    if (is_char_node(p) && link(p) == null) {
      f = font(p);
      v = char_width(f, char_info(f, character(p)));
      if (v != box_width(b)) 
        link(p) = new_kern(box_width(b) - v);
    }
    free_node(b, BOX_NODE_SIZE);
    b = new_glue(ss_glue);
    link(b) = p;
    while (link(p) != null)
      p = link(p);
    link(p) = new_glue(ss_glue); 
    return (hpack(b, w, EXACTLY));
  } else {
    box_width(b) = w;
    return b;
  }
}

#define mu_mult(x) \
  nx_plus_y(n, x, xn_over_d(x, f, 0200000))

  ptr
math_glue (g, m)
  ptr	g;
  scal	m;
{
  scal	f;
  int	n;
  ptr	p;

  n = x_over_n(m, 0200000);
  f = remainder;
  p = new_node(GLUE_SPEC_SIZE);
  glue_width(p) = mu_mult(glue_width(g));
  stretch_order(p) = stretch_order(g);
  if (stretch_order(p) == NORMAL)
    stretch(p) = mu_mult(stretch(g));
  else stretch(p) = stretch(g);
  shrink_order(p) = shrink_order(g);
  if (shrink_order(p) == NORMAL)
    shrink(p) = mu_mult(shrink(g));
  else shrink(p) = shrink(g);

  return p;
}

  void
math_kern (p, m)
  ptr	p;
  scal	m;
{
  scal	f;
  int	n;

  if (subtype(p) == MU_GLUE) {
    n = x_over_n(m, 0200000);
    f = remainder;
    kern_width(p) = mu_mult(kern_width(p));
    subtype(p) = NORMAL;
  }
}

  void
flush_math ()
{
  flush_node_list(link(head));
  flush_node_list(incompleat_noad);
  link(head) = null;
  tail = head;
  incompleat_noad = null;
}

  void
_math_init ()
{
}

  void
_math_init_once ()
{
}

/*
 * Common TeX:mathlist.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX mathlist.c 3.2@(#)";
#endif

#include "tex.h"

  void
push_math (c)
  int	c;
{
  push_nest();
  mode = -MMODE;
  incompleat_noad = null;
  new_save_level(c);
}

  void
init_math ()
{
  fnt	f;
  int	m, n;
  ptr	p, q;
  scal	d, l, s, v, w;

  get_token();
  if (cur_cmd == MATH_SHIFT && mode > 0) {
    if (head == tail) {
      pop_nest();
      w = -MAX_DIMEN;
    } else {
      line_break(display_widow_penalty);
      v = shift_amount(just_box) + 2 * quad(cur_font);
      w = -MAX_DIMEN;
      p = list_ptr(just_box);
      while (p != null) {
reswitch:
        if (is_char_node(p)) {
          f = font(p);
          d = char_width(f,
              char_info(f, character(p)));
          goto found;
        }
        switch (type(p))
        {
          case HLIST_NODE:
          case VLIST_NODE:
          case RULE_NODE:
            d = box_width(p);
            goto found;

          case LIGATURE_NODE:
            p = make_char_from_lig(p);
            goto reswitch;

          case KERN_NODE:
            d = kern_width(p);
            break;

          case MATH_NODE:
            d = box_width(p);
            break;

          case GLUE_NODE:
            q = glue_ptr(p);
            d = box_width(q);
            if (glue_sign(just_box) ==
                STRETCHING) {
              if (glue_order(just_box) ==
                  stretch_order(q)
                  && stretch(q) != 0)
                v = MAX_DIMEN;
            } else if (glue_sign(just_box) ==
                SHRINKING)  {
              if (glue_order(just_box) ==
                  shrink_order(q)
                  && shrink(q) != 0) 
                v = MAX_DIMEN;
            }
            if (subtype(p) >= A_LEADERS)	
              goto found;
            break;

          case WHATSIT_NODE: 
            d = 0;
            break;

          default:
            d = 0;
            break;
        }
        if (v < MAX_DIMEN)
          v += d;
        goto not_found;

found:
        if (v < MAX_DIMEN) {
          v += d;
          w = v;
        } else {
          w = MAX_DIMEN;
          break;
        }

not_found:
        p = link(p);
      }
    }
    if (par_shape_ptr == null) {
      if (hang_indent != 0
          && (hang_after >= 0
            && prev_graf + 2 > hang_after
            || prev_graf + 1 < -hang_after)) {
        l = hsize - abs(hang_indent);
        s = (hang_indent > 0) ? hang_indent : 0;
      } else {
        l = hsize;
        s = 0;
      }
    } else {
      n = info(par_shape_ptr);
      if (prev_graf + 2 >= n)
        m = n;
      else m = prev_graf + 2;
      l = par_shape_width(m);
      s = par_shape_indent(m);
    }
    push_math(MATH_SHIFT_GROUP);
    mode = MMODE;
    reg_define(int_reg[CUR_FAM_CODE], INT_REG, -1);
    reg_define(dimen_reg[PRE_DISPLAY_SIZE_CODE], DIMEN_REG, w);
    reg_define(dimen_reg[DISPLAY_WIDTH_CODE], DIMEN_REG, l);
    reg_define(dimen_reg[DISPLAY_INDENT_CODE], DIMEN_REG, s);
    if (every_display != null)
      begin_token_list(every_display, EVERY_DISPLAY_TEXT);
    if (nest_ptr == nest + 1)
      build_page();
  } else {
    back_input(); 
    push_math(MATH_SHIFT_GROUP);
    reg_define(int_reg[CUR_FAM_CODE], INT_REG, -1);
    if (every_math != null)
      begin_token_list(every_math, EVERY_MATH_TEXT);
  }
}

  void
start_eq_no ()
{
  saved(0) = cur_chr;
  incr(save_ptr);
  push_math(MATH_SHIFT_GROUP);
  reg_define(int_reg[CUR_FAM_CODE], INT_REG, -1);
  if (every_math != null)
    begin_token_list(every_math, EVERY_MATH_TEXT);
}

#define fam_in_range()	(cur_fam >= 0 && cur_fam < 16)

  void
scan_math (p)
  ptr	p;
{
  int	c;

restart:
  get_nbrx_token();

reswitch:
  switch (cur_cmd)
  {
    case LETTER:
    case OTHER_CHAR:
    case CHAR_GIVEN:
      c = math_code(cur_chr);
      if (c == 0100000) {
        cur_cs = active_base[cur_chr];
        cur_cmd = eq_type(cur_cs);
        cur_chr = equiv(cur_cs);
        x_token();
        back_input();
        goto restart;
      }
      break;

    case CHAR_NUM:
      scan_char_num();
      cur_chr = cur_val;
      cur_cmd = CHAR_GIVEN;
      goto reswitch;

    case MATH_CHAR_NUM:
      scan_fifteen_bit_int();
      c = cur_val;
      break;

    case MATH_GIVEN:
      c = cur_chr;
      break;

    case DELIM_NUM:
      scan_twenty_seven_bit_int();
      c = cur_val / 010000;
      break;

    default:
      back_input();
      scan_left_brace();
      saved(0) = p;
      incr(save_ptr);
      push_math(MATH_GROUP);
      return;
  }
  math_type(p) = MATH_CHAR;
  character(p) = c % 256;
  if (c >= VAR_CODE && fam_in_range()) {
    fam(p) = cur_fam;
  } else {
    fam(p) = c / 256 % 16;
  }
}

  void
set_math_char (c)
  int	c;
{
  ptr	p;

  if (c >= 0100000) {
    cur_cs = active_base[cur_chr];
    cur_cmd = eq_type(cur_cs);
    cur_chr = equiv(cur_cs);
    x_token();
    back_input();
  } else {
    p = new_noad();
    math_type(nucleus(p)) = MATH_CHAR;
    character(nucleus(p)) = c % 256;
    fam(nucleus(p)) = c / 256 % 16;
    if (c >= VAR_CODE) {
      if (fam_in_range())
        fam(nucleus(p)) = cur_fam;
      type(p) = ORD_NOAD;
    } else {
      type(p) = ORD_NOAD + c / 010000;
    }
    tail_append(p);
  }
}

  void
math_limit_switch ()
{
  if (head != tail && type(tail) == OP_NOAD) {
    subtype(tail) = cur_chr;
    return;
  }
  print_err("Limit controls must follow a math operator");
  help_limits();
  error();
}

  void
scan_delimiter (p, r)
  ptr	p;
  bool	r;
{
  if (r) {
    scan_twenty_seven_bit_int();
  } else {
    get_nbrx_token();
    switch (cur_cmd) 
    {
      case LETTER:
      case OTHER_CHAR:
        cur_val = del_code(cur_chr);
        break;

      case DELIM_NUM:
        scan_twenty_seven_bit_int();
        break;

      default:
        cur_val = -1;
        break;
    }
  }
  if (cur_val < 0) {
    print_err("Missing delimiter (. inserted)");
    help_delimiter();
    back_error();
    cur_val = 0;
  }
  small_fam(p) = cur_val / 04000000 % 16;
  small_char(p) = cur_val / 010000 % 256;
  large_fam(p) = cur_val / 256 % 16;
  large_char(p) = cur_val % 256;
}

  void
math_radical ()
{
  tail_append(new_node(RADICAL_NOAD_SIZE));
  type(tail) = RADICAL_NOAD;
  subtype(tail) = NORMAL;
  mzero(nucleus(tail));
  mzero(supscr(tail));
  mzero(subscr(tail));
  scan_delimiter(left_delimiter(tail), TRUE);
  scan_math(nucleus(tail));
}

  void
math_ac ()
{
  if (cur_cmd == ACCENT) {
    print_err("Please use ");
    print_esc("mathaccent");
    print(" for accents in math mode");
    help_math_accent();
    error();
  }
  tail_append(new_node(ACCENT_NOAD_SIZE));
  type(tail) = ACCENT_NOAD;
  subtype(tail) = NORMAL;
  mzero(nucleus(tail));
  mzero(subscr(tail));
  mzero(supscr(tail));
  math_type(accent_chr(tail)) = MATH_CHAR;
  scan_fifteen_bit_int();
  character(accent_chr(tail)) = cur_val % 256;
  if (cur_val >= VAR_CODE && fam_in_range()) {
    fam(accent_chr(tail)) = cur_fam;
  } else {
    fam(accent_chr(tail)) = cur_val / 256 % 16;
  }
  scan_math(nucleus(tail));
}

  void
append_choices ()
{
  tail_append(new_choice());
  incr(save_ptr);
  saved(-1) = 0;
  scan_left_brace();
  push_math(MATH_CHOICE_GROUP);
}

  void
build_choices ()
{
  ptr	p;

  unsave();
  p = fin_mlist(null);
  switch (saved(-1)) {
    case 0: display_mlist(tail) = p; break;
    case 1: text_mlist(tail) = p; break;
    case 2: script_mlist(tail) = p; break;
    case 3: script_script_mlist(tail) = p; decr(save_ptr); return;
  }
  incr(saved(-1));
  scan_left_brace();
  push_math(MATH_CHOICE_GROUP);
}

  void
sub_sup ()
{
  ptr	p;
  int	t;

  p = null;
  t = EMPTY;
  if (tail != head && scripts_allowed(tail)) {
    p = (cur_cmd == SUP_MARK) ? supscr(tail) : subscr(tail);
    t = math_type(p);
  }
  if (p == null || t != EMPTY) {
    tail_append(new_noad());
    p = (cur_cmd == SUP_MARK) ? supscr(tail) : subscr(tail);
    if (t != EMPTY) {
      if (cur_cmd == SUP_MARK) {
        print_err("Double superscript");
        help_double_sup();
      } else {
        print_err("Double subscript");
        help_double_sub();
      }
      error();
    }
  }
  scan_math(p);
}

  void
math_fraction ()
{
  int	c;
  mcell	garbage;

  c = cur_chr;
  if (incompleat_noad != null) {
    if (c >= DELIMITED_CODE) {
      scan_delimiter((ptr)&garbage, FALSE);
      scan_delimiter((ptr)&garbage, FALSE);
    }
    if (c % DELIMITED_CODE == ABOVE_CODE)
      scan_normal_dimen();
    print_err("Ambiguous; you need another { and }");
    help_fraction();
    error();
  } else {
    incompleat_noad = new_node(FRACTION_NOAD_SIZE);
    type(incompleat_noad) = FRACTION_NOAD;
    subtype(incompleat_noad) = NORMAL;
    math_type(numerator(incompleat_noad)) = SUB_MLIST;
    math_link(numerator(incompleat_noad)) = link(head);
    mzero(denominator(incompleat_noad));
    mzero(left_delimiter(incompleat_noad));
    mzero(right_delimiter(incompleat_noad));
    link(head) = null;
    tail = head;
    if (c >= DELIMITED_CODE) {
      scan_delimiter(left_delimiter(incompleat_noad), FALSE);
      scan_delimiter(right_delimiter(incompleat_noad), FALSE);
    }
    switch (c % DELIMITED_CODE)
    {
      case ABOVE_CODE:
        scan_normal_dimen();
        thickness(incompleat_noad) = cur_val;
        break;

      case OVER_CODE:
        thickness(incompleat_noad) = DEFAULT_CODE;
        break;

      case ATOP_CODE:
        thickness(incompleat_noad) = 0;
        break;
    }
  }
}


  void
math_left_right ()
{
  ptr	p;
  int	t;
  mcell	garbage;

  t = cur_chr;
  if (t == RIGHT_NOAD && cur_group != MATH_LEFT_GROUP) {
    if (cur_group == MATH_SHIFT_GROUP) {
      scan_delimiter((ptr)&garbage, FALSE);
      print_err("Extra ");
      print_esc("right");
      help_xtra_right();
      error();
    } else {
      off_save();
    }
  } else {
    p = new_noad();
    type(p) = t;
    scan_delimiter(delimiter(p), FALSE);
    if (t == LEFT_NOAD) {
      push_math(MATH_LEFT_GROUP);
      tail = link(head) = p;
    } else {
      p = fin_mlist(p);
      unsave();
      tail_append(new_noad());
      type(tail) = INNER_NOAD;
      math_type(nucleus(tail)) = SUB_MLIST;
      math_link(nucleus(tail)) = p;
    }
  }
}

  ptr
fin_mlist (p)
  ptr	p;
{
  ptr	q;

  if (incompleat_noad != null) {
    math_type(denominator(incompleat_noad)) = SUB_MLIST;
    math_link(denominator(incompleat_noad)) = link(head);
    if (p == null) {
      q = incompleat_noad;
    } else {
      q = info(numerator(incompleat_noad));
      if (type(q) != LEFT_NOAD)
        confusion("right");
      math_link(numerator(incompleat_noad)) = link(q);
      link(q) = incompleat_noad;
      link(incompleat_noad) = p;
    }
  } else {
    link(tail) = p;
    q = link(head);
  }
  pop_nest();
  return q;
}

  void
after_math ()
{
  ptr	a;
  ptr	b;
  scal	d;
  scal	e;
  bool	l;
  int	m;
  ptr	p;
  scal	q;
  ptr	r;
  scal	s;
  scal	t;
  scal	w;
  scal	z;
  int	g1;
  int	g2;
  bool	danger;

  danger = check_font_params();
  m = mode;
  l = FALSE;
  p = fin_mlist(null);
  if (mode == -m) {
    check_dollar();
    a = hpack(mlist_to_hlist(p, TEXT_STYLE, FALSE), NATURAL);
    unsave();
    decr(save_ptr);
    if (saved(0) == 1)
      l = TRUE;
    danger = check_font_params();
    m = mode;
    p = fin_mlist(null);
  } else {
    a = null;
  }
  if (m < 0) {
    tail_append(new_math(math_surround, BEFORE));
    link(tail) = mlist_to_hlist(p, TEXT_STYLE, mode > 0);
    while (link(tail) != null)
      tail = link(tail);
    tail_append(new_math(math_surround, AFTER));
    space_factor = 1000;
    unsave();
  } else {
    if (a == null)
      check_dollar();
    p = mlist_to_hlist(p, DISPLAY_STYLE, FALSE);
    adjust_tail = adjust_head;
    b = hpack(p, NATURAL);
    p = list_ptr(b);
    t = adjust_tail;
    adjust_tail = null;
    w = box_width(b);
    z = display_width;
    s = display_indent;
    if (a == null || danger) {
      e = q = 0;
    } else {
      e = box_width(a);
      q = e + math_quad(TEXT_SIZE);
    }
    if (w + q > z) {
      if (e != 0
          && (w - total_shrink[NORMAL] + q <= z
            || total_shrink[FIL] != 0
            || total_shrink[FILL] != 0
            || total_shrink[FILLL] != 0)) {
        free_node(b, BOX_NODE_SIZE);
        b = hpack(p, z - q, EXACTLY);
      } else {
        e = 0;
        if (w > z) {
          free_node(b, BOX_NODE_SIZE);
          b = hpack(p, z, EXACTLY);
        }
      }
      w = box_width(b);
    }
    d = half(z - w);
    if (e > 0 && d < 2 * e) {
      d = half(z - w - e);
      if (p != null && type(p) == GLUE_NODE) {
        d = 0;
      }
    }
    tail_append(new_penalty(pre_display_penalty));
    if (d + s <= pre_display_size || l) {
      g1 = ABOVE_DISPLAY_SKIP_CODE;
      g2 = BELOW_DISPLAY_SKIP_CODE;
    } else {
      g1 = ABOVE_DISPLAY_SHORT_SKIP_CODE;
      g2 = BELOW_DISPLAY_SHORT_SKIP_CODE;
    }
    if (l && e == 0) {
      shift_amount(a) = s;
      append_to_vlist(a);
      tail_append(new_penalty(INF_PENALTY));
    } else {
      tail_append(new_param_glue(g1));
    }
    if (e != 0) {
      r = new_kern(z - w - e - d);
      if (l) {
        link(a) = r;
        link(r) = b;
        b = a;
        d = 0;
      } else {
        link(b) = r;
        link(r) = a;
      }
      b = hpack(b, NATURAL);
    }
    shift_amount(b) = s + d;
    append_to_vlist(b);
    if (a != null && e == 0 && !l) {
      tail_append(new_penalty(INF_PENALTY));
      shift_amount(a) = s + z - box_width(a);
      append_to_vlist(a);
      g2 = 0;
    }
    if (t != adjust_head) {
      link(tail) = link(adjust_head);
      tail = t;
    }
    tail_append(new_penalty(post_display_penalty));
    if (g2 > 0) {
      tail_append(new_param_glue(g2));
    }
    resume_after_display();
  }
}

  void
check_dollar ()
{
  get_x_token();
  if (cur_cmd != MATH_SHIFT) {
    print_err("Display math should end with $$");
    help_doldol();
    back_error();
  }
}

  bool
check_font_params ()
{
#define CHK_SY(S) font_params(fam_fnt(2 + (S))) < TOTAL_MATHSY_PARAMS
#define CHK_EX(S) font_params(fam_fnt(3 + (S))) < TOTAL_MATHEX_PARAMS
  if (CHK_SY(TEXT_SIZE)
      || CHK_SY(SCRIPT_SIZE)
      || CHK_SY(SCRIPT_SCRIPT_SIZE)) {
    print_err("Math formula deleted: Insufficient symbol fonts");
    help_math_sy();
    error();
    flush_math();
    return TRUE;
  } 
  if (CHK_EX(TEXT_SIZE)
      || CHK_EX(SCRIPT_SIZE)
      || CHK_EX(SCRIPT_SCRIPT_SIZE)) {
    print_err("Math formula deleted: Insufficient extension fonts");
    help_math_ex();
    error();
    flush_math();
    return TRUE;
  }
  return FALSE;
}

  void
resume_after_display ()
{
  if (cur_group != MATH_SHIFT_GROUP)
    confusion("display");
  unsave();
  prev_graf += 3;
  push_nest();
  mode = HMODE;
  space_factor = 1000;
  clang = 0;
  scan_optional_space();
  if (nest_ptr == nest + 1) 
    build_page();
}

  void
_mathlist_init ()
{
}

  void
_mathlist_init_once ()
{
}

/*
 **	Help text
 */

help_math_accent ()
{
  help2("I'm changing \\accent to \\mathaccent here; wish me luck.",
      "(Accents are not the same in formulas as they are in text.)" );
}

help_math_sy ()
{
  help3("Sorry, but I can't typeset math unless \\textfont 2",
      "and \\scriptfont 2 and \\scriptscriptfont 2 have all",
      "the \\fontdimen values needed in math symbol fonts." );
}

help_math_ex ()
{
  help3("Sorry, but I can't typeset math unless \\textfont 3",
      "and \\scriptfont 3 and \\scriptscriptfont 3 have all",
      "the \\fontdimen values needed in math extension fonts." );
}

help_limits ()
{
  help1("I'm ignoring this misplaced \\limits or \\nolimits command.");
}

help_delimiter ()
{
  help6("I was expecting to see something like `(' or `\\{' or",
      "`\\}' here. If you typed, e.g., `{' instead of `\\{', you",
      "should probably delete the `{' by typing `1' now, so that",
      "braces don't get unbalanced. Otherwise just proceed.",
      "Acceptable delimiters are characters whose \\delcode is",
      "nonnegative, or you can use `\\delimiter <delimiter code>'.");
}

help_fraction ()
{
  help3("I'm ignoring this fraction specification, since I don't",
      "know whether a construction like `x \\over y \\over z'",
      "means `{x \\over y} \\over z' or `x \\over {y \\over z}'.");
}

help_xtra_right ()
{
  help1("I'm ignoring a \\right that had no matching \\left.");
}

help_doldol ()
{
  help2("The `$' that I just saw supposedly matches a previous `$$'.",
      "So I shall assume that you typed `$$' both times.");
}

help_double_sub ()
{
  help1("I treat `x_1_2' essentially like `x_1{}_2'.");
}

help_double_sup ()
{
  help1("I treat `x^1^2' essentially like `x^1{}^2'.");
}

/*
 * Common TeX:mlst-hlst.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX mlst-hlst.c 3.2@(#)";
#endif

#include "tex.h"

fnt	cur_f;
int	cur_c;
qcell	cur_i;

int	cur_style;
int	cur_size;
scal	cur_mu;

#define change_size_and_mu() \
{	if (cur_style < SCRIPT_STYLE) { \
  cur_size = TEXT_SIZE; \
                                } else { \
                                  cur_size = 16 * ((cur_style - TEXT_STYLE) / 2); \
                                } \
  cur_mu = x_over_n(math_quad(cur_size), 18); \
}

int	magic_offset = - 9 * ORD_NOAD;

char	math_spacing[] = 
"0234000122*4000133**3**344*0400400*000000234000111*1111112341011";

#define new_hlist(N)		link(nucleus(N))
#define choose_mlist(L)	{p = L(q); L(q) = null;}

  ptr
mlist_to_hlist (mlist, style, penalties)
  ptr	mlist;
  int	style;
  bool	penalties;
{
  ptr	h;
  ptr	p;
  ptr	q;
  ptr	r;
  int	s;
  int	t;
  ptr	x;
  ptr	y;
  ptr	z;
  int	pen;
  scal	delta;
  scal	max_d;
  scal	max_h;
  int	r_type;
  int	save_style;

  q = mlist;
  r = null;
  r_type = OP_NOAD;
  max_h = 0;
  max_d = 0;
  cur_style = style;
  change_size_and_mu();

  while (q != null)
  {
reswitch:
    delta = 0;
    switch (type(q))
    {
      case BIN_NOAD:	
        switch (r_type)
        {
          case BIN_NOAD:
          case OP_NOAD:
          case REL_NOAD:
          case OPEN_NOAD:
          case PUNCT_NOAD:
          case LEFT_NOAD:
            type(q) = ORD_NOAD;
            goto reswitch;

          default:
            break;
        }
        break;

      case REL_NOAD:
      case CLOSE_NOAD:
      case PUNCT_NOAD:
      case RIGHT_NOAD:
        if (r_type == BIN_NOAD)
          type(r) = ORD_NOAD;
        if (type(q) == RIGHT_NOAD)
          goto done_with_noad;
        break;

      case LEFT_NOAD:
        goto done_with_noad;

      case FRACTION_NOAD:
        make_fraction(q);
        goto check_dimensions;

      case OP_NOAD:
        delta = make_op(q);
        if (subtype(q) == LIMITS)
          goto check_dimensions;
        break;

      case ORD_NOAD:
        make_ord(q);
        break;

      case OPEN_NOAD:
      case INNER_NOAD:
        break;

      case RADICAL_NOAD:
        make_radical(q);
        break;

      case OVER_NOAD:
        make_over(q);
        break;

      case UNDER_NOAD:
        make_under(q);
        break;

      case ACCENT_NOAD:
        make_math_accent(q);
        break;

      case VCENTER_NOAD:
        make_vcenter(q);
        break;

      case STYLE_NODE:
        cur_style = subtype(q);
        change_size_and_mu();
        goto done_with_node;

      case CHOICE_NODE:
        switch (cur_style / 2) {
          case 0: choose_mlist(display_mlist); break;
          case 1: choose_mlist(text_mlist); break;
          case 2: choose_mlist(script_mlist); break;
          case 3: choose_mlist(script_script_mlist); break;
        }
        flush_node_list(display_mlist(q));
        flush_node_list(text_mlist(q));
        flush_node_list(script_mlist(q));
        flush_node_list(script_script_mlist(q));
        memset((void *)q, 0, STYLE_NODE_SIZE);
        type(q) = STYLE_NODE;
        subtype(q) = cur_style;
        if (p != null) {
          z = link(q);
          link(q) = p;
          while (link(p) != null)
            p = link(p);
          link(p) = z;
        }
        goto done_with_node;

      case INS_NODE:
      case MARK_NODE:
      case ADJUST_NODE:
      case WHATSIT_NODE:
      case PENALTY_NODE:
      case DISC_NODE:
        goto done_with_node;

      case RULE_NODE:
        if (rule_height(q) > max_h)
          max_h = rule_height(q);
        if (rule_depth(q) > max_d)
          max_d = rule_depth(q);
        goto done_with_node;

      case GLUE_NODE:
        if (subtype(q) == MU_GLUE) {
          x = glue_ptr(q);
          y = math_glue(x, cur_mu);
          delete_glue_ref(x);
          glue_ptr(q) = y;
          subtype(q) = NORMAL;
        } else if (cur_size != TEXT_SIZE &&
            subtype(q) == COND_MATH_GLUE) {
          p = link(q);
          if (p != null &&
              (type(p) == GLUE_NODE ||
               type(p) == KERN_NODE)) {
            link(q) = link(p);
            link(p) = null;
            flush_node_list(p);
          }
        }
        goto done_with_node;

      case KERN_NODE:
        math_kern(q, cur_mu);
        goto done_with_node;

      default:
        confusion("mlist1");
    }

    switch (math_type(nucleus(q)))
    {
      case MATH_CHAR:
      case MATH_TEXT_CHAR:
        fetch(nucleus(q));
        if (char_exists(cur_i)) {
          delta = char_italic(cur_f, cur_i);
          p = new_character(cur_f, cur_c);
          if (math_type(nucleus(q)) == MATH_TEXT_CHAR &&
              space(cur_f) != 0)
            delta = 0;
          if (math_type(subscr(q)) == EMPTY &&
              delta != 0) {
            link(p) = new_kern(delta);
            delta = 0;
          }
        } else {
          p = null;
        }
        break;

      case EMPTY:
        p = null;
        break;

      case SUB_BOX:
        p = math_link(nucleus(q));
        break;

      case SUB_MLIST:
        save_style = cur_style;
        p = mlist_to_hlist(math_link(nucleus(q)),
            cur_style, FALSE);
        cur_style = save_style;
        change_size_and_mu();
        p = hpack(p, NATURAL); 
        break;

      default:
        confusion("mlist2");
    }
    new_hlist(q) = p;
    if (math_type(subscr(q)) == EMPTY && 
        math_type(supscr(q)) == EMPTY)
      goto check_dimensions;
    make_scripts(q, delta);

check_dimensions:
    z = hpack(new_hlist(q), NATURAL);
    if (box_height(z) > max_h)
      max_h = box_height(z);
    if (box_depth(z) > max_d)
      max_d = box_depth(z);
    free_node(z, BOX_NODE_SIZE);

done_with_noad:
    r = q;
    r_type = type(r);

done_with_node:
    q = link(q);
  } 

  if (r_type == BIN_NOAD)
    type(r) = ORD_NOAD;
  h = p = new_avail();
  link(p) = null;
  q = mlist;
  r_type = 0;
  cur_style = style;
  change_size_and_mu();

  while (q != null) {
    t = ORD_NOAD;
    s = NOAD_SIZE;
    pen = INF_PENALTY;
    switch (type(q))
    {
      case OP_NOAD:
      case OPEN_NOAD:
      case CLOSE_NOAD:
      case PUNCT_NOAD:
      case INNER_NOAD:
        t = type(q);
        break;

      case BIN_NOAD:
        t = BIN_NOAD;
        pen = bin_op_penalty;
        break;

      case REL_NOAD:
        t = REL_NOAD;
        pen = rel_penalty;
        break; 

      case ORD_NOAD:
      case VCENTER_NOAD:
      case OVER_NOAD:
      case UNDER_NOAD:
        break;

      case RADICAL_NOAD:
        s = RADICAL_NOAD_SIZE;
        break;

      case ACCENT_NOAD:
        s = ACCENT_NOAD_SIZE;
        break;

      case FRACTION_NOAD:
        t = INNER_NOAD;
        s = FRACTION_NOAD_SIZE;
        break;

      case LEFT_NOAD:
      case RIGHT_NOAD:
        t = make_left_right(q, style, max_d, max_h);
        break;

      case STYLE_NODE:
        s = STYLE_NODE_SIZE;
        cur_style = subtype(q);
        change_size_and_mu();
        goto delete_q;

      case WHATSIT_NODE:
      case PENALTY_NODE:
      case RULE_NODE:
      case DISC_NODE:
      case ADJUST_NODE:
      case INS_NODE:
      case MARK_NODE:
      case GLUE_NODE:
      case KERN_NODE:
        p = link(p) = q;
        q = link(q);
        link(p) = null;
        continue;

      default:
        confusion("mlist3");
    }

    if (r_type > 0) {
      int	x;

      switch (math_spacing[r_type * 8 + t + magic_offset]) 
      {
        case '0':
          x = -1;
          break;

        case '1':
          if (cur_style < SCRIPT_STYLE)
            x = THIN_MU_SKIP_CODE;
          else x = -1;
          break;

        case '2':
          x = THIN_MU_SKIP_CODE;
          break;

        case '3':
          if (cur_style < SCRIPT_STYLE)
            x = MED_MU_SKIP_CODE;
          else x = -1;
          break;

        case '4':
          if (cur_style < SCRIPT_STYLE)
            x = THICK_MU_SKIP_CODE;
          else x = -1;
          break;

        default:
          confusion("mlist4");
          break;
      }
      if (x >= 0) {
        y = math_glue(mu_glue_par(x), cur_mu);
        z = new_glue(y);
        glue_ref_count(y) = null;
        p = link(p) = z;
        subtype(z) = GLUE_PARS + x + 1;
      }
    }
    if (new_hlist(q) != null) {
      link(p) = new_hlist(q);
      do p = link(p);
      while (link(p) != null);
    }
    if (penalties && link(q) != null && pen < INF_PENALTY) {
      r_type = type(link(q));
      if (r_type != PENALTY_NODE && r_type != REL_NOAD) {
        z = new_penalty(pen);
        p = link(p) = z;
      }
    }
    r_type = t;

delete_q:
    r = q;
    q = link(q);
    free_node(r, s);
  }
  return (link(h));
}

  void
make_over (q)
  ptr	q;
{
  math_link(nucleus(q)) =
    overbar(clean_box(nucleus(q), cramped_style(cur_style)),
        3 * default_rule_thickness, default_rule_thickness);
  math_type(nucleus(q)) = SUB_BOX;
}

  void
make_under (q)
  ptr 	q;
{
  ptr	p;
  ptr	x;
  ptr	y;
  scal	delta;

  x = clean_box(nucleus(q), cur_style);
  link(x) = p = new_kern(3 * default_rule_thickness);
  link(p) = fraction_rule(default_rule_thickness);
  y = vpack(x, NATURAL);
  delta = box_height(y) + box_depth(y) + default_rule_thickness;
  box_height(y) = box_height(x);
  box_depth(y) = delta - box_height(y);
  math_link(nucleus(q)) = y;
  math_type(nucleus(q)) = SUB_BOX;
}

  void
make_vcenter (q)
  ptr	q;
{
  ptr	v;
  scal	delta;

  v = math_link(nucleus(q));
  if (type(v) != VLIST_NODE)
    confusion("vcenter");
  delta = box_height(v) + box_depth(v);
  box_height(v) = axis_height(cur_size) + half(delta);
  box_depth(v) = delta - box_height(v);
}

  void
make_radical (q)
  ptr	q;
{
  ptr	x;
  ptr	y;
  scal	clr;
  scal	delta;

  x = clean_box(nucleus(q), cramped_style(cur_style));
  clr = default_rule_thickness;
  if (cur_style < TEXT_STYLE) {
    clr += abs(math_x_height(cur_size)) / 4;
  } else {
    clr += (abs(clr) / 4);
  }
  y = var_delimiter(left_delimiter(q), cur_size,
      box_height(x) + box_depth(x) + clr + default_rule_thickness);
  delta = box_depth(y) - (box_height(x) + box_depth(x) + clr);
  if (delta > 0)
    clr += half(delta);
  shift_amount(y) = -(box_height(x) + clr);
  link(y) = overbar(x, clr, box_height(y));
  math_link(nucleus(q)) = hpack(y, NATURAL);
  math_type(nucleus(q)) = SUB_BOX;
}

  void
make_math_accent (q)
  ptr	q;
{
  qcell	*a;
  int	c;
  fnt	f;
  scal	h;
  qcell	i;
  ptr	p;
  scal	s;
  scal	w;
  ptr	x;
  ptr	y;
  scal	delta;

  fetch(accent_chr(q)); 
  if (!char_exists(cur_i))
    return;
  i = cur_i;
  c = cur_c;
  f = cur_f;
  s = 0;
  if (math_type(nucleus(q)) == MATH_CHAR) {
    fetch(nucleus(q));
    if (char_tag(cur_i) == LIG_TAG) {
      a = lig_kern_start(cur_f, cur_i);
      cur_i = *a;
      if (skip_byte(cur_i) > STOP_FLAG) {
        a = lig_kern_restart(cur_f, cur_i);
        cur_i = *a;
      }
      loop {
        if (next_char(cur_i) == skew_char(cur_f)) {
          if (op_byte(cur_i) >= KERN_FLAG
              && skip_byte(cur_i) <= STOP_FLAG) {
            s = char_kern(cur_f, cur_i);
          }
          break;
        }
        if (skip_byte(cur_i) >= STOP_FLAG)
          break;
        a += skip_byte(cur_i) + 1;
        cur_i = *a;
      }
    }
  }
  x = clean_box(nucleus(q), cramped_style(cur_style));
  w = box_width(x);
  h = box_height(x);
  loop {
    if (char_tag(i) != LIST_TAG)
      break;
    y = rem_byte(i);
    i = char_info(f, y);
    if (char_width(f, i) > w)
      break;
    c = y;
  }
  delta = (h < x_height(f) ? h : x_height(f));
  if ((math_type(supscr(q)) != EMPTY
        || math_type(subscr(q)) != EMPTY)
      && math_type(nucleus(q)) == MATH_CHAR) {
    flush_node_list(x);
    x = new_noad(); 
    mcopy(nucleus(x), nucleus(q));
    mcopy(supscr(x), supscr(q));
    mcopy(subscr(x), subscr(q));
    mzero(supscr(q));
    mzero(subscr(q));
    math_type(nucleus(q)) = SUB_MLIST;
    math_link(nucleus(q)) = x;
    x = clean_box(nucleus(q), cur_style);
    delta += box_height(x) - h;
    h = box_height(x);
  }
  y = char_box(f, c);
  shift_amount(y) = s + half(w - box_width(y));
  box_width(y) = 0;
  link(y) = p = new_kern(-delta);
  link(p) = x;
  y = vpack(y, NATURAL);
  box_width(y) = box_width(x);
  if (box_height(y) < h) {
    p = new_kern(h - box_height(y));
    link(p) = list_ptr(y);
    list_ptr(y) = p;
    box_height(y) = h;
  }
  math_link(nucleus(q)) = y;
  math_type(nucleus(q)) = SUB_BOX;
}

  void
make_fraction (q)
  ptr	q;
{
  ptr	p;
  ptr	v;
  ptr	x;
  ptr	y;
  ptr	z;
  scal	clr;
  scal	delta;
  scal	delta1;
  scal	delta2;
  scal	shift_up;
  scal	shift_down;

  if (thickness(q) == DEFAULT_CODE)
    thickness(q) = default_rule_thickness;
  x = clean_box(numerator(q), num_style(cur_style));
  z = clean_box(denominator(q), denom_style(cur_style));
  if (box_width(x) < box_width(z))
    x = rebox(x, box_width(z));
  else z = rebox(z, box_width(x));
  if (cur_style < TEXT_STYLE) {
    shift_up = num1(cur_size);
    shift_down = denom1(cur_size);
  } else {
    shift_down = denom2(cur_size);
    if (thickness(q) != 0)
      shift_up = num2(cur_size);
    else shift_up = num3(cur_size);
  }
  if (thickness(q) == 0) {
    if (cur_style < TEXT_STYLE)
      clr = 7 * default_rule_thickness;
    else clr = 3 * default_rule_thickness;
    delta = half(clr - 
        ((shift_up - box_depth(x)) -
         (box_height(z) - shift_down)));
    if(delta > 0) {
      shift_up += delta;
      shift_down += delta;
    }
  } else {
    if (cur_style < TEXT_STYLE)
      clr = 3 * thickness(q);
    else clr = thickness(q);
    delta1 = delta2 = clr;
    delta = half(thickness(q));
    delta1 -= (shift_up - box_depth(x)) -
      (axis_height(cur_size)+delta);
    delta2 -= (axis_height(cur_size) - delta) -
      (box_height(z) - shift_down);
    if (delta1 > 0)
      shift_up += delta1;
    if (delta2 > 0)
      shift_down += delta2;
  }
  v = new_null_box();
  type(v) = VLIST_NODE;
  box_height(v) = shift_up + box_height(x);
  box_depth(v) = box_depth(z) + shift_down;
  box_width(v) = box_width(x);
  if (thickness(q) == 0) {
    p = new_kern((shift_up - box_depth(x)) -
        (box_height(z) - shift_down));
    link(p) = z;
  } else {
    y = fraction_rule(thickness(q));
    p = new_kern((axis_height(cur_size) - delta) -
        (box_height(z) - shift_down));
    link(y) = p;
    link(p) = z;
    p = new_kern((shift_up - box_depth(x)) -
        (axis_height(cur_size)+delta));
    link(p) = y;
  }
  link(x) = p;
  list_ptr(v) = x;
  if (cur_style < TEXT_STYLE)
    delta = delim1(cur_size);
  else delta = delim2(cur_size);
  x = var_delimiter(left_delimiter(q), cur_size, delta);
  link(x) = v;
  z = var_delimiter(right_delimiter(q), cur_size, delta);
  link(v) = z;
  new_hlist(q) = hpack(x, NATURAL);
}

  scal
make_op (q)
  ptr	q;
{
  ptr	p;
  ptr	v;
  ptr	x;
  ptr	y;
  ptr	z;
  scal	delta;
  scal	shift_up;
  scal	shift_down;

  if (subtype(q) == NORMAL && cur_style < TEXT_STYLE)
    subtype(q) = LIMITS;
  if (math_type(nucleus(q)) == MATH_CHAR) {
    fetch(nucleus(q));
    if (cur_style < TEXT_STYLE && char_tag(cur_i) == LIST_TAG) {
      cur_c = rem_byte(cur_i);
      character(nucleus(q)) = cur_c;
      cur_i = char_info(cur_f, cur_c);
    }
    delta = char_italic(cur_f, cur_i);
    x = clean_box(nucleus(q), cur_style);
    if (math_type(subscr(q)) != EMPTY && subtype(q) != LIMITS)
      box_width(x) -= delta;
    shift_amount(x) = half(box_height(x) - box_depth(x)) -
      axis_height(cur_size);
    math_type(nucleus(q)) = SUB_BOX;
    math_link(nucleus(q)) = x;
  } else {
    delta = 0;
  }
  if (subtype(q) == LIMITS) {
    x = clean_box(supscr(q), sup_style(cur_style));
    y = clean_box(nucleus(q), cur_style);
    z = clean_box(subscr(q), sub_style(cur_style));
    v = new_null_box();
    type(v) = VLIST_NODE;
    box_width(v) = box_width(y);
    if (box_width(x) > box_width(v))
      box_width(v) = box_width(x);
    if (box_width(z) > box_width(v))
      box_width(v) = box_width(z);
    x = rebox(x, box_width(v));
    y = rebox(y, box_width(v));
    z = rebox(z, box_width(v));
    shift_amount(x) = half(delta);
    shift_amount(z) = -shift_amount(x);
    box_height(v) = box_height(y);
    box_depth(v) = box_depth(y);
    if (math_type(supscr(q)) == EMPTY) {
      free_node(x, BOX_NODE_SIZE);
      list_ptr(v) = y;
    } else {
      shift_up = big_op_spacing3 - box_depth(x);
      if (shift_up < big_op_spacing1)
        shift_up = big_op_spacing1;
      p = new_kern(shift_up);
      link(x) = p; link(p) = y; 
      p = new_kern(big_op_spacing5);
      list_ptr(v) = p;
      link(p) = x;
      box_height(v) += big_op_spacing5 +
        box_height(x) + box_depth(x) + shift_up;
    }
    if (math_type(subscr(q)) == EMPTY) {
      free_node(z, BOX_NODE_SIZE);
    } else {
      shift_down = big_op_spacing4 - box_height(z);
      if (shift_down < big_op_spacing2)
        shift_down = big_op_spacing2;
      p = new_kern(shift_down);
      link(y) = p;
      link(p) = z;
      p = new_kern(big_op_spacing5);
      link(z) = p;
      box_depth(v) += big_op_spacing5 +
        box_height(z) + box_depth(z) + shift_down;
    }
    new_hlist(q) = v;
  }
  return delta;
}

#define ord_lig()							\
{	switch (op_byte(cur_i))						\
  {								\
    case 1: case 5:							\
                                character(nucleus(q)) = rem_byte(cur_i);		\
    break;							\
    case 2: case 6:							\
                                character(nucleus(p)) = rem_byte(cur_i);		\
    break;							\
    case 3: case 7: case 11:					\
                                      r = new_noad();						\
    character(nucleus(r)) = rem_byte(cur_i);		\
    fam(nucleus(r)) = fam(nucleus(q));			\
    link(q) = r;						\
    link(r) = p;						\
    if (op_byte(cur_i) < 11) {				\
      math_type(nucleus(r)) = MATH_CHAR;		\
    } else {						\
      math_type(nucleus(r)) = MATH_TEXT_CHAR;		\
    }							\
    break;							\
    default:							\
                          link(q) = link(p);					\
    character(nucleus(q)) = rem_byte(cur_i);		\
    mcopy(subscr(q), subscr(p));				\
    mcopy(supscr(q), supscr(p));				\
    free_node(p, NOAD_SIZE);				\
    break;							\
  }								\
  if (op_byte(cur_i) > 3)						\
  return;							\
  math_type(nucleus(q)) = MATH_CHAR;				\
  goto restart;							\
}

  void
make_ord (q)
  ptr	q;
{
  ptr	p, r;
  qcell	*a;

restart:
  if (math_type(subscr(q)) != EMPTY
      || math_type(supscr(q)) != EMPTY
      || math_type(nucleus(q)) != MATH_CHAR) {
    return;
  }
  p = link(q);
  if (p == null
      || type(p) < ORD_NOAD
      || type(p) > PUNCT_NOAD
      || math_type(nucleus(p)) != MATH_CHAR
      || fam(nucleus(p)) != fam(nucleus(q))) {
    return;
  }
  math_type(nucleus(q)) = MATH_TEXT_CHAR;
  fetch(nucleus(q));
  if (char_tag(cur_i) == LIG_TAG) {
    a = lig_kern_start(cur_f, cur_i);
    cur_c = character(nucleus(p));
    cur_i = *a;
    if (skip_byte(cur_i) > STOP_FLAG) {
      a = lig_kern_restart(cur_f, cur_i);
      cur_i = *a;
    }
    loop {
      if (next_char(cur_i) == cur_c
          && skip_byte(cur_i) <= STOP_FLAG) {
        if (op_byte(cur_i) >= KERN_FLAG) {
          p = new_kern(char_kern(cur_f, cur_i));
          link(p) = link(q);
          link(q) = p;
          return;
        } else {
          check_interrupt();
          ord_lig();
        }
      }
      if (skip_byte(cur_i) >= STOP_FLAG)
        return;
      a += skip_byte(cur_i) + 1;
      cur_i = *a;
    }
  }
}

  void
make_scripts (q, delta)
  ptr	q;
  scal	delta;
{
  ptr	p;
  int	t;
  ptr	x;
  ptr	y;
  ptr	z;
  scal	clr;
  scal	shift_up;
  scal	shift_down;

  p = new_hlist(q);
  if (is_char_node(p)) {
    shift_up = 0;
    shift_down = 0;
  } else {
    z = hpack(p, NATURAL);
    if (cur_style < SCRIPT_STYLE)	
      t = SCRIPT_SIZE;
    else t = SCRIPT_SCRIPT_SIZE;
    shift_up = box_height(z) - sup_drop(t);
    shift_down = box_depth(z) + sub_drop(t);
    free_node(z, BOX_NODE_SIZE);
  }
  if (math_type(supscr(q)) == EMPTY) {
    x = clean_box(subscr(q), sub_style(cur_style));
    box_width(x) += script_space;
    if (shift_down < sub1(cur_size))
      shift_down = sub1(cur_size);
    clr = box_height(x) - (abs(math_x_height(cur_size) * 4) / 5);
    if (shift_down < clr)
      shift_down = clr;
    shift_amount(x) = shift_down;
  } else {
    x = clean_box(supscr(q), sup_style(cur_style));
    box_width(x) += script_space;
    if (odd(cur_style))
      clr = sup3(cur_size);
    else if (cur_style < TEXT_STYLE)
      clr = sup1(cur_size);
    else clr = sup2(cur_size);
    if (shift_up < clr)
      shift_up = clr;
    clr = box_depth(x) + (abs(math_x_height(cur_size)) / 4);
    if (shift_up < clr)
      shift_up = clr;
    if (math_type(subscr(q)) == EMPTY) {
      shift_amount(x) = -shift_up;
    } else {
      y = clean_box(subscr(q), sub_style(cur_style));
      box_width(y) += script_space;
      if (shift_down < sub2(cur_size))
        shift_down = sub2(cur_size);
      clr = 4 * default_rule_thickness -
        ((shift_up - box_depth(x)) - 
         (box_height(y) - shift_down));
      if (clr > 0) {
        shift_down += clr;
        clr = (abs(math_x_height(cur_size) * 4) / 5) -
          (shift_up - box_depth(x));
        if (clr > 0) {
          shift_up += clr;
          shift_down -= clr;
        }
      }
      shift_amount(x) = delta;
      p = new_kern((shift_up - box_depth(x)) -
          (box_height(y) - shift_down));
      link(x) = p; link(p) = y;
      x = vpack(x, NATURAL);
      shift_amount(x) = shift_down;
    }
  }
  if (new_hlist(q) == null) {
    new_hlist(q) = x;
  } else {
    p = new_hlist(q);
    while (link(p) != null)
      p = link(p);
    link(p) = x;
  }
}

  int
make_left_right (q, style, max_d, max_h)
  ptr	q;
  int	style;
  scal	max_d;
  scal	max_h;
{
  scal	delta;
  scal	delta1;
  scal	delta2;

  if (style < SCRIPT_STYLE)
    cur_size = TEXT_SIZE;
  else cur_size = 16 * ((style - TEXT_STYLE) / 2);
  delta2 = max_d + axis_height(cur_size);
  delta1 = max_h + max_d - delta2;
  if (delta2 > delta1)
    delta1 = delta2;
  delta = (delta1 / 500) * delimiter_factor;
  delta2 = delta1 + delta1 - delimiter_shortfall;
  if (delta < delta2)
    delta = delta2;
  new_hlist(q) = var_delimiter(delimiter(q), cur_size, delta);
  return (type(q) - (LEFT_NOAD - OPEN_NOAD));
}

  void
fetch (a)
  ptr	a;
{
  cur_c = character(a);
  cur_f = fam_fnt(fam(a) + cur_size);
  if (cur_f == null_font) {
    print_err(null_str);
    print_size(cur_size);
    print(" ");
    print_int(fam(a));
    print(" is undefined (character ");
    print_ASCII(cur_c);
    print(")");
    help_undefd_mathchar();
    error();
    cur_i = null_character;
    math_type(a) = EMPTY;
  } else {
    if (cur_c >= font_bc(cur_f)
        && cur_c <= font_ec(cur_f)) {
      cur_i = char_info(cur_f, cur_c);
    } else {
      cur_i = null_character;
    }
    if (!char_exists(cur_i)) {
      char_warning(cur_f, cur_c);
      math_type(a) = EMPTY;
    }
  }
}

  ptr
clean_box (p, style)
  ptr	p;
  int	style;
{
  ptr	m;
  ptr	q;
  ptr	r;
  ptr	x;
  int	save_style;

  switch (math_type(p))
  {
    case MATH_CHAR:
      m = new_noad();
      mcopy(nucleus(m), p);
      break;

    case SUB_BOX:
      q = math_link(p);
      goto found; 

    case SUB_MLIST:
      m = math_link(p);
      break;

    default:
      q = new_null_box();
      goto found;
  }

  save_style = cur_style;
  q = mlist_to_hlist(m, style, FALSE);
  cur_style = save_style;
  change_size_and_mu();

found:
  if (is_char_node(q) || q == null) {
    x = hpack(q, NATURAL);	
  } else if (link(q) == null
      && type(q) <= VLIST_NODE
      && shift_amount(q) == 0) {
    x = q;
  } else {
    x = hpack(q, NATURAL);
  }
  q = list_ptr(x);
  if (is_char_node(q)) {
    r = link(q);
    if (r != null
        && link(r) == null
        && type(r) == KERN_NODE) {
      free_node(r, SMALL_NODE_SIZE);
      link(q) = null;
    }
  }
  return x;
}

  void
_mlst_hlst_init ()
{
}

  void
_mlst_hlst_init_once ()
{
}

/*
 **	Help text
 */

help_undefd_mathchar ()
{
  help4("Somewhere in the math formula just ended, you used the",
      "stated character from an undefined font family. For example,",
      "plain TeX doesn't allow \\it or \\sl in subscripts. Proceed,",
      "and I'll try to forget that I needed that character.");
}

/*
 * Common TeX:pack.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX pack.c 3.2@(#)";
#endif

#include "tex.h"

ptr	adjust_head;
ptr	adjust_tail;
scal	total_shrink[4];
scal	total_stretch[4];
int	last_badness;
int	pack_begin_line;

#define clr_dimens() \
{total_stretch[FILLL] = 0; \
  total_stretch[FILL] = 0; \
  total_stretch[FIL] = 0; \
  total_stretch[NORMAL] = 0; \
  total_shrink[FILLL] = 0; \
  total_shrink[FILL] = 0; \
  total_shrink[FIL] = 0; \
  total_shrink[NORMAL] = 0;}

  ptr
hpack (p, w, m)
  ptr	p;
  scal	w;
  int	m;
{
  ptr	q, g, r;
  scal	s, h, d, x;
  int	hd, o;
  fnt	f;
  qcell	i;

  last_badness = 0;
  r = new_node(BOX_NODE_SIZE);
  type(r) = HLIST_NODE;
  subtype(r) = MIN_QUARTERWORD;
  shift_amount(r) = 0;
  q = node_list(r);
  link(q) = p;
  x = h = d = 0;
  clr_dimens();

  while (p != null) {
reswitch:
    while (is_char_node(p)) {
      f = font(p);
      i = char_info(f, character(p));
      hd = height_depth(i);
      x += char_width(f, i);
      s = char_height(f, hd);
      if (s > h)
        h = s;
      s = char_depth(f, hd);
      if (s > d)
        d = s;
      p = link(p);
    }
    if (p != null) {
      switch (type(p))
      {
        case HLIST_NODE:
        case VLIST_NODE:
        case RULE_NODE:
        case UNSET_NODE:
          x += box_width(p);
          if (type(p) >= RULE_NODE)
            s = 0;
          else s = shift_amount(p);
          if (box_height(p) - s > h)
            h = box_height(p) - s;
          if (box_depth(p) + s > d)
            d = box_depth(p) + s;
          break;

        case INS_NODE:
        case MARK_NODE:
        case ADJUST_NODE:
          if (adjust_tail == null) {
            break;
          }
          while (link(q) != p)
            q = link(q);
          if (type(p) == ADJUST_NODE) {
            link(adjust_tail) = adjust_ptr(p);
            while (link(adjust_tail) != null)
              adjust_tail =
                link(adjust_tail);
            p = link(p);
            free_node(link(q), SMALL_NODE_SIZE);
          } else {
            adjust_tail = link(adjust_tail) = p;
            p = link(p);
          }
          link(q) = p;
          p = q;
          break;

        case WHATSIT_NODE:
          break;

        case GLUE_NODE:
          g = glue_ptr(p);
          x += glue_width(g);
          o = stretch_order(g);
          total_stretch[o] += stretch(g);
          o = shrink_order(g);
          total_shrink[o] += shrink(g);
          if (subtype(p) >= A_LEADERS) {
            g = leader_ptr(p);
            if (box_height(g) > h)
              h = box_height(g);
            if (box_depth(g) > d)
              d = box_depth(g);
          }
          break;

        case KERN_NODE:
        case MATH_NODE:
          x += kern_width(p);
          break;

        case LIGATURE_NODE:
          p = make_char_from_lig(p);
          goto reswitch;

        default:
          break;
      }
      p = link(p);
    }
  }
  if (adjust_tail != null)
    link(adjust_tail) = null;
  box_height(r) = h;
  box_depth(r) = d;
  if (m == ADDITIONAL)
    w += x;
  box_width(r) = w;
  x = w - x;
  if (x == 0) {
    glue_sign(r) = NORMAL;
    glue_order(r) = NORMAL;
    glue_set(r) = 0.0;
    return r;
  } else if (x > 0) {
    o = get_stretch_order(); 
    glue_order(r) = o;
    glue_sign(r) = STRETCHING;
    if (total_stretch[o] != 0) {
      glue_set(r) = (float) x / (float) total_stretch[o];
    } else {
      glue_sign(r) = NORMAL;
      glue_set(r) = 0.0;
    }
    if (o == NORMAL
        && list_ptr(r) != null) {
      last_badness = badness(x, total_stretch[NORMAL]);
      if (last_badness > hbadness) {
        print_ln();
        if (last_badness > 100)
          print_nl("Underfull");
        else print_nl("Loose");
        print(" \\hbox (badness ");
        print_int(last_badness);
        goto common_end;
      }
    }
    return r;
  } else {
    o = get_shrink_order();
    glue_order(r) = o;
    glue_sign(r) = SHRINKING;
    if (total_shrink[o] != 0) {
      glue_set(r) = (float) -x / total_shrink[o];
    } else {
      glue_sign(r) = NORMAL;
      glue_set(r) = 0.0;
    }
    if (total_shrink[o] < -x
        && o == NORMAL && list_ptr(r) != null) {
      last_badness = 1000000;
      glue_set(r) = 1.0;
      if (-x - total_shrink[NORMAL] > hfuzz
          || hbadness < 100) {
        if (overfull_rule > 0
            && -x - total_shrink[NORMAL] > hfuzz) {
          while (link(q) != null)
            q = link(q);
          link(q) = new_rule();
          rule_width(link(q)) = overfull_rule;
        }
        print_ln();
        print_nl("Overfull \\hbox ("); 
        print_scaled(-x - total_shrink[NORMAL]);
        print("pt too wide");
        goto common_end;
      }
    } else if (o == NORMAL && list_ptr(r) != null) {
      last_badness = badness(-x, total_shrink[NORMAL]);
      if (last_badness > hbadness) {
        print_ln();
        print_nl("Tight \\hbox (badness ");
        print_int(last_badness);
        goto common_end;
      }
    }
    return r;
  }

common_end:
  if (output_active) {
    print(") has occurred while \\output is active");
  } else {
    if (pack_begin_line != 0) {
      if (pack_begin_line > 0)
        print(") in paragraph at lines ");
      else print(") in alignment at lines ");
      print_int(abs(pack_begin_line));
      print("--");
    } else {
      print(") detected at line ");
    }
    print_int(line);
  }
  print_ln();
  font_in_short_display = null_font;
  short_display(list_ptr(r));
  print_ln();
  begin_diagnostic();
  show_box(r);
  end_diagnostic(TRUE);
  return r;
}

  ptr
vpackage (p, h, m, l)
  ptr	p;
  scal	h;
  int	m;
  scal	l;
{
  scal	s, w, d, x;
  ptr	g, r;
  int	o;

  last_badness = 0;
  r = new_node(BOX_NODE_SIZE);
  type(r) = VLIST_NODE;
  subtype(r) = MIN_QUARTERWORD;
  shift_amount(r) = 0;
  list_ptr(r) = p;
  d = x = w = 0;
  clr_dimens();
  while (p != null) {
    if (is_char_node(p))
      confusion("vpack");
    switch (type(p))
    {
      case HLIST_NODE:
      case VLIST_NODE:
      case RULE_NODE:
      case UNSET_NODE:
        x += d + box_height(p);
        d = box_depth(p);
        if (type(p) >= RULE_NODE)
          s = 0;
        else s = shift_amount(p);
        if (box_width(p) + s > w)
          w = box_width(p) + s;
        break;

      case WHATSIT_NODE:
        break;

      case GLUE_NODE:
        x += d;
        d = 0;
        g = glue_ptr(p);
        x += glue_width(g);
        o = stretch_order(g);
        total_stretch[o] += stretch(g);
        o = shrink_order(g);
        total_shrink[o] += shrink(g);
        if (subtype(p) >= A_LEADERS) {
          g = leader_ptr(p);
          if (box_width(g) > w)
            w = box_width(g);
        }
        break;

      case KERN_NODE:
        x += d + kern_width(p);
        d = 0;
        break;

      default:
        break;
    }
    p = link(p);
  }
  box_width(r) = w;
  if (d > l) {
    x += d - l;
    box_depth(r) = l;
  } else {
    box_depth(r) = d;
  }
  if (m == ADDITIONAL)
    h += x;
  box_height(r) = h;
  x = h - x;
  if (x == 0) {
    glue_sign(r) = NORMAL;
    glue_order(r) = NORMAL;
    glue_set(r) = 0.0;
    return r;
  } else if (x > 0) {
    o = get_stretch_order();
    glue_order(r) = o;
    glue_sign(r) = STRETCHING;
    if (total_stretch[o] != 0) {
      glue_set(r) = (float) x / total_stretch[o];
    } else {
      glue_sign(r) = NORMAL;
      glue_set(r) = 0.0;
    }
    if (o == NORMAL && list_ptr(r) != NULL) {
      last_badness = badness(x, total_stretch[NORMAL]);
      if (last_badness > vbadness) {
        print_ln();
        if (last_badness > 100) {
          print_nl("Underfull");
        } else {
          print_nl("Loose");
        }
        print(" \\vbox (badness ");
        print_int(last_badness);
        goto common_end;
      }
    }
    return r;
  } else {
    o = get_shrink_order();
    glue_order(r) = o;
    glue_sign(r) = SHRINKING;
    if (total_shrink[o] != 0) {
      glue_set(r) = (float) -x / total_shrink[o];
    } else {
      glue_sign(r) = NORMAL;
      glue_set(r) = 0.0;
    }
    if (total_shrink[o] < -x
        && o == NORMAL && list_ptr(r) != NULL) {
      last_badness = 1000000;
      glue_set(r) = 1.0;
      if (-x - total_shrink[NORMAL] > vfuzz
          || vbadness < 100) {
        print_ln();
        print_nl("Overfull \\vbox (");
        print_scaled(-x - total_shrink[NORMAL]);
        print("pt too high");
        goto common_end;
      }
    } else if (o == NORMAL && list_ptr(r) != null) {
      last_badness = badness(-x, total_shrink[NORMAL]);
      if (last_badness > vbadness) {
        print_ln();
        print_nl("Tight \\vbox (badness ");
        print_int(last_badness);
        goto common_end;
      }
    }
    return r;
  }

common_end:
  if (output_active) {
    print(") has occurred while \\output is active");
  } else {
    if (pack_begin_line != 0) {
      print(") in alignment at lines ");
      print_int(abs(pack_begin_line));
      print("--");
    } else {
      print(") detected at line ");
    }
    print_int(line);
    print_ln();
  }
  begin_diagnostic();
  show_box(r);
  end_diagnostic(TRUE);
  return r;
}

  void
_pack_init ()
{
  pack_begin_line = 0;
  last_badness = 0;
  adjust_tail = null;
}

  void
_pack_init_once ()
{
  adjust_head = new_avail();
}

/*
 * Common TeX:page.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX page.c 3.2@(#)";
#endif

#include "tex.h"

ptr	contrib_head;
ptr	page_ins_head;
ptr	page_head;
ptr	page_tail;
int	page_contents;
scal	page_so_far[8];
scal	page_max_depth;
ptr	last_glue;
scal	last_kern;
int	last_penalty;
bool	output_active;
int	insert_penalties;
int	least_page_cost;
ptr	best_page_break;
scal	best_size;
scal	best_height_plus_depth;

#define set_page_so_far_zero(P)	(page_so_far[P] = 0)
#define set_height_zero(H)	(active_height[H] = 0)

  void
build_page ()
{
  int	pi, b, c;
  ptr	p, q, r;

#define INF_SHRINK_PAGE "Infinite glue shrinkage found on current page"

  if (link(contrib_head) == null || output_active)
    return;
  do {
    p = link(contrib_head);
    if (last_glue != null)
      delete_glue_ref(last_glue);
    last_penalty = 0;
    last_kern = 0;
    if (type(p) == GLUE_NODE) {
      last_glue = glue_ptr(p);
      add_glue_ref(last_glue);
    } else {
      last_glue = null;
      if (type(p) == PENALTY_NODE) {
        last_penalty = penalty(p);
      } else if (type(p) == KERN_NODE) {
        last_kern = kern_width(p);
      }
    }
    switch (type(p))
    {
      case HLIST_NODE:
      case VLIST_NODE:
      case RULE_NODE:
        if (page_contents < BOX_THERE) {
          if (page_contents == EMPTY) {
            freeze_page_specs(BOX_THERE);
          } else {
            page_contents = BOX_THERE;
          }
          q = new_skip_param(TOP_SKIP_CODE);
          link(q) = p;
          link(contrib_head) = q;
          r = glue_ptr(q);
          if (glue_width(r) > box_height(p)) {
            glue_width(r) -= box_height(p);
          } else {
            glue_width(r) = 0;
          }
          continue;
        } else {
          page_total += page_depth + box_height(p);
          page_depth = box_depth(p);
          goto contribute;
        }

      case GLUE_NODE:
        if (page_contents < BOX_THERE) {
          goto done;
        } else if (precedes_break(page_tail)) {
          pi = 0;
        } else {
          goto update_heights;
        }
        break;

      case KERN_NODE:
        if (page_contents < BOX_THERE) {
          goto done;
        } else if (link(p) == null) {
          return;
        } else if (type(link(p)) == GLUE_NODE) {
          pi = 0;
        } else {
          goto update_heights;
        }
        break;

      case PENALTY_NODE:
        if (page_contents < BOX_THERE) {
          goto done;
        } else {
          pi = penalty(p);
        }
        break;

      case WHATSIT_NODE:
        goto contribute;

      case MARK_NODE:
        goto contribute;

      case INS_NODE:
        insert_page(p);
        goto contribute;

      default:
        confusion("page");
        break;
    }
    if (pi < INF_PENALTY) {
      b = page_badness();
      if (b < AWFUL_BAD) {
        if (pi <= EJECT_PENALTY) {
          c = pi;
        } else if (b < INF_BAD) {
          c = b + pi + insert_penalties;
        } else {
          c = DEPLORABLE;
        }
      } else {
        c = b;
      }
      if (insert_penalties >= 10000)
        c = AWFUL_BAD;
      if (tracing_pages > 0)
        show_page_stats(b, pi, c);
      if (c <= least_page_cost) {
        best_page_break = p;
        best_size = page_goal;
        least_page_cost = c;
        r = link(page_ins_head);
        while (r != page_ins_head) {
          best_ins_ptr(r) = last_ins_ptr(r);
          r = link(r);
        }
      }
      if (c == AWFUL_BAD || pi <= EJECT_PENALTY) {
        fire_up(p);
        if (output_active)
          return;
        continue;
      }
    }
    if (type(p) < GLUE_NODE || type(p) > KERN_NODE) {
      goto contribute;
    }

update_heights:
    if (type(p) == KERN_NODE) {
      page_total += page_depth + kern_width(p);
    } else {
      q = glue_ptr(p);
      page_so_far[2 + stretch_order(q)] += stretch(q);
      page_shrink += shrink(q);
      if (shrink_order(q) != NORMAL && shrink(q) != 0) {
        print_err(INF_SHRINK_PAGE);
        help_inf_shrink_page();
        error();
        r = new_spec(q);
        shrink_order(r) = NORMAL;
        delete_glue_ref(q);
        q = glue_ptr(p) = r;
      }
      page_total += page_depth + glue_width(q);
    }
    page_depth = 0;

contribute:
    if (page_depth > page_max_depth) {
      page_total = page_total + page_depth - page_max_depth;
      page_depth = page_max_depth;
    }
    page_tail = link(page_tail) = p;
    link(contrib_head) = link(p);
    link(p) = null;
    continue;

done:
    link(contrib_head) = link(p);
    link(p) = null;
    flush_node_list(p);
  } while (link(contrib_head) != null);
  if (nest_ptr == nest) {
    tail = contrib_head;
  } else {
    contrib_tail = contrib_head;
  }
}

  void
insert_page (p)
  ptr	p;
{
  int	n;
  ptr	q, r;
  scal	h, w;
  scal	delta;

  if (page_contents == EMPTY)
    freeze_page_specs(INSERTS_ONLY);
  n = subtype(p);
  r = page_ins_head;
  while (n >= subtype(link(r)))
    r = link(r);
  if (subtype(r) != n) {
    q = new_node(PAGE_INS_NODE_SIZE);
    link(q) = link(r);
    r = link(r) = q;
    subtype(r) = n;
    type(r) = INSERTING;
    ensure_vbox(n);
    page_ins_height(r) = (box(n) == null) ? 0 :
      box_height(box(n)) + box_depth(box(n));
    best_ins_ptr(r) = null;
    q = skip(n);
    if (count(n) == 1000)
      h = page_ins_height(r);
    else h = x_over_n(page_ins_height(r), 1000) * count(n);
    page_goal -= h + glue_width(q);
    page_so_far[2 + stretch_order(q)] += stretch(q);
    page_shrink += shrink(q);
    if (shrink_order(q) != NORMAL && shrink(q) != 0) {
      print_err("Infinite glue shrinkage inserted from ");
      print_esc("skip");
      print_int(n);
      help_inf_shrink_ins();
      error();
    }
  }
  if (type(r) == SPLIT_UP) {
    insert_penalties += float_cost(p);
    return;
  }
  last_ins_ptr(r) = p;
  delta = page_goal - page_total - page_depth + page_shrink;
  if (count(n) == 1000) {
    h = ins_height(p);
  } else {
    h = x_over_n(ins_height(p), 1000) * count(n);
  }
  if ((h <= 0 || h <= delta)
      && ins_height(p) + page_ins_height(r) <= dimen(n)) {
    page_goal -= h;
    page_ins_height(r) += ins_height(p);
  } else {
    if (count(n) <= 0) {
      w = MAX_DIMEN;
    } else {
      w = page_goal - page_total - page_depth;
      if (count(n) != 1000)  {
        w = x_over_n(w, count(n)) * 1000;
      }
    }
    if (w > dimen(n) - page_ins_height(r)) {
      w = dimen(n) - page_ins_height(r);
    }
    q = vert_break(ins_ptr(p), w, ins_depth(p));
    page_ins_height(r) += best_height_plus_depth;
    if (tracing_pages > 0)
      show_split(n, w, q);
    if (count(n) != 1000) {
      best_height_plus_depth =
        x_over_n(best_height_plus_depth, 1000) *
        count(n);
    }
    page_goal -= best_height_plus_depth;
    type(r) = SPLIT_UP;
    broken_ptr(r) = q;
    broken_ins(r) = p;
    if (q == null) {
      insert_penalties += EJECT_PENALTY;
    } else if (type(q) == PENALTY_NODE) {
      insert_penalties += penalty(q);
    }
  }
}

  ptr
vert_break (p, h, d)
  ptr	p;
  scal	h;
  scal	d;
{
  int	b;
  ptr	q;
  ptr	r;
  int	t;
  int	pi;
  ptr	prev_p;
  scal	prev_dp;
  ptr	best_place;
  int	least_cost;

#define	INF_SHRINK_BOX "Infinite glue shrinkage found in box being split"

  prev_p = p;
  least_cost = AWFUL_BAD;
  do_all_six(set_height_zero);
  prev_dp = 0;
  loop {
    if (p == null) {
      pi = EJECT_PENALTY;
    } else {
      switch (type(p))
      {
        case HLIST_NODE:
        case VLIST_NODE:
        case RULE_NODE:
          cur_height += prev_dp + box_height(p);
          prev_dp = box_depth(p);
          goto not_found;

        case WHATSIT_NODE:
          goto not_found;

        case GLUE_NODE:
          if (precedes_break(prev_p)) {
            pi = 0;
          } else {
            goto update_heights;
          }
          break;

        case KERN_NODE:
          if (link(p) == null) {
            t = PENALTY_NODE;
          } else {
            t = type(link(p));
          }
          if (t == GLUE_NODE) {
            pi = 0;
          } else {
            goto update_heights;
          }
          break;

        case PENALTY_NODE:
          pi = penalty(p);
          break;

        case MARK_NODE:
        case INS_NODE:
          goto not_found;

        default:
          confusion("vertbreak");
      }
    }
    if (pi < INF_PENALTY) {
      b = vert_badness(h);
      if (b < AWFUL_BAD) {
        if (pi <= EJECT_PENALTY) {
          b = pi;
        } else if (b < INF_BAD) {
          b += pi;
        } else {
          b = DEPLORABLE;
        }
      }
      if (b <= least_cost) {
        best_place = p;
        least_cost = b;
        best_height_plus_depth = cur_height + prev_dp;
      }
      if (b == AWFUL_BAD || pi <= EJECT_PENALTY) {
        return best_place;
      }
    }
    if (type(p) < GLUE_NODE || type(p) > KERN_NODE) {
      goto not_found;
    }

update_heights:
    if (type(p) == KERN_NODE) {
      cur_height += prev_dp + kern_width(p);
    } else {
      q = glue_ptr(p);
      active_height[2 + stretch_order(q)] += stretch(q);
      active_height[6] += shrink(q);
      if (shrink_order(q) != NORMAL && shrink(q) != 0) {
        print_err(INF_SHRINK_BOX);
        help_inf_shrink_box();
        error();
        r = new_spec(q);
        delete_glue_ref(q);
        shrink_order(r) = NORMAL;
        glue_ptr(p) = r;
      }
      cur_height += prev_dp + glue_width(q);
    }
    prev_dp = 0;

not_found:
    if (prev_dp > d) {
      cur_height = cur_height + prev_dp - d;
      prev_dp = d;
    }
    prev_p = p;
    p = link(prev_p);
  }
}

  ptr
vsplit (n, h)
  int	n;
  scal	h;
{
  ptr	p;
  ptr	q;
  ptr	v;

  v = box(n);
  if (split_first_mark != null) {
    delete_token_ref(split_first_mark);
    split_first_mark = null;
    delete_token_ref(split_bot_mark);
    split_bot_mark = null;
  }
  if (v == null)
    return null;
  if (type(v) != VLIST_NODE) {
    print_err(null_str);
    print_esc("vsplit");
    print(" needs a ");
    print_esc("vbox");
    help_vsplit_vbox();
    error();
    return null;
  }
  q = vert_break(list_ptr(v), h, split_max_depth);
  p = list_ptr(v);
  if (p == q) {
    list_ptr(v) = null;
  } else {
    loop {
      if (type(p) == MARK_NODE) {
        if (split_first_mark == null) {
          split_first_mark = mark_ptr(p);
          split_bot_mark = split_first_mark;
          token_ref_count(split_first_mark) += 2;
        } else {
          delete_token_ref(split_bot_mark);
          split_bot_mark = mark_ptr(p);
          add_token_ref(split_bot_mark);
        }
      }
      if (link(p) == q) {
        link(p) = null;
        break;
      }
      p = link(p);
    }
  }
  q = prune_page_top(q);
  p = list_ptr(v);
  free_node(v, BOX_NODE_SIZE);
  if (q == null) {
    box(n) = null;
  } else {
    box(n) = vpack(q, NATURAL);
  }
  return (vpackage(p, h, EXACTLY, split_max_depth));
}

  ptr
prune_page_top (p)
  ptr	p;
{
  mcell	m;
  ptr	prev_p;
  ptr	q, s, t;

  t = (ptr)&m;
  prev_p = t;
  link(t) = p;
  while (p != null) {
    switch (type(p))
    {
      case HLIST_NODE:
      case VLIST_NODE:
      case RULE_NODE:
        q = new_skip_param(SPLIT_TOP_SKIP_CODE);
        link(prev_p) = q;
        link(q) = p;
        s = glue_ptr(q);
        if (glue_width(s) > box_height(p)) {
          glue_width(s) -= box_height(p);
        } else {
          glue_width(s) = 0;
        }
        p = null;
        break;

      case WHATSIT_NODE:
      case MARK_NODE:
      case INS_NODE:
        prev_p = p;
        p = link(p);
        break;

      case GLUE_NODE:
      case KERN_NODE:
      case PENALTY_NODE:
        q = p;
        p = link(p);
        link(q) = null;
        link(prev_p) = p;
        flush_node_list(q);
        break;

      default:
        confusion("pruning");
        break;
    }
  }
  return (link(t));
}

  int
page_badness ()
{
  int	b;

  if (page_total < page_goal) {
    if (page_so_far[3] != 0
        || page_so_far[4] != 0
        || page_so_far[5] != 0) {
      b = 0;
    } else {
      b = badness(page_goal - page_total, page_so_far[2]);
    }
  } else if (page_total - page_goal > page_shrink) {
    b = AWFUL_BAD;
  } else {
    b = badness(page_total - page_goal, page_shrink);
  }
  return b;
}

  int
vert_badness (height)
  int	height;
{
  int	b;

  if (cur_height < height) {
    if (active_height[3] != 0
        || active_height[4] != 0
        || active_height[5] != 0) {
      b = 0;
    } else {
      b = badness(height - cur_height, active_height[2]);
    }
  } else if (cur_height - height > active_height[6]) {
    b = AWFUL_BAD;
  } else  {
    b = badness(cur_height - height, active_height[6]);
  }
  return b;
}

  void
freeze_page_specs (s)
  int	s;
{
  page_contents = s;
  page_goal = vsize;
  page_max_depth = max_depth;
  page_depth = 0;
  do_all_six(set_page_so_far_zero);
  least_page_cost = AWFUL_BAD;
  if (tracing_pages > 0) {
    begin_diagnostic();
    print_nl("%% goal height=");
    print_scaled(page_goal);
    print(", max depth=");
    print_scaled(page_max_depth);
    end_diagnostic(FALSE);
  }
}

  void
box_error (n)
  int	n;
{
  error();
  begin_diagnostic();
  print_nl("The following box has been deleted:");
  show_box(box(n));
  end_diagnostic(TRUE);
  flush_node_list(box(n));
  box(n) = null;
}

  void
ensure_vbox (n)
  int	n;
{
  ptr	p;

  p = box(n);
  if (p != null && type(p) == HLIST_NODE) {
    print_err("Insertions can only be added to a vbox");
    help_tut();
    box_error(n);
  }
}

  void
print_plus (s, o)
  int	s;
  str	o;
{
  if (page_so_far[s] != 0) {
    print(" plus ");
    print_scaled(page_so_far[s]);
    print(o);
  }
}

  void
print_totals ()
{
  print_scaled(page_total);
  print_plus(2, null_str);
  print_plus(3, "fil");
  print_plus(4, "fill");
  print_plus(5, "filll");
  if (page_shrink != 0) {
    print(" minus ");
    print_scaled(page_shrink);
  }
}

  void
show_split(n, w, q)
  int	n;
  scal	w;
  ptr	q;
{
  begin_diagnostic();
  print_nl("% split");
  print_int(n);
  print(" to ");
  print_scaled(w);
  print_char(',');
  print_scaled(best_height_plus_depth);
  print(" p=");
  if (q == null) {
    print_int(EJECT_PENALTY);
  } else if (type(q) == PENALTY_NODE) {
    print_int(penalty(q));
  } else {
    print("0");
  }
  end_diagnostic(FALSE);
}

  void
show_page_stats (b, pi, c)
  int	b;
  int	pi;
  int	c;
{
  begin_diagnostic();
  print_nl("%");
  print(" t=");
  print_totals();
  print(" g=");
  print_scaled(page_goal);
  print(" b=");
  if (b == AWFUL_BAD) {
    print_char('*');
  } else {
    print_int(b);
  }
  print(" p=");
  print_int(pi);
  print(" c=");
  if (c == AWFUL_BAD) {
    print("*");
  } else {
    print_int(c);
  }
  if (c <= least_page_cost) {
    print("#");
  }
  end_diagnostic(FALSE);
}

#define set_output_penalty(P) \
  reg_gdefine(int_reg[OUTPUT_PENALTY_CODE], INT_REG, P)


  void
fire_up (c)
  ptr	c;
{
  int	n;
  bool	wait;
  ptr	prev_p;
  scal	save_vfuzz;
  int	save_vbadness;
  ptr	save_split_top_skip;
  ptr	p, q, r;

  if (type(best_page_break) == PENALTY_NODE) {
    set_output_penalty(penalty(best_page_break));
    penalty(best_page_break) = INF_PENALTY;
  } else {
    set_output_penalty(INF_PENALTY);
  }
  if (bot_mark != null) {
    if (top_mark != null)
      delete_token_ref(top_mark);
    top_mark = bot_mark;
    add_token_ref(top_mark);
    delete_token_ref(first_mark);
    first_mark = null;
  }
  if (c == best_page_break) {
    best_page_break = null;
  }
  if (box(255) != null) {
    print_err(null_str);
    print_esc("box");
    print("255 is not void");
    help_box_255();
    box_error(255);
  }
  insert_penalties = 0;
  save_split_top_skip = split_top_skip;
  if (holding_inserts <= 0) {
    r = link(page_ins_head);
    while (r != page_ins_head) {
      if (best_ins_ptr(r) != null) {
        n = subtype(r);
        ensure_vbox(n);
        if (box(n) == null)
          box(n) = new_null_box();
        p = node_list(box(n));
        while (link(p) != null) {
          p = link(p);
        }
        last_ins_ptr(r) = p;
      }
      r = link(r);
    }
  }
  q = hold_head;
  link(q) = null;
  prev_p = page_head;
  p = link(prev_p);
  while (p != best_page_break) {
    if (type(p) == INS_NODE) {
      if (holding_inserts <= 0) {
        wait = insert_box(p);
        link(prev_p) = link(p);
        link(p) = null;
        if (wait) {
          q = link(q) = p;
          incr(insert_penalties);
        } else {
          delete_glue_ref(split_top_ptr(p));
          free_node(p, INS_NODE_SIZE);
        }
        p = prev_p;
      }
    } else if (type(p) == MARK_NODE) {
      if (first_mark == null) {
        first_mark = mark_ptr(p);
        add_token_ref(first_mark);
      }
      if (bot_mark != null)
        delete_token_ref(bot_mark);
      bot_mark = mark_ptr(p);
      add_token_ref(bot_mark);
    }
    prev_p = p;
    p = link(prev_p);
  }
  split_top_skip = save_split_top_skip;
  if (p != null) {
    if (link(contrib_head) == null) {
      if (nest_ptr == nest) {
        tail = page_tail;
      } else {
        contrib_tail = page_tail;
      }
    }
    link(page_tail) = link(contrib_head);
    link(contrib_head) = p;
    link(prev_p) = null;
  }
  save_vbadness = vbadness;
  save_vfuzz = vfuzz;
  vbadness = INF_BAD;
  vfuzz = MAX_DIMEN;
  box(255) = vpackage(link(page_head),
      best_size, EXACTLY, page_max_depth);
  vbadness = save_vbadness;
  vfuzz = save_vfuzz;
  if (last_glue != null)
    delete_glue_ref(last_glue);
  start_new_page();
  if (q != hold_head) {
    link(page_head) = link(hold_head);
    page_tail = q;
  }
  r = link(page_ins_head);
  while (r != page_ins_head) {
    q = link(r);
    free_node(r, PAGE_INS_NODE_SIZE);
    r = q;
  }
  link(page_ins_head) = page_ins_head;
  if (top_mark != null && first_mark == null) {
    first_mark = top_mark;
    add_token_ref(top_mark);
  }
  if (output_routine != null) {
    if (dead_cycles >= max_dead_cycles) {
      print_err("Output loop---");
      print_int(dead_cycles);
      print(" consecutive dead cycles");
      help_dead_cycles();
      error();
    } else {
      output_active = TRUE;
      incr(dead_cycles);
      push_nest();
      mode = -VMODE;
      prev_depth = IGNORE_DEPTH;
      mode_line = -line;
      begin_token_list(output_routine, OUTPUT_TEXT);
      new_save_level(OUTPUT_GROUP);
      normal_paragraph();
      scan_left_brace();
      return;
    }
  }
  if (link(page_head) != null) {
    if (link(contrib_head) == null) {
      if (nest_ptr == nest) {
        tail = page_tail;
      } else {
        contrib_tail = page_tail;
      }
    } else {
      link(page_tail) = link(contrib_head);
    }
    link(contrib_head) = link(page_head);
    link(page_head) = null;
    page_tail = page_head;
  }
  ship_out(box(255));
  box(255) = null;
}

  bool
insert_box (p)
  ptr	p;
{
  bool	wait;
  ptr	q, r, t;
  int	n;

  q = link(page_ins_head);
  while (subtype(q) != subtype(p))
    q = link(q);
  if (best_ins_ptr(q) == null)
    return TRUE;
  wait = FALSE;
  r = last_ins_ptr(q);
  link(r) = ins_ptr(p);
  if (best_ins_ptr(q) == p) {
    if (type(q) == SPLIT_UP
        && broken_ins(q) == p
        && broken_ptr(q) != null) {
      while (link(r) != broken_ptr(q))
        r = link(r);
      link(r) = null;
      split_top_skip = split_top_ptr(p);
      ins_ptr(p) = prune_page_top(broken_ptr(q));
      if (ins_ptr(p) != null) {
        t = vpack(ins_ptr(p), NATURAL);
        ins_height(p) = box_height(t) + box_depth(t);
        free_node(t, BOX_NODE_SIZE);
        wait = TRUE;
      }
    }
    best_ins_ptr(q) = null;
    n = subtype(q);
    t = list_ptr(box(n));
    free_node(box(n), BOX_NODE_SIZE);
    box(n) = vpack(t, NATURAL);
  } else {
    while (link(r) != null)
      r = link(r);
    last_ins_ptr(q) = r;
  }
  return wait;
}

  void
show_cur_page ()
{
  ptr	p, q;
  int	t;

  if (page_head == page_tail)
    return;
  print_nl("### current page:");
  if (output_active)
    print(" (held over for next output)");
  show_box(link(page_head));
  if (page_contents > EMPTY) {
    print_nl("total height ");
    print_totals();
    print_nl(" goal height ");
    print_scaled(page_goal);
    p = link(page_ins_head);
    while (p != page_ins_head) {
      print_ln();
      print_esc("insert");
      t = subtype(p);
      print_int(t);
      print(" adds ");
      t = x_over_n(page_ins_height(p), 1000) * count(t);
      print_scaled(t);
      if (type(p) == SPLIT_UP) {
        q = page_head;
        t = 0;
        do {
          q = link(q);
          if (type(q) == INS_NODE
              && subtype(q) == subtype(p))
            incr(t);
        } while (q != broken_ins(p));
        print(", #");
        print_int(t);
        print(" might split");
      }
      p = link(p);
    }
  }
}

  void
_page_init ()
{
  start_new_page();
  prev_depth = IGNORE_DEPTH;
  output_active = FALSE;
  insert_penalties = 0;
}

  void
_page_init_once ()
{
  contrib_head = new_avail();
  page_tail = page_head = new_glue(zero_glue);
  page_ins_head = new_node(PAGE_INS_NODE_SIZE);
  link(page_ins_head) = page_ins_head;
  type(page_ins_head) = SPLIT_UP;
  subtype(page_ins_head) = 255;
}

/*
 **	Help text
 */

help_tut ()
{
  help3("Tut tut: You're trying to \\insert into a",
      "\\box register that now contains an \\hbox.",
      "Proceed, and I'll discard its present contents.");
}

help_vsplit_vbox ()
{
  help2("The box you are trying to split is an \\hbox.",
      "I can't split such a box, so I'll leave it alone.");
}

help_inf_shrink_ins ()
{
  help3("The correction glue for page breaking with insertions",
      "must have finite shrinkability. But you may proceed,",
      "since the offensive shrinkability has been made finite.");
}

help_inf_shrink_box ()
{
  help4("The box you are \\vsplitting contains some infinitely",
      "shrinkable glue, e.g., `\\vss' or `\\vskip 0pt minus 1fil'.",
      "Such glue doesn't belong there; but you can safely proceed,",
      "since the offensive shrinkability has been made finite.");
}

help_inf_shrink_page ()
{
  help4("The page about to be output contains some infinitely",
      "shrinkable glue, e.g., `\\vss' or `\\vskip 0pt minus 1fil'.",
      "Such glue doesn't belong there; but you can safely proceed,",
      "since the offensive shrinkability has been made finite.");
}

help_box_255 ()
{
  help2("You shouldn't use \\box255 except in \\output routines.",
      "Proceed, and I'll discard its present contents.");
}

help_dead_cycles ()
{
  help3("I've concluded that your \\output is awry; it never does a",
      "\\shipout, so I'm shipping \\box255 out myself. Next time",
      "increase \\maxdeadcycles if you want me to be more patient!");
}

/*
 * Common TeX:par.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX par.c 3.2@(#)";
#endif

#include "tex.h"

ptr	cur_p;

ptr	active;
ptr	passive;

scal	background[7];
scal	break_width[7];
scal	active_width[7];
scal	cur_active_width[7];

int	threshold;
bool	second_pass;
bool	final_pass;
scal	first_indent;
scal	first_width;
scal	second_indent;
scal	second_width;

int	fewest_demerits;
int	minimum_demerits;
int	minimal_demerits[4];
int	best_pl_line[4];
ptr	best_place[4];
ptr	best_bet;
int	best_line;
int	fit_class;

int	easy_line;
int	last_special_line;
int	line_diff;
scal	line_width;
scal	disc_width;
int	pass_number;
ptr	printed_node;
int	actual_looseness;
bool	no_shrink_error_yet;

ptr	just_box;

#define act_width	active_width[1]

#define store_background(W) \
  (active_width[W] = background[W])

#define store_break_width(W) \
  (active_width[W] = break_width[W])

#define update_active(W) \
  (active_width[W] += deltas(r)[W])

#define copy_to_cur_active(W) \
  (cur_active_width[W] = active_width[W])

#define downdate_width(W) \
  (cur_active_width[W] -= deltas(prev_r)[W])

#define update_width(W) \
  (cur_active_width[W] += deltas(r)[W])

#define set_break_width_to_background(W) \
  (break_width[W] = background[W])

#define combine_two_deltas(W) \
  deltas(prev_r)[W] += deltas(r)[W]

#define convert_to_break_width(W) \
  deltas(prev_r)[W] = deltas(prev_r)[W] + break_width[W] \
- cur_active_width[W]

#define new_delta_to_break_width(W) \
  deltas(q)[W] = break_width[W] - cur_active_width[W]

#define new_delta_from_break_width(W) \
  deltas(q)[W] = cur_active_width[W] - break_width[W]

#define width_lig_char(C) \
  char_width(font(lig_char(C)), \
      char_info(font(lig_char(C)), character(lig_char(C))))

#define width_char(C) \
  char_width(font(C), char_info(font(C), character(C)))

#define kern_break() \

#define check_shrinkage(G) \
{if (shrink_order(G) != NORMAL && shrink(G) != 0) \
  G = finite_shrink(G);}

  void
line_break (final_widow_penalty)
  int	final_widow_penalty;
{
  ptr	q, r, s, t;
  ptr	prev_p;
  bool	auto_breaking;

  pack_begin_line = mode_line;
  t = new_avail();
  link(t) = link(head);
  if (is_char_node(tail)) {
    tail_append(new_penalty(INF_PENALTY));
  } else if (type(tail) != GLUE_NODE) {
    tail_append(new_penalty(INF_PENALTY));
  } else {
    type(tail) = PENALTY_NODE;
    delete_glue_ref(glue_ptr(tail));
    flush_node_list(leader_ptr(tail));
    penalty(tail) = INF_PENALTY;
  }
  link(tail) = new_param_glue(PAR_FILL_SKIP_CODE);
  pop_nest();
  no_shrink_error_yet = TRUE;
  check_shrinkage(left_skip);
  check_shrinkage(right_skip);
  q = left_skip;
  r = right_skip;
  background[1] = glue_width(q) + glue_width(r);
  background[2] = 0;
  background[3] = 0;
  background[4] = 0;
  background[5] = 0;
  background[2 + stretch_order(q)] = stretch(q);
  background[2 + stretch_order(r)] += stretch(r);
  background[6] = shrink(q) + shrink(r);
  minimum_demerits = AWFUL_BAD;
  minimal_demerits[VERY_LOOSE_FIT] = AWFUL_BAD;
  minimal_demerits[LOOSE_FIT] = AWFUL_BAD;
  minimal_demerits[DECENT_FIT] = AWFUL_BAD;
  minimal_demerits[TIGHT_FIT] = AWFUL_BAD;
  if (par_shape_ptr == null) {
    if (hang_indent == 0) {
      last_special_line = 0;
      second_width = hsize;
      second_indent = 0;
    } else {
      last_special_line = abs(hang_after);
      if (hang_after < 0) {
        first_width = hsize - abs(hang_indent);
        first_indent = hang_indent >= 0 ?
          hang_indent : 0;
        second_width = hsize;
        second_indent = 0;
      } else {
        first_width = hsize;
        first_indent = 0;
        second_width = hsize - abs(hang_indent);
        second_indent = hang_indent >= 0 ?
          hang_indent : 0;
      }
    }
  } else {
    last_special_line = info(par_shape_ptr) - 1;
    second_width = par_shape_width(last_special_line + 1);
    second_indent = par_shape_indent(last_special_line + 1);
  }
  easy_line = looseness == 0 ? last_special_line : MAX_HALFWORD;
  threshold = pretolerance;
  if (threshold >= 0) {
    if (tracing_paragraphs > 0) {
      begin_diagnostic();
      print_nl("@firstpass");
    }
    second_pass = FALSE;
    final_pass = FALSE;
  } else {
    threshold = tolerance;
    second_pass = TRUE;
    final_pass = emergency_stretch <= 0;
    if (tracing_paragraphs > 0) {
      begin_diagnostic();
    }
  }

next_pass:
  if (threshold >= INF_BAD) {
    threshold = INF_BAD;
  }
  if (second_pass) {
    init_hyph();
  }
  q = new_node(ACTIVE_NODE_SIZE);
  type(q) = UNHYPHENATED;
  fitness(q) = DECENT_FIT;
  link(q) = last_active;
  break_node(q) = null;
  line_number(q) = prev_graf + 1;
  total_demerits(q) = 0;
  link(active) = q;
  do_all_six(store_background);
  passive = null;
  printed_node = t;
  pass_number = 0;
  font_in_short_display = null_font;
  prev_p = cur_p = link(t);
  auto_breaking = TRUE;
  while (cur_p != null && link(active) != last_active) {
    if (is_char_node(cur_p)) {
      prev_p = cur_p;
      while (is_char_node(cur_p)) {
        act_width += width_char(cur_p);
        cur_p = link(cur_p);
      }
    }
    switch (type(cur_p))
    {
      case HLIST_NODE:
      case VLIST_NODE:
      case RULE_NODE:
        act_width += box_width(cur_p);
        break;

      case GLUE_NODE:
        if (auto_breaking) {
          if (is_char_node(prev_p)
              || precedes_break(prev_p)) {
            try_break(0, UNHYPHENATED);
          }
        }
        check_shrinkage(glue_ptr(cur_p));
        q = glue_ptr(cur_p);
        act_width += glue_width(q);
        active_width[2 + stretch_order(q)] += stretch(q);
        active_width[6] += shrink(q);
        if (second_pass && auto_breaking) {
          try_hyph();
        }
        break;

      case KERN_NODE:
        if (!is_char_node(link(cur_p))
            && auto_breaking
            && type(link(cur_p)) == GLUE_NODE) {
          try_break(0, UNHYPHENATED);
        }
        act_width += kern_width(cur_p);
        break;

      case LIGATURE_NODE:
        act_width += width_lig_char(cur_p);
        break;

      case DISC_NODE:
        disc_width = 0;
        if (pre_break(cur_p) == null) {
          try_break(ex_hyphen_penalty, HYPHENATED);
        } else {
          set_disc_width();
          act_width += disc_width;
          try_break(hyphen_penalty, HYPHENATED);
          act_width -= disc_width;
        }
        prev_p = cur_p;
        set_act_width();
        continue;

      case MATH_NODE:
        auto_breaking = subtype(cur_p) == AFTER;
        if (!is_char_node(link(cur_p))
            && auto_breaking
            && type(link(cur_p)) == GLUE_NODE) {
          try_break(0, UNHYPHENATED);
        }
        act_width += math_width(cur_p);
        break;

      case PENALTY_NODE:
        try_break(penalty(cur_p), UNHYPHENATED);
        break;

      case MARK_NODE:
      case INS_NODE:
      case ADJUST_NODE:
        break;

      case WHATSIT_NODE:
        line_break_whatsit(cur_p);
        break;

      default:
        confusion("paragraph");
        break;
    }
    prev_p = cur_p;
    cur_p = link(cur_p);
  }
  if (cur_p == null) {
    try_break(EJECT_PENALTY, HYPHENATED);
    if (link(active) != last_active) {
      if (get_best_bet())
        goto done;
    }
  }
  for (q = link(active); q != last_active; q = cur_p) {
    cur_p = link(q);
    if (type(q) == DELTA_NODE) {
      free_node(q, DELTA_NODE_SIZE);
    } else {
      free_node(q, ACTIVE_NODE_SIZE);
    }
  }
  for (q = passive; q != null; q = cur_p) {
    cur_p = link(q);
    free_node(q, PASSIVE_NODE_SIZE);
  }
  if (!second_pass) {
    if (tracing_paragraphs > 0) {
      print_nl("@secondpass");
    }
    threshold = tolerance;
    second_pass = TRUE;
    final_pass = emergency_stretch <= 0;
  } else {
    if (tracing_paragraphs > 0) {
      print_nl("@emergencypass");
    }
    background[2] += emergency_stretch;
    final_pass = TRUE;
  }	
  goto next_pass;

done:
  if (tracing_paragraphs > 0) {
    end_diagnostic(TRUE);
    normalize_selector();
  }
  post_line_break(t, final_widow_penalty);
  for (q = link(active); q != last_active; q = cur_p) {
    cur_p = link(q);
    if (type(q) == DELTA_NODE)
      free_node(q, DELTA_NODE_SIZE);
    else free_node(q, ACTIVE_NODE_SIZE);
  }
  for (q = passive; q != null; q = cur_p) {
    cur_p = link(q);
    free_node(q, PASSIVE_NODE_SIZE);
  }
  pack_begin_line = 0;
}

/* this will become a subroutine */
#define get_active_nodes() 						\
{	if (no_break_yet) {						\
  no_break_yet = FALSE;					\
  set_break_width(break_type);				\
                    }								\
  if (type(prev_r) == DELTA_NODE) {				\
    do_all_six(convert_to_break_width);			\
  } else if (prev_r == active) {					\
    do_all_six(store_break_width);				\
  } else {							\
    q = new_node(DELTA_NODE_SIZE);				\
    link(q) = r;						\
    type(q) = DELTA_NODE;					\
    subtype(q) = 0;						\
    do_all_six(new_delta_to_break_width);			\
    prev_prev_r = prev_r;					\
    prev_r = link(prev_r) = q;				\
  }								\
  if (abs(adj_demerits) >= AWFUL_BAD - minimum_demerits) {	\
    minimum_demerits = AWFUL_BAD - 1;			\
  } else {							\
    minimum_demerits += abs(adj_demerits);			\
  }								\
  fit_class = VERY_LOOSE_FIT;					\
  while (fit_class <= TIGHT_FIT) {				\
    if (minimal_demerits[fit_class] <= minimum_demerits) {	\
      q = get_break_node(fit_class, break_type);	\
      link(q) = r;					\
      prev_r = link(prev_r) = q;			\
    }							\
    minimal_demerits[fit_class] = AWFUL_BAD;		\
    incr(fit_class);					\
  }								\
  minimum_demerits = AWFUL_BAD;					\
  if (r != last_active) {						\
    q = new_node(DELTA_NODE_SIZE);				\
    link(q) = r;						\
    type(q) = DELTA_NODE;					\
    subtype(q) = 0;						\
    do_all_six(new_delta_from_break_width);			\
    prev_prev_r = prev_r;					\
    prev_r = link(prev_r) = q;				\
  }								\
}

  void
try_break (pi, break_type)
  int	pi;
  int	break_type;
{
  int	b, d;
  int	l, old_l;
  bool	artificial_demerits;
  ptr	q, r, prev_r, prev_prev_r;
  bool	node_r_stays_active;
  bool	no_break_yet;

  if (abs(pi) >= INF_PENALTY) {
    if (pi > 0) {
      update_printed_node();
      return;
    } else {
      pi = EJECT_PENALTY;
    }
  }
  no_break_yet = TRUE;
  prev_r = active;
  old_l = 0;
  do_all_six(copy_to_cur_active);
  loop {
    r = link(prev_r);
    if (type(r) == DELTA_NODE) {
      do_all_six(update_width);
      prev_prev_r = prev_r;
      prev_r = r;
      continue;
    }
    l = line_number(r);
    if (l > old_l) {
      if (minimum_demerits < AWFUL_BAD
          && (old_l != easy_line || r == last_active)) {
        get_active_nodes();
      }
      if (r == last_active) {
        update_printed_node();
        return;
      }
      if (l > easy_line) {
        line_width = second_width;
        old_l = MAX_HALFWORD - 1;
      } else {
        old_l = l;
        if (l > last_special_line) {
          line_width = second_width;
        } else if (par_shape_ptr == null) {
          line_width = first_width;
        } else {
          line_width = par_shape_width(l);
        }
      }
    }
    artificial_demerits = FALSE;
    b = get_badness();
    if (b > INF_BAD || pi == EJECT_PENALTY) {
      if (final_pass && minimum_demerits == AWFUL_BAD
          && link(r) == last_active && prev_r == active) {
        artificial_demerits = TRUE;
      } else if (b > threshold) {
        goto deactivate;
      }
      node_r_stays_active = FALSE;
    } else {
      prev_r = r;
      if (b > threshold)
        continue;
      node_r_stays_active = TRUE;
    }
    if (artificial_demerits) {
      d = 0;
    } else {
      d = line_penalty + b;
      if (abs(d) >= 10000) {
        d = 100000000;
      } else {
        d = d * d;
      }
      if (pi != 0) {
        if (pi > 0) {
          d += pi * pi;
        } else if (pi > EJECT_PENALTY) {
          d -= pi * pi;
        }
      }
      if (break_type == HYPHENATED
          && type(r) == HYPHENATED) {
        if (cur_p != null) {
          d += double_hyphen_demerits;
        } else {
          d += final_hyphen_demerits;
        }
      }
      if (abs(fit_class - (int)fitness(r)) > 1)
        d += adj_demerits;
    }
    if (tracing_paragraphs > 0)
      show_break_status(r, artificial_demerits, b, pi, d);
    d += total_demerits(r);
    if (d <= minimal_demerits[fit_class]) {
      minimal_demerits[fit_class] = d;
      best_place[fit_class] = break_node(r);
      best_pl_line[fit_class] = l;
      if (d < minimum_demerits)
        minimum_demerits = d;
    }
    if (node_r_stays_active)
      continue;

deactivate:
    link(prev_r) = link(r);
    free_node(r, ACTIVE_NODE_SIZE);
    if (prev_r == active) {
      r = link(active);
      if (type(r) == DELTA_NODE) {
        do_all_six(update_active);
        do_all_six(copy_to_cur_active);
        link(active) = link(r);
        free_node(r, DELTA_NODE_SIZE);
      }
    } else if (type(prev_r) == DELTA_NODE) {
      r = link(prev_r);
      if (r == last_active) {
        do_all_six(downdate_width);
        link(prev_prev_r) = last_active;
        free_node(prev_r, DELTA_NODE_SIZE);
        prev_r = prev_prev_r;
      } else if (type(r) == DELTA_NODE) {
        do_all_six(update_width);
        do_all_six(combine_two_deltas);
        link(prev_r) = link(r);
        free_node(r, DELTA_NODE_SIZE);
      }
    }
  }
}

  void
post_line_break (p, final_widow_penalty)
  ptr	p;
  int	final_widow_penalty;
{
  ptr	q;
  ptr	r;
  ptr	s;
  int	t;
  int	pen;
  int	cur_line;
  scal	cur_width;
  scal	cur_indent;
  bool	disc_break;
  bool	post_disc_break;

  q = break_node(best_bet);
  cur_p = null;
  while (q != null) {
    r = q;
    q = prev_break(q);
    next_break(r) = cur_p;
    cur_p = r;
  }
  cur_line = prev_graf + 1;
  do {
    q = cur_break(cur_p);
    disc_break = FALSE;
    post_disc_break = FALSE;
    if (q != null) {
      if (type(q) == GLUE_NODE) {
        delete_glue_ref(glue_ptr(q));
        glue_ptr(q) = right_skip;
        subtype(q) = RIGHT_SKIP_CODE + 1;
        add_glue_ref(right_skip);
        goto done;
      } else if (type(q) == DISC_NODE) {
        t = replace_count(q);
        if (t == 0) {
          r = link(q);
        } else {
          r = q;
          while (t > 1) {
            r = link(r);
            decr(t);
          }
          s = link(r);
          r = link(s);
          link(s) = null;
          flush_node_list(link(q));
          replace_count(q) = 0;
        }
        if (post_break(q) != null) {
          s = post_break(q);
          while (link(s) != null)
            s = link(s);
          link(s) = r;
          r = post_break(q);
          post_break(q) = null;
          post_disc_break = TRUE;
        }
        if (pre_break(q) != null) {
          s = pre_break(q);
          link(q) = s;
          while (link(s) != null)
            s = link(s);
          pre_break(q) = null;
          q = s;
        }
        link(q) = r;
        disc_break = TRUE;
      } else if (type(q) == MATH_NODE) {
        math_width(q) = 0;
      } else if (type(q) == KERN_NODE) {
        kern_width(q) = 0;
      }
    } else {
      q = p;
      while (link(q) != null)
        q = link(q);
    }
    r = new_param_glue(RIGHT_SKIP_CODE);
    link(r) = link(q);
    q = link(q) = r;

done:
    r = link(q);
    link(q) = null;
    q = link(p);
    link(p) = r;
    if (left_skip != zero_glue) {
      r = new_param_glue(LEFT_SKIP_CODE);
      link(r) = q;
      q = r;
    }
    if (cur_line > last_special_line) {
      cur_width = second_width;
      cur_indent = second_indent;
    } else if (par_shape_ptr == null) {
      cur_width = first_width;
      cur_indent = first_indent;
    } else {
      cur_width = par_shape_width(cur_line);
      cur_indent = par_shape_indent(cur_line);
    }
    adjust_tail = adjust_head;
    just_box = hpack(q, cur_width, EXACTLY);
    shift_amount(just_box) = cur_indent;
    append_to_vlist(just_box);
    if (adjust_head != adjust_tail) {
      link(tail) = link(adjust_head);
      tail = adjust_tail;
    }
    adjust_tail = null;
    if (cur_line + 1 != best_line) {
      pen = inter_line_penalty;
      if (cur_line == prev_graf + 1)
        pen += club_penalty;
      if (cur_line + 2 == best_line)
        pen += final_widow_penalty;
      if (disc_break)
        pen += broken_penalty;
      if (pen != 0) {
        r = new_penalty(pen);
        link(tail) = r;
        tail = r;
      }
    }
    incr(cur_line);
    cur_p = next_break(cur_p);
    if (cur_p != null && !post_disc_break) {
      r = p;
      loop {
        q = link(r);
        if (q == cur_break(cur_p))
          break;
        if (is_char_node(q))
          break;
        if (non_discardable(q))
          break;
        if (subtype(q) == ACC_KERN
            && type(q) == KERN_NODE)
          break;
        r = q;
      }
      if (r != p) {
        link(r) = null;
        flush_node_list(link(p));
        link(p) = q;
      }
    }
  } while (cur_p != null);
  if (cur_line != best_line || link(p) != null)
    confusion("line breaking");
  prev_graf = best_line - 1;
}

  void
set_disc_width ()
{
  ptr	p;

  for (p = pre_break(cur_p); p != null; p = link(p)) {
    if (is_char_node(p)) {
      disc_width += width_char(p);
    } else {
      switch (type(p))
      {
        case LIGATURE_NODE:
          disc_width += width_lig_char(p);
          break;

        case HLIST_NODE:
        case VLIST_NODE:
          disc_width += box_width(p);
          break;

        case RULE_NODE:
          disc_width += rule_width(p);
          break;

        case KERN_NODE:
          disc_width += kern_width(p);
          break;

        default:
          confusion("disc3");
          break;
      }
    }
  }
}

  void
set_act_width ()
{
  ptr	p;
  int	n;

  p = link(cur_p);
  for (n = replace_count(cur_p); n > 0; decr(n)) {
    if (is_char_node(p)) {
      act_width += width_char(p);
    } else {
      switch (type(p))
      {
        case LIGATURE_NODE:
          act_width += width_lig_char(p);
          break;

        case HLIST_NODE:
        case VLIST_NODE:
          act_width += box_width(p);
          break;

        case RULE_NODE:
          act_width += rule_width(p);
          break;

        case KERN_NODE:
          act_width += kern_width(p);
          break;

        default:
          confusion("disc3");
          break;
      }
    }
    p = link(p);
  }
  cur_p = p;
}

  void
set_break_width (break_type)
  int     break_type;
{
  int     t;
  ptr     p, q;

  do_all_six(set_break_width_to_background);
  p = cur_p;
  if (break_type > UNHYPHENATED && cur_p != null) {
    t = replace_count(cur_p);
    q = cur_p;
    while (t > 0) {
      decr(t);
      q = link(q);
      if (is_char_node(q)) {
        break_width[1] -= width_char(q);
      } else {
        switch (type(q))
        {
          case LIGATURE_NODE:
            break_width[1] -= width_lig_char(q);
            break;

          case HLIST_NODE:
          case VLIST_NODE:
            break_width[1] -= box_width(q);
            break;

          case RULE_NODE:
            break_width[1] -= rule_width(q);
            break;

          case KERN_NODE:
            break_width[1] -= kern_width(q);
            break;

          default:
            confusion("disc1");
            break;
        }
      }
    }
    for (p = post_break(cur_p); p != null; p = link(p)) {
      if (is_char_node(p)) {
        break_width[1] += width_char(p);
      } else {
        switch (type(p))
        {
          case LIGATURE_NODE:
            break_width[1] += width_lig_char(p);
            break;

          case HLIST_NODE:
          case VLIST_NODE:
            break_width[1] += box_width(p);
            break;

          case RULE_NODE:
            break_width[1] += rule_width(p);
            break;

          case KERN_NODE:
            if (t == 0 && subtype(p) != ACC_KERN) {
              t = -1;
            } else {
              break_width[1] += kern_width(p);
            }
            break;

          default:
            confusion("disc2");
            break;
        }
      }
      incr(t);
    }
    break_width[1] += disc_width;
    if (t == 0)
      p = link(q);
  }
  while (p != null) {
    if (is_char_node(p))
      return;
    switch (type(p))
    {
      case GLUE_NODE:
        q = glue_ptr(p);
        break_width[1] -= glue_width(q);
        break_width[2 + stretch_order(q)] -= stretch(q);
        break_width[6] -= shrink(q);
        break;

      case PENALTY_NODE:
        break;

      case MATH_NODE:
        break_width[1] -= math_width(p);
        break;

      case KERN_NODE:
        if (subtype(p) == ACC_KERN)
          return;
        break_width[1] -= kern_width(p);
        break;

      default:
        return;
    }
    p = link(p);
  }
}

  int
get_best_bet ()
{
  ptr	p;

  fewest_demerits = AWFUL_BAD;
  for (p = link(active); p != last_active; p = link(p)) {
    if (type(p) != DELTA_NODE) {
      if (total_demerits(p) < fewest_demerits) {
        fewest_demerits = total_demerits(p);
        best_bet = p;
      }
    }
  }
  best_line = line_number(best_bet);
  if (looseness == 0)
    return TRUE;
  actual_looseness = 0;
  for (p = link(active); p != last_active; p = link(p)) {
    if (type(p) != DELTA_NODE) {
      line_diff = line_number(p) - best_line;
      if (line_diff < actual_looseness
          && looseness <= line_diff
          || line_diff > actual_looseness
          && looseness >= line_diff) {
        best_bet = p;
        actual_looseness = line_diff;
        fewest_demerits = total_demerits(p);
      } else if (line_diff == actual_looseness
          && total_demerits(p) < fewest_demerits) {
        best_bet = p;
        fewest_demerits = total_demerits(p);
      }
    }
  }
  best_line = line_number(best_bet);
  if (actual_looseness == looseness || final_pass)
    return TRUE;
  return FALSE;
}

  int
get_badness ()
{
  scal	s;	/* shortfall */
  int	b;	/* badness */

  s = line_width - cur_active_width[1];
  if (s > 0) {
    if (cur_active_width[3] != 0
        || cur_active_width[4] != 0
        || cur_active_width[5] != 0) {
      fit_class = DECENT_FIT;
      return 0;
    }
    if (s > 7230584 && cur_active_width[2] < 1663497) {
      fit_class = VERY_LOOSE_FIT;
      return INF_BAD;
    }
    b = badness(s, cur_active_width[2]);
    if (b > 12) {
      if (b > 99) {
        fit_class = VERY_LOOSE_FIT;
      } else {
        fit_class = LOOSE_FIT;
      }
    } else {
      fit_class = DECENT_FIT;
    }
  } else {
    if (-s > cur_active_width[6]) {
      b = INF_BAD + 1;
    } else {
      b = badness(-s, cur_active_width[6]);
    }
    if (b > 12) {
      fit_class = TIGHT_FIT;
    } else {
      fit_class = DECENT_FIT;
    }
  }
  return b;
}

  ptr
get_break_node (fit_class, break_type)
  int	fit_class, break_type;
{
  ptr	p;

  p = new_node(PASSIVE_NODE_SIZE);
  link(p) = passive;
  passive = p;
  cur_break(p) = cur_p;
  incr(pass_number);
  serial(p) = pass_number;
  prev_break(p) = best_place[fit_class];
  p = new_node(ACTIVE_NODE_SIZE);
  break_node(p) = passive;
  line_number(p) = best_pl_line[fit_class] + 1;
  fitness(p) = fit_class;
  type(p) = break_type;
  total_demerits(p) = minimal_demerits[fit_class];
  if (tracing_paragraphs > 0)
    show_break_node(p, fit_class, break_type);

  return p;
}

  void
show_break_node (p, fit_class, break_type)
  ptr	p;
  int	fit_class;
  int	break_type;
{
  print_nl("@@");
  print_int(serial(passive));
  print(": line ");
  print_int(line_number(p) - 1);
  print(".");
  print_int(fit_class);
  if (break_type == HYPHENATED)
    print("-");
  print(" t=");
  print_int(total_demerits(p));
  print(" -> @@");
  if (prev_break(passive) == null)
    print("0");
  else print_int(serial(prev_break(passive)));
}

  void
show_break_status (r, a, b, p, d)
  ptr	r;
  bool	a;
  int	b;
  int	p;
  int	d;
{
  ptr	save_link;

  if (printed_node != cur_p) {
    print_nl(null_str);
    if (cur_p == null) {
      short_display(link(printed_node));
    } else {
      save_link = link(cur_p);
      link(cur_p) = null;
      print_nl(null_str);
      short_display(link(printed_node));
      link(cur_p) = save_link;
    }
    printed_node = cur_p;
  }
  print_nl("@");
  if (cur_p == null) {
    print_esc("par");
  } else if (type(cur_p) != GLUE_NODE) {
    if (type(cur_p) == PENALTY_NODE) {
      print_esc("penalty");
    } else if (type(cur_p) == DISC_NODE) {
      print_esc("discretionary");
    } else if (type(cur_p) == KERN_NODE) {
      print_esc("kern");
    } else {
      print_esc("math");
    }
  }
  print(" via @@");
  if (break_node(r) == null) {
    print("0");
  } else {
    print_int(serial(break_node(r)));
  }
  print(" b=");
  if (b > INF_BAD) {
    print("*");
  } else {
    print_int(b);
  }
  print(" p=");
  print_int(p);
  print(" d=");
  if (a) {
    print("*");
  } else {
    print_int(d);
  }
}

  void
update_printed_node ()
{
  int	t;

  if (cur_p == printed_node
      && cur_p != null
      && type(cur_p) == DISC_NODE) {
    for (t = replace_count(cur_p); t > 0; decr(t)) {
      printed_node = link(printed_node);
    }
  }
}

  ptr
finite_shrink (p)
  ptr	p;
{
  ptr	q;

  if (no_shrink_error_yet) {
    no_shrink_error_yet = FALSE;
    print_err("Infinite glue shrinkage found in a paragraph");
    help_shrink();
    error();
  }
  q = new_spec(p);
  shrink_order(q) = NORMAL;
  delete_glue_ref(p);
  return q;
}

  void
_par_init ()
{
}

  void
_par_init_once ()
{
  active = new_node(ACTIVE_NODE_SIZE);
  type(active) = HYPHENATED;
  subtype(active) = 0;
  line_number(active) = MAX_HALFWORD;
}

/*
 **	Help text
 */

help_shrink()
{
  help5("The paragraph just ended includes some glue that has",
      "infinite shrinkability, e.g., `\\hskip 0pt minus 1fil'.",
      "Such glue doesn't belong there---it allows a paragraph",
      "of any length to fit on one line. But it's safe to proceed,",
      "since the offensive shrinkability has been made finite.");
}

/*
 * Common TeX:print.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX print.c 3.2@(#)";
#endif

#include "tex.h"

file	log_file;
str	log_name;

int	selector;
int	term_offset;
int	file_offset;
int	tally;
int	first_count;
int	trick_count;
int	trick_buf[ERROR_LINE];
int	dig[23];

  void
print_ln ()
{
  switch (selector)
  {
    case TERM_AND_LOG:
      wterm_cr();
      term_offset = 0;
      wlog_cr();
      file_offset = 0;
      break;

    case LOG_ONLY:
      wlog_cr();
      file_offset = 0;
      break;

    case TERM_ONLY:
      wterm_cr();
      term_offset = 0;
      break;

    case NO_PRINT:
    case PSEUDO:
    case NEW_STRING:
      break;

    default:
      wfile_cr();
      break;
  }
}	

  void
print_char (c)
  int	c;
{
  if (c == new_line_char && selector < PSEUDO) {
    print_ln();
    return;
  }
  switch (selector)
  {
    case TERM_AND_LOG:
      wterm(xchr[c]);
      incr(term_offset);
      wlog(xchr[c]);
      incr(file_offset);
      if (term_offset == MAX_PRINT_LINE)
        print_ln();
      if (file_offset == MAX_PRINT_LINE)
        print_ln();
      break;

    case LOG_ONLY:
      wlog(xchr[c]);
      incr(file_offset);
      if (file_offset == MAX_PRINT_LINE)
        print_ln();
      break;

    case TERM_ONLY:
      wterm(xchr[c]);
      incr(term_offset);
      if (term_offset == MAX_PRINT_LINE)
        print_ln();
      break;

    case NO_PRINT:
      return;

    case PSEUDO:
      if (tally < trick_count) 
        trick_buf[tally % ERROR_LINE] = c;
      break;

    case NEW_STRING:
      if (cur_str_ptr < cur_str_end)
        append_char(c);
      break;

    default:
      wfile(xchr[c]);
      break;
  }
  incr(tally);
}

  void
print_ASCII (c)
  int	c;
{
  if (c == new_line_char && selector < PSEUDO) {
    print_ln();
    return;
  }
  print(ASCII[c]);
}

  void
print (s)
  str	s;
{
  while (*s)
    print_char(*s++);
}

  void
print_nl (s)
  str 	s;
{
  if (term_offset > 0 && odd(selector)
      || file_offset > 0 && selector >= LOG_ONLY)
    print_ln();
  print(s);
}

  void
print_esc (s)
  str	s;
{
  int	c;

  c = escape_char;
  if (c >= 0 && c < 256) 
    print_ASCII(c);
  print(s);
}

  void
print_int (n)
  int	n;
{
  int	m;
  int	k;

  k = 0;
  if (n < 0)  {
    print("-");
    if (n > -100000000) {
      negate(n);
    } else {
      m = -1 - n;
      n = m / 10;
      m = m % 10 + 1;
      k = 1;
      if (m < 10) {
        dig[0] = m;
      } else {
        dig[0] = 0;
        incr(n);
      }
    }
  }
  do {
    dig[k] = n % 10;
    n /= 10;
    incr(k);
  } while (n != 0);
  print_the_digs(k);
}

  void
print_hex (n)
  int	n;
{
  int	k;

  k = 0;
  print("\"");
  do {
    dig[k] = n % 16;
    n /= 16;
    incr(k);
  } while (n != 0);
  print_the_digs(k);
}

  void
print_the_digs (k)
  int	k;
{
  while (k > 0) {
    decr(k);
    if (dig[k] < 10)
      print_char('0' + dig[k]);
    else print_char('A' - 10 + dig[k]);
  }
}

  void
print_two (n)
  int	n;
{
  n = abs(n) % 100;
  print_char('0' + n / 10);
  print_char('0' + n % 10);
}

  void
print_roman_int (n)
  int	n;
{
  str 	s;
  str 	t;
  int	u;
  int	v;

  s = "m2d5c2l5x2v5i";
  v = 1000;
  loop {
    while (n >= v) {
      print_char(*s);
      n -= v;
    }
    if (n <= 0)
      return;
    t = s + 2;
    u = v / (t[-1] - '0');
    if (t[-1] == '2')  {
      t += 2;
      u /= t[-1] - '0';
    }
    if (n + u >= v)  {
      print_char(*t);
      n += u;
    } else {
      s += 2;
      v /= s[-1] - '0';
    }
  }
}

  int
begin_pseudoprint ()
{
  int 	t;

  t = tally;
  tally = 0;
  selector = PSEUDO;
  trick_count = 1000000;

  return t;
}

  void
set_trick_count ()
{
  first_count = tally;
  trick_count = tally + 1 + ERROR_LINE - HALF_ERROR_LINE;
  if (trick_count < ERROR_LINE) 
    trick_count = ERROR_LINE;
}

  void
_print_init ()
{
  selector = TERM_ONLY;
  tally = 0;
  term_offset = 0;
  file_offset = 0;
  fputs(banner, term_out);
  if (format_ident == null_str)
    fputs(" (no format preloaded)", term_out);
  else fputs(format_ident, term_out);
  fputs("\n", term_out);
  log_name = null_str;
}

  void
_print_init_once ()
{
  selector = TERM_ONLY;
  tally = 0;
  term_offset = 0;
  file_offset = 0;
  log_name = null_str;
}

/*
 * Common TeX:scan.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX scan.c 3.2@(#)";
#endif

#include "tex.h"

int	cur_val;
int	cur_val_level;
int	cur_order;
int	radix;

  void
scan_left_brace ()
{
  get_nbrx_token();
  if (cur_cmd != LEFT_BRACE) {
    print_err("Missing { inserted");
    help_left();
    back_error();
    cur_tok = LEFT_BRACE_TOKEN;
    cur_cmd = LEFT_BRACE;
    cur_chr= '{';
    incr(align_state);
  }
}

  void
scan_optional_equals ()
{
  get_nbx_token();
  if (cur_tok != OTHER_TOKEN + '=')
    back_input();
}

  bool
scan_keyword (s)
  str	s;
{
  int	c;
  ptr	h;
  ptr	p;

  h = p = new_token();
  token_link(p) = null;
  c = *s;
  while (c != NUL) {
    get_x_token();
    if (cur_cs == null_sym
        && (cur_chr == c || cur_chr == c - 'a' + 'A')) {
      p = token_link(p) = new_token();
      token(p) = cur_tok;
      c = *++s;
    } else if (cur_cmd != SPACER || p != h) {
      back_input();
      if (p != h)
        back_list(token_link(h));
      free_token(h);
      return FALSE;
    }
  }
  flush_list(h);
  return TRUE;
}

  void
mu_error ()
{
  print_err("Incompatible glue units");
  help_mu_error();
  error();
}

#define scanned_result(CV, CVL) \
  (cur_val = (int) (CV)), (cur_val_level = (int) (CVL))

#define negate_glue() \
{glue_width(cur_val) = -glue_width(cur_val); \
  stretch(cur_val) = -stretch(cur_val); \
  shrink(cur_val) = -shrink(cur_val);}

  void
scan_something_internal (level, negative)
  int	level;
  bool	negative;
{
  int	m;

  m = cur_chr;
  switch (cur_cmd)
  {
    case DEF_CODE:
      scan_eight_bit_int();
      switch (m)
      {
        case CAT_CODE:
          scanned_result(cat_code(cur_val), INT_VAL);
          break;

        case UC_CODE:
          scanned_result(uc_code(cur_val), INT_VAL);
          break;

        case LC_CODE:
          scanned_result(lc_code(cur_val), INT_VAL);
          break;

        case SF_CODE:
          scanned_result(sf_code(cur_val), INT_VAL);
          break;

        case MATH_CODE:
          scanned_result(math_code(cur_val), INT_VAL);
          break;

        case DEL_CODE:
          scanned_result(del_code(cur_val), INT_VAL);
          break;
      }
      break;

    case TOKS_REGISTER:
    case ASSIGN_TOKS:
      if (level != TOK_VAL) {
        print_err("Missing number, treated as zero");
        help_missing_number();
        back_error();
        scanned_result(0, DIMEN_VAL);
      } else {
        if (cur_cmd == TOKS_REGISTER)  {
          scan_eight_bit_int();
          m = TOK_PARS + cur_val;
        }
        m = (int) reg_equiv(toks_reg[m]);
        scanned_result(m, TOK_VAL);
      }
      break;

    case DEF_FAMILY:
    case SET_FONT:
    case DEF_FONT:
      if (level != TOK_VAL) {
        print_err("Missing number, treated as zero");
        help_missing_number();
        back_error();
        scanned_result(0, DIMEN_VAL);
      } else {
        back_input();
        scan_font_ident();
        scanned_result((int)font_id_base[cur_val], IDENT_VAL);
      }
      break;

    case ASSIGN_INT:
      scanned_result(reg_equiv(int_reg[m]), INT_VAL);
      break;

    case ASSIGN_DIMEN:
      scanned_result(reg_equiv(dimen_reg[m]), DIMEN_VAL);
      break;

    case ASSIGN_GLUE:
      scanned_result(reg_equiv(skip_reg[m]), GLUE_VAL);
      break;

    case ASSIGN_MU_GLUE:
      scanned_result(reg_equiv(mu_skip_reg[m]), MU_VAL);
      break;

    case SET_AUX:
      if (abs(mode) != m) {
        print_err("Improper ");
        print_cmd_chr(SET_AUX, m);
        help_aux();
        error();
        if (level != TOK_VAL) {
          scanned_result(0, DIMEN_VAL);
        } else {
          scanned_result(0, INT_VAL);
        }
      } else if (m == VMODE) {
        cur_val = prev_depth; 
        cur_val_level = DIMEN_VAL;
      } else {
        cur_val = space_factor;
        cur_val_level = INT_VAL;
      }
      break;

    case SET_PREV_GRAF: {
                          list	*p;

                          if (mode == 0) {
                            scanned_result(0, INT_VAL);
                          } else {
                            *nest_ptr = cur_list;
                            p = nest_ptr;
                            while (abs(p->mode_field) != VMODE) {
                              decr(p);
                            }
                            scanned_result(p->pg_field, INT_VAL);
                          }
                          break;
                        }

    case SET_PAGE_INT:
                        if (m == 0) {
                          cur_val = dead_cycles;
                        } else {
                          cur_val = insert_penalties;
                        }
                        cur_val_level = INT_VAL;
                        break;

    case SET_PAGE_DIMEN:
                        if (page_contents == EMPTY && !output_active) {
                          if (m == 0) {
                            cur_val = MAX_DIMEN; 
                          } else {
                            cur_val = 0;
                          }
                        } else {
                          cur_val = page_so_far[m];
                        }
                        cur_val_level = DIMEN_VAL;
                        break;

    case SET_SHAPE:
                        if (par_shape_ptr == null) {
                          cur_val = 0;
                        } else {
                          cur_val = info(par_shape_ptr);
                        }
                        cur_val_level = INT_VAL;
                        break;

    case SET_BOX_DIMEN:
                        scan_eight_bit_int();
                        if (box(cur_val) == null) {
                          cur_val = 0;
                        } else {
                          switch (m) {
                            case WD_CODE: cur_val = box_width(box(cur_val)); break;
                            case HT_CODE: cur_val = box_height(box(cur_val)); break;
                            case DP_CODE: cur_val = box_depth(box(cur_val)); break;
                          }
                        }
                        cur_val_level = DIMEN_VAL;
                        break;

    case CHAR_GIVEN:
    case MATH_GIVEN:
                        scanned_result(m, INT_VAL);
                        break;

    case ASSIGN_FONT_DIMEN: {
                              scal	*d;

                              d = find_font_dimen(FALSE);
                              if (d == (scal *) 0) {
                                scanned_result(0, DIMEN_VAL); 
                              } else {
                                scanned_result(*d, DIMEN_VAL); 
                              }
                              break;
                            }

    case ASSIGN_FONT_INT:
                            scan_font_ident();
                            if (m == 0) {
                              scanned_result(hyphen_char(cur_val), INT_VAL);
                            } else {
                              scanned_result(skew_char(cur_val), INT_VAL);
                            }
                            break;

    case REGISTER:
                            scan_eight_bit_int();
                            switch (m) {
                              case INT_VAL: cur_val = count(cur_val); break; 
                              case DIMEN_VAL: cur_val = dimen(cur_val); break; 
                              case GLUE_VAL: cur_val = skip(cur_val); break; 
                              case MU_VAL: cur_val = mu_skip(cur_val); break;
                            }
                            cur_val_level = m;
                            break;

    case LAST_ITEM:
                            if (cur_chr > GLUE_VAL) {
                              if (cur_chr == INPUT_LINE_NO_CODE) {
                                cur_val = line;
                              } else {
                                cur_val = last_badness;
                              }
                              cur_val_level = INT_VAL;
                              break;
                            }
                            if (cur_chr == GLUE_VAL) {
                              cur_val = zero_glue;
                            } else {
                              cur_val = 0;
                            }
                            cur_val_level = cur_chr;
                            if (mode != 0 && tail != head && !is_char_node(tail)) {
                              switch (cur_chr)
                              {
                                case INT_VAL:
                                  if (type(tail) == PENALTY_NODE)
                                    cur_val = penalty(tail);
                                  break;

                                case DIMEN_VAL:
                                  if (type(tail) == KERN_NODE)
                                    cur_val = kern_width(tail);
                                  break;

                                case GLUE_VAL:
                                  if (type(tail) == GLUE_NODE) {
                                    cur_val = glue_ptr(tail); 
                                    if (subtype(tail) == MU_GLUE)
                                      cur_val_level = MU_VAL;
                                  }
                                  break;

                                default:
                                  break;
                              }
                            } else if (mode == VMODE && tail == head) {
                              switch (cur_chr)
                              {
                                case INT_VAL:
                                  cur_val = last_penalty;
                                  break;

                                case DIMEN_VAL:
                                  cur_val = last_kern;
                                  break;

                                case GLUE_VAL:
                                  if (last_glue != null)
                                    cur_val = last_glue;
                                  break;
                              }
                            }
                            break;

    default:
                            print_err("You can't use `");
                            print_cmd_chr(cur_cmd, cur_chr);
                            print("' after ");
                            print_esc("the");
                            help_thee();
                            error();
                            if (level != TOK_VAL) {
                              scanned_result(0, DIMEN_VAL);
                            } else {
                              scanned_result(0, INT_VAL);
                            }
                            break;
  }
  while (cur_val_level > level) {
    if (cur_val_level == GLUE_VAL) {
      cur_val = glue_width(cur_val);
    } else if (cur_val_level == MU_VAL) {
      mu_error();
    }
    decr(cur_val_level);
  }
  if (negative) {
    if (cur_val_level >= GLUE_VAL) {
      cur_val = new_spec(cur_val);
      negate_glue();
    } else {
      cur_val = -cur_val;
    }
  } else if (cur_val_level >= GLUE_VAL && cur_val_level <= MU_VAL) {
    add_glue_ref(cur_val);
  }
}

  void
scan_eight_bit_int ()
{
  scan_int();
  if (cur_val < 0 || cur_val > 255) {
    print_err("Bad register code");
    help_reg();
    int_error(cur_val);
    cur_val = 0;
  }
}

  void
scan_char_num ()
{
  scan_int();
  if (cur_val < 0 || cur_val > 255) {
    print_err("Bad character code");
    help_char_num();
    int_error(cur_val);
    cur_val = 0;
  }
}

  void
scan_four_bit_int()
{
  scan_int();
  if (cur_val < 0 || cur_val > 15) {
    print_err("Bad number");
    help_number();
    int_error(cur_val);
    cur_val = 0;
  }
}


  void
scan_fifteen_bit_int ()
{
  scan_int();
  if (cur_val < 0 || cur_val > 077777) {
    print_err("Bad math code");
    help_mathchar();
    int_error(cur_val);
    cur_val = 0;
  }
}

  void
scan_twenty_seven_bit_int ()
{
  scan_int();
  if (cur_val < 0 || cur_val> 0777777777) {
    print_err("Bad delimiter code");
    help_del();
    int_error(cur_val);
    cur_val = 0;
  }
}

#define get_nbsx_token() \
{negative = FALSE; \
  do {get_nbx_token(); \
    if (cur_tok == MINUS_TOKEN) { \
      negative = !negative; \
      cur_tok = PLUS_TOKEN;} \
  } while (cur_tok == PLUS_TOKEN);}

  void
scan_int ()
{
  int	d;
  int	m;
  sym	s;
  bool	vacuous;
  bool	negative;
  bool	OK_so_far;

  radix = 0;
  OK_so_far = TRUE;
  get_nbsx_token();
  if (cur_tok == ALPHA_TOKEN) {
    get_token();
    if (cur_tok < CS_TOKEN_FLAG) {
      cur_val = cur_chr;
      if (cur_cmd <= RIGHT_BRACE) {
        if (cur_cmd == RIGHT_BRACE) {
          incr(align_state);
        } else {
          decr(align_state);
        }
      }
    } else {
      s = tok2sym(cur_tok);
      if (length(s) == 1) {
        cur_val = *text(s);
      } else {
        cur_val = 256;
      }
    }
    if (cur_val > 255) {
      print_err("Improper alphabetic constant");
      help_char_const();
      cur_val = 0;
      back_error();
    } else {
      scan_optional_space();
    }
  } else if (cur_cmd >= MIN_INTERNAL && cur_cmd <= MAX_INTERNAL) {
    scan_something_internal(INT_VAL, FALSE);
  } else {
    radix = 10;
    m = 214748364;
    if (cur_tok == OCTAL_TOKEN) {
      radix = 8;
      m = 02000000000;
      get_x_token();
    } else if (cur_tok == HEX_TOKEN) {
      radix = 16;
      m = 010000000000;
      get_x_token();
    }
    vacuous = TRUE;
    cur_val = 0;
    loop {
      if (cur_tok < ZERO_TOKEN + radix &&
          cur_tok >= ZERO_TOKEN &&
          cur_tok <= ZERO_TOKEN + 9) {
        d = cur_tok - ZERO_TOKEN;
      } else if (radix == 16) {
        if (cur_tok <= A_TOKEN + 5
            && cur_tok >= A_TOKEN) {
          d = cur_tok - A_TOKEN + 10;
        } else if (cur_tok <= OTHER_A_TOKEN + 5
            && cur_tok >= OTHER_A_TOKEN) {
          d = cur_tok - OTHER_A_TOKEN + 10;
        } else {
          break;
        }
      } else {
        break;
      }
      vacuous = FALSE;
      if (cur_val >= m
          && (cur_val > m || d > 7 || radix != 10)) {
        if (OK_so_far) {
          print_err("Number too big"); 
          help_big_num();
          error();
          cur_val = INFINITY;
          OK_so_far = FALSE;
        }
      } else {
        cur_val = cur_val * radix + d;
      }
      get_x_token();
    }
    if (vacuous) {
      print_err("Missing number, treated as zero");
      help_missing_number();
      back_error();
    } else if (cur_cmd != SPACER) {
      back_input();
    }
  }
  if (negative)
    cur_val = -cur_val;
}	

#define set_conversion(NUM, DENOM) (num = (NUM)), (denom = (DENOM))

  void
scan_dimen (mu, inf, shortcut)
  bool	mu;
  bool	inf;
  bool	shortcut;
{
  scal	v;
  int	f, k, kk;
  ptr	p, q;
  int	num, denom;
  bool	negative;
  int	save_cur_val;

  f = 0;
  negative = FALSE;
  arith_error = FALSE;
  cur_order = NORMAL;
  if (!shortcut) {
    get_nbsx_token();
    if (cur_cmd >= MIN_INTERNAL && cur_cmd <= MAX_INTERNAL)	{
      if (mu) {
        scan_something_internal(MU_VAL, FALSE);
        if (cur_val_level >= GLUE_VAL) {
          v = glue_width(cur_val);
          delete_glue_ref(cur_val);
          cur_val = v;
        }
        if (cur_val_level == MU_VAL)
          goto attach_sign;
        if (cur_val_level != INT_VAL)
          mu_error();
      } else {
        scan_something_internal(DIMEN_VAL, FALSE);
        if (cur_val_level == DIMEN_VAL)
          goto attach_sign;
      }
    } else {
      back_input();
      if (cur_tok == EURO_POINT_TOKEN)
        cur_tok = POINT_TOKEN;
      if (cur_tok != POINT_TOKEN) {
        scan_int();
      } else {
        radix = 10;
        cur_val = 0;
      }
      if (cur_tok == EURO_POINT_TOKEN)
        cur_tok = POINT_TOKEN;
      if (radix == 10 && cur_tok == POINT_TOKEN) {
        k = 0;
        p = null;
        get_token();
        loop {
          get_x_token();
          if (cur_tok > ZERO_TOKEN + 9
              || cur_tok < ZERO_TOKEN) {
            break;
          }
          if (k < 17) {
            q = new_avail();
            link(q) = p;
            info(q) = cur_tok - ZERO_TOKEN;
            p = q;
            incr(k);
          }
        }
        for (kk = k; kk > 0; decr(kk)) {
          dig[kk - 1] = info(p);
          q = p;
          p = link(p);
          free_avail(q);
        }
        f = round_decimals(k);
        if (cur_cmd != SPACER)
          back_input();
      }
    }
  }
  if (cur_val < 0) {
    negative = !negative;
    cur_val = -cur_val;
  }
  if (inf) {
    if (scan_keyword("fil")) {
      cur_order = FIL;
      while (scan_keyword("l")) {
        if (cur_order == FILLL) {
          print_err("Illegal unit of measure (");
          print("replaced by filll)");
          help_filll();
          error();
        } else {
          incr(cur_order);
        }
      }
      goto attach_fraction;
    }
  }
  save_cur_val = cur_val;
  get_nbx_token();
  if (cur_cmd < MIN_INTERNAL || cur_cmd > MAX_INTERNAL) {
    back_input();
  } else {
    if (mu) {
      scan_something_internal(MU_VAL, FALSE);
      if (cur_val_level >= GLUE_VAL) {
        v = glue_width(cur_val);
        delete_glue_ref(cur_val);
        cur_val = v;
      }
      if (cur_val_level != MU_VAL)
        mu_error();
    } else {	
      scan_something_internal(DIMEN_VAL, FALSE);
    }
    v = cur_val;
    goto found;
  }
  if (mu)
    goto not_found;
  if (scan_keyword("em"))
    v = quad(cur_font);
  else if (scan_keyword("ex"))
    v = x_height(cur_font);
  else goto not_found;
  scan_optional_space();

found:
  cur_val = nx_plus_y(save_cur_val, v, xn_over_d(v, f, 0200000));
  goto attach_sign;

not_found:
  if (mu)  {
    if (scan_keyword("mu")) {
      goto attach_fraction;
    } else {
      print_err("Illegal unit of measure (");
      print("mu inserted)");
      help_mu();
      error();
      goto attach_fraction;
    }
  }
  if (scan_keyword("true")) {
    prepare_mag();
    if (mag != 1000) {
      cur_val = xn_over_d(cur_val, 1000, mag);
      f = (1000 * f + 0200000 * remainder) / mag;
      cur_val += f / 0200000;
      f %= 0200000;
    }
  }
  if (scan_keyword("pt"))
    goto attach_fraction;
  if (scan_keyword("in"))		{set_conversion(7227, 100);}
  else if (scan_keyword("pc")) {set_conversion(12, 1);}
  else if (scan_keyword("cm")) {set_conversion(7227, 254);}
  else if (scan_keyword("mm")) {set_conversion(7227, 2540);}
  else if (scan_keyword("bp")) {set_conversion(7227, 7200);}
  else if (scan_keyword("dd")) {set_conversion(1238, 1157);}
  else if (scan_keyword("cc")) {set_conversion(14856, 1157);}
  else if (scan_keyword("sp")) {goto done;}
  else {
    print_err("Illegal unit of measure (");
    print("pt inserted)");
    help_dimen();
    error();
    goto attach_fraction;
  }
  cur_val = xn_over_d(cur_val, num, denom);
  f = (num * f + 0200000 * remainder) / denom;
  cur_val += f / 0200000;
  f %= 0200000;

attach_fraction:
  if (cur_val >= 0400000)
    arith_error = TRUE;
  else cur_val = cur_val * UNITY + f;

done:
  scan_optional_space();

attach_sign:
  if (arith_error || abs(cur_val) >= 010000000000) {
    print_err("Dimension too large");
    help_big_dimen();
    error();
    cur_val = MAX_DIMEN;
    arith_error = FALSE;
  }
  if (negative)
    cur_val = -cur_val;
}

  void
scan_glue (level)
  int	level;
{
  ptr	q;
  bool	mu;
  bool	negative;

  mu = (level == MU_VAL);
  get_nbsx_token();
  if (cur_cmd >= MIN_INTERNAL && cur_cmd <= MAX_INTERNAL) {
    scan_something_internal(level, negative);
    if (cur_val_level >= GLUE_VAL) {
      if (cur_val_level != level)
        mu_error();
      return;
    }
    if (cur_val_level == INT_VAL)
      scan_dimen(mu, FALSE, TRUE);
    else if (level == MU_VAL)
      mu_error();
  } else {
    back_input();
    scan_dimen(mu, FALSE, FALSE);
    if (negative)
      cur_val = -cur_val;
  }
  q = new_spec(zero_glue);
  glue_width(q) = cur_val;
  if (scan_keyword("plus")) {
    scan_dimen(mu, TRUE, FALSE);
    stretch(q) = cur_val;
    stretch_order(q) = cur_order;
  }
  if (scan_keyword("minus")) {
    scan_dimen(mu, TRUE, FALSE);
    shrink(q) = cur_val;
    shrink_order(q) = cur_order;
  }
  cur_val = q;
}

  void
scan_spec (c, three_codes)
  int	c;
  bool	three_codes;
{
  int	s;
  int	spec_code;

  if (three_codes) {
    s = saved(0);
  }
  if (scan_keyword("to")) {
    spec_code = EXACTLY;
  } else if (scan_keyword("spread")) {
    spec_code = ADDITIONAL;
  } else {
    spec_code = ADDITIONAL;
    cur_val = 0;
    goto found;
  }
  scan_normal_dimen();

found:
  if (three_codes) {
    saved(0) = s;
    incr(save_ptr);
  }
  saved(0) = spec_code;
  saved(1) = cur_val;
  save_ptr += 2;
  new_save_level(c);
  scan_left_brace();
}

#define DEFAULT_RULE	26214

  ptr
scan_rule_spec ()
{
  ptr	q;

  q = new_rule();
  if (cur_cmd == VRULE) {
    rule_width(q) = DEFAULT_RULE;
  } else {
    rule_height(q) = DEFAULT_RULE;
    rule_depth(q) = 0;
  }

reswitch:
  if (scan_keyword("width")) {
    scan_normal_dimen();
    rule_width(q) = cur_val;
    goto reswitch;
  }
  if (scan_keyword("height")) {
    scan_normal_dimen();
    rule_height(q) = cur_val;
    goto reswitch;
  }
  if (scan_keyword("depth")) {
    scan_normal_dimen();
    rule_depth(q) = cur_val;
    goto reswitch;
  }
  return q;
}

  void
_scan_init ()
{
  cur_val = 0;
  cur_val_level = 0;
  radix = 0;
}

  void
_scan_init_once ()
{
}

/*
 ** 	Help text
 */

help_mathchar ()
{
  help2("A numeric math code must be between 0 and 32767.",
      "I changed this one to zero.");
}

help_number ()
{
  help2("Since I expected to read a number between 0 and 15,",
      "I changed this one to zero.");
}

help_char_num ()
{
  help2("A character number must be between 0 and 255.",
      "I changed this one to zero.");
}

help_char_const ()
{
  help2("A one-character control sequence belongs after a ` mark.",
      "So I'm essentially inserting \\0 here.");
}

help_big_num ()
{
  help2("I can only go up to 2147483647='17777777777=\"7FFFFFFF,",
      "so I'm using that number instead of yours.");
}

help_missing_number ()
{
  help3("A number should have been here; I inserted `0'.",
      "(If you can't figure out why I needed to see a number,",
      "look up `weird error' in the index to The TeXbook.)");
}

help_filll ()
{
  help1("I dddon't go any higher than filll.");
}

help_mu ()
{
  help4("The unit of measurement in math glue must be mu.",
      "To recover gracefully from this error, it's best to",
      "delete the erroneous units; e.g., type `2' to delete",
      "two letters. (See Chapter 27 of The TeXbook.)");
}

help_mu_error ()
{
  help1("I'm going to assume that 1mu=1pt when they're mixed.");
}

help_dimen ()
{
  help6("Dimensions can be in units of em, ex, in, pt, pc,",
      "cm, mm, dd, cc, bp, or sp; but yours is a new one!",
      "I'll assume that you meant to say pt, for printer's points.",
      "To recover gracefully from this error, it's best to",
      "delete the erroneous units; e.g., type `2' to delete",
      "two letters. (See Chapter 27 of The TeXbook.)");
}

help_big_dimen ()
{
  help2("I can't work with sizes bigger than about 19 feet.",
      "Continue and I'll use the largest value I can.");
}

help_thee ()
{
  help1("I'm forgetting what you said and using zero instead.");
}

help_left ()
{
  help4("A left brace was mandatory here, so I've put one in.",
      "You might want to delete and/or insert some corrections",
      "so that I will find a matching right brace soon.",
      "(If you're confused by all this, try typing `I}' now.)")
}

help_aux ()
{
  help4("You can refer to \\spacefactor only in horizontal mode;",
      "you can refer to \\prevdepth only in vertical mode; and",
      "neither of these is meaningful inside \\write. So",
      "I'm forgetting what you said and using zero instead.");
}

help_del ()
{
  help2("A numeric delimiter code must be between 0 and 2^{27}-1.",
      "I changed this one to zero.");
}

help_reg ()
{
  help2("A register number must be between 0 and 255.",
      "I changed this one to zero.");
}

/*
 * Common TeX:str.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX str.c 3.2@(#)";
#endif

#include "tex.h"

str	null_str;
str	ASCII[256];

str	cur_str;
str	cur_str_end;
str	cur_str_ptr;

  str
make_str ()
{
  int	n;
  str	s;

  n = cur_length();
  s = new_str(n);
  memcpy(s, cur_str, n);
  cur_str_ptr = cur_str;

  return s;
}

  void
str_room (i)
  int	i;
{
  if (cur_str_ptr + i >= cur_str_end)
    overflow("str pool", STR_SIZE);
}

  str
new_str (i)
  int	i;
{
  str	s;

  s = (str) malloc(i+1);
  if (s == (str) 0)
    overflow("new str", i);
  s[i] = NUL;

  return	s;
}

  void
free_str (s)
  str	s;
{
  free(s);
}

  void
_str_init ()
{
}

  void
_str_init_once ()
{
  int 	k, l;

#define app_lc_hex(L) append_char(((L)<10) ? (L)+'0' : (L)-10+'a')

  cur_str = new_str(STR_SIZE);
  cur_str_end = cur_str + STR_SIZE;
  cur_str_ptr = cur_str;
  for (k = 0; k <= 255; incr(k)) {
    if (k < ' ' || k > '~') {
      append_char('^');
      append_char('^');
      if (k < 0100) {
        append_char(k + 0100);
      } else if (k < 0200) {
        append_char(k - 0100);
      } else {
        app_lc_hex(k / 16);
        app_lc_hex(k % 16);
      }
    } else {
      append_char(k);
    }
    ASCII[k] = make_str();
  }
  null_str = "";
}

/*
 * Common TeX:sym.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX sym.c 3.2@(#)";
#endif

#include "tex.h"

sym	cur_cs;
sym	warning_cs;

sym	par_cs;
sym	par_shape_cs;
sym	undefined_cs;
sym	null_cs;

#define HASH_SIZE	1231

sym	*active_base;
sym	*single_base;
sym	*hash_base;
sym	*frozen_cs_base;
sym	*font_id_base;

reg	*box_reg;
reg	*mu_skip_reg;
reg	*skip_reg;
reg	*dimen_reg;
reg	*int_reg;
reg	*fnt_reg;
reg	*toks_reg;
reg	*cat_code_reg;
reg	*lc_code_reg;
reg	*uc_code_reg;
reg	*sf_code_reg;
reg	*math_code_reg;
reg	*del_code_reg;


ptr	hold_head;
ptr	zero_glue;
ptr	fil_glue;
ptr 	fill_glue;
ptr	ss_glue;
ptr	fil_neg_glue;

bool	no_new_control_sequence;

#ifdef STAT
int	cs_count;
#endif

int	cur_level;
int	cur_group;

#define SAVE_SIZE	1000
#define SAVE_INC	300

int	nsaves;
reg	save_stack;
reg	save_end;
reg	save_ptr;
reg	max_save_stack;

  sym
id_lookup (s, l)
  str	s;
  int	l;
{
  int	h;
  int	k;
  sym	c;
  str	t;

  t = s;
  h = *t++;
  k = l - 1;
  while (k--) {
    h = h + h + *t++;
    while (h >= HASH_SIZE)
      h -= HASH_SIZE;
  }
  for (c = hash_base[h]; c; c = hash(c)) {
    if (length(c) == l && str_eqn(text(c), s, l))
      return c;
    if (hash(c) == null_sym) {
      if (no_new_control_sequence)
        return undefined_cs;
      k = l;
      str_room(k);
      while (k--)
        append_char(*s++);
      c = hash(c) = new_sym(make_str(), l);
#ifdef STAT
      incr(cs_count);
#endif
      return c;
    }
  }
  k = l;
  str_room(k);
  while (k--)
    append_char(*s++);
#ifdef STAT
  incr(cs_count);
#endif
  return (hash_base[h] = new_sym(make_str(), l));
}

  bool
isactive (s)
  sym	s;
{
  return (active_base[0] <= s && s <= active_base[255]);
}

  bool
issingle (s)
  sym	s;
{
  return (single_base[0] <= s && s <= single_base[255]);
}

  bool
isfrozen (s)
  sym	s;
{
  int	i;

  for (i = 1; i < FROZEN_CONTROLS; incr(i))
    if (s == FROZEN_CONTROL_SEQUENCE[i])
      return 1;
  return 0;
}

  sym
new_sym (s, l)
  str	s;
  int	l;
{
  sym	c;

  c = (sym) new_node(sizeof(sym_t));
  eq_type(c) = UNDEFINED_CS;
  eq_level(c) = LEVEL_ONE;
  equiv(c) = 0;
  text(c) = s;
  length(c) = l;
  hash(c) = null_sym;

  return  c;
}

  void
print_cs (s)
  sym	s;
{	
  str 	t;
  int	i;

  if (s == null_cs) {
    print_esc("csname");
    print_esc("endcsname");
  } else if (isactive(s)) {
    print_ASCII(*text(s));
  } else if (issingle(s)) {
    print_esc("");
    print_ASCII(*text(s));
    if (cat_code(*text(s)) == LETTER)
      print(" ");
  } else {
    t = text(s);
    print_esc(null_str);
    for (i = 0; i < length(s); incr(i))
      print_ASCII(t[i]);
    print(" ");
  }
}

  void
sprint_cs (s)
  sym	s;
{
  str 	t;
  int	i;

  if (s == null_cs) {
    print_esc("csname");
    print_esc("endcsname");
  } else if (isactive(s)) {
    print_ASCII(*text(s));
  } else if (issingle(s)) {
    print_esc("");
    print_ASCII(*text(s));
  } else {
    t = text(s);
    print_esc(null_str);
    for (i = 0; i < length(s); incr(i))
      print_ASCII(t[i]);
  }
}

  sym
primitive (s, code, order)
  str	s;
  int	code;
  int	order;
{
  int	l;

  if (s[1] == NUL) {
    cur_cs = single_base[s[0]];
  } else {
    l = str_length(s);
    cur_cs = id_lookup(s, l);
  }
  eq_level(cur_cs) = LEVEL_ONE;
  eq_type(cur_cs) = code;
  equiv(cur_cs) = order;

  return cur_cs;
}

  sym
freeze (s)
  sym	s;
{
  sym	t;

  t = new_sym(text(s), length(s));
  eq_level(t) = eq_level(s);
  eq_type(t) = eq_type(s);
  equiv(t) = equiv(s);

  return t;
}

#define check_full_save_stack() \
{	if (save_ptr > max_save_stack) { \
  if (max_save_stack > save_end - 6 \
      && !realloc_save_stack()) \
  overflow("save size", nsaves); \
  max_save_stack = save_ptr; \
                                 } \
}

  bool
realloc_save_stack ()
{
  reg	rtmp;
  int	ntmp;

  nsaves += SAVE_INC;
  ntmp = save_ptr - save_stack;
  rtmp = (reg)realloc(save_stack, nsaves * sizeof(mcell));
  if (rtmp == (reg) 0) {
    overflow("save stack", nsaves);
  }
  save_stack = rtmp;
  save_end = save_stack + nsaves - 1;
  save_ptr = save_stack + ntmp;
}

  void
new_save_level (c)
  int	c;
{
  check_full_save_stack();
  save_type(save_ptr) = LEVEL_BOUNDARY;
  save_level(save_ptr) = cur_group;
  if (cur_level == MAX_QUARTERWORD)
    overflow("grouping levels", MAX_QUARTERWORD - MIN_QUARTERWORD);
  cur_group = c;
  incr(cur_level);
  incr(save_ptr);
}

  void
reg_destroy (r)
  reg	r;
{
  ptr	p;

  switch (reg_type(r))
  {
    case SET_SHAPE:
      p = reg_equiv(r);
      if (p != null)
        free_node(p, sizeof(mcell) + info(p) * sizeof(shape_t));
      break;

    case CALL:
    case LONG_CALL:
    case LONG_OUTER_CALL:
      delete_token_ref(reg_equiv(r));
      break;

    case SKIP_REG:
    case MU_SKIP_REG:
      delete_glue_ref(reg_equiv(r));
      break;

    case BOX_REG:
      flush_node_list(reg_equiv(r));
      break;

    case TOKS_REG:
      if (reg_equiv(r) != null)
        delete_token_ref(reg_equiv(r));
      break;
  }
}

  void
reg_save (r, l)
  reg	r;
  int	l;
{
  check_full_save_stack();
  *save_ptr++ = *r;
  save_type(save_ptr) = RESTORE_OLD_VALUE;
  save_level(save_ptr) = l;
  save_index(save_ptr) = (int) r;
  incr(save_ptr);
}

  void
eq_define (s, t, e)
  sym	s;
  int	t;
  int	e;
{
  if (eq_level(s) == cur_level)
    reg_destroy(sym2reg(s));
  else if (cur_level > LEVEL_ONE)
    reg_save(sym2reg(s), eq_level(s));
  eq_level(s) = cur_level;
  eq_type(s) = t;
  equiv(s) = e;
}

  void
reg_define (r, t, e)
  reg	r;
  int	t;
  int	e;
{
  if (reg_level(r) == cur_level)
    reg_destroy(r);
  else if (cur_level > LEVEL_ONE)
    reg_save(r, reg_level(r));
  reg_level(r) = cur_level;
  reg_type(r) = t;
  reg_equiv(r) = e;
}

  void
eq_gdefine (s, t, e)
  sym	s;
  int	t;
  int	e;
{
  reg_destroy(sym2reg(s));
  eq_level(s) = LEVEL_ONE;
  eq_type(s) = t;
  equiv(s) = e;
}

  void
reg_gdefine (r, t, e)
  reg	r;
  int	t;
  int	e;
{
  reg_destroy(r);
  reg_level(r) = LEVEL_ONE;
  reg_type(r) = t;
  reg_equiv(r) = e;
}

  void
save_for_after (t)
  tok	t;
{
  if (cur_level > LEVEL_ONE) {
    check_full_save_stack();
    save_type(save_ptr) = INSERT_TOKEN;
    save_level(save_ptr) = LEVEL_ZERO;
    save_index(save_ptr) = t;
    incr(save_ptr);
  }
}

  void
unsave ()
{
  reg	r;
  tok	t;

  if (cur_level > LEVEL_ONE) {
    decr(cur_level);
    loop {
      decr(save_ptr);
      if (save_type(save_ptr) == LEVEL_BOUNDARY)	
        break;
      r = (reg) save_index(save_ptr);
      if (save_type(save_ptr) == INSERT_TOKEN) {
        t = cur_tok;
        cur_tok = (tok) r;
        back_input();
        cur_tok = t;
      } else if (save_type(save_ptr) == RESTORE_OLD_VALUE) {
        decr(save_ptr);
        if (reg_level(r) == LEVEL_ONE) {
          reg_destroy(save_ptr);
          if (tracing_restores > 0)
            restore_trace(r, "retaining");
        } else {
          reg_destroy(r);
          *r = *save_ptr;
          if (tracing_restores > 0)
            restore_trace(r, "restoring");
        }
      }
    }
    cur_group = save_level(save_ptr);
  } else {
    confusion("curlevel");
  }
}

  void
off_save ()
{	
  ptr	h;
  ptr	p;

  if (cur_group == BOTTOM_LEVEL) {
    print_err("Extra ");
    print_cmd_chr(cur_cmd, cur_chr);
    help_offsave_xtra();
    error();
  } else {
    back_input();
    h = p = new_token();
    print_err("Missing ");
    switch (cur_group) 
    {
      case SEMI_SIMPLE_GROUP:
        token(p) = sym2tok(FROZEN_END_GROUP);
        print_esc("endgroup");
        break;

      case MATH_SHIFT_GROUP:
        token(p) = MATH_SHIFT_TOKEN + '$';
        print("$");
        break;

      case MATH_LEFT_GROUP:
        token(p) = sym2tok(FROZEN_RIGHT);
        p = token_link(p) = new_token();
        token(p) = OTHER_TOKEN + '.';
        print_esc("right.");
        break;

      default:
        token(p) = RIGHT_BRACE_TOKEN + '}';
        print("}");
        break;
    }
    print(" inserted");
    ins_list(h);
    help_offsave_missing();
    error();
  }
}

  void
restore_trace (r, s)
  reg	r;
  str	s;
{
  begin_diagnostic();
  print("{");
  print(s);
  print(" ");
  show_reg(r);
  print("}");
  end_diagnostic(FALSE);
}

  void
show_reg (r)
  reg	r;
{
  int	n;

  if (reg_type(r) < BOX_REG) {
    sym s = (sym) r;
    sprint_cs(s);
    print("=");
    if (s == par_shape_cs) {
      if (par_shape_ptr == null) {
        print("0");
      } else {
        print_int(info(par_shape_ptr));
      }
    } else {
      print_cmd_chr(eq_type(s), equiv(s));
      if (eq_type(s) >= CALL) {
        print(":");
        show_token_list(token_link(equiv(s)),
            null, 32);
      }
    }
    return;
  }
  switch (reg_type(r))
  {
    case BOX_REG:
      print_esc("box");
      print_int(r - box_reg[0]);
      print("=");
      if (reg_equiv(r) == null) {
        print("void");
      } else {
        depth_threshold = 0;
        breadth_max = 1;
        show_node_list(reg_equiv(r));
      }
      break;

    case MU_SKIP_REG:
      n = r - mu_skip_reg[0];
      if (n < MU_GLUE_PARS) {
        print_mu_skip_param(n);
        print("=");
        print_spec(reg_equiv(r), "mu");
      } else {
        print_esc("muskip");
        print_int(n - MU_GLUE_PARS);
        print("=");
        print_spec(reg_equiv(r), "mu");
      }
      break;

    case SKIP_REG:
      n = r - skip_reg[0];
      if (n < GLUE_PARS) {
        print_skip_param(n);
        print("=");
        print_spec(reg_equiv(r), "pt");
      } else {
        print_esc("skip");
        print_int(n - GLUE_PARS);
        print("=");
        print_spec(reg_equiv(r), "pt");
      }
      break;

    case DIMEN_REG:
      n = r - dimen_reg[0];
      if (n < DIMEN_PARS) {
        print_length_param(n);
      } else {
        print_esc("dimen");
        print_int(n - DIMEN_PARS);
      }
      print("=");
      print_scaled(reg_equiv(r));
      print("pt");
      break;

    case INT_REG:
      n = r - int_reg[0];
      if (n < INT_PARS) {
        print_param(n);
      } else {
        print_esc("count");
        print_int(n - INT_PARS);
      }
      print("=");
      print_int(reg_equiv(r));
      break;

    case TOKS_REG:
      n = r - toks_reg[0];
      if (n < TOK_PARS) {
        print_toks_param(n);
      } else {
        print_esc("toks");
        print_int(n = TOK_PARS);
      }
      print("=");
      if (reg_equiv(r) != null)
        show_token_list(token_link(reg_equiv(r)),
            null, 32);
      break;

    case FNT_REG:
      n = r - fnt_reg[0];
      if (n == 0) {
        print("current font");
      } else if (n <= 16) {
        print_esc("textfont");
        print_int(n - 1);
      } else if (n <= 32) {
        print_esc("scriptfont");
        print_int(n - 17);
      } else {
        print_esc("scriptscriptfont");
        print_int(n - 33);
      }
      print("=");
      sprint_cs(font_id(reg_equiv(r)));
      break;

    case CAT_CODE_REG:
      print_esc("catcode");
      print_int(r - cat_code_reg[0]);
      print("=");
      print_int(reg_equiv(r));
      break;

    case LC_CODE_REG:
      print_esc("lccode");
      print_int(r - lc_code_reg[0]);
      print("=");
      print_int(reg_equiv(r));
      break;

    case UC_CODE_REG:
      print_esc("uccode");
      print_int(r - uc_code_reg[0]);
      print("=");
      print_int(reg_equiv(r));
      break;

    case SF_CODE_REG:
      print_esc("sfcode");
      print_int(r - sf_code_reg[0]);
      print("=");
      print_int(reg_equiv(r));
      break;

    case MATH_CODE_REG:
      print_esc("mathcode");
      print_int(r - math_code_reg[0]);
      print("=");
      print_int(reg_equiv(r));
      break;

    case DEL_CODE_REG:
      print_esc("delcode");
      print_int(r - del_code_reg[0]);
      print("=");
      print_int(reg_equiv(r));
      break;

    default:
      print("?");
      break;
  }
}

  sym *
sym_alloc (i)
  int	i;
{
  sym	*s;

  s = (sym *)malloc(i * sizeof(sym));
  if (s == (sym *) 0)
    overflow("sym alloc", i);
  while (i--)
    s[i] = null_sym;

  return	s;
}

  reg *
reg_alloc (i)
  int	i;
{
  int	ii;
  reg	r;
  reg	*p;

  p = (reg *)malloc(i * sizeof(reg));
  if (p == (reg *) 0)
    overflow("reg alloc", i);
  r = (reg) malloc(i * sizeof(reg_t));
  if (r == (reg) 0)
    overflow("reg alloc", i);
  for (ii = 0; ii < i; ii++)
    p[ii] = r++;

  return	p;
}

  void
_sym_init ()
{
  cur_level = LEVEL_ONE;
  cur_group = BOTTOM_LEVEL;
  no_new_control_sequence = TRUE;
  max_save_stack = save_ptr = save_stack;
  cur_cs = null_sym;
  warning_cs = null_sym;
}

  void
_sym_init_once ()
{
  int	k;
  sym	s;

  nsaves = SAVE_SIZE;
  save_stack = (reg)malloc(nsaves * sizeof(mcell));
  if (save_stack == (reg) 0)
    overflow("save stack", nsaves);
  save_end = save_stack + nsaves - 1;
#ifdef STAT
  cs_count = 0;
#endif

  active_base=sym_alloc(256);
  single_base=sym_alloc(256);
  hash_base=sym_alloc(HASH_SIZE);
  frozen_cs_base=sym_alloc(FROZEN_CONTROLS);
  font_id_base=sym_alloc(FONT_MAX);
  box_reg=reg_alloc(256);
  mu_skip_reg=reg_alloc(MU_GLUE_PARS + 256);
  skip_reg=reg_alloc(GLUE_PARS + 256);
  dimen_reg=reg_alloc(DIMEN_PARS + 256);
  int_reg=reg_alloc(INT_PARS + 256);
  toks_reg=reg_alloc(TOK_PARS + 256);
  cat_code_reg=reg_alloc(256);
  lc_code_reg=reg_alloc(256);
  uc_code_reg=reg_alloc(256);
  sf_code_reg=reg_alloc(256);
  math_code_reg=reg_alloc(256);
  del_code_reg=reg_alloc(256);
  fnt_reg=reg_alloc(49);
  hold_head=new_avail();

  zero_glue=new_node(GLUE_SPEC_SIZE);
  glue_ref_count(zero_glue) = 0;
  glue_width(zero_glue) = 0;
  stretch(zero_glue) = 0;
  stretch_order(zero_glue) = NORMAL;
  shrink(zero_glue) = 0;
  shrink_order(zero_glue) = NORMAL;

  fil_glue=new_spec(zero_glue);
  stretch(fil_glue) = UNITY;
  stretch_order(fil_glue) = FIL;

  fill_glue=new_spec(zero_glue);
  stretch(fill_glue) = UNITY;
  stretch_order(fill_glue) = FILL;

  ss_glue=new_spec(zero_glue);
  stretch(ss_glue) = UNITY;
  stretch_order(ss_glue) = FIL;
  shrink(ss_glue) = UNITY;
  shrink_order(ss_glue) = FIL;

  fil_neg_glue=new_spec(zero_glue);
  stretch(fil_neg_glue) = -UNITY;
  stretch_order(fil_neg_glue) = FIL;

  for (k = 0; k <= 48; incr(k)) {
    reg_type(fnt_reg[k]) = FNT_REG;
    reg_level(fnt_reg[k]) = LEVEL_ONE;
    reg_equiv(fnt_reg[k]) = null_font;
  }
  undefined_cs = new_sym("undefined", 9);
  null_cs = new_sym("",0);
  s = (sym)malloc(256 * sizeof(sym_t));
  if (s == (sym) 0)
    overflow("active cs alloc", 256);
  for (k = 0; k < 256; incr(k), incr(s)) {
    active_base[k] = s;
    hash(s) = null_sym;
    append_char(k);
    text(s) = make_str();
    length(s) = 1;
    eq_type(s) = UNDEFINED_CS;
    eq_level(s) = LEVEL_ONE;
    equiv(s) = 0;
  }
  s = (sym)malloc(256 * sizeof(sym_t));
  if (s == (sym) 0)
    overflow("single cs alloc", 256);
  for (k = 0; k < 256; incr(k), incr(s)) {
    single_base[k] = s;
    hash(s) = null_sym;
    append_char(k);
    text(s) = make_str();
    length(s) = 1;
    eq_type(s) = UNDEFINED_CS;
    eq_level(s) = LEVEL_ZERO;
    equiv(s) = 0;
  }
  s = (sym)malloc(FONT_MAX * sizeof(sym_t));
  if (s == (sym) 0)
    overflow("font id cs alloc", FONT_MAX);
  for (k = 0; k < FONT_MAX; incr(k), incr(s)) {
    font_id_base[k] = s;
    hash(s) = null_sym;
    text(s) = null_str;
    length(s) = 0;
    eq_type(s) = UNDEFINED_CS;
    eq_level(s) = LEVEL_ZERO;
  }
  for (k = 0; k < TOK_PARS + 256; incr(k)) {
    reg_type(toks_reg[k]) = TOKS_REG;
    reg_level(toks_reg[k]) = LEVEL_ZERO;
    reg_equiv(toks_reg[k]) = null;
  }
  for (k = 0; k < MU_GLUE_PARS + 256; incr(k)) {
    reg_type(mu_skip_reg[k]) = MU_SKIP_REG;
    reg_level(mu_skip_reg[k]) = LEVEL_ONE;
    reg_equiv(mu_skip_reg[k]) = zero_glue;
  }
  for (k = 0; k < GLUE_PARS + 256; incr(k)) {
    reg_type(skip_reg[k]) = SKIP_REG;
    reg_level(skip_reg[k]) = LEVEL_ONE;
    reg_equiv(skip_reg[k]) = zero_glue;
  }
  glue_ref_count(zero_glue) += 512 + MU_GLUE_PARS + GLUE_PARS;
  for (k = 0; k < INT_PARS + 256; incr(k)) {
    reg_type(int_reg[k]) = INT_REG;
    reg_level(int_reg[k]) = LEVEL_ONE;
    reg_equiv(int_reg[k]) = 0;
  }
  mag = 1000;
  tolerance = 10000;
  hang_after = 1;
  max_dead_cycles = 25;
  escape_char = '\\';
  end_line_char = CARRIAGE_RETURN;
  for (k = 0; k < DIMEN_PARS + 256; incr(k)) {
    reg_type(dimen_reg[k]) = DIMEN_REG;
    reg_level(dimen_reg[k]) = LEVEL_ONE;
    reg_equiv(dimen_reg[k]) = 0;
  }
  for (k = 0; k < 256; incr(k)) {
    reg_type(box_reg[k]) = BOX_REG;
    reg_level(box_reg[k]) = LEVEL_ONE;
    reg_equiv(box_reg[k]) = null;
  }	
  for (k = 0; k < 256; incr(k)) {
    reg_type(cat_code_reg[k]) = CAT_CODE_REG;
    reg_level(cat_code_reg[k]) = LEVEL_ONE;
    reg_equiv(cat_code_reg[k]) = OTHER_CHAR;
    reg_type(lc_code_reg[k]) = LC_CODE_REG;
    reg_level(lc_code_reg[k]) = LEVEL_ONE;
    reg_equiv(lc_code_reg[k]) = 0;
    reg_type(uc_code_reg[k]) = UC_CODE_REG;
    reg_level(uc_code_reg[k]) = LEVEL_ONE;
    reg_equiv(uc_code_reg[k]) = 0;
    reg_type(sf_code_reg[k]) = SF_CODE_REG;
    reg_level(sf_code_reg[k]) = LEVEL_ONE;
    reg_equiv(sf_code_reg[k]) = 1000;
    reg_type(math_code_reg[k]) = MATH_CODE_REG;
    reg_level(math_code_reg[k]) = LEVEL_ONE;
    reg_equiv(math_code_reg[k]) = k;
    reg_type(del_code_reg[k]) = DEL_CODE_REG;
    reg_level(del_code_reg[k]) = LEVEL_ONE;
    reg_equiv(del_code_reg[k]) = -1;
  }
  cat_code(CARRIAGE_RETURN) = CAR_RET;
  cat_code(' ') = SPACER;
  cat_code('^') = SUP_MARK;
  cat_code('\\') = ESCAPE;
  cat_code('%') = COMMENT;
  cat_code(INVALID_CODE) = INVALID_CHAR;
  cat_code(NULL_CODE) = IGNORE;
  for (k = '0'; k <= '9'; incr(k))
    math_code(k) = k + VAR_CODE;
  for (k = 'A'; k <= 'Z'; incr(k)) {
    cat_code(k) = cat_code(k + 'a' - 'A') = LETTER;
    math_code(k) = k + VAR_CODE + 0x100;
    math_code(k + 'a' - 'A') = k + 'a' - 'A'+ VAR_CODE + 0x100;
    lc_code(k) = lc_code(k + 'a' - 'A') = k + 'a' - 'A';
    uc_code(k) = uc_code(k + 'a' - 'A') = k;
    sf_code(k) = 999;
  }
  del_code('.') = 0;
}

/*
 **	Help text
 */

help_offsave_xtra ()
{	
  help1("Things are pretty mixed up, but I think the worst is over.");
}

help_offsave_missing ()
{
  help5("I've inserted something that you may have forgotten.",
      "(See the <inserted text> above.)",
      "With luck, this will get me unwedged. But if you",
      "really didn't forget anything, try typing `2' now; then",
      "my insertion and my current dilemma will both disappear.");
}

/*
 * Common TeX:tex.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX tex.c 3.2@(#)";
#endif

#include "tex.h"

char	*banner = "This is Common TeX, Version 3.2";
str	format_ident;
int	ready_already;

main (argc, argv)
  int	argc;
  str 	argv[];
{
  signal(SIGINT, handle_int);
  set_paths();
  if (ready_already != 314159) 
    initialize_once();
  ready_already = 314159;
  initialize();
  if (!(decode_args(argc, argv) || init_terminal()))
    exit(history);
  if (end_line_char_active)
    *++limit = end_line_char;
  if (interaction == BATCH_MODE)
    selector = NO_PRINT;
  else selector = TERM_ONLY;
  fix_date_and_time();
  if (next <= limit && cat_code(*next) != ESCAPE)
    start_input();
  history = SPOTLESS;
  main_control();
  final_cleanup();
  /*NOTREACHED*/
}

#define USAGE	"usage: %s [ TeX string ]\n"

  bool
decode_args (argc, argv)
  int	argc;
  str	argv[];
{
  int	i, j;

  if (argc == 1)
    return FALSE;
  for (i = 1; i < argc; i++) {
    j = str_length(argv[i]);
    argv[i][j] = ' ';
    if (limit + j + 1 >= buffer + BUF_SIZE)
      return FALSE;
    (void) strncpy(limit + 1, argv[i], j + 1);
    limit += j + 1;
    argv[i][j] = '\0';
  }
  return TRUE;
}

  void
fix_date_and_time ()
{
  long	clock, begintime();
  struct tm	*tmptr, *localtime();

  clock = begintime();
  tmptr = localtime(&clock);
  time = 60 * tmptr->tm_hour + tmptr->tm_min;
  day = tmptr->tm_mday;
  month = tmptr->tm_mon + 1;
  year = tmptr->tm_year + 1900;
}

#undef time
  long
begintime()
{
  return (time((char *) 0));
}

  void
handle_int ()
{	
  signal(SIGINT, handle_int);
  interrupted = 1;
}

  void
initialize ()
{
  _alloc_init();
  _chr_init();
  _str_init();
  _arith_init();
  _error_init();
  _sym_init();
  _def_init();
  _scan_init();
  _expand_init();
  _fileio_init();
  _cond_init();
  _toklist_init();
  _eval_init();
  _box_init();
  _boxlist_init();
  _math_init();
  _mathlist_init();
  _hyph_init();
  _pack_init();
  _page_init();
  _par_init();
  _mlst_hlst_init();
  _align_init();
  _dvi_init();
  _cmd_init();
  _tfm_init();
  _texext_init();
  _tex_init();
  _print_init();
  _tok_init();
}

  void
initialize_once ()
{
  _chr_init_once();
  _print_init_once();
  _error_init_once();
  _alloc_init_once();
  _str_init_once();
  _arith_init_once();
  _sym_init_once();
  _tok_init_once();
  _def_init_once();
  _scan_init_once();
  _expand_init_once();
  _fileio_init_once();
  _cond_init_once();
  _toklist_init_once();
  _eval_init_once();
  _box_init_once();
  _boxlist_init_once();
  _math_init_once();
  _mathlist_init_once();
  _hyph_init_once();
  _pack_init_once();
  _page_init_once();
  _par_init_once();
  _mlst_hlst_init_once();
  _align_init_once();
  _dvi_init_once();
  _cmd_init_once();
  _tfm_init_once();
  _texext_init_once();
  _tex_init_once();
}

  void
final_cleanup ()
{
  bool dump = cur_chr == 1;

  if (job_name == null_str) {
    open_log_file();
  }
  while (open_parens > 0) {
    print(" )");
    decr(open_parens);
  }
  if (cur_level > LEVEL_ONE) {
    print_nl("(");
    print_esc("end occurred ");
    print("inside a group at level ");
    print_int(cur_level - LEVEL_ONE);
    print(")");
  }
  while (cond_ptr != null) {
    print_nl("(");
    print_esc("end occurred ");
    print("when ");
    print_cmd_chr(IF_TEST, cur_if);
    if (if_line != 0) {
      print(" on line ");
      print_int(if_line);
    }
    print(" was incomplete)");
    if_line = if_line_field(cond_ptr); 
    cur_if = subtype(cond_ptr);
    cond_ptr = link(cond_ptr);
  }
  if (history != SPOTLESS
      && (history == WARNING_ISSUED || interaction < ERROR_STOP_MODE)
      && selector == TERM_AND_LOG) {
    selector = TERM_ONLY;
    print_nl("(see the transcript file for additional information)");
    selector = TERM_AND_LOG;
  }
  close_files_and_terminate(FALSE, dump);
}

  void
close_files_and_terminate (edit, dump)
  bool	edit;
  bool	dump;
{
  int	k;

  for (k = 0; k < 16; incr(k))
    if (write_open[k])
      a_close(write_file[k]);
  wake_up_terminal();
  fin_dvi();
  if (log_name != null_str) {
    wlog_cr();
    a_close(log_file);
    selector -= 2;
    if (selector == TERM_ONLY) {
      print_nl("Transcript written on ");
      print(log_name);
      print(".");
    }
  }
  print_ln();
  if (edit) {
    call_edit();
  }
  fflush(stdout);
  fflush(stderr);
  if (dump) {
    if (trie_not_ready)
      init_trie();
    kill(getpid(), SIGQUIT);
  }
  exit(history);
}

  void
call_edit ()
{
  str	envedit;
  char	edit[STR_SIZE + 17];
  str	texedit = "vi +%d %s";
  str	getenv();

  if ((envedit = getenv("TEXEDIT")) != null)
    texedit = envedit;
  if (!one("%d", texedit) || !one("%s", texedit)) {
    print_err("Edit format consists of 1 filename and 1 linenumber");
    return;
  }
  sprintf(edit, texedit, line, name);
  system(edit);
}

one (s, t)
  str	s;
  str	t;
{	
  int	i;
  int	j;
  int	one;

  one = 0;
  j = strlen(t) - 1;
  for (i = 0; i < j; incr(i))
    if (strncmp(s, &t[i], 2) == 0)
      if (++one > 1)
        return FALSE;
  if (one == 0)
    return FALSE;
  return TRUE;
}

  void
_tex_init ()
{
}

  void
_tex_init_once ()
{
  format_ident = " (INITEX)";
}

/*
 * Common TeX:texext.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX texext.c 3.2@(#)";
#endif

#include "tex.h"

file	write_file[16];
bool	write_open[18];

sym	write_cs;

  void
do_extension ()
{
  switch (cur_chr) {
    case OPEN_NODE: do_open(); break;
    case WRITE_NODE: do_write(); break;
    case CLOSE_NODE: do_close(); break;
    case SPECIAL_NODE: do_special(); break;
    case IMMEDIATE_CODE: do_immediate(); break;
    case SET_LANGUAGE_CODE: do_set_language(); break;
    default: confusion("ext1");
  }
}

  void
do_immediate ()
{
  ptr	p;

  get_x_token();
  if (cur_cmd == EXTENSION && cur_chr <= CLOSE_NODE) {
    p = tail;
    do_extension();
    out_whatsit(tail);
    flush_node_list(tail);
    tail = p;
    link(p) = null;
  } else {
    back_input();
  }
}

  void
show_whatsit (p)
  ptr	p;
{
  switch (subtype(p)) {
    case OPEN_NODE: show_open(p); break;
    case WRITE_NODE: show_write(p); break;
    case CLOSE_NODE: show_close(p); break;
    case SPECIAL_NODE: show_special(p); break;
    case LANGUAGE_NODE: show_language(p); break;
    default:print("whatsit?");break;
  }
}

  ptr
copy_whatsit (p)
  ptr	p;
{
  switch (subtype(p)) {
    case OPEN_NODE: return(copy_open(p));
    case WRITE_NODE: return(copy_write(p));
    case CLOSE_NODE: return(copy_close(p));
    case SPECIAL_NODE: return(copy_special(p));
    case LANGUAGE_NODE: return(copy_language(p)); break;
    default:confusion("ext2");
  }
  /*NOTREACHED*/
}

  void
free_whatsit (p)
  ptr	p;
{
  switch (subtype(p)) {
    case OPEN_NODE: free_open(p); break;
    case WRITE_NODE: free_write(p); break;
    case CLOSE_NODE: free_close(p); break;
    case SPECIAL_NODE: free_special(p); break;
    case LANGUAGE_NODE: free_language(p); break;
    default:confusion("ext3");
  }
}

#define adv_past(P) \
{	if (subtype(P) == LANGUAGE_NODE) { \
  cur_lang = what_lang(P); \
  l_hyf = what_lhm(P); \
  r_hyf = what_rhm(P); \
                                   } \
}

  void
line_break_whatsit (p)
  ptr	p;
{
  adv_past(p);
}

  void
try_hyph_whatsit (p)
  ptr	p;
{
  adv_past(p);
}

  void
out_whatsit (p)
  ptr	p;
{
  switch (subtype(p)) {
    case OPEN_NODE: out_open(p); break;
    case WRITE_NODE: out_write(p); break;
    case CLOSE_NODE: out_close(p); break;
    case SPECIAL_NODE: out_special(p); break;
    case LANGUAGE_NODE:  out_language(p); break;
    default: confusion("ext4");
  }
}

  void
append_whatsit (s, w)
  int	s;
  int	w;
{
  ptr	p;

  p = new_node(w);
  type(p) = WHATSIT_NODE;
  subtype(p) = s;
  tail_append(p);
}

  void
do_open ()
{
  append_whatsit(cur_chr, OPEN_NODE_SIZE);
  scan_four_bit_int();
  open_stream(tail) = cur_val;
  scan_optional_equals();
  scan_file_name();
  open_name(tail) = cur_name;
  open_area(tail) = cur_area;
  open_ext(tail) = cur_ext;
}

  void
show_open (p)
  ptr	p;
{
  print_esc("openout");
  if (open_stream(p) < 16)
    print_int(open_stream(p));
  else if (open_stream(p) == 16)
    print("*");
  else print("-");
  print("=");
  print_file_name(open_name(p), open_area(p), open_ext(p));
}

  ptr
copy_open (p)
  ptr	p;
{
  ptr	q;

  q = new_node(OPEN_NODE_SIZE);
  memcpy((void *) q, (void *) p, OPEN_NODE_SIZE);

  return q;
}

  void
free_open (p)
{
  free_node(p, OPEN_NODE_SIZE);
}

  void
out_open (p)
  ptr	p;
{
  int	j;

  if (doing_leaders)
    return;
  j = open_stream(p);
  cur_name = open_name(p);
  cur_area = open_area(p);
  cur_ext = open_ext(p);
  if (cur_ext == null_str)
    cur_ext = str_tex;
  if (cur_area == null_str)
    cur_area = job_area;
  pack_cur_name();
  if ((write_file[j] = a_open_out()) == null_file) {
    cur_area = null_str;
    pack_cur_name();
    while ((write_file[j] = a_open_out()) == null_file)
      prompt_file_name("output file name", str_tex);
  }
  write_open[j] = TRUE;
}

  void
do_write ()
{
  sym	s;

  s = cur_cs;
  append_whatsit(cur_chr, WRITE_NODE_SIZE);
  scan_int();
  if (cur_val < 0)
    cur_val = 17;
  else if (cur_val > 15)
    cur_val = 16;
  write_stream(tail) = cur_val;
  cur_cs = s;
  scan_toks(FALSE, FALSE);
  write_toks(tail) = def_ref;
}

  void
show_write (p)
  ptr	p;
{
  print_esc("write");
  if (write_stream(p) < 16)
    print_int(write_stream(p));
  else if (write_stream(p) == 16)
    print("*");
  else print("-");
  print_mark(write_toks(p));
}

  ptr
copy_write (p)
{
  ptr	q;

  q = new_node(WRITE_NODE_SIZE);
  memcpy((void *) q, (void *) p, WRITE_NODE_SIZE);
  add_token_ref(write_toks(p));

  return	q;
}

  void
free_write (p)
{
  delete_token_ref(write_toks(p));
  free_node(p, WRITE_NODE_SIZE);
}

  void
out_write (p)
  ptr	p;
{
  int	j;
  ptr	q;
  ptr	r;
  int	old_mode;
  int	old_setting;

  if (doing_leaders)
    return;
  q = new_token();
  token(q) = RIGHT_BRACE_TOKEN + '}';
  r = new_token();
  token_link(q) = r;
  token(r) = END_WRITE_TOKEN;
  ins_list(q);
  begin_token_list(write_toks(p), WRITE_TEXT);
  q = new_token();
  token(q) = LEFT_BRACE_TOKEN + '{';
  ins_list(q);
  old_mode = mode;
  mode = 0;
  cur_cs = write_cs;
  scan_toks(FALSE, TRUE);
  get_token();
  if (cur_tok != END_WRITE_TOKEN) {
    print_err("Unbalanced write command");
    help_unbal_write();
    error();
    do get_token();
    while (cur_tok != END_WRITE_TOKEN);
  }
  mode = old_mode;
  end_token_list();
  old_setting = selector;
  j = write_stream(p);
  if (write_open[j]) {
    selector = j;
  } else {
    if (j == 17 && selector == TERM_AND_LOG)
      selector = LOG_ONLY;
    print_nl(null_str);
  }
  token_show(def_ref);
  print_ln();
  flush_list(def_ref);
  selector = old_setting;
}

  void
do_close ()
{
  append_whatsit(cur_chr, CLOSE_NODE_SIZE);
  scan_int();
  if (cur_val < 0)
    cur_val = 17;
  else if (cur_val > 15)
    cur_val = 16;
  write_stream(tail) = cur_val;
}

  void
show_close (p)
  ptr	p;
{
  print_esc("closeout");
  if (write_stream(p) < 16)
    print_int(write_stream(p));
  else if (write_stream(p) == 16)
    print("*");
  else print("-");
}

  ptr
copy_close (p)
  ptr	p;
{
  ptr	q;

  q = new_node(CLOSE_NODE_SIZE);
  memcpy((void *) q, (void *) p, CLOSE_NODE_SIZE);

  return	q;
}

  void
free_close (p)
  ptr	p;
{
  free_node(p, CLOSE_NODE_SIZE);
}

  void
out_close (p)
  ptr	p;
{
  int	j;

  if (!doing_leaders) {
    j = write_stream(p);
    if (write_open[j]) {
      a_close(write_file[j]);
      write_open[j] = FALSE;
    }
  }
}

  void
do_special ()
{
  append_whatsit(SPECIAL_NODE, SPECIAL_NODE_SIZE);
  scan_toks(FALSE, TRUE);
  special_toks(tail) = def_ref;
}

  void
show_special (p)
  ptr	p;
{
  print_esc("special");
  print_mark(special_toks(p));
}

  ptr
copy_special (p)
  ptr	p;
{
  ptr	q;

  q = new_node(SPECIAL_NODE_SIZE);
  memcpy((void *)q, (void *)p, SPECIAL_NODE_SIZE);
  add_token_ref(special_toks(p));

  return q;
}

  void
free_special (p)
  ptr	p;
{
  delete_token_ref(special_toks(p));
  free_node(p, SPECIAL_NODE_SIZE);
}

  void
out_special (p)
  ptr	p;
{
  str	s;
  int	old_setting;

  synch_h();
  synch_v();
  old_setting = selector;
  selector = NEW_STRING;
  show_token_list(token_link(special_toks(p)), null, STR_SIZE);
  selector = old_setting;
  if (cur_length() < 256) {
    dvi_out(XXX1);
    dvi_out(cur_length());
  } else {
    dvi_out(XXX4);
    dvi_four(cur_length());
  }
  for (s = cur_str; s < cur_str_ptr; incr(s))
    dvi_out(*s);
  flush_str();
}

  void
do_set_language ()
{
  if (abs(mode) != HMODE) {
    report_illegal_case();
    return;
  }
  scan_int();
  if (cur_val <= 0 || cur_val > 255) {
    clang = 0;
  } else {
    clang = cur_val;
  }
  append_whatsit(LANGUAGE_NODE, LANGUAGE_NODE_SIZE);
  what_lang(tail) = clang;
  what_lhm(tail) = norm_min(left_hyphen_min);
  what_rhm(tail) = norm_min(right_hyphen_min);
}		

  void
show_language (p)
  ptr	p;
{
  print_esc("setlanguage");
  print_int(what_lang(p));
  print(" (hyphenmin ");
  print_int(what_lhm(p));
  print(",");
  print_int(what_rhm(p));
  print(")");
}

  ptr
copy_language (p)
  ptr	p;
{
  ptr	q;

  q = new_node(SMALL_NODE_SIZE);
  memcpy((void *) q, (void *) p, SMALL_NODE_SIZE);

  return q;
}

  void
free_language (p)
{
  free_node(p, SMALL_NODE_SIZE);
}

  void
out_language (p)
{
  return;
}

  void
fix_language ()
{
  int	l;

  if (language <= 0 || language > 255) {
    l = 0;
  } else {
    l = language;
  }
  if (l != clang) {
    append_whatsit(LANGUAGE_NODE, SMALL_NODE_SIZE);
    what_lang(tail) = clang = l;
    what_lhm(tail) = norm_min(left_hyphen_min);
    what_rhm(tail) = norm_min(right_hyphen_min);
  }
}

  void
_texext_init ()
{
}

  void
_texext_init_once ()
{
}

/*
 **	Help text
 */

help_unbal_write ()
{
  help2("On this page there's a \\write with fewer real {'s than }'s.",
      "I can't handle that very well; good luck.");
}

/*
 * Common TeX:tfm.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX tfm.c 3.2@(#)";
#endif

#include "tex.h"

file	tfm_file;

int	nfonts;
font_t	**font_info;
font_t	**font_end;
font_t	**font_ptr;

qcell	null_character;

#define abort() goto bad_tfm

#define start_font_error_message() \
{	print_err("Font "); sprint_cs(u); \
  print("="); print_file_name(nom, aire, null_str); \
  if (s >= 0) {print(" at "); print_scaled(s); print("pt");} \
  else if (s != -1000) {print(" scaled "); print_int(-s);} \
}

#define get_font_byte() \
  (font_byte = getc(tfm_file))

#define read_sixteen(X) \
{	get_font_byte(); X = font_byte; \
  if (X > 255) abort(); \
  get_font_byte(); X = (X) * 0400 + font_byte; \
}

#define store_four_quarters(Q) \
{	a = get_font_byte(); b = get_font_byte(); \
  c = get_font_byte(); d = get_font_byte(); \
  qw.b0 = a; qw.b1 = b; \
  qw.b2 = c; qw.b3 = d; \
  Q = qw; \
}

#define store_scaled(S) \
{	a = get_font_byte(); b = get_font_byte(); \
  c = get_font_byte(); d = get_font_byte(); \
  sw = ((((d * z) / 0400) + (c * z)) / 0400 + (b * z)) / beta; \
  if (a == 0) S = sw; \
  else if (a == 255) S = sw - alpha; \
  else abort(); \
}

#define check_byte_range(C) \
{	if (C < bc || C > ec) \
  abort(); \
}

#define check_existence(C) \
{	check_byte_range(C); \
  qw = char_info(f, C); \
  if (!char_exists(qw)) \
  abort(); \
}

  fnt
read_font_info (u, nom, aire, s)
  sym	u;
  str	nom;
  str	aire;
  scal	s;
{
  int	a, b, c, d, k;
  fnt	f, g;
  int	bc, ec, lf, lh;
  int	nw, nh, nd, ni, nk, nl, ne, np;
  qcell	qw;
  scal	sw, z;
  qcell	*fp;
  qcell	*pq;
  scal	*ps;
  int	alpha;
  int	beta;
  int	bchar;
  int	bch_label;
  bool	file_opened;
  int	font_byte;
  qcell	dev_null;
  /*
   ** 	Open tfm_file for input
   */
  g = null_font;
  file_opened = FALSE;
  pack_file_name(nom, aire, str_tfm);
  if ((tfm_file = b_open_in()) == null_file)
    abort();
  file_opened = TRUE;
  /*
   **	Read the TFM size fields
   */
  read_sixteen(lf);
  read_sixteen(lh);
  read_sixteen(bc);
  read_sixteen(ec);
  if (bc > ec + 1 || ec > 255)
    abort();
  if (bc > 255) {
    bc = 1;
    ec = 0;
  }
  read_sixteen(nw);
  read_sixteen(nh);
  read_sixteen(nd);
  read_sixteen(ni);
  read_sixteen(nl);
  read_sixteen(nk);
  read_sixteen(ne);
  read_sixteen(np);
  if (lf != 6 + lh + (ec - bc + 1) +
      nw + nh + nd + ni + nl + nk + ne + np) 
    abort();
  /*
   **	Use size fields to allocate font information
   */
  if (font_ptr == font_end) {
    start_font_error_message();
    print(" not loaded: No more room.");
    help_font();
    error();
    goto done;
  }
  k = (ec - bc + 1 + nl + ne) * sizeof(qcell) + 
    (nw + nh + nd + ni + nk + np + 1) * sizeof(scal);
  if (np < 7)
    k += (7 - np) * sizeof(scal);	
  f = null_font + font_ptr - font_info;
  *font_ptr = (font_t *)malloc(sizeof(font_t) + k);
  if (*font_ptr == (font_t *) 0) {
    start_font_error_message();
    print(" not loaded: No more room.");
    help_font();
    error();
    goto done;
  }
  font_length(f) = sizeof(font_t) + k;
  fp = (qcell *) (fmem(f) + 1);
  char_base(f) = fp - bc;
  width_base(f) = (scal *) (char_base(f) + ec + 1);
  height_base(f) = width_base(f) + nw;
  depth_base(f) = height_base(f) + nh;
  italic_base(f) = depth_base(f) + nd;
  lig_kern_base(f) = (qcell *) (italic_base(f) + ni);
  kern_base(f) = (scal *) (lig_kern_base(f) + nl);
  exten_base(f) = (qcell *) (kern_base(f) + nk);
  param_base(f) = (scal *) (exten_base(f) + ne);
  /*
   **	Read the TFM header
   */
  if (lh < 2)
    abort();
  store_four_quarters(font_check(f));
  read_sixteen(z);
  get_font_byte();
  z = z * 0400 + font_byte;
  get_font_byte();
  z = z * 020 + font_byte / 020;
  if (z < UNITY)
    abort();
  while (lh > 2) {
    store_four_quarters(dev_null);
    decr(lh);
  }
  font_dsize(f) = z;
  if (s != -1000) {
    if (s >= 0)
      z = s;
    else z = xn_over_d(z, -s, 1000L);
  }
  font_size(f) = z;
  /*
   **	Read  character data
   */
  for (pq = fp; pq < (qcell *)width_base(f); incr(pq)) {
    store_four_quarters(*pq);
    if (a >= nw || b / 020 >= nh || b % 020 >= nd || c / 4 > ni) 
      abort();
    switch (c % 4)
    {
      case LIG_TAG:
        if (d >= nl)
          abort();
        break;

      case EXT_TAG:
        if (d >= ne)
          abort();
        break;

      case LIST_TAG:
#define ccbwo ((int) (pq + bc - fp))
        check_byte_range(d);
        while (d < ccbwo) {
          qw = char_info(f, d);
          if (char_tag(qw) != LIST_TAG)
            goto not_found;
          d = rem_byte(qw);
        }
        if (d == ccbwo)
          abort();
        break;

not_found:
        break;
    }
  }
  /*
   **	Read box dimensions
   */
  alpha = 16;
  while (z >= 040000000) {
    z >>= 1;
    alpha <<= 1;
  }
  beta = 256 / alpha;
  alpha *= z;
  for (ps = width_base(f); ps < (scal *)lig_kern_base(f); incr(ps))
    store_scaled(*ps);
  if (width_base(f)[0] != 0)
    abort();
  if (height_base(f)[0] != 0)
    abort();
  if (depth_base(f)[0] != 0)
    abort();
  if (italic_base(f)[0] != 0)
    abort();
  /*
   **	Read ligature/kern programs
   */
  bch_label = 077777;
  bchar = 256;
  if (nl > 0) {
    pq = lig_kern_base(f);
    while (pq < (qcell *) kern_base(f)) {
      store_four_quarters(*pq);
      if (a > 128) {
        if (256 * c + d >= nl)
          abort();
        if (a == 255
            && pq == lig_kern_base(f)) {
          bchar = b;
        }
      } else {
        if (b != bchar) {
          check_existence(b);
        }
        if (c < 128) {
          check_existence(d);
        } else if (256 * (c - 128) + d >= nk) {
          abort();
        }
        if (a < 128
            && pq - lig_kern_base(f) + a + 1 >= nl) {
          abort();
        }
      }
      if (a == 255) {
        bch_label = 256 * c + d;
      }
      incr(pq);
    }
  }
  /*
   **	Read kern dimensions
   */
  for (ps = kern_base(f); ps < (scal *)exten_base(f); incr(ps))
    store_scaled(*ps);
  /*
   **	Read extensible character recipes
   */
  for (pq = exten_base(f); pq < (qcell *)param_base(f); incr(pq)) {
    store_four_quarters(*pq);
    if (a != 0)
      check_existence(a);
    if (b != 0)
      check_existence(b);
    if (c != 0)
      check_existence(c);
    check_existence(d);
  }
  /*
   **	Read font parameters
   */
  for (k = 1; k <= np; incr(k)) {
    if (k == 1) {
      get_font_byte();
      sw = font_byte;
      if (sw > 127)
        sw -= 256;
      get_font_byte();
      sw = sw * 0400 + font_byte;
      get_font_byte();
      sw = sw * 0400 + font_byte;
      get_font_byte();
      param(f, 0) = sw * 020 + font_byte / 020;
    } else {
      store_scaled(param(f, k - 1));
    }
  }
  if (feof(tfm_file))
    abort();

  for (k = np + 1; k <= 7; incr(k))
    param(f, k - 1) = 0;
  /*
   **	Make final adjustments and done
   */
  decr(param_base(f));
  font_params(f) = (np >= 7) ? np : 7;
  hyphen_char(f) = default_hyphen_char;
  skew_char(f) = default_skew_char;
  if (bch_label < nl) {
    bchar_label(f) = lig_kern_base(f) + bch_label;
  } else {
    bchar_label(f) = NON_ADDRESS;
  }
  font_bchar(f) = bchar;
  font_false_bchar(f) = bchar;
  if (bchar <= ec && bchar >= bc) {
    qw = char_info(f, bchar);
    if (char_exists(qw))
      font_false_bchar(f) = NON_CHAR;
  }
  font_name(f) = nom;
  font_area(f) = aire;
  font_bc(f) = bc;
  font_ec(f) = ec;
  font_glue(f) = null;
  font_used(f) = FALSE;
  incr(font_ptr);
  g = f;
  goto done;

bad_tfm:
  start_font_error_message();
  if (file_opened) {
    print(" not loadable: Bad metric (TFM) file");
  } else {
    print(" not loadable: Metric (TFM) file not found");
  }
  help_tfm();
  error();

done:
  if (file_opened)
    b_close(tfm_file);
  return g;
}

  void
scan_font_ident ()
{	
  int	m;
  fnt	f;

  get_nbx_token();
  if (cur_cmd == DEF_FONT) {
    f = cur_font;
  } else if (cur_cmd == SET_FONT) {
    f = cur_chr;
  } else if (cur_cmd == DEF_FAMILY) {
    m = cur_chr;
    scan_four_bit_int();
    f = reg_equiv(fnt_reg[1 + m + cur_val]);
  } else {
    print_err("Missing font identifier");
    help_font_cs();
    back_error();
    f = null_font;
  }
  cur_val = f;
}

  scal *
find_font_dimen (writing)
  bool	writing;
{
  fnt	f;
  int	i, l, m, n, x;
  char	*fp, *nfp;

  scan_int();
  n = cur_val;
  scan_font_ident();
  f = cur_val;
  if (n <= 0) {
    goto err;
  } else {
    if (writing &&
        n <= SPACE_SHRINK_CODE &&
        n >= SPACE_CODE &&
        font_glue(f) != null) {
      delete_glue_ref(font_glue(f));
      font_glue(f) = null;
    }
    if (n > font_params(f)) {
      if (f < null_font + font_ptr - font_info - 1) {
        goto err;
      }
      i = font_params(f);
      l = font_length(f);
      m = l + (n - font_params(f)) * sizeof(scal);
      fp = (char *)fmem(f);
      nfp = realloc(fp, m);
      x = nfp - fp;
      if (fp == (char *) 0)
        confusion("font mem");
      if (nfp == (char *) 0)
        overflow("font mem", m);
      fmem(f) = (font_t *) nfp;
      adjust_bases(f, x);
      font_length(f) = m;
      font_params(f) = n;
      while (i++ < n)
        param(f, i) = 0;
      return (param_base(f) + font_params(f));
    } else {
      return (param_base(f) + n);
    }
  }

err:
  print_nl("! Font ");
  print_esc(font_id_text(f));
  print(" has only ");
  print_int(font_params(f));
  print(" fontdimen parameters");
  help_font_param();
  error();
  return ((scal *) 0);
}

adjust_bases (f, x)
  fnt	f;
  int	x;
{
  char_base(f) = (qcell *) ((char *)char_base(f) + x);
  width_base(f) = (scal *) ((char *)width_base(f) + x);
  height_base(f) = (scal *) ((char *)height_base(f) + x);
  depth_base(f) = (scal *) ((char *)depth_base(f) + x);
  italic_base(f) = (scal *) ((char *)italic_base(f) + x);
  lig_kern_base(f) = (qcell *) ((char *)lig_kern_base(f) + x);
  bchar_label(f) = (qcell *) ((char *)bchar_label(f) + x);
  kern_base(f) = (scal *) ((char *)kern_base(f) + x);
  exten_base(f) = (qcell *) ((char *)exten_base(f) + x);
  param_base(f) = (scal *) ((char *)param_base(f) + x);
}

  ptr
find_font_glue (f)
  fnt	f;
{
  ptr	p;
  scal	*q;

  p = font_glue(f);
  if (p == null) {
    p = new_spec(zero_glue);
    q = param_base(cur_font) + SPACE_CODE;
    glue_width(p) = *q++;
    stretch(p) = *q++;
    shrink(p) = *q++; 
    font_glue(f) = p;
  }
  return p;
}

  void
char_warning (f, c)
  fnt	f;
  int	c;
{
  if (tracing_lost_chars > 0) {
    begin_diagnostic();
    print_nl("Missing character: There is no ");
    print_ASCII(c);
    print(" in font ");
    print(font_name(f));
    print("!");
    end_diagnostic(FALSE);
  }
}

  ptr
new_character (f, c)
  fnt	f;
  int	c;
{
  ptr	p;

  if (font_bc(f) <= c && font_ec(f) >= c
      && char_exists(char_info(f, c))) {
    p = new_avail();
    font(p) = f;
    character(p) = c;
    return p;
  }
  char_warning(f, c);
  return null;
}

  void
_tfm_init ()
{
}

  void
_tfm_init_once ()
{
  int	l;

  nfonts = FONT_MAX-FONT_BASE+1;
  font_info = (font_t **)malloc(nfonts * sizeof(font_t *));
  if (font_info == (font_t **) 0)
    overflow("font info", nfonts);
  font_end = font_info + nfonts;
  font_ptr = font_info + 1;
  l = sizeof(font_t) + 7 * sizeof(scal);
  font_info[0] = (font_t *)malloc(l);
  if (font_info[0] == (font_t *) 0)
    overflow("nullfont", l);
  memset(fmem(null_font), 0, l);
  font_length(null_font) = l;
  param_base(null_font) = (scal *) (fmem(null_font) + 1);
  font_params(null_font) = 7;
  font_area(null_font) = null_str;
  font_name(null_font) = "nullfont";
  font_bc(null_font) = 1;
  font_ec(null_font) = 0;
  font_glue(null_font) = zero_glue;
  hyphen_char(null_font) = '-';
  skew_char(null_font) = -1;
  bchar_label(null_font) = NON_ADDRESS;
  font_bchar(null_font) = NON_CHAR;
  font_false_bchar(null_font) = NON_CHAR;
}

/*
 **	Help text
 */

help_font ()
{
  help4("I'm afraid I won't be able to make use of this font,",
      "because my memory for character-size data is too small.",
      "If you're really stuck, ask a wizard to enlarge me.",
      "Or maybe try `I\\font<same font id>=<name of loaded font>'.");
}

help_tfm ()
{
  help5("I wasn't able to read the size data for this font,",
      "so I will ignore the font specification.",
      "[Wizards can fix TFM files using TFtoPL/PLtoTF.]",
      "You might try inserting a different font spec;",
      "e.g., type `I\\font<same font id>=<substitute font name>'.");
}

help_font_param ()
{
  help2("To increase the number of font parameters, you must",
      "use \\fontdimen immediately after the \\font is loaded.");
}

help_font_cs ()
{
  help2("I was looking for a control sequence whose",
      "current meaning has been defined by \\font.");
}

/*
 * Common TeX:tok.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX tok.c 3.2@(#)";
#endif

#include "tex.h"

tok	cur_tok;
tok	par_tok;

int	cur_cmd;
int	cur_chr;

ptr	def_ref;
ptr	match_toks;
ptr	align_toks;
ptr	omit_template;
ptr	null_list;

int	scanner_status;
int	align_state;

str	name;
int	line;
bool	force_eof;
int	open_parens;

input	cur_input;

int	ninputs;
input	*input_stack;
input	*input_end;
input 	*input_ptr;
input 	*max_in_stack;
input	*base_ptr;

#define STACK_SIZE	200

int	nfiles;
infile	*file_stack;
infile	*file_end;
infile	*file_ptr;
infile	*max_file_stack;

#define INFILE_SIZE	45

int	nparams;
ptr	*param_stack;
ptr	*param_end;
ptr	*param_ptr;
ptr	*max_param_stack;

#define PARAM_SIZE	100

  void
get_token ()
{
  no_new_control_sequence = FALSE;
  get_next();
  no_new_control_sequence = TRUE;
  if (cur_cs == null_sym) {
    cur_tok = cur_cmd * 0400 + cur_chr;
  } else {
    cur_tok = sym2tok(cur_cs);
  }
}

#define any_state(CAT) \
  case MID_LINE + CAT: \
case SKIP_BLANKS + CAT: \
case NEW_LINE + CAT

#define delims(STATE) \
  case STATE + MATH_SHIFT: \
case STATE + TAB_MARK: \
case STATE + MAC_PARAM: \
case STATE + SUB_MARK: \
case STATE + LETTER: \
case STATE + OTHER_CHAR 

#define mid_line(CAT) \
  case MID_LINE + CAT

#define new_line(CAT) \
  case NEW_LINE + CAT

#define skip_blanks(CAT) \
  case SKIP_BLANKS + CAT

#define is_hex(C) \
  ((C) >= '0' && (C) <= '9' || (C) >= 'a' && (C) <= 'f')

#define hex_to_cur_chr(C, CC) \
{	cur_chr = 16 * ((C <= '9') ? C - '0' : C - 'a' + 10); \
  cur_chr += (CC <= '9') ? CC - '0' : CC - 'a' + 10; \
}

  void
get_next ()
{
  tok	t;
  int	c, cc;

#define JOB_ABORT "*** (job aborted, no legal \\end found)"
#define USER_CMD "(Please type a command or say `\\end')"
#define INVAL_CHAR "Text line contains an invalid character"
#define WOVEN_ALIGNS "(interwoven alignment preambles are not allowed)"

restart:
  cur_cs = null_sym;
  if (file_state) {
reread:
    if (next <= limit) {
      cur_chr = *next++;
reswitch:
      cur_cmd = cat_code(cur_chr);
      switch (state + cur_cmd)
      {
        any_state(IGNORE):
          skip_blanks(SPACER):
            new_line(SPACER):
              goto reread;

        any_state(ESCAPE):
          get_cs();
        cur_cmd = eq_type(cur_cs);
        cur_chr = equiv(cur_cs);
        if (cur_cmd >= OUTER_CALL)
          check_outer_validity();
        break;

        any_state(ACTIVE_CHAR):
          cur_cs = active_base[cur_chr];
        cur_cmd = eq_type(cur_cs);
        cur_chr = equiv(cur_cs);
        state = MID_LINE;
        if (cur_cmd >= OUTER_CALL)
          check_outer_validity();
        break;

        any_state(SUP_MARK):
          if (cur_chr != *next
              || next >= limit
              || (c = next[1]) >= 0200) {
            state = MID_LINE;
            break;
          }
        next += 2;
        if (is_hex(c) && next <= limit) {
          cc = *next;
          if (is_hex(cc)) {
            next++;
            hex_to_cur_chr(c, cc);
            goto reswitch;
          }
        }
        if (c < 0100) {
          cur_chr = c + 0100;
        } else {
          cur_chr = c - 0100;
        }
        goto reswitch;

        any_state(INVALID_CHAR):
          print_err(INVAL_CHAR);
        help_funny();
        deletions_allowed = FALSE;
        error();
        deletions_allowed = TRUE;
        goto restart;

        mid_line(SPACER):
          state = SKIP_BLANKS;
        cur_chr = ' ';
        break;

        mid_line(CAR_RET):
          next = limit + 1;
        cur_cmd = SPACER;
        cur_chr = ' ';
        break;

        skip_blanks(CAR_RET):
          any_state(COMMENT):
            next = limit + 1;
        goto reread;

        new_line(CAR_RET):
          next = limit + 1;
        cur_cs = par_cs;
        cur_cmd = eq_type(cur_cs);
        cur_chr = equiv(cur_cs);
        if (cur_cmd >= OUTER_CALL)
          check_outer_validity();
        break;

        mid_line(LEFT_BRACE):
          incr(align_state);
        break;

        skip_blanks(LEFT_BRACE):
          new_line(LEFT_BRACE):
            state = MID_LINE;
        incr(align_state);
        break;

        mid_line(RIGHT_BRACE):
          decr(align_state);
        break;

        skip_blanks(RIGHT_BRACE):
          new_line(RIGHT_BRACE):
            state = MID_LINE;
        decr(align_state);
        break;

        delims(SKIP_BLANKS):
          delims(NEW_LINE):
            state = MID_LINE;
        break;

        default:
        break;
      }
    } else {
      state = NEW_LINE; 
      if (index > 17) {
        incr(line);
        if (!force_eof) {
          if (input_ln(cur_file)) {
            firm_up_the_line();
          } else {
            force_eof = TRUE;
          }
        }
        if (force_eof) {
          print(")");
          decr(open_parens);
          force_eof = FALSE;
          update_terminal();
          end_file_reading();
          check_outer_validity();
          goto restart;
        }
        if (end_line_char_active)
          *++limit = end_line_char;
      } else {
        if (!terminal_input) {
          cur_cmd = 0;
          cur_chr = 0;
          return;
        }
        if (input_ptr > input_stack) {
          end_file_reading();
          goto restart;
        }
        if (selector < LOG_ONLY)
          open_log_file();
        if (interaction > NONSTOP_MODE) {
          if (limit <= buffer)
            print_nl(USER_CMD);
          print_ln();
          prompt_input("*");
          memcpy(buffer, cur_str, cur_length());
          next = buffer;
          limit = buffer + cur_length() - 1;
          if (end_line_char_active)
            *++limit = end_line_char;
          flush_str();
        } else {
          fatal_error(JOB_ABORT);
        }
      }
      check_interrupt();
      goto reread;
    }
  } else {
    if (loc != null) {
      t = token(loc);
      loc = token_link(loc);
      if (t >= CS_TOKEN_FLAG) {
        cur_cs = tok2sym(t);
        cur_cmd = eq_type(cur_cs);
        cur_chr = equiv(cur_cs);
        if (cur_cmd >= OUTER_CALL) {
          if (cur_cmd == DONT_EXPAND) {
            cur_cs = tok2sym(token(loc));
            loc = null;
            cur_cmd = eq_type(cur_cs);
            cur_chr = equiv(cur_cs);
            if (cur_cmd > MAX_COMMAND) {
              cur_cmd = RELAX;
              cur_chr = NO_EXPAND_FLAG;
            }
          } else {
            check_outer_validity();
          }
        }
      } else {
        cur_cmd = t / 0400;
        cur_chr = t % 0400;
        switch (cur_cmd)
        {
          case LEFT_BRACE:
            incr(align_state);
            break;

          case RIGHT_BRACE:
            decr(align_state);
            break;

          case OUT_PARAM:
            begin_token_list(
                param_start[cur_chr - 1],
                PARAMETER
                );
            goto restart;

          default:
            break;
        }
      }
    } else {
      end_token_list();
      goto restart;
    }
  }
  if (cur_cmd <= CAR_RET && cur_cmd >= TAB_MARK && align_state == 0) {
    if (scanner_status == ALIGNING)
      fatal_error(WOVEN_ALIGNS);
    cur_cmd = unset_info(cur_align);
    unset_info(cur_align) = cur_chr;
    if (cur_cmd == OMIT)
      begin_token_list(omit_template, V_TEMPLATE);
    else begin_token_list(v_part(cur_align), V_TEMPLATE);
    align_state = 1000000;
    goto restart;
  }
}

  void
get_cs ()
{
  byte	*nx;
  int	cat;
  int	d, c, cc;

  if (next > limit) {
    cur_cs = null_cs;
    return;
  }

#define reduce_expanded_cc(CC) 						\
  {	if (*CC == cur_chr && cat == SUP_MARK && CC < limit) { 		\
                                                           if ((c = CC[1]) < 0200) {				\
                                                             d = 2;						\
                                                             if (is_hex(c) && CC + 2 <= limit) {		\
                                                               cc = CC[2];				\
                                                               if (is_hex(cc))				\
                                                               incr(d);			\
                                                             }						\
                                                             if (d > 2) {					\
                                                               hex_to_cur_chr(c, cc);			\
                                                               CC[-1] = cur_chr;			\
                                                             } else if (c < 0100) { 				\
                                                               CC[-1] = c + 0100; 			\
                                                             } else {					\
                                                               CC[-1] = c - 0100; 			\
                                                             }						\
                                                             limit -= d;					\
                                                             while (CC <= limit) { 				\
                                                               CC[0] = CC[d];				\
                                                               incr(CC);				\
                                                             }						\
                                                             goto start_cs;					\
                                                           }							\
                                                         }								\
  }

start_cs: 
  nx = next;
  cur_chr = *nx++;
  cat = cat_code(cur_chr);
  if (cat == LETTER)
    state = SKIP_BLANKS;
  else if (cat == SPACER)
    state = SKIP_BLANKS;
  else state = MID_LINE;
  if (cat == LETTER && nx <= limit) {
    do {
      cur_chr = *nx++;
      cat = cat_code(cur_chr);
    } while (cat == LETTER && nx <= limit);
    reduce_expanded_cc(nx);
    if (cat != LETTER)
      decr(nx);
    if (nx > next + 1) {
      cur_cs = id_lookup(next, nx - next);
      next = nx;
      return;
    }
  } else {
    reduce_expanded_cc(nx);
  }
  cur_cs = single_base[*next++];
}

  void
check_outer_validity ()
{
  ptr	p, q;

  if (scanner_status != NORMAL) {
    deletions_allowed = FALSE;
    if (cur_cs != null_sym) {
      if (state == TOKEN_LIST || index < 1 || index > 17) {
        p = new_token();
        token(p) = sym2tok(cur_cs);
        back_list(p);
      }
      cur_cmd = SPACER;
      cur_chr = ' ';
    }
    if (scanner_status > SKIPPING) {
      runaway();
      if (cur_cs == null_sym) {
        print_err("File ended");
      } else {
        cur_cs = null_sym;
        print_err("Forbidden control sequence found");
      }
      print(" while scanning ");
      p = new_token();
      switch (scanner_status)
      {
        case DEFINING:
          print("definition");
          token(p) = RIGHT_BRACE_TOKEN + '}';
          break;

        case MATCHING:
          print("use");
          token(p) = par_tok;
          long_state = OUTER_CALL;
          break;

        case ALIGNING:
          print("preamble");
          token(p) = RIGHT_BRACE_TOKEN + '}';
          q = p;
          p = new_token();
          token_link(p) = q;
          token(p) = sym2tok(FROZEN_CR);
          align_state = -1000000;
          break;

        case ABSORBING:
          print("text"); 
          token(p) = RIGHT_BRACE_TOKEN + '}';
          break;
      }
      ins_list(p);
      print(" of ");
      sprint_cs(warning_cs);
      help_scanner();
      error();
    } else {
      print_err("Incomplete ");
      print_cmd_chr(IF_TEST, cur_if);
      print("; all text was ignored after line ");
      print_int(skip_line);
      if (cur_cs != null_sym) {
        cur_cs = null_sym;
        help_skip();
      } else {
        help_skif();
      }
      cur_tok = sym2tok(FROZEN_FI);
      ins_error();
    }
    deletions_allowed = TRUE;
  }
}

  void
firm_up_the_line ()
{
  byte	*s;

  if (pausing > 0 && interaction > NONSTOP_MODE) {
    wake_up_terminal();
    print_ln();
    if (next <= limit)
      for (s = buffer; s <= limit; incr(s))
        print_char(*s);
    prompt_input("=>");
    if (cur_length()) {
      memcpy(buffer, cur_str, cur_length());
      next = buffer;
      limit = buffer + cur_length() - 1;
    }
    flush_str();
  }
}

  void
push_input()
{
  if (input_ptr > max_in_stack) {
    if (input_ptr == input_end)
      overflow("input stack size", ninputs);
    max_in_stack = input_ptr;
  }
  if (file_state) {
    if (index == 0 || index > 17) {
      file_line = line;
      file_name = name;
    }
  } else {
    cs_name = name;
  }
  *input_ptr++ = cur_input;
}

  void
pop_input()
{
  cur_input = *--input_ptr;
  if (file_state) {
    if (index == 0 || index > 17) {
      line = file_line;
      name = file_name;
    }
  } else {
    name = cs_name;
  }
}

  void
begin_token_list (p, t)
  ptr	p;
  int	t;
{
  push_input();
  state = TOKEN_LIST;
  start = p;
  token_type = t;
  if (t >= MACRO) {
    add_token_ref(p);
    if (t == MACRO) {
      param_start = param_ptr;
    } else {
      loc = token_link(p);
      if (tracing_macros > 1) {
        begin_diagnostic();
        print_nl(null_str);
        switch (t)
        {
          case MARK_TEXT:
            print_esc("mark");
            break;

          case WRITE_TEXT:
            print_esc("write");
            break;

          default:
            print_toks_param(t - OUTPUT_TEXT);
            break;
        }
        print("->");
        token_show(p);
        end_diagnostic(FALSE);
      }
    }
  } else {
    loc = p;
  }
}

  void
end_token_list ()
{
  if (token_type >= BACKED_UP) {
    if (token_type <= INSERTED) {
      flush_list(start);
    } else {
      delete_token_ref(start);
      if (token_type == MACRO)
        while (param_ptr > param_start)
          flush_list(*--param_ptr);
    }
  } else if (token_type == U_TEMPLATE) {
    align_state = 0;
  }
  pop_input();
  check_interrupt();
}

  void
begin_file_reading ()
{
  push_input();
  if (file_ptr > max_file_stack) {
    if (file_ptr == file_end)
      overflow("file stack size", nfiles);
    max_file_stack = file_ptr;
  }
  in_file = (ptr)file_ptr++;
  buffer = (byte *)new_str(BUF_SIZE);
  state = MID_LINE;
  index = 0;
}

  void
end_file_reading ()
{
  if (index > 17) {
    a_close(cur_file);
  }
  free_str(buffer);
  pop_input();
  decr(file_ptr);
}

  void
back_input ()
{
  ptr	p;

  while (state == TOKEN_LIST && loc == null)
    end_token_list();
  p = new_token();
  token(p) = cur_tok;
  if (cur_tok < RIGHT_BRACE_LIMIT) {
    if (cur_tok < LEFT_BRACE_LIMIT) {
      decr(align_state);
    } else {
      incr(align_state);
    }
  }
  push_input();
  state = TOKEN_LIST;
  loc = start = p;
  token_type = BACKED_UP;
}

  void
back_error ()
{
  OK_to_interrupt = FALSE;
  back_input();
  OK_to_interrupt = TRUE;
  error();
}

  void
ins_error ()
{
  OK_to_interrupt = FALSE;
  back_input();
  token_type = INSERTED; 
  OK_to_interrupt = TRUE;
  error();
}

  void
clear_for_error_prompt ()
{
  while (state != TOKEN_LIST
      && terminal_input
      && input_ptr > input_stack
      && next > limit)
    end_file_reading();
  print_ln();
  clear_terminal();
  flush_str();
}

  void
runaway ()
{
  ptr	p;

  if (scanner_status > SKIPPING) {
    print_nl("Runaway ");
    switch (scanner_status) 
    {
      case DEFINING:
        print("definition");
        p = def_ref;
        break;

      case MATCHING:
        print("argument");
        p = match_toks;
        break;

      case ALIGNING:
        print("preamble");
        p = align_toks;
        break;

      case ABSORBING:
        print("text");
        p = def_ref;
        break;
    }
    print("?");
    print_ln();
    show_token_list(token_link(p), null, ERROR_LINE - 10);
  }
}

  void
show_context ()
{
  int	i, j, l, m, n, nn, p, q;
  int	old_setting;
  bool	bottom_line;

  base_ptr = input_ptr;
  push_input();
  nn = -1;
  bottom_line = FALSE;
  loop {
    cur_input = *base_ptr;
    if (state != TOKEN_LIST
        && (index > 17 || base_ptr == input_stack)) {
      bottom_line = TRUE;
    }
    if (base_ptr == input_ptr - 1
        || bottom_line
        || nn < error_context_lines) {
      if (base_ptr == input_ptr - 1
          || state != TOKEN_LIST
          || token_type != BACKED_UP
          || loc != null) {
        tally = 0;
        old_setting = selector;
        if (file_state) {
          if (index <= 17) {
            if (terminal_input) {
              if (base_ptr == input_stack)
                print_nl("<*>");
              else print_nl("<insert> ");
            } else {
              print_nl("<read ");
              if (index == 17)
                print("*");
              else print_int(index - 1);
              print(">");
            }
          } else {
            print_nl("l.");
            print_int(file_line);
          }
          print(" ");
          l = begin_pseudoprint();
          j = limit - buffer;
          if (*limit != end_line_char)
            incr(j);
          if (j > 0) {
            for (i = 0; i < j; incr(i)) {
              if (buffer + i == next)
                set_trick_count();
              print_ASCII(buffer[i]);
            }
          }
        } else {
          switch (token_type)
          {
            case PARAMETER:
              print_nl("<argument> ");
              break;

            case U_TEMPLATE:
            case V_TEMPLATE:
              print_nl("<template> ");
              break;

            case BACKED_UP:
              if (loc == null)
                print_nl("<recently read> "); 
              else  print_nl("<to be read again> ");
              break;

            case INSERTED:
              print_nl("<inserted text> ");
              break;

            case MACRO:
              print_ln();
              print_cs(in_cs);
              break;

            case OUTPUT_TEXT:
              print_nl("<output> ");
              break;

            case EVERY_PAR_TEXT:
              print_nl("<everypar> ");
              break;

            case EVERY_MATH_TEXT:
              print_nl("<everymath> ");
              break;

            case EVERY_DISPLAY_TEXT:
              print_nl("<everydisplay> ");
              break;

            case EVERY_HBOX_TEXT:
              print_nl("<everyhbox> ");
              break;

            case EVERY_VBOX_TEXT:
              print_nl("<everyvbox> ");
              break;

            case EVERY_JOB_TEXT:
              print_nl("<everyjob>");
              break;

            case EVERY_CR_TEXT:
              print_nl("<everycr> ");
              break;

            case MARK_TEXT:
              print_nl("<mark> ");
              break;

            case WRITE_TEXT:
              print_nl("<write> ");
              break;

            default:
              print_nl("? ");
              break;
          }
          l = begin_pseudoprint();
          if (token_type < MACRO)
            show_token_list(start, loc, 100000);
          else
            show_token_list(token_link(start),
                loc, 100000);
        }
        selector = old_setting;
        if (trick_count == 1000000)
          set_trick_count();
        if (tally < trick_count)
          m = tally - first_count;
        else m = trick_count - first_count;
        if (l + first_count <= HALF_ERROR_LINE) {
          p = 0;
          n = l + first_count;
        } else {
          print("...");
          p = l + first_count - HALF_ERROR_LINE + 3;
          n = HALF_ERROR_LINE;
        }
        for (q = p; q < first_count; incr(q))
          print_char(trick_buf[q % ERROR_LINE]);
        print_ln();
        for (q = 1; q <= n; incr(q))
          print(" ");
        if (m + n <= ERROR_LINE)
          p = first_count + m;
        else p = first_count + ERROR_LINE - n - 3;
        for (q = first_count; q < p; q++)
          print_char(trick_buf[q % ERROR_LINE]);
        if (m + n > ERROR_LINE)
          print("...");
        incr(nn);
      }
    } else if (nn == error_context_lines) {
      print_nl("...");
      incr(nn);
    }
    if (bottom_line)
      break;
    decr(base_ptr);
  }
  pop_input();
}

  void
_tok_init ()
{
  base_ptr = max_in_stack = input_ptr = input_stack;
  max_file_stack = file_ptr = file_stack;
  max_param_stack = param_ptr = param_stack;
  state = NEW_LINE;
  index = 0;
  in_file = (ptr)file_ptr++;
  cur_file = stdin;
  name = file_name = "tty";
  line = file_line =  0;
  force_eof = FALSE;
  open_parens = 0;
  scanner_status = NORMAL;
  align_state = 1000000;
  buffer = (byte *)new_str(BUF_SIZE);
  next = buffer;
  limit = next - 1;
  token(omit_template) = END_TEMPLATE_TOKEN;
  token(null_list) = null_tok;
}

  void
_tok_init_once ()
{
  ninputs = STACK_SIZE;
  input_stack = (input *)malloc(ninputs*sizeof(input));
  if (input_stack == (input *) 0)
    overflow("input stack", ninputs);
  input_end = input_stack + ninputs;

  nfiles = INFILE_SIZE;
  file_stack = (infile *)malloc(nfiles * sizeof(infile));
  if (file_stack == (infile *) 0)
    overflow("infile stack", nfiles);
  file_end = file_stack + nfiles;

  nparams = PARAM_SIZE;
  param_stack = (ptr *)malloc(nparams * sizeof(ptr));
  if (param_stack == (ptr *) 0)
    overflow("param stack", nparams);
  param_end = param_stack + nparams;
  match_toks = new_token();
  align_toks = new_token();
  omit_template = new_token();
  null_list = new_token();
}

/*
 **	Help text
 */

help_scanner ()
{
  help4("I suspect you have forgotten a `}', causing me",
      "to read past where you wanted me to stop.",
      "I'll try to recover; but if the error is serious,",
      "you'd better type `E' or `X' now and fix your file.");
}

help_funny ()
{
  help2("A funny symbol that I can't read has just been input.",
      "Continue, and I'll forget that it ever happened.");
}

help_skip ()
{
  help3("A forbidden control sequence occurred in skipped text.",
      "This kind of error happens when you say `\\if...' and forget",
      "the matching `\\fi'. I've inserted a `\\fi'; this might work.");
}

help_skif ()
{
  help3("The file ended while I was skipping conditional text.",
      "This kind of error happens when you say `\\if...' and forget",
      "the matching `\\fi'. I've inserted a `\\fi'; this might work.");
}

/*
 * Common TeX:toklist.c:3.2:EPA
 *
 * Copyright 1987,1988,1991 Pat J Monardo
 *
 * Redistribution of this file is permitted through
 * the specifications in the file COPYING.
 *
 * 
 */

#ifndef lint
static char *sccsid = "@(#)Common TeX toklist.c 3.2@(#)";
#endif

#include "tex.h"

  ptr
str_toks ()
{
  str	s;
  tok	t;
  ptr	p, q;

  p = q = new_token();
  token_link(p) = null;
  token_ref_count(p) = 0;
  for (s = cur_str; s < cur_str_ptr; incr(s)) {
    t = *s;
    if (t == ' ')
      t = SPACE_TOKEN;
    else t += OTHER_TOKEN;
    q = token_link(q) = new_token();
    token(q) = t;
  }
  flush_str();
  return p;
}

  ptr
the_toks ()
{
  ptr	p, q, r;
  int	old_setting;

  get_x_token();
  scan_something_internal(TOK_VAL, FALSE);
  if (cur_val_level >= IDENT_VAL) {
    p = q = new_token();
    token_link(q) = null;
    token_ref_count(q) = 0;
    if (cur_val_level == IDENT_VAL) {
      q = token_link(q) = new_token();
      token(q) = sym2tok(cur_val);
    } else if ((ptr)cur_val != null) {
      r = token_link(cur_val);
      while (r != null ) {
        q = token_link(q) = new_token();
        token(q) = token(r);
        r = token_link(r);
      }
    }
    return p;
  } else {
    old_setting = selector;
    selector = NEW_STRING;
    switch (cur_val_level)
    {
      case INT_VAL:
        print_int(cur_val);
        break;

      case DIMEN_VAL:
        print_scaled(cur_val);
        print("pt");
        break;

      case GLUE_VAL:
        print_spec(cur_val, "pt");
        delete_glue_ref(cur_val);
        break;

      case MU_VAL:
        print_spec(cur_val,"mu");
        delete_glue_ref(cur_val);
        break;
    }
    selector = old_setting;
    return (str_toks());
  }
}

  void
conv_toks ()
{
  int	c;
  ptr	p;
  int	old_setting;
  int	save_scanner_status;

  c = cur_chr;
  switch (c)
  {
    case NUMBER_CODE: 
    case ROMAN_NUMERAL_CODE:
      scan_int();
      break;

    case STRING_CODE:
    case MEANING_CODE:
      save_scanner_status = scanner_status;
      scanner_status = NORMAL;
      get_token();
      scanner_status = save_scanner_status;
      break;

    case FONT_NAME_CODE:
      scan_font_ident();
      break;

    case JOB_NAME_CODE:
      if (job_name == null_str)
        open_log_file();
      break;
  }
  old_setting = selector;
  selector = NEW_STRING;
  switch (c)
  {
    case NUMBER_CODE:
      print_int(cur_val);
      break;

    case ROMAN_NUMERAL_CODE:
      print_roman_int(cur_val);
      break;

    case STRING_CODE:
      if (cur_cs != 0)
        sprint_cs(cur_cs);
      else print_char(cur_chr);
      break;

    case MEANING_CODE:
      print_meaning();
      break;

    case FONT_NAME_CODE:
      print(font_name(cur_val));
      if (font_size(cur_val) != font_dsize(cur_val)) {
        print(" at ");
        print_scaled(font_size(cur_val));
        print("pt");
      }
      break;

    case JOB_NAME_CODE:
      print(job_name);
      break;
  }
  selector = old_setting; 
  p = str_toks();
  ins_list(token_link(p));
  free_token(p);
}

  void
scan_toks (macro_def, xpand)
  bool	macro_def;
  bool	xpand;
{
  ptr	p, q;
  tok	s, t;
  int	unbalance;
  tok	hash_brace;

#define BAD_PARAM_NO "Illegal parameter number in definition of "
#define ONLY_9_PARAMS "You already have nine parameters"
#define CONSEC_PARAMS "Parameters must be numbered consecutively"

  if (macro_def)
    scanner_status = DEFINING;
  else scanner_status = ABSORBING;
  warning_cs = cur_cs;
  def_ref = new_token();
  token_ref_count(def_ref) = 0;
  p = def_ref;
  hash_brace = 0;
  t = ZERO_TOKEN;
  if (macro_def) {
    loop {
      get_token();
      if (cur_tok < RIGHT_BRACE_LIMIT)
        break;
      if (cur_cmd == MAC_PARAM) {
        s = MATCH_TOKEN + cur_chr;
        get_token();
        if (cur_cmd == LEFT_BRACE) {
          hash_brace = cur_tok; 
          store_new_token(cur_tok);
          store_new_token(END_MATCH_TOKEN);
          goto done;
        }
        if (t == ZERO_TOKEN + 9) {
          print_err(ONLY_9_PARAMS);
          help_param_count();
          error();
        } else {
          incr(t);
          if (cur_tok != t) {
            print_err(CONSEC_PARAMS);
            help_param_num();
            back_error();
          }
          cur_tok = s;
        }
      }
      store_new_token(cur_tok);
    }
    store_new_token(END_MATCH_TOKEN);
    if (cur_cmd == RIGHT_BRACE) {
      print_err("Missing { inserted");
      incr(align_state); 
      help_left_brace();
      error();
      goto found;
    }
  } else {
    scan_left_brace();
  }

done:
  unbalance = 1;
  loop {
    if (xpand) {
      loop {
        get_next();
        if (cur_cmd <= MAX_COMMAND)
          break;
        if (cur_cmd != THE) {
          expand();
        } else {
          q = token_link(the_toks()); 
          if (q != null) {
            p = token_link(p) = q;
            while (token_link(q))
              p = q = token_link(q);
          }
        }
      }
      x_token();
    } else {
      get_token();
    }
    if (cur_tok < RIGHT_BRACE_LIMIT) {
      if (cur_cmd < RIGHT_BRACE) {
        incr(unbalance);
      } else {
        decr(unbalance);
        if (unbalance == 0)
          break;
      }
    } else if (cur_cmd == MAC_PARAM && macro_def) {
      s = cur_tok;
      if (xpand)
        get_x_token();
      else get_token();
      if (cur_cmd != MAC_PARAM) {
        if (cur_tok <= ZERO_TOKEN || cur_tok > t) {
          print_err(BAD_PARAM_NO);
          sprint_cs(warning_cs);
          help_param_use();
          back_error(); 
          cur_tok = s;
        } else {
          cur_tok = OUT_PARAM_TOKEN +
            cur_chr - '0';
        }
      }
    }
    store_new_token(cur_tok);
  }

found:
  scanner_status = NORMAL;
  if (hash_brace != 0)
    store_new_token(hash_brace);
}

  void
read_toks (n, r)
  int	n;
  sym	r;
{
  ptr	p, q;
  int	m, s;

  str NO_READ = "*** (cannot \\read from terminal in nonstop modes)";

  scanner_status  = DEFINING;
  warning_cs = r;
  def_ref = new_token();
  token_ref_count(def_ref) = 0;
  p = def_ref;
  store_new_token(END_MATCH_TOKEN);
  if (n < 0 || n > 15)
    m = 16;
  else m = n;
  s = align_state;
  align_state = 1000000;
  do {
    begin_file_reading();
    index = m + 1;
    if (read_open[m] == CLOSED) {
      if (interaction > NONSTOP_MODE) {
        if (n < 0) {
          prompt_input(null_str);
        } else {
          wake_up_terminal();
          print_ln();
          sprint_cs(r);
          prompt_input("=");
          n = -1;
        }
        strcpy(buffer, cur_str);
        limit = buffer + cur_length();
      } else {
        fatal_error(NO_READ);
      }
    } else if (read_open[m] == OPENED) {
      if (input_ln(read_file[m])) {
        read_open[m] = NORMAL;
      } else {	
        a_close(read_file[m]);
        read_open[m] = CLOSED;
      }
    } else {
      if (!input_ln(read_file[m])) {
        a_close(read_file[m]);
        read_open[m] = CLOSED;
        if (align_state != 1000000) {
          runaway();
          print_err("File ended within ");
          print_esc("read");
          help_read();
          align_state = 1000000;
          error();
        }
      }
    }
    if (end_line_char_active)
      *++limit = end_line_char;
    state = NEW_LINE;
    loop {
      get_token();
      if (cur_tok == 0)
        break; 
      store_new_token(cur_tok);
    }
    end_file_reading();
  } while (align_state != 1000000);
  cur_val = def_ref;
  scanner_status = NORMAL; 
  align_state = s;
}

  void
show_token_list	(p, q, l)
  ptr	p;
  ptr	q;
  int	l;
{
  int	c;
  int	m;
  int	n;
  int	match_chr;

  match_chr = '#';
  n = '0';
  for (tally = 0; p != null && tally < l; p = token_link(p)) {
    if (p == q)
      set_trick_count();
    if (token(p) >= CS_TOKEN_FLAG) {
      print_cs(tok2sym(token(p)));
    } else {
      m = token(p) / 0400;
      c = token(p) % 0400;
      if (token(p) < 0 || c > 255) {
        print_esc("BAD.");
      } else {
        switch (m)
        {
          case LEFT_BRACE:
          case RIGHT_BRACE:
          case MATH_SHIFT:
          case TAB_MARK:
          case SUP_MARK:
          case SUB_MARK:
          case SPACER:
          case LETTER:
          case OTHER_CHAR:
            print_ASCII(c); 
            break;

          case MAC_PARAM:
            print_ASCII(c);
            print_ASCII(c);
            break;

          case OUT_PARAM:
            print_ASCII(match_chr);
            if (c <= 9) {
              print_char(c + '0');
            } else {
              print("!");
              return;
            }
            break;

          case MATCH:
            match_chr = c;
            print_ASCII(c);
            incr(n);
            print_char(n);
            if (n > '9')
              return;
            break;

          case END_MATCH:
            print("->");
            break;

          default:
            print_err("BAD.");
            break;
        }
      }
    }
  }
  if (p != null)
    print_esc("ETC.");
}

  void
token_show (p)
  ptr	p;
{ 
  if (p != null) {
    show_token_list(token_link(p), null, 10000000);
  }
}

  void
print_meaning ()
{
  print_cmd_chr(cur_cmd, cur_chr);
  if (cur_cmd >= CALL) {
    print(":");
    print_ln();
    token_show(cur_chr);
  } else if (cur_cmd == TOP_BOT_MARK) {
    print(":");
    print_ln();
    token_show(cur_mark[cur_chr]);
  }
}

  void
flush_list (p)
  ptr	p;
{
  ptr	q;

  while (p != null) {
    q = token_link(p);
    free_token(p);
    p = q;
  }
}

  void
_toklist_init ()
{
}

  void
_toklist_init_once ()
{
}

/*
 **	Help text
 */

help_param_num ()
{
  help2("I've inserted the digit you should have used after the #.",
      "Type `1' to delete what you did use.");
}

help_param_count ()
{
  help1("I'm going to ignore the # sign you just used.");
}

help_left_brace ()
{
  help2("Where was the left brace? You said something like `\\def\\a}',",
      "which I'm going to interpret as `\\def\\a{}'.");
}

help_param_use ()
{
  help3("You meant to type ## instead of #, right?",
      "Or maybe a } was forgotten somewhere earlier, and things",
      "are all screwed up? I'm going to assume that you meant ##.");
}

help_read ()
{
  help1("This \\read has unbalanced braces.");
}
