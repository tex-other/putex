void delete_token_ref(pointer p)
/* |p| points to the reference count of a token list that is losing one reference */
{
  if (token_ref_count(p)==0) flush_list(p);
  else decr(tokenrefcount(p));
}
