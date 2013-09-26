void alter_prev_graf(void)
{
  unsigned int p; /* index into |nest| */
  nest[nest_ptr]=cur_list; p=nest_ptr;
  while (abs(nest[p].mode_field)!=vmode) decr(p);
  scan_optional_equals(); scan_int();
  if (cur_val<0) {
    print_err("Bad "); print_esc("prevgraf");
    help1("I allow only nonnegative values here.");
    int_error(cur_val);
  } else {
    nest[p].pg_field=cur_val; cur_list=nest[nest_ptr];
  }
}
