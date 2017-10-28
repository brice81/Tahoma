////////////////////////////////////////////////////////////////////////////
// Name:        String.cpp
// Purpose:     This class implements String utility
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#include <tahoma/String.hpp>

namespace tahoma
{

String String::Format(const char* format_string, ...)
{
   /* perform first call to snprintf to find requested size */
   va_list argptr;
   va_start(argptr, format_string);
   int needed_size = vsnprintf(NULL, 0, format_string, argptr);
   va_end( argptr );

   char* tmp_buffer = (char*)malloc(needed_size+2);

   /* perform a second time with proper size */
   va_start(argptr, format_string);
   vsnprintf(tmp_buffer, needed_size+1, format_string, argptr);
   va_end( argptr );

   String str(tmp_buffer);

   free(tmp_buffer);

   return str;
}

String::String(const char* str)
{
   this->str = str;
}

String::String(const wxString& str)
{
   this->str = str;
}

String String::operator+(const String &b) const
{
   return String(this->str + b.str);
}

bool String::operator==(const String &b) const
{
   return this->str.compare(b.str) == 0;
}

bool String::operator<(const String& b) const
{
   return this->str.compare(b.str) < 0;
}

const char* String::c_str() const
{
   return this->str.c_str();
}

unsigned long String::Size() const
{
   return this->str.size();
}

}
