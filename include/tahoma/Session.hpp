////////////////////////////////////////////////////////////////////////////
// Name:        Session.hpp
// Purpose:     This class implements Tahoma protocol Session Management
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#ifndef TAHOMA_SESSION_HPP
#define TAHOMA_SESSION_HPP

#include "String.hpp"

#include <list>

#include <curl/curl.h>

namespace tahoma
{

class Session
{
   public:

      typedef struct PostParameter_t
      {
         PostParameter_t(const String& name,const String& value):name(name),value(value){}
         String name;
         String value;
      } PostParameter_t;

      Session();
      ~Session();

      bool Execute(const String& url,
                   const std::list<PostParameter_t>& post_parameters,
                   String& result);

      typedef void(*CurlPrepareCallback_t)(CURL*);
      static void RegisterCurlPrepareCallback(CurlPrepareCallback_t callback);

   private:

      CURL* curl;

      String curl_result;
      static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp);

      static CurlPrepareCallback_t curl_prepare_callback;
};

}

#endif /* TAHOMA_SESSION_HPP */
