Static void printthedigs(eightbits k, char * dig)
{
  while (k > 0) {
    k--;
    if (dig[k] < 10)
      printchar('0' + dig[k]);
    else
      printchar('A' + dig[k] - 10);
  }
}
