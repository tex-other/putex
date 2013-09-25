/* The |prepare_mag| subroutine is called whenever \TeX\ wants to use |mag|
for magnification. */
void prepare_mag(void)
{
  if (mag_set>0 && mag!=mag_set) {
    print_err("Incompatible magnification ("); print_int(mag);
    print(");"); print_nl(" the previous value will be retained");
    help2("I can handle only one magnification ratio per job. So I've",
      "reverted to the magnification you used earlier on this run.");
    int_error(mag_set);
    geq_word_define(int_base+mag_code,mag_set); /* |mag:=mag_set| */
  }
  if (mag<=0 || mag>32768L) {
    print_err("Illegal magnification has been changed to 1000");
    help1("The magnification ratio must be between 1 and 32768.");
    int_error(mag); geq_word_define(int_base+mag_code,1000);
  }
  mag_set=mag;
}

