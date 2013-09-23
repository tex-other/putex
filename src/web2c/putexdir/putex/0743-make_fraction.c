/* The |make_fraction| procedure is a bit different because it sets
|new_hlist(q)| directly rather than making a sub-box. */
void make_fraction(pointer q)
{
  pointer p, v, x, y, z; /* temporary registers for box construction */
  scaled delta, delta1, delta2, shift_up, shift_down, clr;
    /* dimensions for box calculations */
  if (thickness(q)==default_code) thickness(q)=default_rule_thickness;
  x=clean_box(numerator(q),num_style(cur_style));
  z=clean_box(denominator(q),denom_style(cur_style));
  if (width(x)<width(z)) x=rebox(x,width(z));
  else z=rebox(z,width(x));
  if (cur_style<text_style) {
    shift_up=num1(cur_size); shift_down=denom1(cur_size);
  } else {
    shift_down=denom2(cur_size);
    if (thickness(q)!= 0) shift_up=num2(cur_size);
    else shift_up=num3(cur_size);
  }
  if (thickness(q)==0) { 
    if (cur_style<text_style) clr=default_rule_thickness*7;
    else clr=default_rule_thickness*3;
    delta=half(clr-shift_up+depth(x)+height(z)-shift_down);
    if (delta>0) {
      shift_up+=delta;
      shift_down+=delta;
    }
  } else {
    if (cur_style<text_style) clr=thickness(q)*3;
    else clr=thickness(q);
    delta=half(thickness(q));
    delta1=clr-shift_up+depth(x)+axis_height(cur_size)+delta;
    delta2=clr-axis_height(cur_size)+delta+height(z)-shift_down;
    if (delta1>0) shift_up+=delta1;
    if (delta2>0) shift_down+=delta2;
  }
  v=new_null_box(); type(v)=vlist_node;
  height(v)=shift_up+height(x); depth(v)=depth(z)+shift_down;
  width(v)=width(x); /* this also equals |width(z)| */
  if (thickness(q)==0) {
    p=new_kern(shiftup-depth(x)-height(z)+shift_down);
    link(p)=z;
  } else {
    y=fraction_rule(thickness(q));
    p=new_kern(axis_height(cur_size)-delta-height(z)+shift_down);
    link(y)=p; link(p)=z;
    p=new_kern(shift_up-depth(x)-axis_height(cur_size)-delta);
    link(p)=y;
  }
  link(x)=p; list_ptr(v)=x;

  if (cur_style<text_style) delta=delim1(cur_size);
  else delta=delim2(cur_size);
  x=var_delimiter(left_delimiter(q),cur_size,delta); link(x)=v;
  z=var_delimiter(right_delimiter(q),cur_size,delta); link(v)=z;
  new_hlist(q)=hpack(x,0,additional);
}
