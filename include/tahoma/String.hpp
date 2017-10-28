////////////////////////////////////////////////////////////////////////////
// Name:        String.hpp
// Purpose:     This class implements String utility
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#ifndef TAHOMA_STRING_HPP
#define TAHOMA_STRING_HPP

#include "wx/wxprec.h"
#include "wx/wx.h"

namespace tahoma
{

class JSon;
class String
{
   public:

      static String Format(const char* format_str, ...);

      String(const char* str = "");

      String operator+(const String &b) const;
      bool operator==(const String &b) const;
      bool operator<(const String& b) const;

      const char* c_str() const;

      unsigned long Size() const;

   private:

      friend class JSon;
      String(const wxString& str);

      wxString str;
};

}

#endif /* TAHOMA_STRING_HPP */
