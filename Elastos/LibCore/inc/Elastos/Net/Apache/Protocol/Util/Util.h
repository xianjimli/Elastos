#ifndef __UTIL_H__
#define __UTIL_H__

class Util
{
public:
    static String Decode(
        /* [in] */ const String& s,
        /* [in] */ Boolean convertPlus);

    static String Decode(
        /* [in] */ const String &s,
        /* [in] */ Boolean convertPlus,
        /* [in] */ const String& sencoding);

    static String ToASCIILowerCase(
        /* [in] */ const String &s);

    static String ToASCIIUpperCase(
        /* [in] */ const String &s);
}

#endif //__UTIL_H__