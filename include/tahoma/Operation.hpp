////////////////////////////////////////////////////////////////////////////
// Name:        Operation.hpp
// Purpose:     This class implements Tahoma protocol operations
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#ifndef TAHOMA_OPERATION_HPP
#define TAHOMA_OPERATION_HPP

#include <tahoma/Session.hpp>
#include <tahoma/JSon.hpp>

namespace tahoma
{

class Operation
{
   public:

      typedef enum
      {
         Ok,
         InvalidUserPass,
         UnknownError
      } Status_t;

      static Status_t Login(Session& session, const String& username, const String& password);
      static Status_t RefreshAllStates(Session& session);
      static Status_t GetSetup(Session& session, String& result);
      static Status_t ApplyCommand(Session& session, const String& url, const String& command, const std::list<JSon::Value>& parameters, String& result);
      static Status_t GetExecutions(Session& session, std::map<String,std::list<String> >& result);
      static Status_t StopExecution(Session& session, const String& execution);
};

}

#endif /* TAHOMA_OPERATION_HPP */
