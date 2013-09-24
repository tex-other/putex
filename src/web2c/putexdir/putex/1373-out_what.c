/* The |out_what| procedure takes care of outputting whatsit nodes for
|vlist_out| and |hlist_out|\kern-.3pt. */
void out_what(pointer p)
{
  small_number j; /* write stream number */
  switch (subtype(p)) {
  case open_node: case write_node: case close_node:
    if (!doing_leaders) {
      j = write_stream(p);
      if (subtype(p)==write_node) write_out(p);
      else {if (write_open[j]) a_close(&writefile[j]);
        if (subtype(p)==close_node) write_open[j]=false;
        else if (j<16) {
           cur_name=open_name(p); cur_area=open_area(p);
           cur_ext=open_ext(p);
           if (cur_ext=="")
               cur_ext=".tex";
           pack_file_name(cur_name,cur_area,cur_ext);
           while (!aopenout(&writefile[j]))
               prompt_file_name("output file name",".tex");
           write_open[j]=true;
        }
      }
    }
    break;
  case special_node: special_out(p); break;
  case language_node: break;
  default: confusion("ext4"); break;
  }
}
