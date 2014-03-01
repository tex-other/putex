rc_header = [[
/* script for resources generationl. (TeXLive/W32TeX)

   Copyright 2014 Clerk Ma.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301 USA.  */

#include <windows.h>

]]

rc_dist = [[
#ifdef RCWIN32
  #define TEX_DIST "(W32TeX)"
#else
  #define TEX_DIST "(TeXLive)"
#endif

]]

rc_prev = [[
{
  BLOCK "StringFileInfo"
  {
    BLOCK "040904b0"
    {
]]
rc_tail = [[
    }
  }
  BLOCK "VarFileInfo"
  {
    VALUE "Translation", 0x409, 1252
  }
}
]]

rc_db = {
  {file_name = "yanytex.exe",  file_ver = "0,3,0,0", product_ver = "2,3,0,0"},
  {file_name = "checktfm.exe", file_ver = "0,3,0,0", product_ver = "2,3,0,0"},
  {file_name = "dvidump.exe",  file_ver = "0,3,0,0", product_ver = "2,3,0,0"}
}

function gen_rc(entry)
  local rc_file
  rc_file = io.open(string.sub(entry.file_name, 1, -4).."rc", "w")
  rc_file:write(rc_header)
  rc_file:write(rc_dist)
  rc_file:write("VS_VERSION_INFO VERSIONINFO\n")
  rc_file:write("  FILEVERSION    "..entry.file_ver.."\n")
  rc_file:write("  PRODUCTVERSION "..entry.product_ver.."\n")
  rc_file:write("  FILEOS         VOS__WINDOWS32\n")
  rc_file:write("  FILETYPE       VFT_APP\n")
  rc_file:write(rc_prev)
  rc_file:write("      VALUE \"CompanyName\",      \"Project Fandol.\\0\"\n")
  rc_file:write("      VALUE \"FileDescription\",  \"Y&Y TeX 2.2.3\" TEX_DST \"\\0\"\n")
  rc_file:write("      VALUE \"FileVersion\",      \""..entry.file_ver.."\\0\"\n")
  rc_file:write("      VALUE \"LegalCopyright\",   \"(C) 2014 Clerk Ma.\\0\"\n")
  rc_file:write("      VALUE \"OriginalFilename\", \""..entry.file_name.."\\0\"\n")
  rc_file:write("      VALUE \"ProductName\",      \"Y&Y TeX\\0\"\n")
  rc_file:write("      VALUE \"ProductVersion\",   \"2.2.3.0\\0\"\n")
  rc_file:write(rc_tail)
  rc_file:close()
end

gen_rc({file_name = "yanytex.exe", file_ver = "0,3,0,0", product_ver = "2,3,0,0"})
