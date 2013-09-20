Static void normalparagraph(void)
{
  if (looseness != 0)
    eqworddefine(intbase + loosenesscode, 0);
  if (hangindent != 0)
    eqworddefine(dimenbase + hangindentcode, 0);
  if (hangafter != 1)
    eqworddefine(intbase + hangaftercode, 1);
  if (parshapeptr != 0)
    eqdefine(parshapeloc, shaperef, 0);
}
