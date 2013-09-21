Static void insertrelax(void)
{
  curtok = cstokenflag + curcs;
  backinput();
  curtok = cstokenflag + frozenrelax;
  backinput();
  tokentype = inserted;
} 
