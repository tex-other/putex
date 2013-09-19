Static strnumber makenamestring(void)
{
  /* char */ int k, FORLIM; /* INT */

    {
    FORLIM = namelength;
    for (k = 0; k < namelength; k++) {
      appendchar(xord[nameoffile[k]]);
    }
    return (makestring());
  }
}
