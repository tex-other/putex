Static void backerror(void)
{
  OKtointerrupt = false;
  backinput();
  OKtointerrupt = true;
  error();
}

