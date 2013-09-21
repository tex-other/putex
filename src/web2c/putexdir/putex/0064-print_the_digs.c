/* An array of digits in the range |0..15| is printed by |print_the_digs|. */
static void print_the_digs(eight_bits k, char * dig) /* prints |dig[k-1]|$\,\ldots\,$|dig[0]| */
{
  while (k>0) {
    k--;
    if (dig[k]<10)
      print_char('0'+dig[k]);
    else
      print_char('A'+dig[k]-10);
  }
}
