Static void packjobname(strnumber s)
{
  curarea = S(385);
  curext = s;
  curname = jobname;
  packfilename(curname,curarea,curext);
}

