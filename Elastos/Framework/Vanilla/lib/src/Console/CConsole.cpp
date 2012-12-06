
#include <CConsole.h>
#include <ConsoleApi.h>
#include <stdio.h>

using namespace Elastos;

ECode CConsole::RequestToFinish()
{
    return _Console_RequestToFinish();
}

ECode CConsole::ReleaseAtExit(
    /* [in] */ IInterface* object)
{
    if (object == NULL) return NOERROR;
    return _Console_ReleaseAtExit(object);
}

ECode CConsole::AddExitRoutine(
    /* [in] */ IInterface* object, 
    /* [in] */ PVoid userData)
{
    if (object == NULL) return NOERROR;
    
    return _Console_AddExitRoutine(object, userData);
}

ECode CConsole::WriteLine()
{
    printf("\n");
    return NOERROR;
}

ECode CConsole::WriteLine(
    /* [in] */ Char32 ch)
{
    printf("%c\n", ch);
    return NOERROR;
}

ECode CConsole::WriteLine(
    /* [in] */ CString str)
{
    printf("%s\n", (const char*)str);
    return NOERROR;
}

ECode CConsole::WriteLine(
    /* [in] */ const String& str)
{
    printf("%s\n", str.string());
    return NOERROR;
}
