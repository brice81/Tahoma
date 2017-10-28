////////////////////////////////////////////////////////////////////////////
// Name:        JSon.cpp
// Purpose:     This class implements JSon format parser
// Author:      Brice Andre
// Created:     2017/10/28
// Copyright:   (c) 2010 Brice Andre
// Licence:     LGPL licence (see LICENSE file at root of repository)
// Site:        https://github.com/brice81/Tahoma
/////////////////////////////////////////////////////////////////////////////

#include <tahoma/JSon.hpp>

#include <wx/jsonreader.h>
#include <wx/jsonwriter.h>

namespace tahoma
{

JSon::Value::Value() {}

JSon::Value::Value(const wxJSONValue& value)
              :value(value)
{}

JSon::Value::Value(const String& str)
              :value(str.str)
{}

JSon::Value::Value(bool value)
              :value(value)
{}

JSon::Value::Value(long value)
              :value(value)
{}

JSon::Value::Value(float value)
              :value(value)
{}

JSon::Value::Value(const std::map<String,Value>& array)
{
   value.SetType(wxJSONTYPE_OBJECT);

   std::map<String,Value>::const_iterator it = array.begin();
   while (it != array.end())
   {
      value.Item(it->first.str) = it->second.value;
      it++;
   }
}

JSon::Value::Value(const std::list<Value>& object)
{
   value.SetType(wxJSONTYPE_ARRAY);

   for (std::list<Value>::const_iterator iterator = object.begin(), end = object.end(); iterator != end; ++iterator)
   {
      value.Append(iterator->value);
   }
}

JSon::Value::Type_t JSon::Value::GetType() const
{
   switch (value.GetType())
   {
   case wxJSONTYPE_STRING:
   case wxJSONTYPE_CSTRING:
      return StringType;
   case wxJSONTYPE_BOOL:
      return BooleanType;
   case wxJSONTYPE_INT:
   case wxJSONTYPE_UINT:
   case wxJSONTYPE_DOUBLE:
   case wxJSONTYPE_LONG:
   case wxJSONTYPE_INT64:
   case wxJSONTYPE_ULONG:
   case wxJSONTYPE_UINT64:
   case wxJSONTYPE_SHORT:
   case wxJSONTYPE_USHORT:
      return NumberType;
   case wxJSONTYPE_ARRAY:
      return ArrayType;
   case wxJSONTYPE_OBJECT:
      return ObjectType;
   case wxJSONTYPE_INVALID:
   case wxJSONTYPE_NULL:
   case wxJSONTYPE_MEMORYBUFF:
   default:
      return InvalidType;
   }
}

String JSon::Value::GetString() const
{
   if (GetType() != StringType) throw InvalidOperation("Json::GetString : Not a string");
   return String(value.AsString());
}

bool JSon::Value::GetBoolean() const
{
   if (GetType() != BooleanType) throw InvalidOperation("Json::GetBoolean : Not a boolean");
   return value.AsBool();
}

long JSon::Value::GetInt() const
{
   if (GetType() != NumberType) throw InvalidOperation("Json::GetInt : Not a number");
   return value.AsInt();
}

float JSon::Value::GetFloat() const
{
   if (GetType() != NumberType) throw InvalidOperation("Json::GetFloat : Not a number");
   return value.AsDouble();
}

std::list<String> JSon::Value::GetAllObjectIndices() const
{
   if (GetType() != ObjectType) throw InvalidOperation("Json::GetAllObjectIndices : Not an object");

   std::list<String> result;

   wxArrayString keys = value.GetMemberNames();
   for (long i = 0; i < keys.size(); i++)
   {
      result.push_back(String(keys.Item(i)));
   }

   return result;
}

bool JSon::Value::HasObjectEntry(const String& index) const
{
   if (GetType() != ObjectType) throw InvalidOperation("JSon::HasObjectEntry : Not an object");
   return value.HasMember(index.str);
}

JSon::Value JSon::Value::GetObjectEntry(const String& index) const
{
   if (GetType() != ObjectType) throw InvalidOperation("JSon::GetObjectEntry : Not an object");

   JSon::Value result;
   result.value = value.Get(index.str, result.value);
   return result;
}

long JSon::Value::GetArraySize() const
{
   if (GetType() != ArrayType) throw InvalidOperation("JSon::GetArraySize : Not an array");
   return value.Size();
}

JSon::Value JSon::Value::GetArrayEntry(long i) const
{
   if (GetType() != ArrayType) throw InvalidOperation("JSon::GetArraySize : Not an array");

   JSon::Value result;
   result.value = value.ItemAt(i);
   return result;
}

String JSon::Encode(const Value& value)
{
   wxString result;

   wxJSONWriter writer(wxJSONWRITER_NO_INDENTATION|wxJSONWRITER_NO_LINEFEEDS|wxJSONWRITER_ESCAPE_SOLIDUS);
   writer.Write(value.value, result);

   return String(result);
}

JSon::Value JSon::Decode(const String& str)
{
   wxJSONReader reader;

   JSon::Value result;
   int nb_errors = reader.Parse(str.str, &result.value);
   if (nb_errors != 0)
   {
      throw InvalidFormat(String::Format("JSon::Decode : found %d errors", nb_errors));
   }
   return result;
}

}
