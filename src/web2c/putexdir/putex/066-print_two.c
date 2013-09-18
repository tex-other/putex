Static void printtwo(long n)
{
  n = labs(n) % 100;
  printchar('0' + n / 10);
  printchar('0' + n % 10);
}
