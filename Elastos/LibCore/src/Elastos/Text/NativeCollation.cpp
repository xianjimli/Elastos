#include "NativeCollation.h"
NativeCollation::NativeCollation() {
}
Int32 NativeCollation::OpenCollator(
        /* [in] */ String locale)
{
    return 0;
}

Int32 NativeCollation::OpenCollatorFromRules(
    /* [in] */ String rules,
    /* [in] */ Int32 normalizationmode,
    /* [in] */ Int32 collationstrength)
{
    return 0;
}

Int32 NativeCollation::CloseCollator(
    /* [in] */ Int32 collatoraddress)
{
    return 0;
}

Int32 NativeCollation::Compare(
    /* [in] */ Int32 collatoraddress, 
    /* [in] */ String source,
    /* [in] */ String target)
{
    return 0;
}

Int32 NativeCollation::GetNormalization(
    /* [in] */ Int32 collatoraddress)
{
    return 0;
}

void NativeCollation::SetNormalization(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ Int32 normalizationmode)
{

}

String NativeCollation::GetRules(
    /* [in] */ Int32 collatoraddress)
{
    return String(NULL);
}

ArrayOf<Byte>* NativeCollation::GetSortKey(
    /* [in] */ Int32 collatoraddress, 
    /* [in] */ String source)
{
    return NULL;
}

void NativeCollation::SetAttribute(
    /* [in] */ Int32 collatoraddress, 
    /* [in] */ Int32 type, 
    /* [in] */ Int32 value)
{

}

Int32 NativeCollation::GetAttribute(
    /* [in] */ Int32 collatoraddress, 
    /* [in] */ Int32 type)
{
    return 0;
}

Int32 NativeCollation::SafeClone(
    /* [in] */ Int32 collatoraddress)
{
    return 0;
}

Int32 NativeCollation::GetCollationElementIterator(
    /* [in] */ Int32 collatoraddress,
    /* [in] */ String source)
{
    return 0;
}

void NativeCollation::CloseElements(
    /* [in] */ Int32 address)
{

}

void NativeCollation::Reset(
    /* [in] */ Int32 address)
{

}


Int32 NativeCollation::Next(
    /* [in] */ Int32 address)
{
    return 0;
}


Int32 NativeCollation::Previous(
    /* [in] */ Int32 address)
{
    return 0;
}

Int32 NativeCollation::GetMaxExpansion(
    /* [in] */ Int32 address, 
    /* [in] */ Int32 order)
{
    return 0;
}

void NativeCollation::SetText(
    /* [in] */ Int32 address, 
    /* [in] */ String source)
{

}

Int32 NativeCollation::GetOffset(
    /* [in] */ Int32 address)
{
    return 0;
}

void NativeCollation::SetOffset(
    /* [in] */ Int32 address,
    /* [in] */ Int32 offset)
{

}