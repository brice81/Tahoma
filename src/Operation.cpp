////////////////////////////////////////////////////////////////////////////
// Name:        Operation.cpp
// Purpose:     This class implements Tahoma protocol operations
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#include <tahoma/Operation.hpp>

#include <tahoma/JSon.hpp>

namespace tahoma
{

Operation::Status_t Operation::Login(Session& session, const String& username, const String& password)
{
   String url("https://www.tahomalink.com/enduser-mobile-web/enduserAPI/login");

   std::list<Session::PostParameter_t> parameters;
   parameters.push_back(Session::PostParameter_t(String("userId"), username));
   parameters.push_back(Session::PostParameter_t(String("userPassword"), password));


   String result;
   if (session.Execute(url, parameters, result))
   {
      try
      {
         JSon::Value value = JSon::Decode(result);
         if ((value.GetType() == JSon::Value::ObjectType) &&
             (value.HasObjectEntry("success")) &&
             (value.GetObjectEntry("success").GetType()  == JSon::Value::BooleanType) &&
             (value.GetObjectEntry("success").GetBoolean()))
         {
            return Ok;
         }
         else if ((value.GetType() == JSon::Value::ObjectType) &&
                  (value.HasObjectEntry("errorCode")) &&
                  (value.GetObjectEntry("errorCode").GetType()  == JSon::Value::StringType) &&
                  (value.GetObjectEntry("errorCode").GetString() == String("AUTHENTICATION_ERROR")))
         {
            return InvalidUserPass;
         }
      }
      catch (JSon::InvalidFormat& e)
      {}
   }

   return UnknownError;
}

Operation::Status_t Operation::RefreshAllStates(Session& session)
{
   String url("https://www.tahomalink.com/enduser-mobile-web/externalAPI/refreshAllStates");

   String result;
   if (session.Execute(url, std::list<Session::PostParameter_t>(), result))
   {
      return Ok;
   }

   return UnknownError;
}

Operation::Status_t Operation::GetSetup(Session& session, String& result)
{
   String url("https://www.tahomalink.com/enduser-mobile-web/externalAPI/json/getSetup");

   if (session.Execute(url, std::list<Session::PostParameter_t>(), result))
   {
      return Ok;
   }

   return UnknownError;
}

Operation::Status_t Operation::ApplyCommand(Session& session, const String& deice_url, const String& command_name, const std::list<JSon::Value>& parameters, String& result)
{
   String url("https://www.tahomalink.com/enduser-mobile-web/enduserAPI/exec/apply");

   /* Format the post data */
   std::map<String, JSon::Value> action;
   action["deviceURL"] = deice_url;

   std::map<String, JSon::Value> command;
   command["name"] = command_name;

   if (parameters.size() > 0)
   {
      command["parameters"] = JSon::Value(parameters);
   }

   std::list<JSon::Value> commands;
   commands.push_back(JSon::Value(command));

   action["commands"] = JSon::Value(commands);

   std::list<JSon::Value> actions;
   actions.push_back(JSon::Value(action));

   std::map<String, JSon::Value> post_data;
   post_data["label"] = JSon::Value(String("Equipment"));
   post_data["actions"] = JSon::Value(actions);

   std::list<Session::PostParameter_t> post_parameters;
   post_parameters.push_back(Session::PostParameter_t(JSon::Encode(JSon::Value(post_data)), String("")));

   /* Execute the command */
   if (session.Execute(url, post_parameters, result))
   {
      return Ok;
   }

   return UnknownError;
}

Operation::Status_t Operation::GetExecutions(Session& session, std::map<String,std::list<String> >& result)
{
   String url("https://www.tahomalink.com/enduser-mobile-web/externalAPI/json/getCurrentExecutions");

   std::list<Session::PostParameter_t> post_parameters;
   post_parameters.push_back(Session::PostParameter_t(JSon::Encode(JSon::Value(std::list<JSon::Value>())), String("")));

   String result_str;
   if (session.Execute(url, post_parameters, result_str))
   {
      try
      {
         std::map<String,std::list<String> > operations;

         JSon::Value root = JSon::Decode(result_str);
         JSon::Value executions = root.GetObjectEntry("executions");
         for (long i = 0; i < executions.GetArraySize(); i++)
         {
            JSon::Value execution = executions.GetArrayEntry(i);
            JSon::Value actions = execution.GetObjectEntry("actionGroup").GetObjectEntry("actions");
            for (long i = 0; i < actions.GetArraySize(); i++)
            {
               JSon::Value action = actions.GetArrayEntry(i);

               String url = action.GetObjectEntry("deviceURL").GetString();
               String execution_id = execution.GetObjectEntry("id").GetString();

               if (operations.count(url) <= 0)
               {
                  operations[url] = std::list<String>();
               }
               operations[url].push_back(execution_id);
            }
         }

         result = operations;
         return Ok;
      }
      catch (JSon::InvalidFormat& e) {}
   }

   return UnknownError;
}

Operation::Status_t Operation::StopExecution(Session& session, const String& execution)
{
   String url("https://www.tahomalink.com/enduser-mobile-web/externalAPI/json/cancelExecutions");

   std::map<String,JSon::Value> executions;
   executions["execId"] = execution;

   std::list<Session::PostParameter_t> post_parameters;
   post_parameters.push_back(Session::PostParameter_t(JSon::Encode(JSon::Value(executions)), String("")));

   String result_str;
   if (session.Execute(url, post_parameters, result_str))
   {
      return Ok;
   }

   return UnknownError;
}


}
