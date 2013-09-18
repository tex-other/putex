Static pointer new_skip_param(small_number n)
{
  pointer p;
  tempptr = new_spec(glue_par(n));
  p = new_glue(temp_ptr);
  glue_ref_count(temp_ptr) = 0;
  subtype(p) = n + 1;
  return p;
}
