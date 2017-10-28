////////////////////////////////////////////////////////////////////////////
// Name:        Session.cpp
// Purpose:     This class implements Tahoma protocol Session Management
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#include <tahoma/Session.hpp>

namespace tahoma
{

Session::Session()
{
  curl = curl_easy_init();
}

Session::~Session()
{
   curl_easy_cleanup(curl);
}

Session::CurlPrepareCallback_t Session::curl_prepare_callback = NULL;

void Session::RegisterCurlPrepareCallback(CurlPrepareCallback_t callback)
{
   curl_prepare_callback = callback;
}

bool Session::Execute(const String& url,
                      const std::list<PostParameter_t>& post_parameters,
                      String& result)
{
   curl_easy_reset(curl);

   curl_result = String("");

   curl_easy_setopt(curl, CURLOPT_CONNECTTIMEOUT, 60L);

   curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
   curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)this);

   curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
   curl_easy_setopt(curl, CURLOPT_HEADER, false);
   curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);
   curl_easy_setopt(curl, CURLOPT_COOKIEFILE, "");
   curl_easy_setopt(curl, CURLOPT_USERAGENT, "libcurl-agent/1.0");

   struct curl_slist *list = NULL;

   String post_data;
   for (std::list<PostParameter_t>::const_iterator iterator = post_parameters.begin(), end = post_parameters.end(); iterator != end; ++iterator)
   {
      const PostParameter_t& parameter = *iterator;

      if (post_data.Size() > 0) post_data = post_data + String("&");
      post_data = post_data + parameter.name;

      if (parameter.value.Size() > 0)
      {
         char* escaped_value = curl_easy_escape(curl, parameter.value.c_str(), 0);
         post_data = post_data + String("=") + String(escaped_value);
         curl_free(escaped_value);
      }
      else if (list == NULL)
      {
         list = curl_slist_append(list, "Content-Type: application/json");
         curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);
      }
   }
   curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_data.c_str());

   if (true) //TODO
   {
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
      curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
   }
   else if (false) //TODO
   {
      //curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 2L);
      //curl_easy_setopt(curl, CURLOPT_CAINFO, certificate_path.c_str());
   }

   /* Invoke callback, if any */
   if (curl_prepare_callback)
   {
      curl_prepare_callback(curl);
   }

   /* Perform the transfer */
   CURLcode res = curl_easy_perform(curl);

   /* Release list, if needed */
   if (list != NULL)
   {
      curl_slist_free_all(list);
   }

   /* check status */
   if (res == 0)
   {
      result = curl_result;
      return true;
   }
   else
   {
      return false;
   }
}

size_t Session::WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
   /* retrieve data */
   size_t real_size = size * nmemb;
   Session* instance = (Session*)userp;

   /* push in reception buffer */
   instance->curl_result = instance->curl_result + String::Format(String::Format("%%%ds", (int)real_size).c_str(), contents);

   /* return written size */
   return real_size;
}


}
