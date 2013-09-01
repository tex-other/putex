/* putex-miktex.h: class CTeXApp				-*- C++ -*-

   Copyright (C) 1991-2003 Christian Schenk <cs@miktex.org>
   Copyright (C) 1994-2003 Chey-Woei Tsay <cwtsay@pu.edu.tw>

   This file is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published
   by the Free Software Foundation; either version 2, or (at your
   option) any later version.

   This file is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this file; if not, write to the Free Software
   Foundation, 59 Temple Place - Suite 330, Boston, MA 02111-1307,
   USA. */

#pragma once
#include <iostream>
#define THEDATA(x) TEXDATA.m_##x

#ifdef TRIPTEX
#  include "triptexdefs.h"
#else
#  include "putexdefs.h"
#endif

#include "putex.rc"
#include "texapp.h"

#ifndef MIKTEXHELP_TEX
#  include <miktexhelp.h>
#endif

class TEXCLASS

  : public CTeXAppRoot

{
 protected:
  void __stdcall AddOptions ()
  {
    MiKTeXMF::CTeXAppRoot::AddOptions ();
    AddOption (_T("fmt"), _T("undump"));
  }

 public:
  void __stdcall Init ()
  {
    CTeXAppRoot::Init ();
    SetTeX ();
#ifdef IMPLEMENT_TCX
    SetFeature (FeatureTCX);
#endif
  }

 public:
  LPCTSTR __stdcall GetDumpFileName () const
  {
    return ("putex.fmt");
  }

 public:
  LPCTSTR __stdcall GetPoolFileName () const
  {
    return ("putex.pool");
  }

 public:
  LPCTSTR __stdcall GetInitProgramName () const
  {
    return ("puinitex");
  }

 public:
  LPCTSTR __stdcall GetVirginProgramName () const
  {
    return ("puvirtex");
  }

 public:
  LPCTSTR __stdcall TheNameOfTheGame () const
  {
    return ("PUTeX");
  }

 public:
  DWORD __stdcall GetHelpId () const
  {
    return (MIKTEXHELP_TEX);
  }

};

extern TEXCLASS TEXAPP;
#define THEAPP TEXAPP

#include "texapp.inl"

