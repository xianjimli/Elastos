#include "cmdef.h"
#include "AttributedCharacterIterator_Attribute.h"
//#include "CAttributedCharacterIterator_Attribute.h"

const AutoPtr<IAttributedCharacterIterator_Attribute> AttributedCharacterIterator_Attribute::INPUT_METHOD_SEGMENT;
const AutoPtr<IAttributedCharacterIterator_Attribute> AttributedCharacterIterator_Attribute::LANGUAGE;
const AutoPtr<IAttributedCharacterIterator_Attribute> AttributedCharacterIterator_Attribute::READING;
/*void AttributedCharacterIterator_Attribute::Init()
{
    CAttributedCharacterIterator_Attribute::New(String("input_method_segment"), 
            (IAttributedCharacterIterator_Attribute**)&INPUT_METHOD_SEGMENT);
    CAttributedCharacterIterator_Attribute::New(String("language"),
            (IAttributedCharacterIterator_Attribute**)&LANGUAGE);
    CAttributedCharacterIterator_Attribute::New(String("reading"),
            (IAttributedCharacterIterator_Attribute**)&READING);
}*/


void AttributedCharacterIterator_Attribute::Init(
        /* [in] */ String name)
{
    mName = name;
}

ECode AttributedCharacterIterator_Attribute::GetName(
        /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode AttributedCharacterIterator_Attribute::ReadResolve(
        /* [out] */ IInterface** resolve)
{
    //if (this.getClass() != Attribute.class) {
    //    throw new InvalidObjectException("cannot resolve subclasses");
    //}
    String name1, name2;
    this->GetName(&name1);
    INPUT_METHOD_SEGMENT->GetName(&name2);
    if (name1.Equals(name2)) {
        *resolve = INPUT_METHOD_SEGMENT;
        return NOERROR;
    }
    LANGUAGE->GetName(&name2);
    if (name1.Equals(name2)) {
        *resolve = LANGUAGE;
        return NOERROR;
    }
    READING->GetName(&name2);
    if (name1.Equals(name2)) {
        *resolve = READING;
        return NOERROR;
    }
    //throw new InvalidObjectException("Unknown attribute");
}