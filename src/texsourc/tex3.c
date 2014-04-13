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

#pragma warning(disable:4996)
#pragma warning(disable:4131) // old style declarator
#pragma warning(disable:4135) // conversion between different integral types 
#pragma warning(disable:4127) // conditional expression is constant

#include <setjmp.h>

#define EXTERN extern

#include "texd.h"

#pragma warning(disable:4244)       /* 96/Jan/10 */

/* sec 0440 */
void scan_int (void)
{
  bool negative;
  integer m;
  small_number d;
  bool vacuous;
  bool OKsofar;

  radix = 0;
  OKsofar = true;
  negative = false;

  do {
    do 
      {
        get_x_token();
      }
    while (!(cur_cmd != spacer));

    if (cur_tok == other_token + '-')
    {
      negative = !negative;
      cur_tok = other_token + '+';
    }
  } while (!(cur_tok != other_token + '+'));

  if (cur_tok == alpha_token)
  {
    get_token();

    if (cur_tok < cs_token_flag)
    {
      cur_val = cur_chr;

      if (cur_cmd <= right_brace)
        if (cur_cmd == right_brace)
          incr(align_state);
        else
          decr(align_state);
    }
    else if (cur_tok < cs_token_flag + single_base)
      cur_val = cur_tok - cs_token_flag - active_base;
    else
      cur_val = cur_tok - cs_token_flag - single_base;

    if (cur_val > 255)
    {
      print_err("Improper alphabetic constant");
      help2("A one-character control sequence belongs after a ` mark.",
        "So I'm essentially inserting \\0 here.");
      cur_val = '0';
      back_error();
    }
    else
    {
      get_x_token();

      if (cur_cmd != spacer)
        back_input();
    }
  }
  else if ((cur_cmd >= min_internal) && (cur_cmd <= max_internal))
  {
    scan_something_internal(int_val, false);
  }
  else
  {
    radix = 10;
    m = 214748364L;   /* 7FFFFFFF hex */

    if (cur_tok == octal_token)
    {
      radix = 8;
      m = 268435456L;   /* 2^28 */
      get_x_token();
    }
    else if (cur_tok == hex_token)
    {
      radix = 16;
      m = 134217728L;   /* 2^27 8000000 hex */
      get_x_token();
    }

    vacuous = true;
    cur_val = 0;

    while (true)
    {
      if ((cur_tok < zero_token + radix) && (cur_tok >= zero_token) && (cur_tok <= zero_token + 9))
        d = cur_tok - zero_token;
      else if (radix == 16)
        if ((cur_tok <= A_token + 5) && (cur_tok >= A_token))
          d = cur_tok - A_token + 10;
        else if ((cur_tok <= other_A_token + 5) && (cur_tok >= other_A_token))
          d = cur_tok - other_A_token;
        else
          goto lab30;
      else
        goto lab30;

      vacuous = false;

      if ((cur_val >= m) && ((cur_val > m) || (d > 7) || (radix != 10)))
      {
        if (OKsofar)
        {
          print_err("Number too big");
          help2("I can only go up to 2147483647='17777777777=\"7FFFFFFF,",
            "so I'm using that number instead of yours.");
          error();
          cur_val = 2147483647L;    /* 7FFFFFFF hex */
          OKsofar = false;
        }
      }
      else
        cur_val = cur_val * radix + d;
      get_x_token();
    }
lab30:;

    if (vacuous)
    {
      print_err("Missing number, treated as zero");
      help3("A number should have been here; I inserted `0'.",
        "(If you can't figure out why I needed to see a number,",
        "look up `weird error' in the index to The TeXbook.)");
      back_error();
    } 
    else if (cur_cmd != spacer)
      back_input();
  }

  if (negative)
    cur_val = - (integer) cur_val;
}
/* sec 0448 */
void scan_dimen_(bool mu, bool inf, bool shortcut)
{
  bool negative;
  integer f;
  integer num, denom;
  small_number k, kk;
  halfword p, q;
  scaled v;
  integer savecurval;

  f = 0;
  arith_error = false;
  cur_order = 0;
  negative = false;

  if (!shortcut)
  {
    negative = false;
    do {
      do
        {
          get_x_token();
        }
      while (!(cur_cmd != spacer));

      if (cur_tok == other_token + '-')
      {
        negative = ! negative;
        cur_tok = other_token + '+';
      }
    } while (!(cur_tok != other_token + '+'));

    if ((cur_cmd >= min_internal) && (cur_cmd <= max_internal))
    {
      if (mu)
      {
        scan_something_internal(mu_val, false);

        if (cur_val_level >= glue_val)
        {
          v = width(cur_val);
          delete_glue_ref(cur_val);
          cur_val = v;
        }

        if (cur_val_level == mu_val)
          goto lab89;

        if (cur_val_level != int_val)
          mu_error();
      }
      else
      {
        scan_something_internal(dimen_val, false);

        if (cur_val_level == dimen_val)
          goto lab89;
      }
    }
    else
    {
      back_input();

      if (cur_tok == continental_point_token)
        cur_tok = point_token;

      if (cur_tok != point_token)
      {
        scan_int();
      }
      else
      {
        radix = 10;
        cur_val = 0;
      }

      if (cur_tok == continental_point_token)
        cur_tok = point_token;

      if ((radix == 10) && (cur_tok == point_token))
      {
        k = 0;
        p = 0;      /* p:=null l.8883 */
        get_token();

        while (true)
        {
          get_x_token();

          if ((cur_tok > zero_token + 9) || (cur_tok < zero_token))
            goto lab31;

          if (k < 17)
          {
            q = get_avail();
            link(q) = p;
            info(q) = cur_tok - zero_token;
            p = q;
            incr(k);
          }
        }
lab31:
        for (kk = k; kk >= 1; kk--)
        {
          dig[kk - 1] = info(p);
          q = p;
          p = link(p);
          free_avail(q);
        }

        f = round_decimals(k);

        if (cur_cmd != spacer)
          back_input();
        }
      }
  }

  if (cur_val < 0)
  {
    negative = !negative;
    cur_val = - (integer) cur_val;
  }

  if (inf)
  {
    if (scan_keyword("fil"))
    {
      cur_order = fil;

      while (scan_keyword("l"))
      {
        if (cur_order == filll)
        {
          print_err("Illegal unit of measure(");
          print_string("replaced by filll)");
          help1("I dddon't go any higher than filll.");
          error();
        }
        else
          incr(cur_order);
      }
      goto lab88;
    }
  }

  savecurval = cur_val;

  do
    {
      get_x_token();
    }
  while (!(cur_cmd != spacer));

  if ((cur_cmd < min_internal) || (cur_cmd > max_internal))
    back_input();
  else
  {
    if (mu)
    {
      scan_something_internal(mu_val, false);

      if (cur_val_level >= glue_val)
      {
        v = width(cur_val);
        delete_glue_ref(cur_val);
        cur_val = v;
      }

      if (cur_val_level != mu_val)
      {
        mu_error();
      }
    }
    else
    {
      scan_something_internal(dimen_val, false);
    }
    v = cur_val;
    goto lab40;
  }

  if (mu)
    goto lab45;

  if (scan_keyword("em"))
    v = quad(cur_font);
  else if (scan_keyword("ex"))
    v = x_height(cur_font);
  else
    goto lab45;

  {
    get_x_token();

    if (cur_cmd != spacer)
      back_input();
  }
lab40:
  cur_val = mult_and_add(savecurval, v, xn_over_d(v, f, 65536L), 1073741823L);   /* 2^30 - 1 */
  goto lab89;
lab45:
  if (mu)
  {
    if (scan_keyword("mu"))
      goto lab88;
    else
    {
      print_err("Illegal unit of measure(");
      print_string("mu inserted)");
      help4("The unit of measurement in math glue must be mu.",
          "To recover gracefully from this error, it's best to",
          "delete the erroneous units; e.g., type `2' to delete",
          "two letters. (See Chapter 27 of The TeXbook.)");
      error();
      goto lab88;
    }
  }

  if (scan_keyword("true"))
  {
    prepare_mag();

    if (mag != 1000)
    {
      cur_val = xn_over_d(cur_val, 1000, mag);
      f = (1000 * f + 65536L * tex_remainder) / mag;
      cur_val = cur_val + (f / 65536L);
      f = f % 65536L;
    }
  }

  if (scan_keyword("pt"))
    goto lab88;

  if (scan_keyword("in"))
  {
    num = 7227;
    denom = 100;
  }
  else if (scan_keyword("pc"))
  {
    num = 12;
    denom = 1;
  }
  else if (scan_keyword("cm"))
  {
    num = 7227;
    denom = 254;
  }
  else if (scan_keyword("mm"))
  {
    num = 7227;
    denom = 2540;
  }
  else if (scan_keyword("bp"))
  {
    num = 7227;
    denom = 7200;
  }
  else if (scan_keyword("dd"))
  {
    num = 1238;
    denom = 1157;
  }
  else if (scan_keyword("cc"))
  {
    num = 14856;
    denom = 1157;
  }
  else if (scan_keyword("Q"))
  {
    num = 7227;
    denom = 10160;
  }
  else if (scan_keyword("H"))
  {
    num = 7227;
    denom = 10160;
  }
  else if (scan_keyword("sp"))
    goto lab30;
  else
  {
    print_err("Illegal unit of measure(");
    print_string("pt inserted)");
    help6("Dimensions can be in units of em, ex, in, pt, pc,",
      "cm, mm, dd, cc, bp, or sp; but yours is a new one!",
      "I'll assume that you meant to say pt, for printer's points.",
      "To recover gracefully from this error, it's best to",
      "delete the erroneous units; e.g., type `2' to delete",
      "two letters. (See Chapter 27 of The TeXbook.)");
    error();
    goto lab32;
  }

  cur_val = xn_over_d(cur_val, num, denom);
  f = (num * f + 65536L * tex_remainder) / denom;
  cur_val = cur_val +(f / 65536L);
  f = f % 65536L;
lab32:;
lab88:
  if (cur_val >= 16384)     /* 2^14 */
    arith_error = true;
  else
    cur_val = cur_val * 65536L + f;
lab30:;
  {
    get_x_token();

    if (cur_cmd != spacer)
      back_input();
  }
lab89:
  if (arith_error || (abs(cur_val) >= 1073741824L)) /* 2^30 */
  {
    print_err("Dimension too large");
    help2("I can't work with sizes bigger than about 19 feet.",
        "Continue and I'll use the largest value I can.");
    error();
    cur_val = 1073741823L;  /* 2^30 - 1 */
    arith_error = false;
  }

  if (negative)
    cur_val = - (integer) cur_val;
}
/* sec 0461 */
void scan_glue_(small_number level)
{
  bool negative;
  halfword q;
  bool mu;

  mu = (level == mu_val);
  negative = false;

  do
    {
      do {
          get_x_token();
      }
      while (!(cur_cmd != spacer));

      if (cur_tok == other_token + '-')
      {
        negative = !negative;
        cur_tok = other_token + '+';
      }
    }
  while (!(cur_tok != other_token + '+'));

  if ((cur_cmd >= min_internal) && (cur_cmd <= max_internal))
  {
    scan_something_internal(level, negative);

    if (cur_val_level >= glue_val)
    {
      if (cur_val_level != level)
      {
        mu_error();
      }
      return;
    }

    if (cur_val_level == int_val)
    {
      scan_dimen(mu, false, true);
    }
    else if (level == mu_val)
    {
      mu_error();
    }
  }
  else
  {
    back_input();
    scan_dimen(mu, false, false);

    if (negative)
      cur_val = - (integer) cur_val;
  }
  q = new_spec(zero_glue);
  width(q) = cur_val;

  if (scan_keyword("plus"))
  {
    scan_dimen(mu, true, false);
    stretch(q) = cur_val;
    stretch_order(q) = cur_order;
  }

  if (scan_keyword("minus"))
  {
    scan_dimen(mu, true, false);
    shrink(q) = cur_val;
    shrink_order(q) = cur_order;
  }

  cur_val = q;
}
/* sec 0463 */
halfword scan_rule_spec (void)
{
  halfword q;

  q = new_rule();
  if (cur_cmd == vrule)
    width(q) = default_rule;
  else
  {
    height(q) = default_rule;
    depth(q) = 0;
  }

lab21:

  if (scan_keyword("width"))
  {
    scan_dimen(false, false, false);
    width(q) = cur_val;
    goto lab21;
  }

  if (scan_keyword("height"))
  {
    scan_dimen(false, false, false);
    height(q) = cur_val;
    goto lab21;
  }

  if (scan_keyword("depth"))
  {
    scan_dimen(false, false, false);
    depth(q) = cur_val;
    goto lab21;
  }

  return q;
}
/* sec 0464 */
halfword str_toks_(pool_pointer b)
{
  halfword p;
  halfword q;
  halfword t;
  pool_pointer k;

  str_room(1);
  p = temp_head;
  link(p) = 0;
  k = b;

  while (k < pool_ptr)
  {
    t = str_pool[k];

    if (t == ' ')
      t = space_token;
    else
      t = other_token + t;
    {
      {
        q = avail;
        if (q == 0)
          q = get_avail();
        else {
          avail = mem[q].hh.v.RH;
          mem[q].hh.v.RH = 0;
#ifdef STAT
          incr(dyn_used); 
#endif /* STAT */
        }
      } 
      mem[p].hh.v.RH = q;
      mem[q].hh.v.LH = t;
      p = q;
    }
    incr(k);
  }
  pool_ptr = b;

  return p;
}
/* sec 0465 */
halfword the_toks (void)
{
  register halfword Result;
  char old_setting;
  halfword p, q, r;
  pool_pointer b;

  get_x_token();
  scan_something_internal(tok_val, false);

  if (cur_val_level >= ident_val)
  {
    p = temp_head; 
    link(p) = 0;

    if (cur_val_level == ident_val)
    {
      q = get_avail();
      mem[p].hh.v.RH = q;
      mem[q].hh.v.LH = cs_token_flag + cur_val;
      p = q;
    }
    else if (cur_val != 0)
    {
      r = link(cur_val);

      while (r != 0) { /*   while r<>null do l.9178 */
        {
          {
            q = avail;
            if (q == 0)
              q = get_avail();
            else
            {
              avail = mem[q].hh.v.RH;
              mem[q].hh.v.RH = 0;
#ifdef STAT
              incr(dyn_used);
#endif /* STAT */
            }
          }
          mem[p].hh.v.RH = q;
          mem[q].hh.v.LH = mem[r].hh.v.LH;
          p = q;
        }
        r = link(r);
      }
    }
    Result = p;
  }
  else
  {
    old_setting = selector;
    selector = new_string;
    b = pool_ptr;

    switch (cur_val_level)
    {
      case int_val:
        print_int(cur_val);
        break;
      case dimen_val:
        {
          print_scaled(cur_val);
          print_string("pt");
        }
        break;
      case glue_val:
        {
          print_spec(cur_val, "pt");
          delete_glue_ref(cur_val);
        }
        break;
      case mu_val:
        {
          print_spec(cur_val, "mu");
          delete_glue_ref(cur_val);
        }
        break;
    }
    selector = old_setting;
    Result = str_toks(b);
  }

  return Result;
}
/* sec 0467 */
void ins_the_toks (void) 
{ 
  link(garbage) = the_toks();
  begin_token_list(link(temp_head), 4);
}
/* sec 0470 */
void conv_toks (void)
{
  char old_setting;
  char c;
  small_number savescannerstatus;
  pool_pointer b;

  c = cur_chr;
  switch (c)
  {
    case number_code:
    case roman_numeral_code:
      scan_int();
      break;
    case string_code:
    case meaning_code:
      savescannerstatus = scanner_status;
      scanner_status = 0;
      get_token();
      scanner_status = savescannerstatus;
      break;
    case font_name_code:
      scan_font_ident();
      break;
    case job_name_code:
      if (job_name == 0)
        open_log_file();
      break;
  }
  old_setting = selector;
  selector = new_string;
  b = pool_ptr;

  switch (c)
  {
    case number_code:
      print_int(cur_val);
      break;
    case roman_numeral_code:
      print_roman_int(cur_val);
      break;
    case string_code:
      if (cur_cs != 0)
        sprint_cs(cur_cs);
      else
        print_char(cur_chr);
      break;
    case meaning_code:
      print_meaning();
      break;
    case font_name_code:
      print(font_name[cur_val]);

      if (font_size[cur_val] != font_dsize[cur_val])
      {
        print_string(" at ");
        print_scaled(font_size[cur_val]);
        print_string("pt");
      }
      break;
    case job_name_code:
      print(job_name);
      break;
  }
  selector = old_setting;
  link(garbage) = str_toks(b);
  begin_token_list(link(temp_head), 4);
}
/* sec 0473 */
halfword scan_toks_(bool macrodef, bool xpand)
{
  register halfword Result;
  halfword t;
  halfword s;
  halfword p;
  halfword q;
  halfword unbalance;
  halfword hashbrace;

  if (macrodef)
    scanner_status = defining;
  else
    scanner_status = absorbing;

  warning_index = cur_cs;
  def_ref = get_avail();
  token_ref_count(def_ref) = 0;
  p = def_ref;
  hashbrace = 0;
  t = zero_token;

  if (macrodef)
  {
    while (true)
    {
      get_token();

      if (cur_tok < right_brace_limit)
        goto lab31;

      if (cur_cmd == mac_param)
      {
        s = match_token + cur_chr;
        get_token();

        if (cur_cmd == left_brace)
        {
          hashbrace = cur_tok;
          {
            q = get_avail();
            mem[p].hh.v.RH = q;
            mem[q].hh.v.LH = cur_tok;
            p = q;
          }
          {
            q = get_avail();
            mem[p].hh.v.RH = q;
            mem[q].hh.v.LH = end_match_token;
            p = q;
          }
          goto lab30;
        }

        if (t == zero_token + 9)
        {
          print_err("You already have nine parameters");
          help1("I'm going to ignore the # sign you just used.");
          error();
        }
        else
        {
          incr(t);

          if (cur_tok != t)
          {
            print_err("Parameters must be numbered consecutively");
            help2("I've inserted the digit you should have used after the #.",
                "Type `1' to delete what you did use.");
            back_error();
          }
          cur_tok = s;
        }
      }
      {
        q = get_avail();
        mem[p].hh.v.RH = q;
        mem[q].hh.v.LH = cur_tok;
        p = q;
      }
    }
lab31:
    {
      q = get_avail();
      mem[p].hh.v.RH = q;
      mem[q].hh.v.LH = 3584;
      p = q;
    }

    if (cur_cmd == right_brace)
    {
      print_err("Missing { inserted");
      incr(align_state);
      help2("Where was the left brace? You said something like `\\def\\a}',",
          "which I'm going to interpret as `\\def\\a{}'.");
      error();
      goto lab40;
    }
lab30:;
  }
  else
  {
    scan_left_brace();
  }

  unbalance = 1;

  while (true)
  {
    if (xpand)
    {
      while (true)
      {
        get_next();

        if (cur_cmd <= max_command)
          goto lab32;

        if (cur_cmd != the)
        {
          expand();
        }
        else
        {
          q = the_toks();

          if (link(temp_head) != 0)
          {
            link(p) = link(temp_head);
            p = q;
          }
        }
      }
lab32:
      x_token();
    }
    else
      get_token();

    if (cur_tok < right_brace_limit)
      if (cur_cmd < right_brace)
        incr(unbalance);
      else
      {
        decr(unbalance);

        if (unbalance == 0)
          goto lab40;
      }
    else if (cur_cmd == mac_param)
      if (macrodef)
      {
        s = cur_tok;

        if (xpand)
          get_x_token();
        else
          get_token();

        if (cur_cmd != mac_param)
          if ((cur_tok <= zero_token) || (cur_tok > t))
          {
            print_err("Illegal parameter number in definition of");
            sprint_cs(warning_index);
            help3("You meant to type ## instead of #, right?",
                "Or maybe a } was forgotten somewhere earlier, and things",
                "are all screwed up? I'm going to assume that you meant ##.");
            back_error();
            cur_tok = s;
          }
          else
            cur_tok = out_param_token - '0' + cur_chr;
      }
    {
      q = get_avail();
      mem[p].hh.v.RH = q;
      mem[q].hh.v.LH = cur_tok;
      p = q;
    }
  }
lab40:
  scanner_status = 0;

  if (hashbrace != 0)
  {
    q = get_avail();
    mem[p].hh.v.RH = q;
    mem[q].hh.v.LH = hashbrace;
    p = q;
  }
  Result = p;
  return Result;
}
/* used only in ITEX.C */
/* sec 0482 */
void read_toks_(integer n, halfword r)
{
  halfword p;
  halfword q;
  integer s;
/*  small_number m;  */
  int m; /* 95/Jan/7 */

  scanner_status = defining;
  warning_index = r;
  def_ref = get_avail();
  token_ref_count(def_ref) = 0;
  p = def_ref;
  {
    q = get_avail();
    mem[p].hh.v.RH = q;
    mem[q].hh.v.LH = 3584;
    p = q;
  }
  if ((n < 0) || (n > 15))
    m = 16;
  else
    m = n;
  s = align_state;
  align_state = 1000000L;
  do {
    begin_file_reading();
    cur_input.name_field = m + 1;
    if (read_open[m] == 2)
      if (interaction > nonstop_mode)
        if (n < 0)
        {
          print_string("");
          term_input("", 0);
        }
        else
        {
          print_ln();
          sprint_cs(r);
          {
            print_string("=");
            term_input("=", 0);
          }
          n = -1;
        }
      else
      {
        fatal_error("*** (cannot \\read from terminal in nonstop modes)");
        return;     // abort_flag set
      }
    else if (read_open[m] == 1)
      if (input_ln(read_file[m], false))
        read_open[m] = 0;
      else
      {
        (void) a_close(read_file[m]);
        read_open[m] = 2;
      }
    else
    {
      if (!input_ln(read_file[m], true))
      {
        (void) a_close(read_file[m]);
        read_open[m] = 2;
        if (align_state != 1000000L)
        {
          runaway();
          print_err("File ended within");
          print_esc("read");
          help1("This \\read has unbalanced braces.");
          align_state = 1000000L;
          error();
        }
      }
    }
    cur_input.limit_field = last;
    if ((end_line_char < 0) || (end_line_char > 255))
      decr(cur_input.limit_field);
    else
      buffer[cur_input.limit_field] = end_line_char;
    first = cur_input.limit_field + 1;
    cur_input.loc_field = cur_input.start_field;
    cur_input.state_field = new_line;
    while (true) {
      get_token();
      if (cur_tok == 0)
        goto lab30;
      if (align_state < 1000000L)
      {
        do {
          get_token();
        } while(!(cur_tok == 0));
        align_state = 1000000L;
        goto lab30;
      }
      {
        q = get_avail();
        mem[p].hh.v.RH = q;
        mem[q].hh.v.LH = cur_tok;
        p = q;
      }
    }
lab30:
    end_file_reading();
  } while(!(align_state == 1000000L));
  cur_val = def_ref;
  scanner_status = normal;
  align_state = s;
}
/* sec 0494 */
void pass_text (void)
{
  integer l;
  small_number savescannerstatus;

  savescannerstatus = scanner_status;
  scanner_status = skipping;
  l = 0;
  skip_line = line;
  while (true) {
    get_next();
    if (cur_cmd == fi_or_else)
    {
      if (l == 0)
        goto lab30;
      if (cur_chr == 2)
        decr(l);
    }
    else if (cur_cmd == if_test)
      incr(l);
  }
lab30:
  scanner_status = savescannerstatus;
}
/* sec 0497 */
void change_if_limit_(small_number l, halfword p)
{
  halfword q;
  if (p == cond_ptr)
    if_limit = l;
  else
  {
    q = cond_ptr;
    while (true) {
      if (q == 0)
      {
        confusion("if");
        return;       // abort_flag set
      }
      if (link(q) == p)
      {
        type(p) = l;
        return;
      }
      q = mem[q].hh.v.RH;
    }
  }
}
/* called from tex2.c */
/* sec 0498 */
void conditional (void)
{
  bool b;
  char r;
  integer m, n;
  halfword p, q;
  small_number savescannerstatus;
  halfword savecondptr;
  small_number thisif;
  {
    p = get_node(if_node_size);
    link(p) = cond_ptr;
    type(p) = if_limit;
    subtype(p) = cur_if;
    if_line_field(p) = if_line;
    cond_ptr = p;
    cur_if = cur_chr;
    if_limit = if_code;
    if_line = line;
  }
  savecondptr = cond_ptr;
  thisif = cur_chr;
  switch (thisif)
  {
    case if_char_code:
    case if_cat_code:
      {
        {
          get_x_token();
          if (cur_cmd == relax)
            if (cur_chr == 257)  /* if cur_chr = no_expand_flag then ... p.506 */
            {
              cur_cmd = active_char;
              cur_chr = cur_tok - 4096;
            }
        }
        if ((cur_cmd > active_char) || (cur_chr > 255))
        {
          m = relax;
          n = 256;
        }
        else
        {
          m = cur_cmd;
          n = cur_chr;
        }
        {
          get_x_token();
          if (cur_cmd == relax)
            if (cur_chr == 257)  /* if cur_chr = no_expand_flag then ... p.506 */
            {
              cur_cmd = active_char;
              cur_chr = cur_tok - 4096;
            }
        }
        if ((cur_cmd > active_char) || (cur_chr > 255))
        {
          cur_cmd = relax;
          cur_chr = 256;
        }
        if (thisif == if_char_code)
          b = (n == cur_chr); 
        else
          b = (m == cur_cmd);
      }
      break;
    case if_int_code:
    case if_dim_code:
      {
        if (thisif == if_int_code)
          scan_int();
        else
          scan_dimen(false, false, false);
        n = cur_val;
        
        do {
          get_x_token();
        } while(!(cur_cmd != spacer));

        if ((cur_tok >= 3132) && (cur_tok <= 3134))
          r = cur_tok - 3072;
        else
        {
          print_err("Missing = inserted for ");
          print_cmd_chr(if_test, thisif);
          help1("I was expecting to see `<', `=', or `>'. Didn't.");
          back_error();
          r = '=';
        }
        if (thisif == if_int_code)
          scan_int();
        else 
          scan_dimen(false, false, false);
        switch (r)
        {
          case '<':
            b = (n < cur_val);
            break;
          case '=':
            b = (n == cur_val);
            break;
          case '>':
            b = (n > cur_val);
            break;
        }
      }
      break;
    case if_odd_code:
      scan_int();
      b = odd(cur_val);
      break;
    case if_vmode_code:
      b = (abs(mode) == 1);
      break;
    case if_hmode_code:
      b = (abs(mode) == 102);
      break;
    case if_mmode_code:
      b = (abs(mode) == 203);
      break;
    case if_inner_code:
      b = (mode < 0);
      break;
    case if_void_code:
    case if_hbox_code:
    case if_vbox_code:
      {
        scan_eight_bit_int();
        p = box(cur_val);
        if (thisif == if_void_code)
          b = (p == 0);
        else if (p == 0)
          b = false;
        else if (thisif == if_hbox_code)
          b = (type(p) == hlist_node);
        else
          b = (type(p) == vlist_node);
      }
      break;
    case ifx_code:
      {
        savescannerstatus = scanner_status;
        scanner_status = 0;
        get_next();
        n = cur_cs;
        p = cur_cmd;
        q = cur_chr;
        get_next();
        if (cur_cmd != p)
          b = false;
        else if (cur_cmd < call)
          b = (cur_chr == q);
        else
        {
          p = link(cur_chr);
          q = link(equiv(n));
          if (p == q)
            b = true;
          else
          {
            while ((p != 0) && (q != 0))
              if (info(p) != info(q))
                p = 0;
              else
              {
                p = link(p);
                q = link(q);
              }
            b = ((p == 0) && (q == 0));
          }
        }
        scanner_status = savescannerstatus;
      }
      break;
    case if_eof_code:
      {
        scan_four_bit_int();
        b = (read_open[cur_val] == 2);
      }
      break;
    case if_true_code:
      b = true;
      break;
    case if_false_code:
      b = false;
      break;
    case if_case_code:
      {
        scan_int();
        n = cur_val;
        if (tracing_commands > 1)
        {
          begin_diagnostic();
          print_string("{case ");
          print_int(n); 
          print_char('}');
          end_diagnostic(false);
        }
        while (n != 0) {
          pass_text();
          if (cond_ptr == savecondptr)
            if (cur_chr == or_code)
              decr(n);
            else 
              goto lab50;
          else if (cur_chr == fi_code)
          {
            p = cond_ptr;
            if_line = if_line_field(p);
            cur_if = subtype(p);
            if_limit = type(p);
            cond_ptr = link(p);
            free_node(p, if_node_size);
          }
        }
        change_if_limit(or_code, savecondptr);
        return;
      }
      break;
  }
  if (tracing_commands > 1)
  {
    begin_diagnostic();
    if (b)
      print_string("{true}");
    else
      print_string("{false}");
    end_diagnostic(false);
  }
  if (b)     /* b may be used without ... */
  {
    change_if_limit(else_code, savecondptr);
    return;
  }
  while (true) {
    pass_text();
    if (cond_ptr == savecondptr)
    {
      if (cur_chr != or_code)
        goto lab50;
      print_err("Extra ");
      print_esc("or");
      help1("I'm ignoring this; it doesn't match any \\if.");
      error();
    }
    else if (cur_chr == fi_code)
    {
      p = cond_ptr;
      if_line = if_line_field(p);
      cur_if = subtype(p);
      if_limit = type(p);
      cond_ptr = link(p);
      free_node(p, if_node_size);
    }
  }
lab50:
  if (cur_chr == fi_code)
  {
    p = cond_ptr;
    if_line = if_line_field(p);
    cur_if = subtype(p);
    if_limit = type(p);
    cond_ptr = link(p);
    free_node(p, if_node_size);
  }
  else
    if_limit = fi_code;
}
/* sec 0515 */
void begin_name (void)
{
  area_delimiter = 0; /* index between `file area' and `file name' */
  ext_delimiter = 0;  /* index between `file name' and `file extension' */
}
/* This gathers up a file name and makes a string of it */
/* Also tries to break it into `file area' `file name' and `file extension' */
/* Used by scan_file_name and prompt_file_name */
/* We assume tilde has been converted to pseudo_tilde and space to pseudo_space */
/* returns false if it is given a space character - end of file name */
/* sec 0516 */
bool more_name_(ASCII_code c)
{
  register bool Result;

  if (quoted_file_name == 0 && c == ' ')
    Result = false;
  else if (quoted_file_name != 0 && c == '"')
  {
    quoted_file_name = 0; /* catch next space character */
    Result = true;    /* accept ending quote, but throw away */
  }
  else
  {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  convert pseudo tilde back to '~' 95/Sep/26 */ /* moved here 97/June/5 */
/*  if (pseudo_tilde != 0 && c == pseudo_tilde) c = '~'; */
/*  convert pseudo space back to ' ' 97/June/5 */ /* moved here 97/June/5 */
/*  if (pseudo_space != 0 && c == pseudo_space) c = ' '; */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */      
    str_room(1);
    append_char(c);
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
//  if ((c == 47))   /* / */
//  for DOS/Windows
    if ((c == '/' || c == '\\' || c == ':')) /* 94/Mar/1 */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    {
      area_delimiter = cur_length;
      ext_delimiter = 0;
    } 
    else if (c == '.')
      ext_delimiter = cur_length;
    Result = true;
  }
  return Result;
}
/******************************** 2000 August 15th start ***********************/

// The following code is to save string space used by TeX for filenames
// Not really critical in a system that has dynamic memory allocation
// And may slow it down slightly - although this linear search only
// occurs when opening a file, which is somewhat slow inany case...

// see if string from str_pool[start] to str_pool[end]
// occurs elsewhere in string pool - returns string number
// returns -1 if not found in string pool 2000 Aug 15

int find_string (int start, int end)
{
  int k, nlen = end - start;
  char *s;

//  int trace_flag = 1;     // debugging only

  if (trace_flag)
  {
    sprintf(log_line, "\nLOOKING for string (str_ptr %d nlen %d) ", str_ptr, end-start);
    s = log_line + strlen(log_line);
    strncpy(s, (const char *) str_pool + start, nlen);
    strcpy(s+nlen, "");
    show_line(log_line, 0);
  }

//  avoid problems with(cur_name == flushablestring)by going only up to str_ptr-1
//  code in new_font (tex8.c) will take care of reuse of font name already
//  for (k = 0; k < str_ptr; k++) {
  for (k = 0; k < str_ptr - 1; k++)
  {
    if (length(k) != nlen) continue;
    if (strncmp((const char *) str_pool + start, (const char *) str_pool + str_start[k], nlen) == 0) {
      if (trace_flag) {
        sprintf(log_line, "\nFOUND the string %d (%d) ", k, str_start[k+1]-str_start[k]);
        s = log_line + strlen(log_line);
        strncpy(s, (const char *)str_pool + start, nlen);
        strcpy(s+nlen, "\n");
        show_line(log_line, 0);
      }
      return k;     // return number of matching string
    }
  }
  if (trace_flag) {
    sprintf(log_line, "\nNOT FOUND string ");
    s = log_line + strlen(log_line);
    strncpy(s, (const char*)str_pool + start, nlen);
    strcpy(s+nlen, "\n");
    show_line(log_line, 0);
  }
  return -1;          // no match found
}

// snip out the string from str_pool[start] to str_pool[end]
// and move everything above it down 2000 Aug 15

void remove_string (int start, int end)
{
  int nlen = pool_ptr - end;  // how many bytes to move down
  char *s;
  
//  int trace_flag=1;   // debugging only
//  if (end < start) show_line("\nEND < START", 1);
//  if (pool_ptr < end) show_line("\nPOOLPTR < END", 1);

  if (trace_flag) {
    int n = end-start;
    sprintf(log_line, "\nSTRIPPING OUT %d %d ", n, nlen);
    s = log_line + strlen(log_line);
    strncpy(s, (const char *)str_pool + start, n);
    strcpy(s+n, "\n");
    show_line(log_line, 0);
  }
  if (nlen > 0) memcpy(str_pool+start, str_pool+end, nlen);
  pool_ptr = start + nlen;    // poolprt - (end-start);
}

void show_string (int k)
{   // debugging code
  int nlen = length(k);
  char *s;
  
  sprintf(log_line, "\nSTRING %5d (%3d) %5d--%5d ",
      k, nlen, str_start[k], str_start[k+1]);
  s = log_line + strlen(log_line);      
  strncpy(s, (const char *)str_pool + str_start[k], nlen);
  strcpy(s + nlen, "");
  show_line(log_line, 0);
}

void show_all_strings (void)
{   // debugging code
  int k;
  for (k = 0; k < str_ptr; k++) show_string(k);
}

// int notfirst=0;    // debugging only

/********************************** 2000 August 15 end ****************************/
/* sec 0517 */
void end_name (void) 
{ 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATESTRING
  if (str_ptr + 3 > current_max_strings)
/*    str_start = realloc_str_start(increment_max_strings); */
    str_start = realloc_str_start(increment_max_strings + 3);
  if (str_ptr + 3 > current_max_strings)
  {  /* in case it failed 94/Jan/24 */
    overflow("number of strings", current_max_strings - init_str_ptr);  /* 97/Mar/7 */
    return;     // abort_flag set
  }
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (str_ptr + 3 > max_strings)
  {
    overflow("number of strings", max_strings - init_str_ptr); /* number of strings */
    return;     // abort_flag set
  }
#endif

//  if (notfirst++ == 0) show_all_strings();  // debugging only
  
  if (area_delimiter == 0)   // no area delimiter ':' '/' or '\' found
    cur_area = 335;     // "" default area 
  else {
    if (save_strings_flag &&
      (cur_area = find_string(str_start[str_ptr], str_start[str_ptr]+area_delimiter)) > 0) {
      remove_string(str_start[str_ptr], str_start[str_ptr] + area_delimiter);
      area_delimiter = 0; // area_delimiter - area_delimiter;
      if (ext_delimiter != 0) ext_delimiter = ext_delimiter - area_delimiter;
//      str_start[str_ptr + 1]= str_start[str_ptr]+ area_delimiter; // test only
//      incr(str_ptr);    // test only
    }
    else {          // carve out string for "cur_area"
      cur_area = str_ptr; 
      str_start[str_ptr + 1]= str_start[str_ptr]+ area_delimiter; 
      incr(str_ptr);
    }
  } 
  if (ext_delimiter == 0){ // no extension delimiter '.' found
    cur_ext = 335;        // "" default extension 
  if (save_strings_flag &&
      (cur_name = find_string(str_start[str_ptr], pool_ptr)) > 0) {
    remove_string(str_start[str_ptr], pool_ptr);
//    (void) make_string();  // test only
  }
  else            // Make string from str_start[str_ptr]to pool_ptr
    cur_name = make_string();
  } 
  else {            // did find an extension
  if (save_strings_flag &&
      (cur_name = find_string(str_start[str_ptr], str_start[str_ptr] + ext_delimiter - area_delimiter-1)) > 0) {
    remove_string(str_start[str_ptr], str_start[str_ptr] + ext_delimiter - area_delimiter - 1);
//    str_start[str_ptr + 1]= str_start[str_ptr]+ ext_delimiter - area_delimiter - 1;   // test only
//    incr(str_ptr);    // test only
  }
  else {            // carve out string for "cur_name"
    cur_name = str_ptr; 
    str_start[str_ptr + 1]= str_start[str_ptr]+ ext_delimiter - area_delimiter - 1; 
    incr(str_ptr);
  }
  if (save_strings_flag &&
      (cur_ext = find_string(str_start[str_ptr], pool_ptr)) > 0) {
    remove_string(str_start[str_ptr], pool_ptr);
//    (void) make_string();  // test only
  }
  else            // Make string from str_start[str_ptr]to pool_ptr
    cur_ext = make_string();
  }
}

/* n current name, a current area, e current extension */
/* result in name_of_file[] */
/* sec 0519 */
void pack_file_name_(str_number n, str_number a, str_number e)
{
  integer k;
  ASCII_code c;
  pool_pointer j;
  k = 0;

  for (j = str_start[a]; j <= str_start[a + 1] - 1; j++)
  {
    c = str_pool[j];
    incr(k);
    if (k <= PATHMAX)
      name_of_file[k] = xchr[c];
  }
  for (j = str_start[n]; j <= str_start[n + 1] - 1; j++)
  {
    c = str_pool[j];
    incr(k);
    if (k <= PATHMAX)
      name_of_file[k] = xchr[c];
  }
  for (j = str_start[e]; j <= str_start[e + 1] - 1; j++)
  {
    c = str_pool[j];
    incr(k);
    if (k <= PATHMAX)
      name_of_file[k] = xchr[c];
  }

  if (k < PATHMAX)
    name_length = k;
  else
    name_length = PATHMAX - 1;
/*  pad it out with spaces ... what for ? in case we modify and forget  ? */
  for (k = name_length + 1; k <= PATHMAX; k++) name_of_file[k]= ' ';
  name_of_file[PATHMAX]= '\0';    /* paranoia 94/Mar/24 */
  {
    name_of_file [name_length+1] = '\0';
    if (trace_flag)
    {
      sprintf(log_line, " pack_file_name `%s' (%d) ", name_of_file + 1, name_length); /* debugging */
      show_line(log_line, 0);
    }
    name_of_file [name_length + 1] = ' ';
  }
}
/* Called only from two places tex9.c for format name - specified and default */
/* for specified format name args are 0, a, b name in buffer[a] --- buffer[b] */
/* for default args are format_default_length-4, 1, 0 */
/* sec 0523 */
void pack_buffered_name_(small_number n, integer a, integer b)
{
  integer k;
  ASCII_code c;
  integer j;
  if (n + b - a + 5 > PATHMAX)
    b = a + PATHMAX - n - 5;
  k = 0;
/*  This loop kicks in when we want the default format name */
  for (j = 1; j <= n; j++)
  {
    c = xord[TEX_format_default[j]];
    incr(k);
    if (k <= PATHMAX)
      name_of_file[k] = xchr[c];
  }
/*  This loop kicks in when we want a specififed format name */
  for (j = a; j <= b; j++)
  {
    c = buffer[j];
    incr(k);
    if (k <= PATHMAX)
      name_of_file[k] = xchr[c];
  }
/*  This adds the extension from the default format name */
  for (j = format_default_length - 3; j <= format_default_length; j++)
  {
    c = xord[TEX_format_default[j]];
    incr(k);
    if (k <= PATHMAX)
      name_of_file[k] = xchr[c];
  }
  if (k < PATHMAX)
    name_length = k;
  else
    name_length = PATHMAX - 1;
/*  pad it out with spaces ... what for ? */
  for (k = name_length + 1; k <= PATHMAX; k++)
    name_of_file[k]= ' ';
  name_of_file[PATHMAX]= '\0';    /* paranoia 94/Mar/24 */
}
/* sec 0525 */
str_number make_name_string (void)
{
  register str_number Result;
  integer k;

#ifdef ALLOCATESTRING
  if (pool_ptr + name_length > current_pool_size)
    str_pool = realloc_str_pool(increment_pool_size + name_length);

  if (str_ptr == current_max_strings)
    str_start = realloc_str_start(increment_max_strings);

  if ((pool_ptr + name_length > current_pool_size) ||
    (str_ptr == current_max_strings) || (cur_length > 0))
#else
  if ((pool_ptr + name_length > pool_size) || (str_ptr == max_strings) ||
      (cur_length > 0))
#endif
    Result = 63;
  else {
    for (k = 1; k <= name_length; k++)
    {
      str_pool[pool_ptr]= xord[name_of_file[k]];
//      sprintf(log_line, "%d => %d ", name_of_file[k], xord[name_of_file[k]]);
//      show_line(log_line, 0);  // debugging only
      incr(pool_ptr);
    }
    Result = make_string();
  }
  return Result;
}
/* sec 0525 */
str_number a_make_name_string_(alpha_file * f)
{
  register str_number Result;
  Result = make_name_string();
  return Result;
}   /* f unreferenced ? bkph */
/* sec 0525 */
str_number b_make_name_string_(byte_file * f)
{
  register str_number Result;
  Result = make_name_string();
  return Result; 
}   /* f unreferenced ? bkph */
/* sec 0525 */
str_number w_make_name_string_(word_file * f)
{
  register str_number Result;
  Result = make_name_string();
  return Result;
}   /* f unreferenced ? bkph */

/* Used by start_input to scan file name on command line */
/* Also in tex8.c new_font_, open_or_close_in, and do_extension */
/* sec 0526 */
void scan_file_name (void)
{
  name_in_progress = true;
  begin_name();
  do {
    get_x_token(); 
  } while (!(cur_cmd != spacer));
  quoted_file_name = 0;         /* 98/March/15 */
  if (allow_quoted_names)
  {       /* check whether quoted name */
    if (cur_chr == '"')
    {
      quoted_file_name = 1;
      get_x_token();
    }
  }
  while (true) {
    if ((cur_cmd > other_char)||(cur_chr > 255)) 
    {
      back_input();  /* not a character put it back and leave */
      goto lab30; 
    } 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  convert tilde '~' to pseudo tilde */
/*  if (pseudo_tilde != 0 && cur_chr == '~') cur_chr = pseudo_tilde; */
/*  convert space ' ' to pseudo space */
/*  if (pseudo_space != 0 && cur_chr == ' ') cur_chr = pseudo_space; */
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (!more_name(cur_chr))    /* up to next white space */
      goto lab30;
    get_x_token();
  }
lab30:
  end_name();
  name_in_progress = false;
}
/* argument is string .fmt, .log, or .dvi */
/* sec 0529 */
void pack_job_name_(str_number s)
{
  cur_area = 335;       /* "" */
  cur_ext = s;
  cur_name = job_name;
  pack_file_name(cur_name, cur_area, cur_ext);
}

/**********************************************************************/
/* show TEXINPUTS=... or format specific  */
/* only show this if name was not fully qualified ? */
void show_tex_inputs (void)
{     /* 98/Jan/28 */
  char *s, *t, *v;
  s = "TEXINPUTS";        /* default */
  if (format_specific) {
    s = format_name;                /* try specific */
    if (grabenv(s) == NULL) s = "TEXINPUTS";  /* no format specific */
  }

  if (grabenv(s) == NULL) s = "TEXINPUT";     /* 94/May/19 */

  print_nl("  ");
  print_char(' ');
  print_char(40);   /*(*/
  t = s;
  while (*t > '\0') print_char(*t++);
  print_char('=');
  v = grabenv(s);
  if (v != NULL) {
    t = v;
    while (*t > '\0') print_char(*t++);
  }
  print_char(')');
}

/**********************************************************************/
/* sec 0530 */
void prompt_file_name_(str_number s, str_number e)/*  s - what can't be found, e - default */ 
{
  integer k;
  if (interaction == scroll_mode);
  if (s == 781)
    print_err("I can't find file `");
  else
    print_err("I can't write on file `");
  print_file_name(cur_name, cur_area, cur_ext);
  print_string("'.");
  if (s == 781)
  {   /* input file name */
    if (cur_area == 335)
    {    /* "" only if path not specified */
      if (show_texinput_flag)
        show_tex_inputs();
    }
  }
  if (e == 785)    /* .tex */
    show_context();
  print_nl("Please type another ");
  print(s);
  if (interaction < 2)
  {
    fatal_error("*** (job aborted, file error in nonstop mode)");
    return;     // abort_flag set
  }
  if (!knuth_flag)
#ifdef _WINDOWS
    show_line(" (or ^z to exit)", 0);
#else
    show_line(" (or Ctrl-Z to exit)", 0);
#endif
  {
    print_string(": ");
    term_input(": ", 0);
  }
/*  should we deal with tilde and space in file name here ??? */
  {
    begin_name();
    k = first;
/*  step over leading spaces ... */
    while ((buffer[k]== 32) && (k < last))
      incr(k);
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    quoted_file_name = 0;         /* 98/March/15 */
    if (allow_quoted_names && k < last)
    { /* check whether quoted name */
      if (buffer[k]== '"')
      {
        quoted_file_name = 1;
        incr(k);
      }
    }
    while (true) {
      if (k == last)
        goto lab30;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  convert tilde '~' to pseudo tilde */
      if (pseudo_tilde != 0 && buffer[k]== '~')
        buffer[k]= pseudo_tilde;
/*  convert space ' ' to pseudo space */
      if (pseudo_space != 0 && buffer[k]== ' ')
        buffer[k]= pseudo_space;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
      if (!more_name(buffer[k]))
        goto lab30;
      incr(k);
    }
lab30:
    end_name();
  }
  if (cur_ext == 335)    /* "" */
    cur_ext = e;      /* use default extension */
  pack_file_name(cur_name, cur_area, cur_ext);
}
/* sec 0534 */
void open_log_file (void)
{
  char old_setting;
  integer k;
  integer l;
  ccharpointer months;

  old_setting = selector;

  if (job_name == 0)
    job_name = 790;   /* default:  texput */
  pack_job_name(791); /* .log */
  while (!a_open_out(log_file)) {
    selector = term_only;
    prompt_file_name(793, 791); /* transcript file name  texput */
  }
  texmf_log_name = a_make_name_string(log_file);
  selector = log_only;
  log_opened = true;
  {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
//  for our version DOS/Windows
  if (want_version) {
//    showversion (log_file);       /* in local.c - bkph */
//    showversion (stdout);
    stamp_it(log_line);         // ??? use log_line ???
    strcat(log_line, "\n");
    (void) fputs(log_line, log_file);
//    show_line(buffer, 0);        // ??? show on screen as well
//    print_ln(); 
    stampcopy(log_line);
    strcat(log_line, "\n");
//    show_line(buffer, 0);        // ??? show on screen as well
    (void) fputs(log_line, log_file);
//    print_ln(); 
  }
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  also change following in itex.c - bkph */
  (void) fputs(tex_version,  log_file); 
  (void) fprintf(log_file, " (%s %s)", application, yandyversion);
  if (format_ident > 0)
    slow_print(format_ident);     /* bkph */
  print_string("  ");
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (civilize_flag)
    print_int(year);
  else
    print_int(day);
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    print_char(' ');
    months = " JANFEBMARAPRMAYJUNJULAUGSEPOCTNOVDEC";
    for (k = 3 * month - 2; k <= 3 * month; k++)
      (void) putc(months[k],  log_file);
    print_char(' ');
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if (civilize_flag)
    print_int(day);
  else
    print_int(year);
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    print_char(' ');
    print_two(tex_time / 60);  /* hour */
    print_char(':');
    print_two(tex_time % 60);  /* minute */
  }
  input_stack[input_ptr] = cur_input;
  print_nl("**");
  l = input_stack[0].limit_field;
  if (buffer[l] == end_line_char)
    decr(l);
  for (k = 1; k <= l; k++)
    print(buffer[k]);
  print_ln(); 
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/* a good place to show the fmt file name or pool file name ? 94/June/21 */
  if (show_fmt_flag)
  {
    if (string_file != NULL)
    {
      fprintf(log_file, "(%s)\n", string_file);
      free(string_file);  /* this was allocated by strdup in openinou */
      string_file = NULL;   /* for safety */
    }
    if (format_file != NULL)
    {
      fprintf(log_file, "(%s)\n", format_file);
      free(format_file);  /* this was allocated by strdup in openinou */
      format_file = NULL;   /* for safety */
    }
  }
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  selector = old_setting + 2;
}

/**************************** start of insertion 98/Feb/7 **************/

// Attempt to deal with foo.bar.tex given as foo.bar on command line
// Makes copy of job_name with extension

void morenamecopy(ASCII_code c)
{
#ifdef ALLOCATESTRING
  if (pool_ptr + 1 > current_pool_size)
    str_pool = realloc_str_pool (increment_pool_size);
  if (pool_ptr + 1 > current_pool_size)   { /* in case it failed 94/Jan/24 */
    overflow("pool size", current_pool_size - init_pool_ptr); /* 97/Mar/7 */
    return;     // abort_flag set
  }
#else
  if (pool_ptr + 1 > pool_size){
    overflow("pool size", pool_size - init_pool_ptr); /* pool size */
    return;     // abort_flag set
  }
#endif
  str_pool[pool_ptr]= c; 
  incr(pool_ptr); 
} 

int endnamecopy(void)
{
#ifdef ALLOCATESTRING
    if (str_ptr + 1 > current_max_strings)
      str_start = realloc_str_start(increment_max_strings + 1);
    if (str_ptr + 1 > current_max_strings) { /* in case it failed 94/Jan/24 */
      overflow("number of strings", current_max_strings - init_str_ptr);  /* 97/Mar/7 */
      return 0;     // abort_flag set
    }
#else
    if (str_ptr + 1 > max_strings){
      overflow("number of strings", max_strings - init_str_ptr); /* number of strings */
      return 0;     // abort_flag set
    }
#endif
    return make_string();
} 

void jobnameappend (void)
{ /* add extension to job_name */
  int k, n;
/*  copy job_name */
  k = str_start[job_name];
  n = str_start[job_name + 1];
  while (k < n) morenamecopy(str_pool[k++]);
/*  copy `extension' */
  k = str_start[cur_ext];
  n = str_start[cur_ext + 1];
  while (k < n) morenamecopy(str_pool[k++]);
  job_name = endnamecopy();
}

/**************************** end of insertion 98/Feb/7 **************/
/* sec 0537 */
void start_input (void)
{
  bool addedextension = false;
  scan_file_name();
  pack_file_name(cur_name, cur_area, cur_ext); 

  while (true) {        /* loop until we get a valid file name */
    addedextension = false;
    begin_file_reading(); 
/* *** *** *** *** *** following is new in 3.14159 *** *** *** *** *** *** */
/*  if current extension is *not* empty, try to open using name as is */
/*  string 335 is "" the empty string */
    if ((cur_ext != 335) && a_open_in(input_file[cur_input.index_field], TEXINPUTPATH))
      goto lab30;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
/*  we get here if extension is "", or file with extension failed to open */
/*  if current extension is not `tex,' and `tex' is not irrelevant, try it */
/*  string 785 is .tex */
/*    (! extensionirrelevantp(name_of_file, "tex"))){ */
    if ((cur_ext != 785) && (name_length + 5 < PATHMAX) &&
        (! extensionirrelevantp(name_of_file, name_length, "tex")))
    {
      name_of_file[name_length + 1] = '.';
      name_of_file[name_length + 2] = 't';
      name_of_file[name_length + 3] = 'e';
      name_of_file[name_length + 4] = 'x';
      name_of_file[name_length + 5] = ' ';  /* 96/Jan/20 ??? */
      name_length = name_length + 4;

      addedextension = true;
/* *** *** *** ***  following new in 3.14159 *** *** *** *** *** *** *** */
      if (a_open_in(input_file[cur_input.index_field], TEXINPUTPATH))
        goto lab30;
      name_length = name_length - 4;      /* strip extension again */
      name_of_file[name_length + 1] = ' ';  /* ' ' */
      addedextension = false;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    }
/* *** *** *** *** major changes here in 3.14159 *** *** *** *** *** *** */
/*  string 335 is "" the empty string */
    if ((cur_ext == 335) && a_open_in(input_file[cur_input.index_field], TEXINPUTPATH))
      goto lab30;
    if (maketextex() && a_open_in(input_file[cur_input.index_field], TEXINPUTPATH))
      goto lab30;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    end_file_reading();
    prompt_file_name(781, 785); /* input file name  .tex */
  }   /* end of while(true)trying to get valid file name */

/* maybe set  pseudo_tilde = 0  at this point ? 95/Sep/26 */
lab30: 
  cur_input.name_field = a_make_name_string(input_file[cur_input.index_field]);

  if (job_name == 0)       /* only the first time */
  {
    job_name = cur_name;        /* here we set the job_name */
/*  did file name have an `extension' already and we added ".tex" ? */
    if (cur_ext != 335 && addedextension)     /* 98/Feb/7 */
      jobnameappend();   /* append `extension' to job_name */
    open_log_file();
  }
  if (term_offset + length(cur_input.name_field) > max_print_line - 2) /* was 3 ? */
    print_ln();
  else if ((term_offset > 0) || (file_offset > 0))
    print_char(' ');
  print_char('(');
//  print_char(64);       // debugging only marker
  incr(open_parens);
  if (open_parens > max_open_parens)
    max_open_parens = open_parens;    /* 1999/Jan/17 */
  slow_print(cur_input.name_field);
//  print_char(64);       // debugging only marker
#ifndef _WINDOWS
  fflush(stdout);
#endif
  cur_input.state_field = new_line;
  {
    line = 1;
    if (input_ln(input_file[cur_input.index_field], false));
    firm_up_the_line();
    if ((end_line_char < 0) || (end_line_char > 255))
      decr(cur_input.limit_field);
    else
      buffer[cur_input.limit_field] = end_line_char;
    first = cur_input.limit_field + 1;
    cur_input.loc_field = cur_input.start_field;
  }
}

/**********************************************************************/
/* show TEXFONTS=... or format specific  */
/* only show this if name was not fully qualified ? */
void show_tex_fonts (void)
{     /* 98/Jan/28 */
  char *s, *t, *v, *u;
  int n;
  s = "TEXFONTS";
  if (encoding_specific) {
    u = encoding_name;                /* try specific */
    if ((t = grabenv(u)) != NULL) {
      if (strchr(t, ':') != NULL &&
        sscanf(t, "%d", &n) == 0) {
        s = u;        /* look here instead of TEXFONTS=... */
      }
    }
  }
  print_nl("  ");
  print_char(' ');
  print_char('(');
  t = s;
  while (*t > '\0') print_char(*t++);
  print_char('=');
  v = grabenv(s);
  if (v != NULL) {
    t = v;
    while (*t > '\0') print_char(*t++);
  }
  print_char(')');
}

/**********************************************************************/

/* called only from tex8.c */
/* sec 0560 */
internal_font_number read_font_info_(halfword u, str_number nom, str_number aire, scaled s)
{
  register internal_font_number Result;
  font_index k;
  bool fileopened;
/*  halfword lf, lh, bc, ec, nw, nh, nd, ni, nl, nk, ne, np;  */
  halfword lf, lh, nw, nh, nd, ni, nl, nk, ne, np;
/*  halfword bc, ec; */
  int bc, ec;             /* 95/Jan/7 */
  internal_font_number f;
  internal_font_number g;
  eight_bits a, b, c, d;
  ffourquarters qw;
  scaled sw;
  integer bchlabel;
  short bchar;
  scaled z;
  integer alpha;
  char beta;

  g = 0;
  fileopened = false;
  pack_file_name(nom, aire, 805); /* .tfm */
  if (!b_open_in(tfm_file)) /* new in C version d */
  {
    if (maketextfm ())
    {
      if (!b_open_in(tfm_file))
        goto lab11;
    }
    else goto lab11;
  } 
/*   was just: goto lab11; */
  fileopened = true; 
  {
/*  tfm_temp = getc(tfm_file);  */ /* done already in open_input, but why? */
    {
      lf = tfm_temp;
      if (lf > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      lf = lf * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      lh = tfm_temp;
      if (lh > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      lh = lh * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      bc = tfm_temp;
      if (bc > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      bc = bc * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      ec = tfm_temp;
      if (ec > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      ec = ec * 256 + tfm_temp;
    }
    if ((bc > ec + 1)||(ec > 255))
      goto lab11;
    if (bc > 255)
    {
      bc = 1;
      ec = 0;
    }
    tfm_temp = getc(tfm_file);
    {
      nw = tfm_temp;
      if (nw > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      nw = nw * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      nh = tfm_temp;
      if (nh > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      nh = nh * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      nd = tfm_temp;
      if (nd > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      nd = nd * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      ni = tfm_temp;
      if (ni > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      ni = ni * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      nl = tfm_temp;
      if (nl > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      nl = nl * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      nk = tfm_temp;
      if (nk > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      nk = nk * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      ne = tfm_temp;
      if (ne > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      ne = ne * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    {
      np = tfm_temp;
      if (np > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      np = np * 256 + tfm_temp;
    }
    if (lf != 6 + lh + (ec - bc + 1) + nw + nh + nd + ni + nl + nk + ne + np)
      goto lab11;
    if ((nw == 0) || (nh == 0) || (nd == 0) || (ni == 0))
      goto lab11;
  }
  lf = lf - 6 - lh;
  if (np < 7)
    lf = lf + 7 - np;
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
#ifdef ALLOCATEFONT
  if ((fmem_ptr + lf > current_font_mem_size))   /* 93/Nov/28 */
    font_info = realloc_font_info (increment_font_mem_size + lf);
  if ((font_ptr == font_max) || (fmem_ptr + lf > current_font_mem_size))
#else
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
  if ((font_ptr == font_max) || (fmem_ptr + lf > font_mem_size))
#endif
  {
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    if (trace_flag)
    {
      sprintf(log_line, "font_ptr %d font_max %d fmem_ptr %d lf %d font_mem_size %d\n",
          font_ptr, font_max, fmem_ptr, lf, font_mem_size); /* debugging */
      show_line(log_line, 0);
    }
/* *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** *** */
    print_err("Font ");
    sprint_cs(u);
    print_char('=');
    print_file_name(nom, aire, 335); /* "" */
    if (s >= 0)
    {
      print_string(" at ");
      print_scaled(s);
      print_string("pt");
    }
    else if (s != -1000)
    {
      print_string(" scaled ");
      print_int(- (integer) s);
    }
    print_string(" not loaded: Not enough room left");
    help4("I'm afraid I won't be able to make use of this font,",
        "because my memory for character-size data is too small.",
        "If you're really stuck, ask a wizard to enlarge me.",
        "Or maybe try `I\\font<same font id>=<name of loaded font>'.");
    error();
    goto lab30;
  }
  f = font_ptr + 1;
  char_base[f] = fmem_ptr - bc;
  width_base[f] = char_base[f] + ec + 1;
  height_base[f] = width_base[f] + nw;
  depth_base[f] = height_base[f] + nh;
  italic_base[f] = depth_base[f] + nd;
  lig_kern_base[f] = italic_base[f] + ni;
  kern_base[f] = lig_kern_base[f] + nl - 256 * (128);
  exten_base[f] = kern_base[f] + 256 *(128) + nk;
  param_base[f] = exten_base[f] + ne;
  {
    if (lh < 2)
      goto lab11;
/*  build the font checksum now */
    {
      tfm_temp = getc(tfm_file);
      a = tfm_temp;
      qw.b0 = a;
      tfm_temp = getc(tfm_file);
      b = tfm_temp;
      qw.b1 = b;
      tfm_temp = getc(tfm_file);
      c = tfm_temp;
      qw.b2 = c;
      tfm_temp = getc(tfm_file);
      d = tfm_temp;
      qw.b3 = d;
      font_check[f] = qw;
    }
    tfm_temp = getc(tfm_file);
    {
      z = tfm_temp;
      if (z > 127)
        goto lab11;
      tfm_temp = getc(tfm_file);
      z = z * 256 + tfm_temp;
    }
    tfm_temp = getc(tfm_file);
    z = z * 256 + tfm_temp;
    tfm_temp = getc(tfm_file);
    z =(z * 16) + (tfm_temp / 16);
    if (z < 65536L)
      goto lab11; 
    while (lh > 2) {
      tfm_temp = getc(tfm_file);
      tfm_temp = getc(tfm_file);
      tfm_temp = getc(tfm_file);
      tfm_temp = getc(tfm_file);
      decr(lh);
    }
    font_dsize[f] = z;
    if (s != -1000)
      if (s >= 0)
        z = s;
      else
        z = xn_over_d(z, - (integer) s, 1000);
    font_size[f] = z;
  }

  for (k = fmem_ptr; k <= width_base[f] - 1; k++)
  {
    {
      tfm_temp = getc(tfm_file);
      a = tfm_temp;
      qw.b0 = a;
      tfm_temp = getc(tfm_file);
      b = tfm_temp;
      qw.b1 = b;
      tfm_temp = getc(tfm_file);
      c = tfm_temp;
      qw.b2 = c;
      tfm_temp = getc(tfm_file);
      d = tfm_temp;
      qw.b3 = d;
      font_info[k].qqqq = qw;
    }
    if ((a >= nw) || (b / 16 >= nh) || (b % 16 >= nd) || (c / 4 >= ni))
      goto lab11;
    switch (c % 4)
    {
      case 1 :
        if (d >= nl)
          goto lab11;
        break;
      case 3 :
        if (d >= ne)
          goto lab11;
        break;
      case 2 :
        {
          {
            if ((d < bc)||(d > ec))
              goto lab11;
          }
          while (d < k + bc - fmem_ptr) {
            qw = font_info[char_base[f]+ d].qqqq;
            if (((qw.b2)% 4)!= 2)
              goto lab45;
            d = qw.b3;
          }
          if (d == k + bc - fmem_ptr)
            goto lab11;
lab45:; 
        }
        break;
      default:
        break;
    }
  }
  {
    {
      alpha = 16;
      while (z >= 8388608L) {   /* 2^23 */
        z = z / 2;
        alpha = alpha + alpha;
      }
      beta = (char) (256 / alpha);
      alpha = alpha * z;
    }

    for (k = width_base[f]; k <= lig_kern_base[f] - 1; k++)
    {
      tfm_temp = getc(tfm_file);
      a = tfm_temp;
      tfm_temp = getc(tfm_file);
      b = tfm_temp;
      tfm_temp = getc(tfm_file);
      c = tfm_temp;
      tfm_temp = getc(tfm_file);
      d = tfm_temp;
      sw = (((((d * z) / 256) + (c * z)) / 256) + (b * z)) / beta;
      if (a == 0)
        font_info[k].cint = sw;
      else if (a == 255)
        font_info[k].cint = sw - alpha;
      else
        goto lab11;
    }
    if (font_info[width_base[f]].cint != 0)
      goto lab11;
    if (font_info[height_base[f]].cint != 0)
      goto lab11;
    if (font_info[depth_base[f]].cint != 0)
      goto lab11;
    if (font_info[italic_base[f]].cint != 0)
      goto lab11;
  }
/*  read ligature/kern program */
  bchlabel = 32767;     /* '77777 */
  bchar = 256;
  if (nl > 0)
  {
    for (k = lig_kern_base[f]; k <= kern_base[f] + 256 * (128)- 1; k++)
    {
      {
        tfm_temp = getc(tfm_file);
        a = tfm_temp;
        qw.b0 = a;
        tfm_temp = getc(tfm_file);
        b = tfm_temp;
        qw.b1 = b;
        tfm_temp = getc(tfm_file);
        c = tfm_temp;
        qw.b2 = c;
        tfm_temp = getc(tfm_file);
        d = tfm_temp;
        qw.b3 = d;
        font_info[k].qqqq = qw; /* store_four_quarters(font_info[k].qqqq */
      }
      if (a > 128)
      {
        if (256 * c + d >= nl)
          goto lab11;       /* error in TFM, abort */
        if (a == 255)
          if (k == lig_kern_base[f])
            bchar = b;
      }
      else
      {
        if (b != bchar)
        {
          {
            if ((b < bc) || (b > ec))  /* check-existence(b) */
              goto lab11;         /* error in TFM, abort */
          }
          qw = font_info[char_base[f] + b].qqqq;
          if (!(qw.b0 > 0))
            goto lab11;         /* error in TFM, abort */
        }
        if (c < 128)
        {
          {
            if ((d < bc) || (d > ec))  /* check-existence(d) */
              goto lab11;         /* error in TFM, abort */
          }
          qw = font_info[char_base[f] + d].qqqq;
          if (!(qw.b0 > 0))
            goto lab11;         /* error in TFM, abort */
        }
        else if (256 * (c - 128) + d >= nk)
          goto lab11;           /* error in TFM, abort */
        if (a < 128)
          if (k - lig_kern_base[f] + a + 1 >= nl)
            goto lab11;         /* error in TFM, abort */
      }
    }
    if (a == 255)
      bchlabel = 256 * c + d;
  }

  for (k = kern_base[f] + 256 * (128); k <= exten_base[f] - 1; k++)
  {
    tfm_temp = getc(tfm_file);
    a = tfm_temp;
    tfm_temp = getc(tfm_file);
    b = tfm_temp;
    tfm_temp = getc(tfm_file);
    c = tfm_temp;
    tfm_temp = getc(tfm_file);
    d = tfm_temp;
    sw = (((((d * z) / 256) + (c * z)) / 256) + (b * z)) / beta;
    if (a == 0)
      font_info[k].cint = sw;
    else if (a == 255)
      font_info[k].cint = sw - alpha;
    else goto lab11;
  }
/*  read extensible character recipes */
/*  for k:=exten_base[f] to param_base[f]-1 do */
  for (k = exten_base[f]; k <= param_base[f] - 1; k++)
  {
    {
      tfm_temp = getc(tfm_file);
      a = tfm_temp;
      qw.b0 = a;
      tfm_temp = getc(tfm_file);
      b = tfm_temp;
      qw.b1 = b;
      tfm_temp = getc(tfm_file);
      c = tfm_temp;
      qw.b2 = c;
      tfm_temp = getc(tfm_file);
      d = tfm_temp;
      qw.b3 = d;
/*    store_four_quarters(font_info[k].qqqq); */
      font_info[k].qqqq = qw;
    }
    if (a != 0)
    {
      {
        if ((a < bc) || (a > ec))
          goto lab11;
      }
      qw = font_info[char_base[f] + a].qqqq;
      if (!(qw.b0 > 0))
        goto lab11;
    }
    if (b != 0)
    {
      {
        if ((b < bc) || (b > ec))
          goto lab11;
      }
      qw = font_info[char_base[f] + b].qqqq;
      if (!(qw.b0 > 0))
        goto lab11;
    }
    if (c != 0)
    {
      {
        if ((c < bc) || (c > ec))
          goto lab11;
      }
      qw = font_info[char_base[f] + c].qqqq;
      if (!(qw.b0 > 0))
        goto lab11;
    }
    {
      {
        if ((d < bc) || (d > ec))
          goto lab11;
      }
      qw = font_info[char_base[f] + d].qqqq;
      if (!(qw.b0 > 0))
        goto lab11;
    }
  }
  {
    for (k = 1; k <= np; k++)
      if (k == 1)
      {
        tfm_temp = getc(tfm_file);
        sw = tfm_temp;
        if (sw > 127)
          sw = sw - 256;
        tfm_temp = getc(tfm_file);
        sw = sw * 256 + tfm_temp;
        tfm_temp = getc(tfm_file);
        sw = sw * 256 + tfm_temp;
        tfm_temp = getc(tfm_file);
        font_info[param_base[f]].cint = (sw * 16) + (tfm_temp / 16);
      }
      else
      {
        tfm_temp = getc(tfm_file);
        a = tfm_temp;
        tfm_temp = getc(tfm_file);
        b = tfm_temp;
        tfm_temp = getc(tfm_file);
        c = tfm_temp;
        tfm_temp = getc(tfm_file);
        d = tfm_temp;
        sw = (((((d * z) / 256) + (c * z)) / 256) + (b * z)) / beta;
        if (a == 0)
          font_info[param_base[f] + k - 1].cint = sw;
        else if (a == 255)
          font_info[param_base[f] + k - 1].cint = sw - alpha;
        else goto lab11;
      }
/*  use test_eof() here instead ? */
    if (feof(tfm_file))
      goto lab11;
    for (k = np + 1; k <= 7; k++)
      font_info[param_base[f] + k - 1].cint = 0;
  }
/* @<Make final adjustments...@>= l.11174 */
  if (np >= 7)
    font_params[f] = np;
  else
    font_params[f] = 7;
  hyphen_char[f] = default_hyphen_char;
  skew_char[f] = default_skew_char;
  if (bchlabel < nl)
    bchar_label[f] = bchlabel + lig_kern_base[f];
  else
    bchar_label[f]= non_address; /* i.e. 0 --- 96/Jan/15 */
  font_bchar[f] = bchar;
  font_false_bchar[f] = bchar;
  if (bchar <= ec)
    if (bchar >= bc)
    {
      qw = font_info[char_base[f] + bchar].qqqq;
      if ((qw.b0 > 0))
        font_false_bchar[f] = 256;
    }
  font_name[f] = nom;
  font_area[f] = aire;
  font_bc[f] = bc;
  font_ec[f] = ec;
  font_glue[f] = 0;  /* font_glue[f]:=null; l.11184 */
  char_base[f] = char_base[f];
  width_base[f] = width_base[f];
  lig_kern_base[f] = lig_kern_base[f];
  kern_base[f] = kern_base[f];
  exten_base[f] = exten_base[f];
  decr(param_base[f]);
  fmem_ptr = fmem_ptr + lf;
  font_ptr = f;
  g = f;
  goto lab30;
lab11:
  print_err("Font ");
  sprint_cs(u); 
  print_char('=');
  print_file_name(nom, aire, 335);  /* "" */
  if (s >= 0)
  {
    print_string(" at ");
    print_scaled(s);
    print_string("pt");
  }
  else if (s != -1000)
  {
    print_string("scaled");
    print_int(- (integer) s);
  } 
  if (fileopened)
    print_string(" not loadable: Bad metric (TFM) file");
  else
    print_string(" not loadable: Metric (TFM) file not found");
  if (aire == 335)
  {    /* "" only if path not specified */
    if (show_texinput_flag) show_tex_fonts();   /* 98/Jan/31 */
  }
  help5("I wasn't able to read the size data for this font,",
      "so I will ignore the font specification.",
      "[Wizards can fix TFM files using TFtoPL/PLtoTF.]",
      "You might try inserting a different font spec;",
      "e.g., type `I\\font<same font id>=<substitute font name>'.");
  error();
lab30:
  if (fileopened)
    b_close(tfm_file);
  Result = g;
  return Result;
}
