#include "Header.h"

Header::Header()
{
}

Header::Header(Map<String, List<String>> map)
{
}

void Header::Add(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
}

void Header::RemoveAll(
    /* [in] */ const String& key)
{
}

void Header::AddAll(
    /* [in] */ const String& key,
    /* [in] */ List<String> headers)
{
}

void Header::AddIfAbsent(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
}

void Header::Set(
    /* [in] */ const String& key,
    /* [in] */ const String& value)
{
}

Map<String, List<String>> Header::GetFieldMap()
{
}

String Header::Get(
    /* [in] */ Int32 pos)
{
}

String Header::GetKey(
    /* [in] */ Int32 pos)
{
}

String Header::Get(
    /* [in] */ const String& key)
{
}

Int32 Header::Length()
{
}

void Header::SetStatusLine(
    /* [in] */ const String& statusLine)
{
}

String Header::GetStatusLine()
{
}