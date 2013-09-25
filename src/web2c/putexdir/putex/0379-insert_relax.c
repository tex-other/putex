/* Sometimes the expansion looks too far ahead, so we want to insert
a harmless \.{\\relax} into the user's input. */
void insert_relax(void)
{
  cur_tok=cs_token_flag+cur_cs; back_input();
  cur_tok=cs_token_flag+frozen_relax; back_input(); token_type=inserted;
} 
