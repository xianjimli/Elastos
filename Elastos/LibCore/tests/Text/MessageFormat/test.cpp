#include "test.h"
#include <stdio.h>

#include "Elastos.Text.h"
#include <elastos/AutoPtr.h>
#include <elastos/System.h>
#include <stdio.h>

using namespace Elastos;

int CTest::test_applyPattern(int argc, char* argv[])
{

    AutoPtr<IMessageFormat> format;
    CMessageFormat::New(String("test"), (IMessageFormat**)&format);
    AutoPtr<ILocale> Locale_FRENCH;
    CLocale::New(String("fr"), String(""), (ILocale**)&Locale_FRENCH);
    format->SetLocale(Locale_FRENCH);
    format->ApplyPattern(String("{0,time, Full}"));
    String pattern;
    format->ToPattern(&pattern);
    if (!pattern.Equals(String("{0,time, Full}"))) {
        printf("Wrong full time pattern\n");
        return 0;
    }
    return 0;
}

int CTest::test_formatToCharacterIterator(int argc, char* argv[])
{
    AutoPtr<IMessageFormat> format;
    CMessageFormat::New(String("{1, number}"), (IMessageFormat**)&format);
    AutoPtr<IAttributedCharacterIterator> aci;
    format->FormatToCharacterIterator(NULL, (IAttributedCharacterIterator**)&aci);

    //AutoPtr<IMessageFormat> format2;
    //CMessageFormat::New(String("{0, time}"), (IMessageFormat**)&format2);
    //AutoPtr<IAttributedCharacterIterator> aci2;
    //format->FormatToCharacterIterator(new Object[]{""}, (IAttributedCharacterIterator**)&aci2);    
    return 0;
}

int CTest::test_getLocale(int argc, char* argv[])
{
    ArrayOf<ILocale*> * l = ArrayOf<ILocale*>::Alloc(7);
    AutoPtr<ILocale> Locale_FRANCE, Locale_KOREA, l1, l2, l3, l4, l5;
    CLocale::New(String("fr"), String("FR"), (ILocale**)&Locale_FRANCE);
    CLocale::New(String("ko"), String("KR"), (ILocale**)&Locale_KOREA);
    String country, language;
    Locale_FRANCE->GetCountry(&country);
    Locale_FRANCE->GetLanguage(&language);
    CLocale::New(country, language, (ILocale**)&l1);
    CLocale::New(String("mk"), (ILocale**)&l2);
    CLocale::New(String("mk"), String("MK"), (ILocale**)&l3);
    CLocale::New(String("en"), String("US"), (ILocale**)&l4);
    CLocale::New(String("#ru"), String("@31230"), (ILocale**)&l5);
    (*l)[0] = Locale_FRANCE;
    (*l)[1] = Locale_KOREA;
    (*l)[2] = l1;
    (*l)[3] = l2;
    (*l)[4] = l3;
    (*l)[5] = l4;
    (*l)[6] = l5;
    String pattern("getLocale test {0,number,#,####}");
    AutoPtr<IMessageFormat> mf;
    for (Int32 i = 0; i < 0; i++) {
        CMessageFormat::New(pattern, (*l)[i], (IMessageFormat**)&mf);
        AutoPtr<ILocale> result;
        mf->GetLocale((ILocale**)&result);
        assert((*l)[i] == result);

        String language1, language2;
        (*l)[i]->GetLanguage(&language1);
        result->GetLanguage(&language2);
        assert(language1.Equals(language2));

        String country1, country2;
        (*l)[i]->GetCountry(&country1);
        result->GetCountry(&country2);
        assert(country1.Equals(country2));
    }
    CMessageFormat::New(pattern, (IMessageFormat**)&mf);
    mf->SetLocale(NULL);
    AutoPtr<ILocale> result;
    mf->GetLocale((ILocale**)&result);
    assert(result == NULL);
    return 0;
}
/*
            MessageFormat f1 = (MessageFormat) format1.clone();
            f1.setFormat(0, DateFormat.getTimeInstance());
            f1.setFormat(1, DateFormat.getTimeInstance());
            f1.setFormat(2, NumberFormat.getInstance());
            f1.setFormat(3, new ChoiceFormat("0#off|1#on"));
            f1.setFormat(4, new ChoiceFormat("1#few|2#ok|3#a lot"));
            f1.setFormat(5, DateFormat.getTimeInstance());
*/
int CTest::test_setFormat(int argc, char* argv[])
{
    AutoPtr<IMessageFormat> format1;
    AutoPtr<ILocale> Locale_US;
    CLocale::New(String("en"), String("US"), (ILocale**)&Locale_US);
    AutoPtr<ILocaleHelper> lh;
    CLocaleHelper::AcquireSingleton((ILocaleHelper**)&lh);
    lh->SetDefault(Locale_US);

    String pattern("A {3, number, currency} B {2, time} C {0, number, percent} D {4}  E {1,choice,0#off|1#on} F {0, date}");
    CMessageFormat::New(pattern, (IMessageFormat**)&format1);

    AutoPtr<IMessageFormat> f1 = format1;
    f1->SetFormat(0, )
}


