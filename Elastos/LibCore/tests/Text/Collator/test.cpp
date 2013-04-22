#include "test.h"
#include <stdio.h>

#include "Elastos.Text.h"
#include <elastos/AutoPtr.h>
#include <elastos/System.h>
#include <StringBuffer.h>

using namespace Elastos;

int CTest::test_setStrength(int argc, char* argv[])
{
    printf("%s %d\n", __FILE__, __LINE__);
    AutoPtr<ICollatorHelper> collatorHelper;
    printf("%s %d\n", __FILE__, __LINE__);
    CCollatorHelper::AcquireSingleton((ICollatorHelper**)&collatorHelper);
    printf("%s %d\n", __FILE__, __LINE__);
    AutoPtr<ICollator> collator;
    printf("%s %d\n", __FILE__, __LINE__);
    collatorHelper->GetInstance((ICollator**)&collator);
    printf("%s %d\n", __FILE__, __LINE__);
    collator->SetStrength(Collator_PRIMARY);
    printf("%s %d\n", __FILE__, __LINE__);
    /*Int32 strength;
    collator->GetStrength(&strength);
    assert(Collator_PRIMARY == strength);
    collator->SetStrength(Collator_SECONDARY);
    collator->GetStrength(&strength);
    assert(Collator_SECONDARY == strength);
    collator->SetStrength(Collator_TERTIARY);
    collator->GetStrength(&strength);
    assert(Collator_TERTIARY == strength);
    collator->SetStrength(Collator_IDENTICAL);
    collator->GetStrength(&strength);
    assert(Collator_IDENTICAL == strength);
    collator->SetStrength(-1);
    collator->GetStrength(&strength);
    printf("strength=%d,IllegalArgumentException was not thrown.\n", strength);*/
    return 0;
}

int CTest::test_stackCorruption(int argc, char* argv[])
{
    /*AutoPtr<ICollatorHelper> collatorHelper;
    CCollatorHelper::AcquireSingleton((ICollatorHelper**)&collatorHelper);
    AutoPtr<ICollator> mColl;
    collatorHelper->GetInstance((ICollator**)&mColl);
    mColl->SetStrength(Collator_PRIMARY);
    AutoPtr<IICUCollationKey> icu;
    mColl->GetCollationKey(String("2d294f2d3739433565147655394f3762f3147312d3731641452f310"),
        (IICUCollationKey**)&icu);*/
    printf("asd");
    return 0;
}

int CTest::test_collationKeySize(int argc, char* argv[])
{
    StringBuffer b("");
    for (Int32 i = 0; i < 1024; i++) {
        b += "0123456789ABCDEF";
    }
    String sixteen = b.Substring(0, b.GetLength());
    b += "_THE_END";
    String sixteenplus = b.Substring(0, b.GetLength());

/*    AutoPtr<ICollatorHelper> collatorHelper;
    CCollatorHelper::AcquireSingleton((ICollatorHelper**)&collatorHelper);
    AutoPtr<ICollator> mColl;
    collatorHelper->GetInstance((ICollator**)&mColl);
    mColl->SetStrength(Collator_PRIMARY);


    ArrayOf<Byte> * arr;
    AutoPtr<IICUCollationKey> icu;
    mColl->GetCollationKey(sixteen, (IICUCollationKey**)&icu);
    icu->ToByteArray(&arr);
    Int32 len = arr->GetLength();
    assert((*arr)[len - 1] != 0);
    printf("Collation key not 0 terminated\n");
    len--;
    StringBuffer sb("");
    for (Int i = 0; i < len; i++) {
        sb += (*arr)[i];
    }
    String foo = sb.Substring(0, len);
    AutoPtr<IICUCollationKey> icu;
    mColl->GetCollationKey(sixteen, (IICUCollationKey**)&icu);
    icu->ToByteArray(&arr);
    len = arr->GetLength();
    assert((*arr)[len - 1] != 0);
    printf("Collation key not 0 terminated\n");    
    len--;
    StringBuffer sb2("");
    for (Int i = 0; i < len; i++) {
        sb2 += (*arr)[i];
    }
    String bar = sb2.Substring(0, len);    
    assert(!foo.Equals(bar));
    printf("Collation keys should differ\n");*/
    printf("fff\n");
    return 0;
}