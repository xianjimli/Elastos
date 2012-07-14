
#include "os/SystemProperties.h"
#include "ext/frameworkdef.h"
#include <cutils/properties.h>
#include <stdio.h>


const Int32 SystemProperties::PROP_NAME_MAX;
const Int32 SystemProperties::PROP_VALUE_MAX;

String SystemProperties::Get(
    /* [in] */ CString key)
{
    if (key.IsNull() || key.GetCharCount() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    return NativeGet(key);
}

String SystemProperties::Get(
    /* [in] */ CString key,
    /* [in] */ CString def)
{
    if (key.IsNull() || key.GetCharCount() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    return NativeGet(key, def);
}

Int32 SystemProperties::GetInt32(
    /* [in] */ CString key,
    /* [in] */ Int32 def)
{
    if (key.IsNull() || key.GetCharCount() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    return NativeGetInt32(key, def);
}

Int64 SystemProperties::GetInt64(
    /* [in] */ CString key,
    /* [in] */ Int64 def)
{
    if (key.IsNull() || key.GetCharCount() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    return NativeGetInt64(key, def);
}

Boolean SystemProperties::GetBoolean(
    /* [in] */ CString key,
    /* [in] */ Boolean def)
{
    if (key.IsNull() || key.GetCharCount() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    return NativeGetBoolean(key, def);
}

void SystemProperties::Set(
    /* [in] */ CString key,
    /* [in] */ CString val)
{
    if (key.IsNull() || key.GetCharCount() > PROP_NAME_MAX) {
//        throw new IllegalArgumentException("key.length > " + PROP_NAME_MAX);
        assert(0);
    }
    if (!val.IsNull() && val.GetCharCount() > PROP_VALUE_MAX) {
//        throw new IllegalArgumentException("val.length > " +
//            PROP_VALUE_MAX);
        assert(0);
    }
    NativeSet(key, val);
}

String SystemProperties::NativeGet(
    /* [in] */ CString key)
{
    return NativeGet(key, NULL);
}

String SystemProperties::NativeGet(
    /* [in] */ CString key,
    /* [in] */ CString def)
{
    int len;
    char buf[PROP_VALUE_MAX + 1];

    len = property_get((const char*)key, buf, "");
    if ((len <= 0) && (!def.IsNull())) {
        return String(def);
    }
    else if (len >= 0) {
        return String(buf);
    }
    else {
        return String("");
    }
}

Int32 SystemProperties::NativeGetInt32(
    /* [in] */ CString key,
    /* [in] */ Int32 def)
{
    int len;
    char buf[PROP_VALUE_MAX + 1];
    Int32 result = def;

    len = property_get(key, buf, "");
    if (len > 0) {
        Int32 temp;
        if (sscanf(buf, "%d", &temp) == 1) {
            result = temp;
        }
    }

    return result;
}

Int64 SystemProperties::NativeGetInt64(
    /* [in] */ CString key,
    /* [in] */ Int64 def)
{
    int len;
    char buf[PROP_VALUE_MAX + 1];
    Int64 result = def;

    len = property_get(key, buf, "");
    if (len > 0) {
        Int64 temp;
        if (sscanf(buf, "%lld", &temp) == 1)
            result = temp;
    }

    return result;
}

Boolean SystemProperties::NativeGetBoolean(
    /* [in] */ CString key,
    /* [in] */ Boolean def)
{
    int len;
    char buf[PROP_VALUE_MAX + 1];
    Boolean result = def;

    len = property_get(key, buf, "");
    if (len == 1) {
        char ch = buf[0];
        if (ch == '0' || ch == 'n') {
            result = FALSE;
        }
        else if (ch == '1' || ch == 'y') {
            result = TRUE;
        }
    }
    else if (len > 1) {
        if (!strcmp(buf, "no") || !strcmp(buf, "false") || !strcmp(buf, "off")) {
            result = FALSE;
        }
        else if (!strcmp(buf, "yes") || !strcmp(buf, "true") || !strcmp(buf, "on")) {
            result = TRUE;
        }
    }

    return result;
}

void SystemProperties::NativeSet(
    /* [in] */ CString key,
    /* [in] */ CString val)
{
    int err;

    if (val.IsNull()) {
        val = "";       /* NULL pointer not allowed here */
    }

    err = property_set(key, val);
}
