
#include "cmdef.h"
#include "AttributedCharacterIteratorAttribute.h"
#include "CAttributedCharacterIteratorAttribute.h"
#include <Elastos.IO.h>


static AutoPtr<IAttributedCharacterIteratorAttribute> sInit(const String& name)
{
    AutoPtr<CAttributedCharacterIteratorAttribute> attribute;
    CAttributedCharacterIteratorAttribute::NewByFriend(name, (CAttributedCharacterIteratorAttribute**)&attribute);
    return attribute.Get();
}

const AutoPtr<IAttributedCharacterIteratorAttribute> AttributedCharacterIteratorAttribute::INPUT_METHOD_SEGMENT =
        sInit(String("input_method_segment"));
const AutoPtr<IAttributedCharacterIteratorAttribute> AttributedCharacterIteratorAttribute::LANGUAGE =
        sInit(String("language"));
const AutoPtr<IAttributedCharacterIteratorAttribute> AttributedCharacterIteratorAttribute::READING =
        sInit(String("reading"));


ECode AttributedCharacterIteratorAttribute::Init(
    /* [in] */ const String& name)
{
    mName = name;
    return NOERROR;
}

ECode AttributedCharacterIteratorAttribute::GetName(
    /* [out] */ String* name)
{
    VALIDATE_NOT_NULL(name);
    *name = mName;
    return NOERROR;
}

ECode AttributedCharacterIteratorAttribute::ReadResolve(
    /* [out] */ IInterface** resolve)
{
    ClassID clsid;
    GetClassID(&clsid);
    if (clsid != ECLSID_CAttributedCharacterIteratorAttribute) {
    //    throw new InvalidObjectException("cannot resolve subclasses");
        return E_INVALID_OBJECT_EXCEPTION;
    }

    String name;
    INPUT_METHOD_SEGMENT->GetName(&name);
    if (mName.Equals(name)) {
        *resolve = INPUT_METHOD_SEGMENT;
        (*resolve)->AddRef();
        return NOERROR;
    }
    LANGUAGE->GetName(&name);
    if (mName.Equals(name)) {
        *resolve = LANGUAGE;
        (*resolve)->AddRef();
        return NOERROR;
    }
    READING->GetName(&name);
    if (mName.Equals(name)) {
        *resolve = READING;
        (*resolve)->AddRef();
        return NOERROR;
    }
    //throw new InvalidObjectException("Unknown attribute");
    return E_INVALID_OBJECT_EXCEPTION;
}
