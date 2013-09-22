static void print_word(memory_word w) /* prints |w| in all ways */
{
  print_int(w.int_); print_char(' ');
  print_scaled(w.sc); print_char(' ');
  print_scaled((long)floor(unity*w.gr+0.5)); print_ln();
  print_int(w.hh.UU.lh); print_char('='); print_int(w.hh.UU.U2.b0); print_char(':');
  print_int(w.hh.UU.U2.b1); print_char(';'); print_int(w.hh.rh); print_char(' ');
  print_int(w.qqqq.b0); print_char(':'); print_int(w.qqqq.b1); print_char(':');
  print_int(w.qqqq.b2); print_char(':'); print_int(w.qqqq.b3);
}
