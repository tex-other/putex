/* cmd.cpp: setup types of DVI commands

   Copyright (C) 1997-2004 Chey-Woei Tsay <cwtsay@pu.edu.tw>

   This file is part of the cdi2dvi program.

   The cdi2dvi program is a free software; you can redistribute it
   and/or modify it under the terms of the GNU General Public License
   as published by the Free Software Foundation; either version 2, or
   (at your option) any later version.
   
   The cdi2dvi program is distributed in the hope that it will be
   useful, but WITHOUT ANY WARRANTY; without even the implied warranty
   of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
   
   You should have received a copy of the GNU General Public License
   along with the MiKTeX Core Library; if not, write to the Free
   Software Foundation, 59 Temple Place - Suite 330, Boston, MA
   02111-1307, USA.
*/

#include "cmd.h"

int cmd_type[256];

// cmd_type array is setup for reducing cases in determination of dvi commands and
// providing information about number of arguments of each dvi commands.
//
// For a dvi command 'cmd',
//
//  (1) If cmd should be copy to dvi file verbosely, then the number of arguments of cmd 
//      is assigned to cmd_type[cmd]. In this case, cmd_type[cmd] is always a nonnegative
//      value.
//
//  (2) If cmd needs additional care, the value of cmd_type[cmd] is either -cmd or
//      CMD_GROUP(cmd). In the later case, the lengh of the first parameter of cmd can
//      be calculated by the expression 'cmd + CMD_GROUP(cmd)'.

void init_cmd_type ()
{
	int i;

	for (i = set_char_0; i < set1; i++)
		cmd_type[i] = -set_char_1;


	cmd_type[set1] = cmd_type[set3] = CMD_GROUP(set1);
	cmd_type[set2] = -set2; cmd_type[set4] = -set4;

	cmd_type[set_rule] = 8;

	// Note: TeX and PUTeX never generate put DVI commands. 

	cmd_type[put1] = cmd_type[put3] = CMD_GROUP(put1);
	cmd_type[put2] = -put2;  cmd_type[put4] = -put4;

	cmd_type[put_rule] = 8;
	cmd_type[nop] = 0;
	cmd_type[bop] = -bop;
	cmd_type[eop] = 0;
	cmd_type[push] = 0;
	cmd_type[pop] = 0;
	cmd_type[right1] = 1;
	cmd_type[right2] = 2;
	cmd_type[right3] = 3;
	cmd_type[right4] = 4;
	cmd_type[w0] = 0;
	cmd_type[w1] = 1;
	cmd_type[w2] = 2;
	cmd_type[w3] = 3;
	cmd_type[w4] = 4;
	cmd_type[x0] = 0;
	cmd_type[x1] = 1;
	cmd_type[x2] = 2;
	cmd_type[x3] = 3;
	cmd_type[x4] = 4;
	cmd_type[down1] = 1;
	cmd_type[down2] = 2;
	cmd_type[down3] = 3;
	cmd_type[down4] = 4;
	cmd_type[y0] = 0;
	cmd_type[y1] = 1;
	cmd_type[y2] = 2;
	cmd_type[y3] = 3;
	cmd_type[y4] = 4;
	cmd_type[z0] = 0;
	cmd_type[z1] = 1;
	cmd_type[z2] = 2;
	cmd_type[z3] = 3;
	cmd_type[z4] = 4;

	for (i = fnt_num_0; i < fnt1; i++)
		cmd_type[i] = -fnt_num_0;

	cmd_type[fnt1] = cmd_type[fnt2] = cmd_type[fnt3] = cmd_type[fnt4] = CMD_GROUP(fnt1);

	cmd_type[xxx1] = cmd_type[xxx2] = cmd_type[xxx3] = cmd_type[xxx4] = CMD_GROUP(xxx1);

	cmd_type[fnt_def1] = cmd_type[fnt_def2] = cmd_type[fnt_def3] = cmd_type[fnt_def4] = CMD_GROUP(fnt_def1);

	cmd_type[pre] = -pre;
	cmd_type[post] = -post;
	cmd_type[post_post] = -post_post;

	cmd_type[cfnt] = -cfnt;
	cmd_type[cfnt_def] = -cfnt_def;
}
