////////////////////////////////////////////////////////////////////////////
// Name:        RollerShutter.cpp
// Purpose:     This class implements Tahoma protocol RollerShutter specific commands
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#include <tahoma/RollerShutter.hpp>

namespace tahoma
{

RollerShutter::Status_t RollerShutter::SetClosingState(Session& session, const String& url, long closing_percentage, bool silent_mode, String& operation_id)
{
   /* Prepare arguments */
   std::list<JSon::Value> params;
   params.push_back(JSon::Value(closing_percentage));

   /* Check if we enable silent mode */
   String command("setClosure");
   if (silent_mode)
   {
      command = String("setClosureAndLinearSpeed");
      params.push_back(JSon::Value(String("lowspeed")));
   }

   /* Prepare result*/
   String result;

   /* Execute operation */
   Status_t status = Operation::ApplyCommand(session, url, command, params, result);

   /* Retrieve status of command */
   if (status == Ok)
   {
      try
      {
         JSon::Value root = JSon::Decode(result);
         if ((root.GetType() == JSon::Value::ObjectType) &&
             (root.GetObjectEntry("execId").GetType() == JSon::Value::StringType))
         {
            operation_id = root.GetObjectEntry("execId").GetString();
            return Ok;
         }
      }
      catch(JSon::InvalidFormat& e) {}
   }

   return UnknownError;
}

RollerShutter::Status_t RollerShutter::SetClosingStateOrStop(Session&session, const String& url, long closing_percentage, bool silent_mode)
{
   /* Update status */
   RefreshAllStates(session);

   /* First, get list of running operations on roller shutter */
   std::map<String,std::list<String> > operations;
   Operation::GetExecutions(session, operations);
   if (operations.count(url))
   {
      const std::list<String>& l = operations[url];
      for (std::list<String>::const_iterator iterator = l.begin(), end = l.end(); iterator != end; ++iterator)
      {
         StopExecution(session, *iterator);
      }

      /* We have stopped running operation -> Stop everything */
      return Ok;
   }

   /* If we did not stop execution, start requested one */
   String operation_id;
   return SetClosingState(session, url, closing_percentage, silent_mode, operation_id);
}

}
