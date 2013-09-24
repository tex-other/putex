void debug_help(void) /* routine to display various things */
{
  integer k, l, m, n;
  while (true) {
    print_nl("debug # (-1 to exit):");
    fflush(term_out);
    fscanf(term_in," %ld",&m);
    if (m<0) {
      goto PUexit; /* go to every label at least once */
      continue;
    }
    if (m==0) {
      goto PUbreakpoint;
PUbreakpoint: m=0;
      continue;
    }
    fscanf(term_in," %ld",&n);
    switch (m) {

    case 1: print_word(mem[n-mem_min]); break; /* display |mem[n]| in all forms */
    case 2: print_int(info(n)); break;
    case 3: print_int(link(n)); break;
    case 4: print_word(eqtb[n-active_base]); break;
    case 5: print_word(font_info[n]); break;
    case 6: print_word(save_stack[n]); break;
    case 7: showbox(n); break;
    /* show a box, abbreviated by |show_box_depth| and |show_box_breadth| */
    case 8: breadth_max=10000; depth_threshold=str_adjust_to_room(poolsize)-10;
      show_node_list(n); break;
    case 9: show_token_list(n,0,1000); break;
    case 10: slow_print(n); break;
    case 11: check_mem(n>0); break;
    /* check wellformedness; print new busy locations if |n>0| */
    case 12: search_mem(n); break; /* look for pointers to |n| */
    case 13: fscanf(term_in, " %ld", &l); print_cmd_chr(n, l); break;
    case 14: for (k=0;k<=n;k++) print(buffer[k]); break;
    case 15: font_in_short_display=null_font; short_display(n); break;
    case 16: panicking=!panicking; break;
    default: print('?'); break;
    }
  }
PUexit: ;
}
