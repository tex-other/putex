/* Hexadecimal printing of nonnegative integers is accomplished by |print_hex|. */
void print_hex(integer n) /* prints a positive integer in hexadecimal form */
{
  unsigned char k;
  char digs[23];
  k=0; print_char('"');
  do {
    digs[k]=n&15; n/=16; k++;
  } while (n!=0);
  print_the_digs(k, digs);
}
