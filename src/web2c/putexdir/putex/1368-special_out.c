Static void specialout(halfword p)
{
  char oldsetting;

  synchh();
  synchv();
  oldsetting = selector;
  selector = newstring;
  showtokenlist(link(writetokens(p)), 0, poolsize /* - poolptr */ );
  selector = oldsetting;
  str_room(1);
  { int p_len=get_cur_length(); /* XXXX - Assumes byte=strASCIIcode */
#define xxx1            239
#define xxx4            242

    if (p_len< 256) {
	dviout(xxx1);
	dviout(p_len);
    } else { 
	dviout(xxx4);
	dvifour(p_len);
    }
    str_cur_map(dviout_helper);
  }
}
