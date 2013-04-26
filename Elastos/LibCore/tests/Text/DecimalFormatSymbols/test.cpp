#include "test.h"

#include "Elastos.Text.h"
#include <elastos/AutoPtr.h>
#include <elastos/System.h>

using namespace Elastos;

void CheckLocaleIsEquivalentToRoot(ILocale* locale)
{
    AutoPtr<IDecimalFormatSymbols> dfs, dfs2;
    AutoPtr<IDecimalFormatSymbolsHelper> dfsh;
    CDecimalFormatSymbolsHelper::AcquireSingleton((IDecimalFormatSymbolsHelper**)&dfsh);
    dfsh->GetInstanceEx(locale, (IDecimalFormatSymbols**)&dfs);
    AutoPtr<ILocale> Locale_ROOT;
    CLocale::New(String(""), String(""), String(""), (ILocale**)&Locale_ROOT);
    dfsh->GetInstanceEx(Locale_ROOT, (IDecimalFormatSymbols**)&dfs2);
    assert(dfs2 == dfs);
}

int CTest::test_getInstance_unknown_or_invalid_locale(int argc, char* argv[])
{
    AutoPtr<ILocale> locale, locale2;
    CLocale::New(String("xx"), String("XX"), (ILocale**)&locale);
    CLocale::New(String("not exist language"), String("not exist country"), (ILocale**)&locale2);
    CheckLocaleIsEquivalentToRoot(locale);
    CheckLocaleIsEquivalentToRoot(locale2);
    return 0;
}


