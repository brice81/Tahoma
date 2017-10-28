////////////////////////////////////////////////////////////////////////////
// Name:        RollerShutter.hpp
// Purpose:     This class implements Tahoma protocol RollerShutter specific commands
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#ifndef TAHOMA_ROLLER_SHUTTER_HPP
#define TAHOMA_ROLLER_SHUTTER_HPP

#include <tahoma/Session.hpp>
#include <tahoma/Operation.hpp>
#include <tahoma/JSon.hpp>

namespace tahoma
{

class RollerShutter: public Operation
{
   public:

      static Status_t SetClosingState(Session& session, const String& url, long closing_percentage, bool silent_mode, String& operation_id);

      static Status_t SetClosingStateOrStop(Session&session, const String& url, long closing_percentage, bool silent_mode);
};

}

#endif /* TAHOMA_ROLLER_SHUTTER_HPP */
