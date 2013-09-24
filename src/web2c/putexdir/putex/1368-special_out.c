/* After all this preliminary shuffling, we come finally to the routines
that actually send out the requested data. Let's do \.{\\special} first
(it's easier). */
void special_out(pointer p)
{
  unsigned char old_setting; /* holds print |selector| */
  synch_h(); synch_v();
  old_setting=selector; selector=new_string;
  show_token_list(link(write_tokens(p)),0,poolsize);
  selector=old_setting;
  str_room(1);
  { int cur_len=get_cur_length(); 
#define xxx1 239
#define xxx4 242
    if (cur_len<256) {
        dvi_out(xxx1); dvi_out(cur_len);
    } else {
        dvi_out(xxx4); dvi_four(cur_len);
    }
    str_cur_map(dviout_helper);
  }
}
