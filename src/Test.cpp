////////////////////////////////////////////////////////////////////////////
// Name:        Test.cpp
// Purpose:     This class is a console application showing how to use Tahoma library
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#include "wx/wxprec.h"
#include "wx/wx.h"

#include <tahoma/Session.hpp>
#include <tahoma/Operation.hpp>
#include <tahoma/JSon.hpp>
#include <tahoma/RollerShutter.hpp>

#include <vector>

using tahoma::Session;
using tahoma::String;
using tahoma::Operation;
using tahoma::JSon;
using tahoma::RollerShutter;

class Test : public wxApp
{
   private:

      typedef struct
      {
         String Name;
         String url;
         long closed_state;
      } RollerShutterEntry_t;

      String username;
      String password;


      static void OnCurlPrepareCallback(CURL* curl)
      {
         //TODO put your proxy settings here
         //curl_easy_setopt(curl, CURLOPT_PROXY, "my-proxy.toto");
         //curl_easy_setopt(curl, CURLOPT_PROXYPORT, 8080L);

         //TODO you could also enable check of server certificate, but then, you need to provide a root certificate
      }

      bool SetShutterClosedState(const String& url, long closed_percentage, bool silent)
      {
         Session session;

         Operation::Status_t status = Operation::Login(session, username, password);
         switch (status)
         {
            case Operation::Ok:
               break;
            case Operation::InvalidUserPass:
               return false;
            case Operation::UnknownError:
            default:
               return false;
         }

         String unused;
         status = RollerShutter::SetClosingState(session, url, closed_percentage, silent, unused);

         return status == Operation::Ok;
      }

      std::vector<RollerShutterEntry_t> GetRollerShutterClosedStates()
      {
         Session session;

         Operation::Status_t status = Operation::Login(session, username, password);
         switch (status)
         {
            case Operation::Ok:
               break;
            case Operation::InvalidUserPass:
               printf("InvalidUserPass\n"); fflush(stdout);
               return std::vector<RollerShutterEntry_t>();
            case Operation::UnknownError:
            default:
               printf("UnknownError\n"); fflush(stdout);
               return std::vector<RollerShutterEntry_t>();
         }

         status = Operation::RefreshAllStates(session);
         if (status != Operation::Ok)
         {
            printf("Unexpected error\n"); fflush(stdout);
            return std::vector<RollerShutterEntry_t>();
         }

         String result_str;
         status = Operation::GetSetup(session, result_str);
         switch (status)
         {
            case Operation::Ok:
            {
               /* Parse JSon */
               try
               {
                  JSon::Value root = JSon::Decode(result_str);

                  std::vector<RollerShutterEntry_t> result;

                  JSon::Value devices = root.GetObjectEntry("setup").GetObjectEntry("devices");
                  for (ulong i = 0; i < devices.GetArraySize(); i++)
                  {
                     const JSon::Value& module = devices.GetArrayEntry(i);

                     if (module.GetObjectEntry("uiClass").GetString() == String("RollerShutter"))
                     {
                        const JSon::Value& states = module.GetObjectEntry("states");
                        for (ulong i = 0; i < states.GetArraySize(); i++)
                        {
                           const JSon::Value& state = states.GetArrayEntry(i);

                           if (state.GetObjectEntry("name").GetString() == String("core:ClosureState"))
                           {
                              RollerShutterEntry_t entry;
                              entry.Name = module.GetObjectEntry("label").GetString();
                              entry.url = module.GetObjectEntry("deviceURL").GetString();
                              entry.closed_state = state.GetObjectEntry("value").GetInt();
                              result.push_back(entry);
                           }
                        }
                     }
                  }

                  return result;
               }
               catch (JSon::InvalidFormat& e)
               {
                  printf("Error parsing result : %s", e.description.c_str());
               }
               catch (JSon::InvalidOperation& e)
               {
                  printf("Error parsing result : %s", e.description.c_str());
               }

               break;
            }
            case Operation::UnknownError:
            default:
               printf("UnknownError"); fflush(stdout);
               break;
         }

         return std::vector<RollerShutterEntry_t>();
      }

      bool OnInit()
      {
         char buffer[1024];

         /* Configure username password */
         char _username[1024];
         char _password[1024];

         printf("Please enter username : ");fflush(stdout);if (!fgets(_username, 1000, stdin)) return false;
         printf("Please enter password : ");fflush(stdout);if (!fgets(_password, 1000, stdin)) return false;

         {char* tmp = _username; while (tmp[0] != 0) {if (tmp[0] == '\n') tmp[0] = 0; else tmp++;}}
         {char* tmp = _password; while (tmp[0] != 0) {if (tmp[0] == '\n') tmp[0] = 0; else tmp++;}}

         this->username = String(_username);
         this->password = String(_password);

         /* Register Curl callback */
         Session::RegisterCurlPrepareCallback(&Test::OnCurlPrepareCallback);

         while (true)
         {
            /* Get state of roller shutters */
            std::vector<RollerShutterEntry_t> states = GetRollerShutterClosedStates();
            printf("Found devices :\n");
            for (ulong i = 0; i < states.size(); i++)
            {
               printf("   (%d) %s -> %d%%\n", (int)i+1, states[i].Name.c_str(), (int)states[i].closed_state);
            }
            fflush(stdout);

            /* Propose to command a shutter */
            char command[10];
            printf("Please enter sutter ID to control (0 to refresh, -1 to exit) : ");fflush(stdout);
            if (!fgets(command, 9, stdin))
            {
               printf("Invalid command !\n");
               continue;
            }
            int parsed_command = atoi(command);


            if (parsed_command == -1)
            {
               printf("Bye!\n");
               return false;
            }
            else if (parsed_command == 0)
            {
               continue;
            }
            else if ((parsed_command < 1) || (parsed_command > states.size()))
            {
               printf("Invalid command !\n");
               continue;
            }

            /* Request percentage */
            char percent[10];
            printf("Please enter close percentage [0-100] : ");fflush(stdout);
            if (!fgets(percent, 9, stdin))
            {
               printf("Invalid command !\n");
               continue;
            }

            int parsed_percent = atoi(percent);

            SetShutterClosedState(states[parsed_command-1].url, parsed_percent, false);
            printf("Done!\n");
         }

         return false;
      }

};

IMPLEMENT_APP_NO_MAIN (Test)

int main(int argc, char** argv)
{
   wxEntry(argc, argv);
   return 0;
}
