Static void inserror(void)
{
  OKtointerrupt = false;
  backinput();
  tokentype = inserted;
  OKtointerrupt = true;
  error();
}
