////////////////////////////////////////////////////////////////////////////
// Name:        JSon.hpp
// Purpose:     This class implements JSon format parser
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#ifndef TAHOMA_JSON_HPP
#define TAHOMA_JSON_HPP

#include <tahoma/String.hpp>

#include <wx/jsonval.h>

#include <list>
#include <map>

namespace tahoma
{

class JSon
{
   public:

      class InvalidOperation
      {
         public:

            InvalidOperation(const String& description):description(description) {}

            const String description;
      };

      class InvalidFormat
      {
         public:
            InvalidFormat(const String& description):description(description) {}

            const String description;

      };

      class Value
      {
         public:

            typedef enum
            {
               StringType,
               BooleanType,
               NumberType,
               ArrayType,
               ObjectType,
               InvalidType,
               EncodedType
            } Type_t;

            Value();
            Value(const String& str);
            Value(bool value);
            Value(long value);
            Value(float value);
            Value(const std::map<String,Value>& array);
            Value(const std::list<Value>& object);

            Type_t GetType() const;

            String GetString() const;
            bool GetBoolean() const;
            long GetInt() const;
            float GetFloat() const;

            std::list<String> GetAllObjectIndices() const;
            bool HasObjectEntry(const String& index) const;
            Value GetObjectEntry(const String& index) const;

            long GetArraySize() const;
            Value GetArrayEntry(long i) const;

         private:

            Value(const wxJSONValue& value);

            friend class JSon;
            wxJSONValue value;
      };

      static String Encode(const Value& value);
      static Value Decode(const String& str);
};

}

#endif /* TAHOMA_JSON_HPP */
