/* It is convenient to have a procedure that converts a |math_char|
field to an ``unpacked'' form. The |fetch| routine sets |cur_f|, |cur_c|,
and |cur_i| to the font code, character code, and character information bytes of
a given noad field. It also takes care of issuing error messages for
nonexistent characters; in such cases, |char_exists(cur_i)| will be |false|
after |fetch| has acted, and the field will also have been reset to |empty|. */
void fetch(pointer a)
{
  cur_c=character(a); cur_f=famfnt(fam(a)+cur_size);
  if (cur_f==nullfont) {
    print_err(""); print_size(cur_size); print_char(' ');
    print_int(fam(a)); print(" is undefined (character ");
    print_ASCII(qo(cur_c)); print_char(")");
    help4("Somewhere in the math formula just ended, you used the",
      "stated character from an undefined font family. For example,",
      "plain TeX doesn't allow \it or \sl in subscripts. Proceed,",
      "and I'll try to forget that I needed that character.");
    error(); cur_i=null_character; math_type(a)=empty;
    return;
  }
  if (qo(cur_c)>=font_bc[cur_f] && qo(cur_c)<=font_ec[cur_f])
    cur_i=char_info(cur_f, cur_c);
  else cur_i=null_character;
  if (!charexists(cur_i)) {
    char_warning(cur_f,qo(curc));
    math_type(a)=empty;
  }
}
