/* Old versions of \TeX\ needed a procedure called |print_ASCII| whose function
is now subsumed by |print|. We retain the old name here as a possible aid to
future software arch\ae ologists.
Roman numerals are produced by the |print_roman_int| routine.  Readers
who like puzzles might enjoy trying to figure out how this tricky code
works; therefore no explanation will be given. Notice that 1990 yields
\.{mcmxc}, not \.{mxm}. */
static void printromanint(long n)
{
  pool_pointer j, k; /* mysterious indices into |str_pool| */
  nonnegative_integer u, v; /* mysterious numbers */
  static char rom_str[]="m2d5c2l5x2v5i";

  j=0; v=1000;
  while (true) {
    while (n>=v) {
      print_char(rom_str[j]); n-=v;
    }
    if (n<=0)
      goto PUexit; /* nonpositive input produces no output */
    k=j+2; u=v/(rom_str[k-1]-'0');
    if (rom_str[k-1]=='2') {
      k+=2; u/=rom_str[k-1]-'0';
    }
    if (n+u>=v) {
      print_char(rom_str[k]); n+=u;
    } else {
      j+=2; v/=rom_str[j-1]-'0';
    }
  }
PUexit: ;
}
