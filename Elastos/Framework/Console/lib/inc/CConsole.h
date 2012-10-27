
#ifndef __CCONSOLE_H__
#define __CCONSOLE_H__

#include <elastos.h>
#include <ConsoleApi.h>

_ELASTOS_NAMESPACE_BEGIN

class CConsole
{
public:
    static CARAPI RequestToFinish();

    //object
    static CARAPI ReleaseAtExit(
        /* [in] */ IInterface* object);

    //input & output
    static CARAPI WriteLine();

    static CARAPI WriteLine(
        /* [in] */ Char32 ch);

    static CARAPI WriteLine(
        /* [in] */ CString str);

    static CARAPI WriteLine(
        /* [in] */ const String& str);
};

_ELASTOS_NAMESPACE_END


#endif //__CCONSOLE_H__
