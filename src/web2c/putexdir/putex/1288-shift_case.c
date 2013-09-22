static void shift_case(void)
{
  pointer b; /* |lc_code_base| or |uc_code_base| */
  pointer p; /* runs through the token list */
  halfword t; /* token */
  eight_bits c; /* character code */

  b=cur_chr; p=scan_toks(false,false); p=link(def_ref);
  while (p!=0) {
    t=info(p);
    if (t<cs_token_flag+single_base) {
      c=t&(dwa_do_8-1);
      if (equiv(b+c)!=0) info(p)=t-c+equiv(b+c);
    }
    p=link(p);
  }
  back_list(link(def_ref)); free_avail(def_ref); /* omit reference count */
}
