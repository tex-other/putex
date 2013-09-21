Static void finrow(void)
{
  pointer p;

  if (mode == -hmode) {
    p = hpack(link(head), 0, additional);
    popnest();
    appendtovlist(p);
    if (curhead != curtail) {
      link(tail) = link(curhead);
      tail = curtail;
    }
  } else {
    p = vpack(link(head), 0, additional);
    popnest();
    link(tail) = p;
    tail = p;
    spacefactor = 1000;
  }
  type(p) = unsetnode;
  gluestretch(p) = 0;
  if (everycr != 0)
    begintokenlist(everycr, everycrtext);
  alignpeek();
}

