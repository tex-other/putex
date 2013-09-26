str_number make_name_string(void)
{
  unsinged int k; /* index into |name_of_file| */
  {
    for (k=0; k<name_length; k++) {
      append_char(xord[name_of_file[k]]);
    }
    return (make_string());
  }
}
