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
    CCollatorHelper::AcquireSingleton((ICollatorHelper**)&collatorHelper);
    AutoPtr<ICollator> collator;
    collatorHelper->GetInstance((ICollator**)&collator);
    collator->SetStrength(ICollator_PRIMARY);
    Int32 strength;
    collator->GetStrength(&strength);
    assert(ICollator_PRIMARY == strength);
    collator->SetStrength(ICollator_SECONDARY);
    collator->GetStrength(&strength);
    assert(ICollator_SECONDARY == strength);
    collator->SetStrength(ICollator_TERTIARY);
    collator->GetStrength(&strength);
    assert(ICollator_TERTIARY == strength);
    collator->SetStrength(ICollator_IDENTICAL);
    collator->GetStrength(&strength);
    assert(ICollator_IDENTICAL == strength);
    collator->SetStrength(-1);
    collator->GetStrength(&strength);
    printf("strength=%d,IllegalArgumentException was not thrown.\n", strength);
    return 0;
}

int CTest::test_stackCorruption(int argc, char* argv[])
{
    AutoPtr<ICollatorHelper> collatorHelper;
    CCollatorHelper::AcquireSingleton((ICollatorHelper**)&collatorHelper);
    AutoPtr<ICollator> mColl;
    collatorHelper->GetInstance((ICollator**)&mColl);
    mColl->SetStrength(ICollator_PRIMARY);
    AutoPtr<IICUCollationKey> icu;
    mColl->GetCollationKey(
        String("2d294f2d3739433565147655394f3762f3147312d3731641452f310"),
        (IICUCollationKey**)&icu);

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

    AutoPtr<ICollatorHelper> collatorHelper;
    CCollatorHelper::AcquireSingleton((ICollatorHelper**)&collatorHelper);
    AutoPtr<ICollator> mColl;
    collatorHelper->GetInstance((ICollator**)&mColl);
    mColl->SetStrength(ICollator_PRIMARY);


    ArrayOf<Byte> * arr;
    AutoPtr<IICUCollationKey> icu;
    mColl->GetCollationKey(sixteen, (IICUCollationKey**)&icu);
    icu->ToByteArray(&arr);
    Int32 len = arr->GetLength();
    assert((*arr)[len - 1] != 0);
    printf("Collation key not 0 terminated\n");
    len--;
    StringBuffer sb("");
    for (Int32 i = 0; i < len; i++) {
        sb += (*arr)[i];
    }
    String foo = sb.Substring(0, len);

    mColl->GetCollationKey(sixteen, (IICUCollationKey**)&icu);
    icu->ToByteArray(&arr);
    len = arr->GetLength();
    assert((*arr)[len - 1] != 0);
    printf("Collation key not 0 terminated\n");
    len--;
    StringBuffer sb2("");
    for (Int32 i = 0; i < len; i++) {
        sb2 += (*arr)[i];
    }
    String bar = sb2.Substring(0, len);
    assert(!foo.Equals(bar));
    printf("Collation keys should differ\n");
    return 0;
}

int CTest::test_decompositionCompatibility(int argc, char* argv[])
{
    AutoPtr<ICollatorHelper> collatorHelper;
    CCollatorHelper::AcquireSingleton((ICollatorHelper**)&collatorHelper);
    AutoPtr<ICollator> myCollator;
    collatorHelper->GetInstance((ICollator**)&myCollator);
    myCollator->SetDecomposition(ICollator_NO_DECOMPOSITION);
    Int32 value;
    myCollator->CompareEx(String("\u00e0\u0325"), String("a\u0325\u0300"), &value);
    if (value == 0) {
        printf("Error: \u00e0\u0325 should not equal to a\u0325\u0300 without decomposition");
        assert(0);
    }
    myCollator->SetDecomposition(ICollator_CANONICAL_DECOMPOSITION);
    myCollator->CompareEx(String("\u00e0\u0325"), String("a\u0325\u0300"), &value);
    if (value != 0) {
        printf("Error: \u00e0\u0325 should equal to a\u0325\u0300 with decomposition");
        assert(0);
    }

    return 0;
}

int CTest::test_EqualsObject(int argc, char* argv[])
{
    String rule("< a < b < c < d < e");
    AutoPtr<IRuleBasedCollator> coll;
    CRuleBasedCollator::New(rule, (IRuleBasedCollator**)&coll);

    Int32 value;
    coll->GetStrength(&value);
    assert(ICollator_TERTIARY == value);

    AutoPtr<IRuleBasedCollator> other;
    CRuleBasedCollator::New(rule, (IRuleBasedCollator**)&other);

    coll->SetStrength(ICollator_PRIMARY);
    assert(coll != other);

    coll->SetStrength(ICollator_TERTIARY);
    coll->SetDecomposition(ICollator_CANONICAL_DECOMPOSITION);
    other->SetDecomposition(ICollator_NO_DECOMPOSITION);
    assert(coll != other);
    return 0;
}

int CTest::test_Harmony(int argc, char* argv[])
{
    String rule("< a< b< c< d");
    AutoPtr<IRuleBasedCollator> coll;
    CRuleBasedCollator::New(rule, (IRuleBasedCollator**)&coll);
    AutoPtr<ICollationElementIterator> cei;
    coll->GetCollationElementIteratorEx((ICharacterIterator*)NULL,
        (ICollationElementIterator**)&cei);
    printf("NullPointerException expected\n");

    return 0;
}