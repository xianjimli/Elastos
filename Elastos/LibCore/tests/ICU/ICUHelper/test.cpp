#include "test.h"
#include <stdio.h>
#include <Elastos.Utility.h>
#include <elastos/AutoPtr.h>

using namespace Elastos;

int CTest::test1(int argc, char* argv[])
{
    ArrayOf<String> *languages;
    AutoPtr<IICUHelper> pICUHelper;
    ECode ec = NOERROR;
    ec = CICUHelper::AcquireSingleton((IICUHelper **)&pICUHelper);

    //Get ICUHelper
    printf("========================================\n");
    pICUHelper->GetISOLanguages(&languages);

    for(Int32 count = 0; count < languages->GetLength(); count++) {
        printf("the str is %s\n", (const char *)(*languages)[count]);
    }

    ArrayOf<String>::Free(languages);

    String localName("jp_JPddd_TRANDITIONAL");
    AutoPtr<ILocale> locale;
    pICUHelper->LocaleFromString(localName, (ILocale **)&locale);
    return 0;
}

int CTest::test2(int argc, char* argv[])
{
    ArrayOf<String> *str = ArrayOf<String>::Alloc(2);
    (*str)[0] = String("jp_JP_TRADITION");
    (*str)[1] = String("zh_ZH_FFHHFF");
    printf("%s %d", __FILE__, __LINE__);
    ArrayOf<ILocale*> *ppLocales;
    AutoPtr<IICUHelper> pICUHelper;
    ECode ec = NOERROR;
    ec = CICUHelper::AcquireSingleton((IICUHelper **)&pICUHelper);

    pICUHelper->LocalesFromStrings((*str), &ppLocales);
    printf("***%s %d\n", __FILE__, __LINE__);
    String strCountry;
    (*ppLocales)[0]->GetLanguage(&strCountry);
    printf("the languages is %s\n", (const char*)strCountry);
    (*ppLocales)[1]->GetLanguage(&strCountry);
    printf("the languages is %s\n", (const char*)strCountry);
    ArrayOf<ILocale*>::Free(ppLocales);
    return 0;
}



int CTest::test3(int argc, char* argv[])
{
    return 0;
}

int CTest::test4(int argc, char* argv[])
{
    return 0;
}


int CTest::test5(int argc, char* argv[])
{
    return 0;
}
